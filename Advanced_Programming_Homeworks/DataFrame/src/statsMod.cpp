#include"statsMod.hpp"

//////////////////
// CONSTRUCTORS //            
//////////////////

// Constructor using csv file to build the data-set object.
statsMod::statsMod(const std::string &filePath){

    // Opening the file, throwing an error if something goes wrong in the process.
    std::ifstream file(filePath);
    if(!file.is_open()){
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Obtaining the names of the columns and saving them in the object.
    std::string colNames;
    if (!std::getline(file, colNames)) {
        std::cerr << "Failed to read column names from file." << std::endl;
        return;
    }

    // Trimming whitespaces from the beginning and end of each column name.
    auto trim = [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
            }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    };

    // Adding the names of the columns to the data-set. We assume they are located in the first row of the csv file.
    std::stringstream ss(colNames);
    std::string column;
    while (std::getline(ss, column, ',')) {
        trim(column);
        columns.push_back(column);
    }

    // Using a std::map to store the data in the object. The name of the column is 
    // used as a key for that variable. The value assigned to that key is a std::vector
    // containing std::optional (to account for NAs) of std::variant (which allow different
    // data types to be stored).
    ncol = columns.size();
    for (const auto& column : columns){
        data[column] = std::vector<std::optional<std::variant<double, std::string>>>();
    }

    std::string line;
    std::string val;
    nrow = 0;

    // Iterating over the rows of the data-set.
    while (std::getline(file, line)){

        nrow++;
        ss.clear();
        ss.str(line);

        // Adding each value to the corresponding column in the data-set.
        for(const auto& column : columns) {

            std::getline(ss, val, ',');

            if(val.empty()){ // If no value is found appen an NA 
                std::optional<std::variant<double, std::string>>  NAval;
                data[column].push_back(NAval);
            }
            else try{ // Trying to convert the std::string to double.

                double value = std::stod(val);
                if(nrow == 1) numCols.push_back(column);
                data[column].push_back(value);
            }
            catch (const std::invalid_argument &){ // Append a string to the map otherwise.

                if(nrow == 1) catCols.push_back(column);
                data[column].push_back(val);
            }
        }

        // Manages the case of an NA at the end of a row.
        if (line.back() == '\r' && line[line.size()-2]==','){

            std::optional<std::variant<double, std::string>> NAval;
            data[columns[columns.size() - 1]].pop_back();
            data[columns[columns.size() - 1]].push_back(NAval);
        }
    }

    file.close();
}


// Constructor that uses a std::map of data to build the data-set object.
statsMod::statsMod(std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> &data) : data(data){

    for(const auto &pair : data){
        columns.push_back(pair.first);

        if(std::holds_alternative<double>(pair.second[0].value())){
            numCols.push_back(pair.first);
        }
        else{
            catCols.push_back(pair.first);
        }
    }

    nrow = data[columns[0]].size();
    ncol = columns.size();
}


// Copy constructor.
statsMod::statsMod(const statsMod& other) : 
    data(other.data), 
    columns(other.columns), 
    numCols(other.numCols), 
    catCols(other.catCols), 
    ncol(other.ncol), 
    nrow(other.nrow){}


// Copy assignment operator
statsMod& statsMod::operator=(const statsMod& other) noexcept{
    if (this != &other){

        data = other.data;
        columns = other.columns;
        numCols = other.numCols;
        catCols = other.catCols;
        ncol = other.ncol;
        nrow = other.nrow;
    }
    return *this;
}


// Move constructor.
statsMod::statsMod(statsMod&& other) : data(std::move(other.data)), 
                                       columns(std::move(other.columns)), 
                                       numCols(std::move(other.numCols)), 
                                       catCols(std::move(other.catCols)), 
                                       ncol(other.ncol), 
                                       nrow(other.nrow){

    other.ncol = 0;
    other.nrow = 0;
}


// Move assignment operator.
statsMod& statsMod::operator=(statsMod&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
        columns = std::move(other.columns);
        numCols = std::move(other.numCols);
        catCols = std::move(other.catCols);
        ncol = other.ncol;
        nrow = other.nrow;

        // Reset the other object
        other.ncol = 0;
        other.nrow = 0;
    }
    return *this;
}


// Destructor.
statsMod::~statsMod() = default;


//------------------------------------------------------------------------------------------//


//////////////////////////
// SLICING THE DATA-SET //
//////////////////////////

// Overloading three times the () operator to perform some manipulations of the data. This allows
// to obtain sub matrices of the data-set, slicing on both rows and columns.

statsMod statsMod::operator()(int start, int end){

    if(start < 0 || start > end || end > nrow - 1) throw std::invalid_argument("Invalid indexes. \n");
    std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> newData;

    for(const auto &col : columns){

        newData[col] = std::vector<std::optional<std::variant<double, std::string>>>();
        std::copy(data[col].begin() + start, data[col].begin() + end + 1, std::back_inserter(newData[col]));
    }

    statsMod newobj{newData};
    return newobj;
}

statsMod statsMod::operator()(std::string var){

    std::vector<std::string> vars = {var};
    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");

    std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> newData;

    newData[var] = data[var];

    statsMod newobj{newData};
    return newobj;
}

statsMod statsMod::operator()(std::vector<std::string> vars){

    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");

    std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> newData;

    for(const auto &var : vars){
        
        newData[var] = data[var];
    }

    statsMod newobj{newData};
    return newobj;
}


// Method that returns the subset of the rows that contain a given value in a given column.
// Throws an error if the column does not belong to the data-set.
statsMod statsMod::find(const std::string &var, const std::string &value){

    std::vector<std::string> vars = {var};
    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");
    if(std::find(catCols.begin(), catCols.end(), var) == catCols.end()) throw std::invalid_argument("Can only find observations based on vategorical values. \n");

    std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> newData;
    for (const auto& col : columns){
        newData[col] = std::vector<std::optional<std::variant<double, std::string>>>();
    }

    for(int i=0; i<nrow; i++){
        if(data[var][i].has_value()){

            const auto varValue = data[var][i].value();
            std::string varVal;
            if(std::holds_alternative<std::string>(varValue)) varVal = std::get<std::string>(varValue);
            else varVal = std::to_string(std::get<double>(varValue));

            if(varVal == value){

                for(const auto &col : columns){

                    newData[col].push_back(data[col][i]);
                }
            }
        }
    }

    statsMod newObj{newData};
    return newObj;
}

//------------------------------------------------------------------------------------------//


/////////////
// METHODS //
/////////////

// Method to compute the mean of a given number of columns. The user may specify a contiguous
// subset of rows on which the mean will be computed, If they do not, the whole vector of 
// observation will be used. The function will automatically skip NA values if any are found. 
std::map<std::string, double> statsMod::mean(std::vector<std::string> vars, int start, int end) const{

    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");

    if(end == 0) end = nrow - 1;
    if(start < 0 || start > end || end > nrow - 1) std::invalid_argument("Invalid indexes. \n");

    std::map<std::string, double> means;
    uint count = 0;

    // Lambda function needed to deal with std::variant data types, used in std::accumulate()
    // to obtain the sum of the values of the vector. It is also responsible for checking
    // type-correctness of the data and throws an exception when the latter is not satisfied. 
    // Automatically skippes NA values.
    auto sumVariant = [&count](double sum, const auto& val) -> double{        

        if(val.has_value()){    

            count++;
            const auto &value = val.value();
            if(std::holds_alternative<double>(value)) return sum + std::get<double>(value);
            else throw std::invalid_argument("Mean can only be applied to numerical variables" + std::to_string(count));
        }
        return sum;
    };

    // If an empty vector / no argument is passed to the function it will return the mean for 
    // each numeric variable in the data-set.
    if(vars.size() == 0) vars = numCols;

    for(const auto &col : vars){

        count = 0;
        double sum = std::accumulate(data[col].begin() + start, data[col].begin() + end + 1, 0.0, sumVariant);
        means[col] = sum / count;
    }

    return means;
}


// Method to compute the variance of a given number of columns. The user may specify a contiguous
// subset of rows on which the variance will be computed, If they do not, the whole vector of 
// observation will be used. The function will automatically skip NA values if any are found.  
std::map<std::string, double> statsMod::var(std::vector<std::string> vars, int start, int end) const{

    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");

    if(end == 0) end = nrow - 1;
    if(start < 0 || end < start || end > nrow - 1) std::invalid_argument("Invalid indexes. \n");

    std::map<std::string, double> variances;
    uint count = 0;

    auto sumVariant = [&count](double sum, const auto& val) -> double{        

        if(val.has_value()){    

            count++;
            const auto &value = val.value();
            if(std::holds_alternative<double>(value)) return sum + std::get<double>(value);
            else throw std::invalid_argument("Mean can only be applied to numerical variables");
        }
        return sum;
    };

    // Lambda function needed to deal with std::variant data types, used in std::accumulate()
    // to obtain the sum of the squared values of the vector. It is responsible for type-correctness,
    // throwing an exception if wrong type is given. Automatically skips NA values.
    auto sumVariantSquared = [](double sum, const auto& val) -> double{        

        if(val.has_value()){    

            const auto &value = val.value();
            if(std::holds_alternative<double>(value)) return sum + std::pow(std::get<double>(value), 2);
            else throw std::invalid_argument("Can only take variance of numerical variables");
        }
        return sum;
    };

    // If an empty vector / no argument is passed to the function it will return the mean for 
    // each numeric variable in the data-set. 
    if(vars.size() == 0) vars = numCols;

    for(const auto &col : vars){

        count = 0;
        double sum = std::accumulate(data[col].begin() + start, data[col].begin() + end + 1, 0.0, sumVariant);
        double sumSquared = std::accumulate(data[col].begin() + start, data[col].begin() + end + 1, 0.0, sumVariantSquared); 

        // variance is computed as V(X) = E[X^2] - E[X]^2
        variances[col] = (sumSquared / count) - std::pow(sum / count, 2);
    }

    return variances;
}



// Method used to calculate the standard deviation of a given set of variables in the data-set.
// It simply calls the var() function and takes the squared root of the result.
std::map<std::string, double> statsMod::sd(std::vector<std::string> vars, int start, int end) const{

    std::map<std::string, double> sd = var(vars, start, end);
    for(const auto &pair : sd){
        sd[pair.first] = std::sqrt(sd[pair.first]);
    }

    return sd;
}


// Method used to calculate the median of a number of columns of the data-set. If no argument
// is passed as input to the function, it will return the median of every numerical variable
// within the dataset.
std::map<std::string, double> statsMod::median(std::vector<std::string> vars, int start, int end) const{

    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");

    if(end == 0) end = nrow - 1;
    if(start < 0 || end < start || end > nrow - 1) std::invalid_argument("Invalid indexes. \n");

    std::map<std::string, double> medians;

    // Lambda function needed to deal with std::variant types, used in std::sort()
    // It is also accountable for throwing an exception in case NA values are present.
    auto sortVariant = [](const auto& preVal, const auto& val) -> int{        
        
        if(val.has_value() && preVal.has_value()){    

            const auto &value = val.value();
            const auto &preValue = preVal.value();

            if(std::holds_alternative<std::string>(value) || std::holds_alternative<std::string>(preValue)) 
                throw std::invalid_argument("Can only obtain median of numerical variables. \n");
            return std::get<double>(preValue) < std::get<double>(value);
        }
        throw std::invalid_argument("Median is not able to work with NA values.");
    }; 

    // If an empty vector / no argument is passed to the function it will return the median for 
    // each numeric variable in the data-set.
    if(vars.size() == 0) vars = numCols; 

    // Obtaining the medians.
    for(const auto &col : vars){

        std::vector<std::optional<std::variant<double, std::string>>> data1;
        std::copy(data[col].begin() + start, data[col].begin() + end + 1, std::back_inserter(data1));
        std::sort(data1.begin(), data1.end(), sortVariant);

        uint len = end - start + 1;
        double med;

        // Separating the cases n%2==0 and n%2==1.
        if(len % 2 == 0){

            const auto v1 = data1[len / 2 - 1].value();
            const auto v2 = data1[len / 2].value();
            double med1 = std::get<double>(v1);
            double med2 = std::get<double>(v2);

            med = (med1 + med2) / 2;
        } 
        else{
            
            const auto v = data1[len / 2].value();
            med = std::get<double>(v);
        }

        medians[col] = med;
    }

    return medians;
}


// Method used to calculate the correlation between two variables of the data-set. Likewise the user
// previous functions, the user might specify two indexes, and the correlation will be computed
// only using the rows in between them. Throws an error if any NA value or incorret type input is given.
double statsMod::corr(const std::vector<std::string> &vars, int start, int end) const{

    if(vars.size() != 2) throw std::invalid_argument("Correlation needs exactly to argument as input. \n");
    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");

    if(end == 0) end = nrow - 1;
    if(start < 0 || end < start || end > nrow - 1) std::invalid_argument("Invalid indexes. \n");

    // Lambda function needed to multiply std::optional<std::variant> data-types. It is responsible
    // for type-correctness missing values checks. Throws an error in both cases.
    const auto multVariant = [](const auto &val1, const auto&val2) -> double{

        if(val1.has_value() && val2.has_value()){    

            const auto &v1 = val1.value();
            const auto &v2 = val2.value();

            if(std::holds_alternative<std::string>(v1) || std::holds_alternative<std::string>(v2)) 
                throw std::invalid_argument("Can only obtain correlation of numerical variables. \n");
            return std::get<double>(v1) * std::get<double>(v2);
        }
        throw std::invalid_argument("Correlation is not able to work with NA values.");
    };

    int len = end - start + 1;

    // Means of the two variables.
    std::map<std::string, double> meanVars = mean(vars, start, end);

    // Obtaining XY vector and E[XY]
    std::vector<double> XY(len);
    std::transform(data[vars[0]].begin() + start, data[vars[0]].begin() + end + 1, data[vars[1]].begin() + start, XY.begin(), multVariant);
    double meanXY = std::accumulate(XY.begin() + start, XY.begin() + end + 1, 0.0);
    meanXY /= len;

    // Covariance is obtained by E[XY] - E[X] * E[Y]
    double covariance = meanXY - meanVars[vars[0]] * meanVars[vars[1]];

    // Variances of the two variables.
    std::map<std::string, double> sdVars = sd(vars, start, end);

    // Corr(X,Y) is obtained by COV(X,Y) / (sd[X] * sd[Y])
    return covariance / (sdVars[vars[0]] * sdVars[vars[1]]);
}


// Method that counts the number of observations for every modality in a categorical value.
// Returns a std::map in which the modalities map to the respective absolute frequency.
std::map<std::string, uint> statsMod::freq(const std::string &var) const{

    std::vector<std::string> vars = {var};
    if(! isVar(vars)) throw std::invalid_argument("Object does not possess one or more variables passed as input. \n");
    if(std::find(catCols.begin(), catCols.end(), var) == catCols.end()) throw std::invalid_argument("Can only find observations based on vategorical values. \n");

    std::map<std::string, uint> frequencies;

    for(const auto &val : data[var]){

        if(val.has_value()){

            const auto varValue = val.value();
            std::string varVal; 
            if(std::holds_alternative<std::string>(varValue)){
                varVal = std::get<std::string>(varValue);
            } 
            else varVal = std::to_string(static_cast<int>(std::get<double>(varValue)));

            frequencies[varVal]++;
        }
        else frequencies["NA"]++;
    }

    return frequencies;
}

//------------------------------------------------------------------------------------------//


///////////////
// UTILITIES //
///////////////

// Method that prints the name of the columns of the data-set object.
void statsMod::colnames() const{

    for(const auto &col : columns) std::cout << col << std::endl;
}


// Method that check whether a list of variables is stored in the object.
bool statsMod::isVar(std::vector<std::string> vars) const{

    for(const auto &var : vars){

        if(data.find(var) == data.end()) return 0;
    }
    return 1;
}


// Method that removes every row that possesses one or more NA values.
void statsMod::dropNA(){

    std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> newData;
    for (const auto& col : columns){
        newData[col] = std::vector<std::optional<std::variant<double, std::string>>>();
    }

    for(int i=0; i<nrow; i++){

        int foundNA = 0;

        for(const auto &col : columns){

            if(! data[col][i].has_value()) foundNA = 1;
        }
        if(! foundNA){
            
            for(const auto &col : columns){
                newData[col].push_back(data[col][i]);
            }
        }
    }

    data = newData;
    nrow = data[columns[0]].size();
}


// Method that summarizes the the data-set calculating various statistics on its numerical variables.
void statsMod::summary(){

    head();
    std::cout << "Number of rows: " << nrow << "\n";
    std::cout << "Number of columns: " << ncol << "\n" << std::endl;

    std::cout << "Name of the variables: " << std::endl;
    colnames();
    std::cout << std::endl;

    std::map<std::string, double> means = mean();
    std::cout << "Means: " << std::endl;
    for(auto &col : numCols){
        std::cout << col << ": " << means[col] << std::endl;
    }

    std::map<std::string, double> variances = var();
    std::cout << "\nVariances: " << std::endl;
    for(const auto &col : numCols){
        std::cout << col << ": " << variances[col] << std::endl;
    }

    std::map<std::string, double> sds = sd();
    std::cout << "\nStandard deviations: " << std::endl;
    for(const auto &col : numCols){
        std::cout << col << ": " << sds[col] << std::endl;
    }

    std::map<std::string, double> medians = median();
    std::cout << "\nMedians: " << std::endl;
    for(const auto &col : numCols){
        std::cout << col << ": " << medians[col] << std::endl;
    }
    std::cout << "\n";

    std::map<std::string, uint> freqs;
    for(const auto &col : catCols){
        freqs.clear();
        freqs = freq(col);
        std::cout << col << std::endl;
        for(const auto &pair : freqs){
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
        std::cout << "\n";

    }
}


// Method that prints the first n lines of the data-set.
void statsMod::head(const uint n) {

    std::setprecision(3);

    std::cout << std::endl;
    for(uint i=0; i<ncol; ++i) std::cout << "------------------------";
    std::cout << std::endl;

    // Name of the columns
    std::cout << "|";
    for (const auto& col : columns) {
        std::cout << std::setw(23) << std::left << col;
        std::cout << "|";
    }

    std::cout << std::endl;
    for(uint i=0; i<ncol; ++i) std::cout << "------------------------";
    std::cout << std::endl;

    // Print data
    for (uint i = 0; i < nrow && i <= n; ++i) {

        std::cout << "|";
        for (const auto& column : columns) {
            
            if (data[column][i].has_value()) {
                std::visit([](const auto& value) {
                    std::cout << std::setw(23) << std::left << value;
                    std::cout << "|";
                }, data[column][i].value());

            } else {
                std::cout << std::setw(23) << std::left << "NA";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


// Function to print a row
void statsMod::printRow(const std::map<std::string, std::optional<std::variant<double, std::string>>>& row) const {
    for(const auto& pair : row) {
        const auto& col = pair.first;
        const auto& value = pair.second;

        std::cout << col << ": ";

        if (value.has_value()) {
            std::visit([](const auto& v) {
                std::cout << v;
            }, value.value());
        } else {
            std::cout << "NA";
        }

        std::cout << " | ";
    }

    std::cout << std::endl;
}

//------------------------------------------------------------------------------------------//


//////////////////////////
// WRITE TO OUTPUT FILE //
//////////////////////////

// Method that sends the data-set held by the object in csv format to a file passed as input
void statsMod::writeToFile(const std::string &filePath){

    std::ofstream outputFile(filePath);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    for (const auto& col : columns) {
        outputFile << col << ",";
    }
    outputFile << "\n";

    for (int i = 0; i < nrow; ++i) {
        for (const auto& col : columns) {
            if (data[col][i].has_value()) {
                std::visit([&outputFile](const auto& value) {
                    outputFile << value << ",";
                }, data.at(col)[i].value());
            } else {
                outputFile << "NA,";
            }
        }
        outputFile << "\n";
    }

    outputFile.close();
    std::cout << "Data written to file: " << filePath << std::endl;
}

//------------------------------------------------------------------------------------------//