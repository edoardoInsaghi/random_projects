#include "SparseMatrix.hpp"
#include <iomanip>


int main(){

	std::cout << std::setprecision(2) << std::fixed;

	std::cout << "\n";
	std::cout << "###############################################" << std::endl;
	std::cout << "COO Format" << std::endl;
	std::cout << "###############################################" << std::endl;
	std::cout << "\n";

	std::vector<double> values  = {3.1, 4, 5, 7.4, 2, 6};
	std::vector<unsigned int> rows = {0, 0, 1, 1, 3, 3};
	std::vector<unsigned int> columns = {2, 4, 2, 4, 1, 3};

	SparseMatrixCOO A{values, rows, columns};

	std::cout << "Printing the matrix:" << std::endl;
	A.printMatrix();
	std::cout << "\n";

	std::cout << "It has " << A.getRows() << " rows and " << A.getColumns() << " columns." << std::endl;
	std::cout << "The number of non zero elements inside the matrix is " << A.getNNZ() << ".\n" << std::endl;

	const SparseMatrixCOO& cooRef = A;
	std::cout << "Reading a value from the matrix:" << std::endl;
	std::cout << "Value in position (1, 4) = " << cooRef(1, 4) << std::endl;
	std::cout << "\n";

	std::cout << "Adding the value 2.11 in position (2, 0)" << std::endl;
	A(2, 0) = 2.11;
	A.printMatrix();
	std::cout << "The number of non zeros is now " << A.getNNZ() <<".\n" << std::endl;

	std::cout << "Matrix vector product with v = [1, 1, 1, 1, 1]" << std::endl;
	std::cout << "Returns the sum of the values in each row." << std::endl;
	std::vector<double> v = {1, 1, 1, 1, 1};
	std::vector<double> result = A*v;

	for(int i = 0; i < result.size(); i++){

		std::cout << result[i] << std::endl;
	}
	std::cout << "\n";

	std::cout << "Matrix vector product with v = [0, 0, 1, 0, 0]" << std::endl;
	std::cout << "Returns third column of the matrix." << std::endl;
	std::vector<double> e3 = {0, 0, 1, 0, 0};
	std::vector<double> result1 = A*e3;

	for(int i = 0; i < result1.size(); i++){

		std::cout << result1[i] << std::endl;
	}
	std::cout << "\n";

	std::cout << "Converting the matrix to CSR format." << std::endl;
	SparseMatrixCSR B = COO_to_CSR(A);
	B.printMatrix();



	std::cout << "\n\n";
	std::cout << "###############################################" << std::endl;
	std::cout << "CSR Format" << std::endl;
	std::cout << "###############################################" << std::endl;
	std::cout << "\n";

	std::vector<double> values1  = {3.1, 4, 5, 7.4, 2, 6};
	std::vector<unsigned int> rows1 = {0, 2, 4, 4, 6};
	std::vector<unsigned int> columns1 = {2, 4, 2, 4, 1, 3};

	SparseMatrixCSR D{values1, rows1, columns1};

	std::cout << "Printing the matrix:" << std::endl;
	D.printMatrix();
	std::cout << "\n";

	std::cout << "It has " << D.getRows() << " rows and " << D.getColumns() << " columns." << std::endl;
	std::cout << "The number of non zero elements inside the matrix is " << D.getNNZ() << ".\n" << std::endl;

	const SparseMatrixCSR& csrRef = D;
	std::cout << "Reading a value from the matrix:" << std::endl;
	std::cout << "Value in position (1, 4) = " << csrRef(1, 4) << std::endl;
	std::cout << "\n";

	std::cout << "Adding the value 2.11 in position (2, 0)" << std::endl;
	D(2, 0) = 2.11;
	A.printMatrix();
	std::cout << "The number of non zeros is now " << D.getNNZ() <<".\n" << std::endl;

	std::cout << "Matrix vector product with v = [1, 1, 1, 1, 1]" << std::endl;
	std::cout << "Returns the sum of the values in each row." << std::endl;
	std::vector<double> v2 = {1, 1, 1, 1, 1};
	std::vector<double> result2 = D*v2;

	for(int i = 0; i < result2.size(); i++){

		std::cout << result2[i] << std::endl;
	}
	std::cout << "\n";

	std::cout << "Matrix vector product with v = [0, 0, 1, 0, 0]" << std::endl;
	std::cout << "Returns third column of the matrix." << std::endl;
	std::vector<double> E3 = {0, 0, 1, 0, 0};
	std::vector<double> result3 = D*E3;

	for(int i = 0; i < result3.size(); i++){

		std::cout << result3[i] << std::endl;
	}
	std::cout << "\n";

	std::cout << "Converting the matrix to COO format." << std::endl;
	SparseMatrixCOO E = CSR_to_COO(D);
	E.printMatrix();

	return 0;
}



