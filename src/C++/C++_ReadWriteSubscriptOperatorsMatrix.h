//Goal
// Implement double subscript oprators for a Matrix to separate read write operations
// Refer MM_CommonUtils -> class Matrix

#include <vector>

using namespace std;

namespace mm {

	template<typename T>
	class MM_Matrix_subscript
	{
	public:
		//typedef unsigned long long MM_MatrixElementType;

		MM_Matrix_subscript(unsigned int rows, unsigned int columns);
		MM_Matrix_subscript(const MM_Matrix_subscript& copy);
		MM_Matrix_subscript(MM_Matrix_subscript&& moveCopy);
		//const MM_Matrix_subscript& operator=(const MM_Matrix_subscript& rhs);
		//const MM_Matrix_subscript& operator=(MM_Matrix_subscript&& rhs);

		const MM_Matrix_subscript operator*(const MM_Matrix_subscript& rhs);
		void operator*=(const MM_Matrix_subscript& rhs);
		bool operator==(const MM_Matrix_subscript& rhs);
		const MM_Matrix_subscript raiseToPower(int exponent);

		void makeUniqueMatrix();

		//To differentiate reads from writes
		//To avoid explicit exposure to row vector of matrix
		class MM_RowMatrix
		{
		public:
			MM_RowMatrix(MM_Matrix_subscript& refMatrix, unsigned int row)
				: m_refMatrix(refMatrix),
				m_row(row)
			{
			}
			MM_RowMatrix(MM_RowMatrix& copy);
			//	: m_refMatrix(copy.m_refMatrix)
			//	m_row(copy.m_row)
			//{
			//}

			MM_RowMatrix& operator=(MM_RowMatrix& copy) = delete;
			MM_RowMatrix(MM_RowMatrix&& moveCopy) = delete;

			class Proxy
			{
			public:
				Proxy(MM_Matrix_subscript& refMatrix, const unsigned int index)
					:m_refData(refObject),
					m_index(index)
				{
				}

				Proxy(const Proxy& rhs)
					: m_refData(rhs.m_refData)
					m_index(rhs.m_index)
				{

				}

				// This is required when we do: Array<int> intArray(5); intArray[0] = intArray[1];
				//Proxy& operator=(const Proxy& rhs) //No need to take const Proxy argument, as const Proxy is not created
				Proxy& operator=(Proxy& rhs)
				{
					T rhsValue = rhs; // This fails if rhs is const. operator const T&() is not const
					*this = rhsValue;
					return *this;
				}

				//Write (non-const member function)
				//void operator=(const T& rval)
				// Issues: Chain assignment is NOT possible with below approach: e.g.:
				// Array<int>[0] = Array<int>[1] = Array<int>[2] = 10
				// Array<int>[2] returns Proxy() object
				// Array<int>[0] = Array<int>[1] = Proxy::operator=(10)
				// Array<int>[0] = Array<int>[1] = void

				//T& operator=(const T& rval) 
				// Issues: Chain assignement possible but avoids reading path. e.g.:
				// Array<int>[0] = Array<int>[1] = Array<int>[2] = 10
				// Array<int>[2] returns Proxy() object
				// Array<int>[0] = Array<int>[1] = Proxy::operator=(10)
				// Array<int>[0] = Array<int>[1] = T&
				// Instead it should return Proxy

				//Array<T>& operator=(const T& rval)
				// Issues: Chain assignement is NOT possible but avoids reading path. e.g.:
				// Array<int>[0] = Array<int>[1] = Array<int>[2] = 10
				// Array<int>[2] returns Proxy() object
				// Array<int>[0] = Array<int>[1] = Proxy::operator=(10)
				// Array<int>[0] = Array<int>[1] = Array<int>
				// ERROR: Proxy::operator=(Array<int> obj) NOT defined

				Proxy& operator=(const T& rval)
				{
					m_refData.callMeWhileWriting();
					++(m_refData.m_pWriteCount[m_index]);
					m_refData.m_pData[m_index] = rval;
					//return rval;
					//return m_refData;
					return *this;
				}

				//Read
				//operator const T&() const // const member function is not required, as no const Proxy objectes are created
				operator const T&()
				{
					m_refData.callMeWhileReading();
					++(m_refData.m_pReadCount[m_index]);
					return m_refData.m_pData[m_index];
				}

			private:
				MM_Matrix_subscript& m_refMatrix;
				const unsigned int row;
				const unsigned int column;
			};

			// This operator[] absorbs column index
			Proxy operator[](int column)
			{
				return Proxy(m_refMatrix, m_row, column);
			}

			//const int& operator=(const int& rhs); //Write only
			//operator const T&() const; //Read only

		private:
			//vector< vector<int> >& m_refToValues;
			MM_Matrix_subscript& m_refMatrix;
			unsigned int m_row;

			const MM_RowMatrix& operator=(const MM_RowMatrix& rhs) = delete;
			const MM_RowMatrix& operator=(MM_RowMatrix&& rhs) = delete;
			MM_RowMatrix* operator&() = delete;
			//This will never be called because we do not create const MM_RowMatrix
			MM_RowMatrix operator[](int index) const = delete;
		};

		
		class MM_const_RowMatrix
		{
		public:
			MM_const_RowMatrix(const MM_Matrix_subscript& refMatrix, unsigned int row)
				: m_constRefMatrix(refMatrix),
				m_row(row)
			{

			}
			//MM_const_RowMatrix(const MM_const_RowMatrix& copy);
			//MM_const_RowMatrix(MM_const_RowMatrix&& moveCopy);

			// This operator[] absorbs column index
			// This is used only for reading. It will never be used for writing
			const T& operator[](unsigned int column) const
			{
				return m_constRefMatrix.m_values[m_row * m_constRefMatrix.m_columns + column];
			}

			//operator const int&() const; //Read only

		private:
			//const vector< vector<int> >& m_constRefToValues;
			const MM_Matrix_subscript& m_constRefMatrix;
			unsigned int m_row;

			const MM_const_RowMatrix& operator=(const MM_const_RowMatrix& rhs) = delete;
			const MM_const_RowMatrix& operator=(MM_const_RowMatrix&& rhs) = delete;
			MM_const_RowMatrix* operator&() = delete;
			//This will never be called because we do not create const MM_const_RowMatrix
			MM_const_RowMatrix operator[](int index) const = delete;
			// This will never be called for writing
			const int& operator=(const int& rhs) = delete; //Write only
		};
		

		// This operator[] absorbs row index
		MM_RowMatrix operator[](const unsigned int row)
		{
			return MM_RowMatrix(*this, row);
		}

		// This can only be used for reading, so take short cuts. 
		MM_RowMatrix operator[](const unsigned int row) const
		{
			return MM_const_RowMatrix(*this, row);
		}
		// OR it can be:
		//const vector<int>& operator[](const unsigned int index) const;

	private:
		unsigned int m_rows;
		unsigned int m_columns;
		vector<T> m_values; // Use single dimention vector so that memory is allocated only once

	public:
		unsigned int readCount_;
		unsigned int writeCount_;
	};

	template<typename T>
	MM_Matrix_subscript<T>::MM_Matrix_subscript(unsigned int rows, unsigned int columns)
		: m_rows(rows),
		m_columns(columns),
		m_values(vector< vector<T>>(rows, vector<T>(columns)))
	{
	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_Matrix_subscript(const MM_Matrix_subscript& copy)
		: m_rows(copy.m_rows),
		m_columns(copy.m_columns),
		m_values(copy.m_values)
	{
	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_Matrix_subscript(MM_Matrix_subscript&& moveCopy)
		: m_rows(moveCopy.m_rows),
		m_columns(moveCopy.m_columns),
		m_values(std::move(moveCopy.m_values))
	{

	}

	/*
	// Do not allow (move/copy) asignment operators so that we can differentiate and control read v/s write
	template<typename T>
	const MM_Matrix_subscript<T>& MM_Matrix_subscript<T>::operator=(const MM_Matrix_subscript<T>& rhs)
	{
		MyAssert::myRunTimeAssert(m_rows == rhs.m_rows && m_columns == rhs.m_columns);
		m_values = rhs.m_values;

		return *this;
	}

	template<typename T>
	const MM_Matrix_subscript<T>& MM_Matrix_subscript<T>::operator=(MM_Matrix_subscript<T>&& rhs)
	{
		MyAssert::myRunTimeAssert(m_rows == rhs.m_rows && m_columns == rhs.m_columns);
		m_values = std::move(rhs.m_values);

		return *this;
	}
	*/

	template<typename T>
	const MM_Matrix_subscript<T> MM_Matrix_subscript<T>::operator*(const MM_Matrix_subscript<T>& rhs)
	{
		MyAssert::myRunTimeAssert(m_columns == rhs.m_rows);

		MM_Matrix_subscript result(m_rows, rhs.m_columns);
		for (int i = 0; i < m_rows; ++i)
		{
			for (int j = 0; j < rhs.m_columns; ++j)
			{
				int temp = 0;
				for (int k = 0; k < m_columns; ++k) //OR for (int k = 0; k < rhs.m_rows; ++k)
				{
					temp += (m_values[i][k] * rhs.m_values[k][j]);
				}
				result.m_values[i][j] = temp;
			}
		}

		return result;
	}

	template<typename T>
	void MM_Matrix_subscript<T>::operator*=(const MM_Matrix_subscript<T>& rhs)
	{
		//There is no extra benefit of defining this function.
		//Matrix multiplication is not possible in place
		*this = (*this) * rhs;
	}

	template<typename T>
	bool MM_Matrix_subscript<T>::operator==(const MM_Matrix_subscript<T>& rhs)
	{
		if (m_rows != rhs.m_rows || m_columns != rhs.m_columns)
			return false;
		for (int i = 0; i < m_rows; ++i)
			for (int j = 0; j < m_columns; ++j)
				if (m_values[i][j] != rhs.m_values[i][j])
					return false;

		return true;
	}

	/*
	template<typename T>
	MM_Matrix_subscript<T>::MM_RowMatrix MM_Matrix_subscript<T>::operator[](const unsigned int row)
	{
		return MM_Matrix_subscript<T>::MM_RowMatrix(*this, row);
	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_const_RowMatrix MM_Matrix_subscript<T>::operator[](const unsigned int row) const
	{
		return MM_Matrix_subscript<T>::MM_const_RowMatrix(*this, row);
	}
	*/
	//=================== Member Function definitions of class MM_RowMatrix ===================
	/*
	template<typename T>
	MM_Matrix_subscript<T>::MM_RowMatrix::MM_RowMatrix(vector< vector<int> >& refToValues, unsigned int row)
		: m_refToValues(refToValues),
		m_row(row)
	{

	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_RowMatrix::MM_RowMatrix(const MM_Matrix_subscript::MM_RowMatrix& copy)
		: m_refToValues(copy.m_refToValues),
		m_row(copy.m_row)
	{

	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_RowMatrix::MM_RowMatrix(MM_Matrix_subscript::MM_RowMatrix&& moveCopy)
		: m_refToValues(moveCopy.m_refToValues),
		m_row(moveCopy.m_row)
	{

	}

	// This operator[] absorbs column index
	template<typename T>
	MM_Matrix_subscript<T>::MM_RowMatrix MM_Matrix_subscript<T>::MM_RowMatrix::operator[](int index)
	{
		m_column = index;
		return *this;
	}

	template<typename T>
	const int& MM_Matrix_subscript<T>::MM_RowMatrix::operator=(const int& rhs) //Write only
	{
		m_refToValues[m_row][m_column] = rhs;
		return rhs;
	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_RowMatrix::operator const MM_Matrix_subscript<T>::T&() const//Read only
	{
		return m_refToValues[m_row][m_column];
	}
	*/
	/*
	//=================== Member Function definitions of class MM_const_RowMatrix ===================

	template<typename T>
	MM_Matrix_subscript<T>::MM_const_RowMatrix::MM_const_RowMatrix(const vector< vector<int> >& refToValues, unsigned int row)
		: m_constRefToValues(refToValues),
		m_row(row),
		m_column(0)
	{

	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_const_RowMatrix::MM_const_RowMatrix(const MM_const_RowMatrix& copy)
		: m_constRefToValues(copy.m_constRefToValues),
		m_row(copy.m_row),
		m_column(copy.m_column)
	{

	}

	template<typename T>
	MM_Matrix_subscript<T>::MM_const_RowMatrix::MM_const_RowMatrix(MM_const_RowMatrix&& moveCopy)
		: m_constRefToValues(moveCopy.m_constRefToValues),
		m_row(moveCopy.m_row),
		m_column(moveCopy.m_column)
	{

	}

	// This operator[] absorbs column index
	// This is used only for reading. It will never be used for writing
	template<typename T>
	MM_Matrix_subscript<T>::MM_const_RowMatrix MM_Matrix_subscript<T>::MM_const_RowMatrix::operator[](int index)
	{
		m_column = index;
		return *this;
	}

	template<typename T>
	const int& MM_Matrix_subscript<T>::MM_const_RowMatrix::operator const int&() const //Read only
	{
		return m_constRefToValues[m_row][m_column];
	}
	*/

	// The algorithm is same as function "double power_iterative(double base, int exponent)" in file "my_math_lib/ArithmeticOperations.cpp"
	template<typename T>
	const MM_Matrix_subscript<T> MM_Matrix_subscript<T>::raiseToPower(int exponent)
	{
		MyAssert::myRunTimeAssert(m_rows == m_columns, "This should be square matrix");

		MM_Matrix_subscript<T> result(m_rows, m_columns);
		result.makeUniqueMatrix();
		MM_Matrix_subscript<T> factor(*this);
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

	template<typename T>
	void MM_Matrix_subscript<T>::makeUniqueMatrix()
	{
		MyAssert::myRunTimeAssert(m_rows == m_columns, "This should be square matrix");

		for (int i = 0; i < m_rows; ++i)
		{
			for (int j = 0; j < m_columns; ++j)
			{
				if (i == j)
					m_values[i][j] = 1.0;
				else
					m_values[i][j] = 0.0;
			}
		}
	}
}