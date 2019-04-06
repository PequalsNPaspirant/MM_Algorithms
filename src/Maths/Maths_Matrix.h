//Goal
//Matrix Operations

#include <vector>

using namespace std;

namespace mm {

	class MM_Matrix
	{
	public:
		typedef unsigned long long MM_MatrixElementType;

		MM_Matrix(unsigned int rows, unsigned int columns);
		MM_Matrix(const MM_Matrix& copy);
		MM_Matrix(MM_Matrix&& moveCopy);
		const MM_Matrix& operator=(const MM_Matrix& rhs);
		const MM_Matrix& operator=(MM_Matrix&& rhs);

		const MM_Matrix operator*(const MM_Matrix& rhs);
		void operator*=(const MM_Matrix& rhs);
		bool operator==(const MM_Matrix& rhs);
		const MM_Matrix raiseToPower(int exponent);

		void makeUniqueMatrix();

		//To differentiate reads from writes
		//To avoid explicit exposure to row vector of matrix
		class MM_RowMatrix
		{
		public:
			MM_RowMatrix(vector< vector<MM_MatrixElementType> >& refToValues, unsigned int row);
			MM_RowMatrix(const MM_RowMatrix& copy);
			MM_RowMatrix(MM_RowMatrix&& moveCopy);

			// This operator[] absorbs column index
			MM_RowMatrix operator[](int index);

			const MM_MatrixElementType& operator=(const MM_MatrixElementType& rhs); //Write only
			operator const MM_MatrixElementType&() const; //Read only
		private:
			vector< vector<MM_MatrixElementType> >& m_refToValues;
			unsigned int m_row;
			unsigned int m_column;

			const MM_RowMatrix& operator=(const MM_RowMatrix& rhs) = delete;
			const MM_RowMatrix& operator=(MM_RowMatrix&& rhs) = delete;
			MM_RowMatrix* operator&() = delete;
			//This will never be called because we do not create const MM_RowMatrix
			MM_RowMatrix operator[](int index) const = delete;
		};

		class MM_const_RowMatrix
		{
		public:
			MM_const_RowMatrix(const vector< vector<MM_MatrixElementType> >& refToValues, unsigned int row);
			MM_const_RowMatrix(const MM_const_RowMatrix& copy);
			MM_const_RowMatrix(MM_const_RowMatrix&& moveCopy);

			// This operator[] absorbs column index
			// This is used only for reading. It will never be used for writing
			MM_const_RowMatrix operator[](int index);

			operator const MM_MatrixElementType&() const; //Read only

		private:
			const vector< vector<MM_MatrixElementType> >& m_constRefToValues;
			unsigned int m_row;
			unsigned int m_column;

			const MM_const_RowMatrix& operator=(const MM_const_RowMatrix& rhs) = delete;
			const MM_const_RowMatrix& operator=(MM_const_RowMatrix&& rhs) = delete;
			MM_const_RowMatrix* operator&() = delete;
			//This will never be called because we do not create const MM_const_RowMatrix
			MM_const_RowMatrix operator[](int index) const = delete;
			// This will never be called for writing
			const MM_MatrixElementType& operator=(const MM_MatrixElementType& rhs) = delete; //Write only
		};

		// This operator[] absorbs row index
		MM_RowMatrix operator[](const unsigned int index);
		// This can only be used for reading, so take short cuts. 
		MM_const_RowMatrix operator[](const unsigned int index) const;
		// OR it can be:
		//const vector<MM_MatrixElementType>& operator[](const unsigned int index) const;

	private:
		unsigned int m_rows;
		unsigned int m_columns;
		vector< vector<MM_MatrixElementType> > m_values;
	};
}