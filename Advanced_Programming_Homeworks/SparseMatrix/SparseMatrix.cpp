#include "SparseMatrix.hpp"


///////////////////////////////////////////////////////
// Implementation of the SparseMatrix abstract class //
///////////////////////////////////////////////////////

SparseMatrix::SparseMatrix(std::vector<double> input_values,
						   std::vector<unsigned int> input_columns)

    : values(input_values), columns(input_columns) {

    nnz = input_values.size();
}


unsigned int SparseMatrix::getColumns() const {

    unsigned int max = columns[0];

    for (unsigned int i = 1; i < this->nnz; i++) {
        max = (columns[i] > max) ? columns[i] : max;
    }

    return max + 1;
}


/////////////////////////////////////////////////
// Implementation of the SparseMatrixCOO class //
/////////////////////////////////////////////////

// Constructor
SparseMatrixCOO::SparseMatrixCOO(std::vector<double> input_values, 
	                    		 std::vector<unsigned int> input_rows, 
	                    		 std::vector<unsigned int> input_columns)

	: SparseMatrix(input_values, input_columns), rows(input_rows) {}



// Copy constructor
SparseMatrixCOO::SparseMatrixCOO(const SparseMatrixCOO& other)

	: SparseMatrix(other.values, other.columns), rows(other.rows) {

    nnz = other.nnz;
}



// Copy assignment operator
SparseMatrixCOO& SparseMatrixCOO::operator=(const SparseMatrixCOO &other){

	if (this != &other){

		values = other.values;
		rows = other.rows;
		columns = other.columns;
		nnz = other.nnz;
    }

    return *this;
}



// Method to get the number of rows
unsigned int SparseMatrixCOO::getRows() const{

    unsigned int max = 0;

    for (unsigned int i = 0; i < this->nnz; i++) {
        max = (rows[i] > max) ? rows[i] : max;
    }

    return max + 1;
}




// Method to get the number of non zero values
unsigned int SparseMatrixCOO::getNNZ(){

	// Cleaning values, in case a zero value was added to the matrix
	for(unsigned int i = 0; i < this->nnz; i++){

		if (values[i] == 0){

			values.erase(values.begin() + i);
			rows.erase(rows.begin() + i);
			columns.erase(columns.begin() + i);
		}
	}

	// Updating the number of nnz values in the matrix
	this->nnz = values.size();
	return this->nnz;
}



// Overloading the () operator to read one entry of the matrix
const double SparseMatrixCOO::operator()(const unsigned int i, const unsigned int j) const{

	// Obtaining the bounds of the matrix
	int rowBound = getRows();
	int colBound = getColumns();

	// Throwing an error if the indexes are out of bounds
	if(i < 0 || i >= rowBound || j < 0 || j >= colBound){
		throw std::invalid_argument("Indexes out of bound.");
	}
	
	// Return value is initialized to zero
	double val = 0;

	for(unsigned int k = 0; k < this->nnz; k++){

		// If there is a nnz value corresponding to the indexes it is assigned to the return value
		if (rows[k] == i && columns[k] == j){

			val = values[k];
			break;
		}
	}
	
	return val;
}



// Overloading the () operator to write one entry of the matrix
double& SparseMatrixCOO::operator()(const unsigned int i, const unsigned int j){

	int rowBound = getRows();
	int colBound = getColumns();

	// Throwing an error if the indexes are out of bounds
	if(i < 0 || i >= rowBound || j < 0 || j >= colBound){
		throw std::invalid_argument("Indexes out of bound.");
	}

	// Checking if any value already exists at indexes i, j
	// Replacing it with new value if there is
	for(unsigned int k = 0; k < this->nnz; k++){

		if(rows[k] == i && columns[k] == j){

			return values[k];
		}
	}

	// Adding the specifics of the new elements to the vectors containing the data if
	// its indexes are compatible with the size of the matrix but was not yet allocated
	rows.push_back(i);
	columns.push_back(j);
	values.push_back(0);

	// Updating the number of non zero elements in the matrix
	this->nnz = values.size();
	return values[values.size() - 1];
}



// Overloading * operator to perform matrix vector multiplication
std::vector<double> SparseMatrixCOO::operator*(const std::vector<double> v) const{

	int rowBound = getRows();
	int colBound = getColumns();

	if(v.size() != colBound){
		throw std::invalid_argument("The size of the vector should be equal"
									"to the number of columns of the matrix.");
	}

	// Solution vector, initially simply the zero vector
	std::vector<double> result(rowBound);
	std::fill(result.begin(), result.end(), 0);

	// Iterating over all the nonzero elements of the matrix
	for(unsigned int k = 0; k < this->nnz; k++){

		// result[i] += A(i,j) * v[j]
		result[rows[k]] += values[k] * v[columns[k]];
	}

	return result;
}




// Method to print the matrix
void SparseMatrixCOO::printMatrix() const{

	int rowBound = getRows();
	int colBound = getColumns();

	// Iterating over the rows
	for(unsigned int i = 0; i < rowBound; i++){

		// Iterating over the columns
		for(unsigned int j = 0; j < colBound; j++){

			double valToPrint = 0;

			for(unsigned int k = 0; k < this->nnz; k++){

				// Checking if A(i,j) is a nnz value
				if(rows[k] == i && columns[k] == j){

					valToPrint = values[k];
					break;
				}
			}
			std::cout << valToPrint << "      ";
		}
		std::cout << "\n";
	}
} 





/////////////////////////////////////////////////
// Implementation of the SparseMatrixCSR class //
/////////////////////////////////////////////////

// Constructor
SparseMatrixCSR::SparseMatrixCSR(std::vector<double> input_values, 
	                    		 std::vector<unsigned int> input_rows, 
	                    		 std::vector<unsigned int> input_columns)

	: SparseMatrix(input_values, input_columns), rows(input_rows) {}




// Copy constructor
SparseMatrixCSR::SparseMatrixCSR(const SparseMatrixCSR& other)

	: SparseMatrix(other.values, other.columns), rows(other.rows) {

    nnz = other.nnz;
}



// Copy assignment operator
SparseMatrixCSR& SparseMatrixCSR::operator=(const SparseMatrixCSR &other){

	if (this != &other){

		values = other.values;
		rows = other.rows;
		columns = other.columns;
		nnz = other.nnz;
    }

    return *this;
}



// Method to get the number of rows
unsigned int SparseMatrixCSR::getRows() const{

	return rows.size() - 1;
}



// Method to get the number of non zero values
unsigned int SparseMatrixCSR::getNNZ(){

	return this->nnz;
}



// Overloading the () operator to read one entry of the matrix
const double SparseMatrixCSR::operator()(const unsigned int i, const unsigned int j) const{

	// Obtaining the bounds of the matrix
	int rowBound = getRows();
	int colBound = getColumns();

	// Throwing an error if the indexes are out of bounds
	if(i < 0 || i >= rowBound || j < 0 || j >= colBound){
		throw std::invalid_argument("Indexes out of bound.");
	}

	// Position of the first nonzero value for the ith row in the values array 
	int start_row = rows[i];

	// Return value initialized to zero
	double val = 0;

	// Iterating over the nnz values of row i
	for(int k = 0; k < rows[i+1] - rows[i]; k++){

		if (columns[start_row + k] == j){

			val = values[start_row + k];
			break;
		}
	}

	return val;
}



// Overloading the () operator to write one entry of the matrix
double& SparseMatrixCSR::operator()(const unsigned int i, const unsigned int j){

	int rowBound = getRows();
	int colBound = getColumns();

	// Throwing an error if the indexes are out of bounds
	if(i < 0 || i >= rowBound || j < 0 || j >= colBound){
		throw std::invalid_argument("Indexes out of bound.");
	}

	// Position of the first nonzero value for the ith row in the values array 
	int start_row = rows[i];

	// Iterating over the nnz values in the ith row
	for(int k = 0; k < rows[i + 1] - rows[i]; k++){

		// Checking if a value already exists in position i,j
		if(columns[start_row + k] == j){

			return values[start_row + k];
		}
	}

	// Allocating the value if not already present in the matrix at position i,j
	// Iterating over the nnz values in the ith row
	for(int k = 0;  k < rows[i + 1] - rows[i]; k++){

		// Checking if column index of given value is less than the column index of
		// the next nnz value in row i
		if (j < columns[start_row + k]){

			// Updating the rows vector
			for(int u = i + 1; u < rows.size(); u++){
				rows[u]++;
			}

			// Inserting column index of new number in the columns vector
			columns.insert(columns.begin() + start_row + k, j);

			// Inserting given value in the values vector
			values.insert(values.begin() + start_row + k, 0);

			// Updating the number of non zeros
			this->nnz = values.size();

			return values[start_row + k];
		}
	}

	// If the function arrives here the given value is the last nnz of row i

	// Updating the rows vector
	for(unsigned int u = i + 1; u < rows.size(); u++){
		rows[u]++;
	}

	// Inserting column index of new value in the columns vector
	columns.insert(columns.begin() + start_row + rows[i + 1] - rows[i] - 1, j);

	// Inserting given value in the values vector
	values.insert(values.begin() + start_row + rows[i + 1] - rows[i] - 1, 0);

	// Updating the number of non zeros
	this->nnz = values.size();

	return values[start_row + rows[i + 1] - rows[i] - 1];
}



// Overloading * operator to perform matrix vector multiplication
std::vector<double> SparseMatrixCSR::operator*(const std::vector<double> v) const{

	int rowBound = getRows();
	int colBound = getColumns();

	if(v.size() != colBound){
		throw std::invalid_argument("The size of the vector should be equal"
									"to the number of columns of the matrix.");
	}

	// Solution vector, initially simply the zero vector
	std::vector<double> result(rowBound);
	std::fill(result.begin(), result.end(), 0);

	// Iterating over the rows
	for(unsigned int i = 0; i < rowBound; i++){

		int start_row = rows[i];

		// Iterating over the non zero elements in row i
		for(unsigned int k = 0;  k < rows[i + 1] - rows[i]; k++){

			result[i] += v[columns[start_row + k]] * values[start_row + k];
		}
	}

	return result;
}



// Method to print the matrix
void SparseMatrixCSR::printMatrix() const{

	int rowBound = getRows();
	int colBound = getColumns();

	// Iterating on the rows
	for(unsigned int i = 0; i < rowBound; i++){

		int start_row = rows[i];

		// Iterating on the columns
		for(unsigned int j = 0; j < colBound; j++){

			double valToPrint = 0;

			// Iterating over the nnz values in row i
			for(unsigned int k = 0; k < rows[i + 1] - rows[i]; k++){

				// Checking if A(i,j) is a nnz value
				if(columns[start_row + k] == j){

					valToPrint = values[start_row + k];
					break;
				}
			}
			std::cout << valToPrint << "      ";
		}
		std::cout << "\n";
	}
}





/////////////////////////////////////////////
// Implementation of the utility functions //
/////////////////////////////////////////////

// Method to convert a SparseMatrixCOO to a SparseMatrixCSR
SparseMatrixCSR COO_to_CSR(const SparseMatrixCOO& cooMatrix){

	const SparseMatrixCOO& cooRef = cooMatrix;

	std::vector<double> csr_values;
	std::vector<unsigned int> csr_rows;
	csr_rows.push_back(0);
	std::vector<unsigned int> csr_columns;


	unsigned int curr_nnz = 0;
	for(unsigned int i = 0; i < cooMatrix.getRows(); i++){

		for(unsigned int j = 0; j < cooMatrix.getColumns(); j++){

			if(cooRef(i, j) != 0){

				csr_values.push_back(cooRef(i, j));
                csr_columns.push_back(j);
				curr_nnz++;
			}
		}

		csr_rows.push_back(curr_nnz);
	}

	SparseMatrixCSR csr_matrix(csr_values, csr_rows, csr_columns);

	return csr_matrix;
}



// Method to convert a SparseMatrixCSR to a SparseMatrixCOO
SparseMatrixCOO CSR_to_COO(const SparseMatrixCSR& csrMatrix){

	const SparseMatrixCSR& csrRef = csrMatrix;

	std::vector<double> coo_values;
	std::vector<unsigned int> coo_rows;	 
	std::vector<unsigned int> coo_columns;

	for(unsigned int i = 0; i < csrMatrix.getRows(); i++){

		for(unsigned int j = 0; j < csrMatrix.getColumns(); j++){

			if(csrRef(i, j) != 0){

				coo_values.push_back(csrRef(i, j));
				coo_rows.push_back(i);
				coo_columns.push_back(j);
			}
		}
	}

	SparseMatrixCOO coo_matrix(coo_values, coo_rows, coo_columns);

	return coo_matrix;
}



