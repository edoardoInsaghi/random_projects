#ifndef STATSMOD__
#define STATSMOD__

#include<iostream>
#include<iomanip>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<map>
#include<variant>
#include<optional>
#include<numeric>
#include<cmath> 

typedef unsigned int uint;

class statsMod{
public:

    // Constructors
    statsMod(const std::string &filePath);
    statsMod(std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> &data);
    statsMod(const statsMod &other);
    statsMod& operator=(const statsMod &other) noexcept;
    statsMod(statsMod &&other);
    statsMod& operator=(statsMod &&other) noexcept;
    ~statsMod();

    // Iterator on the rows
    class df_iterator{
    public:
        using iterator_type       = std::forward_iterator_tag;
        using difference_type     = std::ptrdiff_t;
        using value_type          = std::map<std::string, std::optional<std::variant<double, std::string>>>;
        using pointer             = value_type*;
        using reference           = value_type&;

        df_iterator(statsMod& df, uint pos = 0) : pos(pos), df(df) {
            for (const auto &col : df.columns){
                row[col] = df.data[col][pos];
            }
        }
        value_type operator*() const{
            return row;
        }
        df_iterator& operator++(){
            ++pos;
            for (const auto &col : df.columns){
                row[col] = df.data[col][pos];
            }
            return *this;
        }
        df_iterator operator++(int){
            df_iterator tmp{*this};
            ++pos;
            for (const auto &col : df.columns){
                row[col] = df.data[col][pos];
            }
            return tmp;
        }
        bool operator==(const df_iterator& other) const{
            return (&this->df == &other.df && this->pos == other.pos);
        }
        bool operator!=(const df_iterator& other) const{
            return (&this->df != &other.df || this->pos != other.pos);
        }

        value_type row;
        uint pos;   
        statsMod& df;
    };

    df_iterator begin() { return df_iterator(*this); }
    df_iterator end() { return df_iterator(*this, nrow); }

    // Slicing the data-set
    statsMod operator()(int start, int end); // selecting rows between start and end (included).
    statsMod operator()(std::string var); // selecting given column.
    statsMod operator()(std::vector<std::string> vars); // selecting given columns.
    statsMod find(const std::string &var, const std::string &value); // select rows for which var == value.

    // Methods
    std::map<std::string, double> mean(std::vector<std::string> vars = {}, int start = 0, int end = 0) const;
    std::map<std::string, double> var(std::vector<std::string> vars = {}, int start = 0, int end = 0) const;
    std::map<std::string, double> sd(std::vector<std::string> vars = {}, int start = 0, int end = 0) const;
    std::map<std::string, double> median(std::vector<std::string> vars = {}, int start = 0, int end = 0) const;
    std::map<std::string, uint> freq(const std::string &var) const;
    double corr(const std::vector<std::string> &vars, int start = 0, int end = 0) const;
    
    // Utilities
    void colnames() const; // prints name of the variables of the data-frame.
    bool isVar(std::vector<std::string> vars) const; // checks if list of variables is present in data-frame colnames.
    void dropNA(); // removes all rows with at least one NA value.
    void head(const uint n = 10); // prints first n lines of the data-frame.
    void printRow(const std::map<std::string, std::optional<std::variant<double, std::string>>>& row) const; // prints a row of the data-frame.
    void summary(); // brief summary of the data-set.
    void writeToFile(const std::string &outputFile); // writes to output file the data-set.

    // Attributes
    mutable std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> data; // std::map containing the data
    std::vector<std::string> columns; // names of the variables.
    std::vector<std::string> numCols; // names of the numerical variables.
    std::vector<std::string> catCols; // names of the categoriacal variables.
    int ncol; // number of columns
    int nrow; // number of rows
};

#endif