#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <vector>
#include <iostream>
#include <stdexcept>


// Parent abstract class
class SparseMatrix {
public:

    //Constructor
    SparseMatrix(std::vector<double> input_values,
         std::vector<unsigned int> input_columns); 

	// Destructor
    virtual ~SparseMatrix(){}

	// Method to get the number of rows
    virtual unsigned int getRows() const = 0;

    // Method to get the number of columns
    unsigned int getColumns() const;

    // Method to get the number of non zero values
    virtual unsigned int getNNZ() = 0;

    // Overloading the () operator to read one entry of the matrix
    virtual const double operator()(const unsigned int i, const unsigned int j) const = 0;

    // Overloading the () operator to write one entry of the matrix
    virtual double& operator()(const unsigned int i, const unsigned int j) = 0;

    // Overloading the * operator to perform matrix vector multiplication
    virtual std::vector<double> operator*(const std::vector<double> v) const = 0;

    // Method to print the matrix
    virtual void printMatrix() const = 0;

protected:
    std::vector<double> values;
    std::vector<unsigned int> columns;
    unsigned int nnz;
};




// Declaration of SparseMatrixCOO, inheriting from parent class
class SparseMatrixCOO : public SparseMatrix{
public:

	// Constructor
    SparseMatrixCOO(std::vector<double> input_values, 
                    std::vector<unsigned int> input_rows,  
                    std::vector<unsigned int> input_columns);

    // Copy constructor
    SparseMatrixCOO(const SparseMatrixCOO& other);

    // Copy assignment operator
    SparseMatrixCOO& operator=(const SparseMatrixCOO& other);

    // Destructor
    ~SparseMatrixCOO(){

       	std::vector<double>().swap(values);
       	std::vector<unsigned int>().swap(rows);
       	std::vector<unsigned int>().swap(columns);
    }

	// Method to get the number of rows
    unsigned int getRows() const override;

    // Method to get the number of non zero values
    unsigned int getNNZ() override;

    // Overloading the () operator to read one entry of the matrix
    const double operator()(const unsigned int i, const unsigned int j) const override;

    // Overloading the () operator to write one entry of the matrix
    double& operator()(const unsigned int i, const unsigned int j) override;

    // Overloading the * operator to perform matrix vector multiplication
    std::vector<double> operator*(const std::vector<double> v) const override;

    // Method to print the matrix
    void printMatrix() const override;


private:
    std::vector<unsigned int> rows;
};




// Declaration of SparseMatrixCSR, inheriting from parent class
class SparseMatrixCSR : public SparseMatrix{
public:

	// Constructor
    SparseMatrixCSR(std::vector<double> input_values,
                    std::vector<unsigned int> input_rows,
                    std::vector<unsigned int> input_columns);

    // Copy constructor
    SparseMatrixCSR(const SparseMatrixCSR& other);

    // Copy assignment operator
    SparseMatrixCSR& operator=(const SparseMatrixCSR& other);

    // Destructor
    ~SparseMatrixCSR(){

       	std::vector<double>().swap(values);
       	std::vector<unsigned int>().swap(rows);
       	std::vector<unsigned int>().swap(columns);
    }

	// Method to get the number of rows
    unsigned int getRows() const override;

    // Method to get the number of non zero values
    unsigned int getNNZ() override;

    // Overloading the () operator to read one entry of the matrix
    const double operator()(const unsigned int i, const unsigned int j) const override;

    // Overloading the () operator to write one entry of the matrix
    double& operator()(const unsigned int i, const unsigned int j) override;

    // Overloading the * operator to perform matrix vector multiplication
    std::vector<double> operator*(const std::vector<double> v) const override;

    // Method to print the matrix
    void printMatrix() const override;


private:

    std::vector<unsigned int> rows;
};



// Utility functions

// Method to convert a SparseMatrixCOO to a SparseMatrixCSR
SparseMatrixCSR COO_to_CSR(const SparseMatrixCOO& cooMatrix);


// Method to convert a SparseMatrixCSR to a SparseMatrixCOO
SparseMatrixCOO CSR_to_COO(const SparseMatrixCSR& csrMatrix);


#endif