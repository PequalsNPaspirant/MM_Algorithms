//Goal
//Matrix Operations

#include "Maths/Maths_Matrix.h"
#include "Utils/Utils_MM_Assert.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	MM_Matrix::MM_Matrix(unsigned int rows, unsigned int columns)
		: m_rows(rows), 
		m_columns(columns), 
		m_values(vector< vector<MM_MatrixElementType>>(rows, vector<MM_MatrixElementType>(columns)))
	{
	}

	MM_Matrix::MM_Matrix(const MM_Matrix& copy)
		: m_rows(copy.m_rows), 
		m_columns(copy.m_columns), 
		m_values(copy.m_values)
	{
	}

	MM_Matrix::MM_Matrix(MM_Matrix&& moveCopy)
		: m_rows(moveCopy.m_rows), 
		m_columns(moveCopy.m_columns),
		m_values(std::move(moveCopy.m_values))
	{

	}

	const MM_Matrix& MM_Matrix::operator=(const MM_Matrix& rhs)
	{
		MM_Assert::mmRunTimeAssert(m_rows == rhs.m_rows && m_columns == rhs.m_columns);
		m_values = rhs.m_values;

		return *this;
	}

	const MM_Matrix& MM_Matrix::operator=(MM_Matrix&& rhs)
	{
		MM_Assert::mmRunTimeAssert(m_rows == rhs.m_rows && m_columns == rhs.m_columns);
		m_values = std::move(rhs.m_values);

		return *this;
	}

	const MM_Matrix MM_Matrix::operator*(const MM_Matrix& rhs)
	{
		MM_Assert::mmRunTimeAssert(m_columns == rhs.m_rows);

		MM_Matrix result(m_rows, rhs.m_columns);
		for (unsigned int i = 0; i < m_rows; ++i)
		{
			for (unsigned int j = 0; j < rhs.m_columns; ++j)
			{
				MM_MatrixElementType temp = 0;
				for (unsigned int k = 0; k < m_columns; ++k) //OR for (int k = 0; k < rhs.m_rows; ++k)
				{
					temp += (m_values[i][k] * rhs.m_values[k][j]);
				}
				result.m_values[i][j] = temp;
			}
		}

		return result;
	}

	void MM_Matrix::operator*=(const MM_Matrix& rhs)
	{
		//There is no extra benefit of defining this function.
		//Matrix multiplication is not possible in place
		*this = (*this) * rhs;
	}

	bool MM_Matrix::operator==(const MM_Matrix& rhs)
	{
		if (m_rows != rhs.m_rows || m_columns != rhs.m_columns)
			return false;
		for (unsigned int i = 0; i < m_rows; ++i)
			for (unsigned int j = 0; j < m_columns; ++j)
				if (m_values[i][j] != rhs.m_values[i][j])
					return false;

		return true;
	}

	MM_Matrix::MM_RowMatrix MM_Matrix::operator[](const unsigned int index)
	{
		return MM_Matrix::MM_RowMatrix(m_values, index);
	}

	MM_Matrix::MM_const_RowMatrix MM_Matrix::operator[](const unsigned int index) const
	{
		return MM_Matrix::MM_const_RowMatrix(m_values, index);
	}

	//=================== Member Function definitions of class MM_RowMatrix ===================

	MM_Matrix::MM_RowMatrix::MM_RowMatrix(vector< vector<MM_MatrixElementType> >& refToValues, unsigned int row)
		: m_refToValues(refToValues),
		m_row(row),
		m_column(0)
	{

	}

	MM_Matrix::MM_RowMatrix::MM_RowMatrix(const MM_Matrix::MM_RowMatrix& copy)
		: m_refToValues(copy.m_refToValues),
		m_row(copy.m_row),
		m_column(copy.m_column)
	{

	}

	MM_Matrix::MM_RowMatrix::MM_RowMatrix(MM_Matrix::MM_RowMatrix&& moveCopy)
		: m_refToValues(moveCopy.m_refToValues),
		m_row(moveCopy.m_row),
		m_column(moveCopy.m_column)
	{

	}

	// This operator[] absorbs column index
	MM_Matrix::MM_RowMatrix MM_Matrix::MM_RowMatrix::operator[](int index)
	{
		m_column = index;
		return *this;
	}

	const MM_Matrix::MM_MatrixElementType& MM_Matrix::MM_RowMatrix::operator=(const MM_Matrix::MM_MatrixElementType& rhs) //Write only
	{
		m_refToValues[m_row][m_column] = rhs;
		return rhs;
	}

	MM_Matrix::MM_RowMatrix::operator const MM_Matrix::MM_MatrixElementType&() const//Read only
	{
		return m_refToValues[m_row][m_column];
	}


	//=================== Member Function definitions of class MM_const_RowMatrix ===================

	MM_Matrix::MM_const_RowMatrix::MM_const_RowMatrix(const vector< vector<MM_MatrixElementType> >& refToValues, unsigned int row)
		: m_constRefToValues(refToValues),
		m_row(row),
		m_column(0)
	{

	}

	MM_Matrix::MM_const_RowMatrix::MM_const_RowMatrix(const MM_const_RowMatrix& copy)
		: m_constRefToValues(copy.m_constRefToValues),
		m_row(copy.m_row),
		m_column(copy.m_column)
	{

	}

	MM_Matrix::MM_const_RowMatrix::MM_const_RowMatrix(MM_const_RowMatrix&& moveCopy)
		: m_constRefToValues(moveCopy.m_constRefToValues),
		m_row(moveCopy.m_row),
		m_column(moveCopy.m_column)
	{

	}

	// This operator[] absorbs column index
	// This is used only for reading. It will never be used for writing
	MM_Matrix::MM_const_RowMatrix MM_Matrix::MM_const_RowMatrix::operator[](int index)
	{
		m_column = index;
		return *this;
	}

	MM_Matrix::MM_const_RowMatrix::operator const MM_Matrix::MM_MatrixElementType&() const //Read only
	{
		return m_constRefToValues[m_row][m_column];
	}




	// The algorithm is same as function "double power_iterative(double base, int exponent)" in file "my_math_lib/ArithmeticOperations.cpp"
	const MM_Matrix MM_Matrix::raiseToPower(int exponent)
	{
		MM_Assert::mmRunTimeAssert(m_rows == m_columns, "This should be square matrix");

		MM_Matrix result(m_rows, m_columns);
		result.makeUniqueMatrix();
		MM_Matrix factor(*this);
		while (true)
		{
			if (exponent % 2 == 1) // instead we can also use (exponent & 1) == 1 
				result *= factor;

			exponent /= 2; // instead we can also use exponent = (exponent >> 1)
			if (exponent == 0)
				break;

			factor *= factor;
		}

		return result;
	}

	void MM_Matrix::makeUniqueMatrix()
	{
		MM_Assert::mmRunTimeAssert(m_rows == m_columns, "This should be square matrix");

		for (unsigned int i = 0; i < m_rows; ++i)
		{
			for (unsigned int j = 0; j < m_columns; ++j)
			{
				if (i == j)
					m_values[i][j] = 1;
				else
					m_values[i][j] = 0;
			}
		}
	}

	void testMatrix(double base, int exponent)
	{
		int mul1 = 0, mul2 = 0, mul3 = 0, mul4 = 0;
		MM_EXPECT_TRUE(mul1 == mul2 && mul2 == mul3 && mul3 == mul4, mul1, mul2, mul3, mul4)
	}

	MM_DECLARE_FLAG(arithmeticOperations_matrix);

	MM_UNIT_TEST(arithmeticOperations_matrix_test_1, arithmeticOperations_matrix)
	{
		double base = 2;
		for (int exponent = 0; exponent < 100; exponent++)
			testMatrix(2, exponent);
	}
}