//Goal
// Implement subscript oprators for a vector/array to separate read write operations

namespace mm {

	template<typename T>
	class Array
	{
	public:
		//forward declaration
		class Proxy;

		Array(unsigned const int size = 0);
		Array(const std::initializer_list<T>& list);
		~Array();

		//Usual way of implementing subscript operators
		//T& operator[](const unsigned int index)
		//{
		//	return m_pData[index];
		//}
		//const T& operator[](const unsigned int index) const
		//{
		//	return m_pData[index];
		//}

		//Read or Write (non-const member function)
		Proxy operator[](const unsigned int index)
		{
			return Proxy(*this, index); //Need copy constructor in Proxy
		}

		//Read (const member function)
		const T& operator[](const unsigned int index) const
		{
			callMeWhileReading();
			return m_pData[index];
		}
		//Do not use below approach to return Proxy from const member function, Proxy can not keep const reference to this class
		//const Proxy operator[](const unsigned int index) const
		//{
		//	return Proxy(*this, index);
		//}

		class Proxy
		{
		public:
			Proxy(Array& refObject, const unsigned int index)
				:m_refData(refObject),
				m_index(index)
			{
			}

			Proxy(const Proxy& rhs)
				: m_refData(rhs.m_refData),
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
			Array<T>& m_refData;
			const unsigned int m_index;
		};

	private:
		unsigned const int m_size;
		T* m_pData;
		mutable unsigned int* m_pReadCount; // Stores the count of how many times each object in array is read
		mutable unsigned int* m_pWriteCount; // Stores the count of how many times each object in array is written

		void callMeWhileReading() const
		{
			//cout << "\n Log: debug -> Somebody is reading the value";
		}

		//void callMeWhileWriting() const // No need to make it const member function, const objects can not be called for writing
		void callMeWhileWriting()
		{
			//cout << "\n Log: debug -> Somebody is writing the value";
		}

	public:
		unsigned int getReadCount(const unsigned int index) const
		{
			return m_pReadCount[index];
		}
		void setReadCount(const unsigned int index, const unsigned int value) const
		{
			m_pReadCount[index] = value;
		}

		unsigned int getWriteCount(const unsigned int index) const
		{
			return m_pWriteCount[index];
		}
		void setWriteCount(const unsigned int index, const unsigned int value) const
		{
			m_pWriteCount[index] = value;
		}
	};

	template<typename T>
	Array<T>::Array(unsigned const int size /*= 0*/)
		:m_size(size),
		m_pData(size > 0 ? new T[m_size] : nullptr),
		m_pReadCount(size > 0 ? new unsigned int[m_size] : nullptr),
		m_pWriteCount(size > 0 ? new unsigned int[m_size] : nullptr)
	{
		// Initialize read and write counts with zero
		for (size_t i = 0; i < size; ++i)
		{
			m_pReadCount[i] = 0;
			m_pWriteCount[i] = 0;
		}
	}

	template<typename T>
	Array<T>::Array(const std::initializer_list<T>& list)
		:m_size(static_cast<unsigned int>(list.size())),
		m_pData(m_size > 0 ? new T[m_size] : nullptr),
		m_pReadCount(m_size > 0 ? new unsigned int[m_size] : nullptr),
		m_pWriteCount(m_size > 0 ? new unsigned int[m_size] : nullptr)
	{
		int i = 0;
		for (const T& var : list)
		{
			m_pData[i++] = var;
		}

		// Initialize read and write counts with zero
		for (size_t i = 0; i < m_size; ++i)
		{
			m_pReadCount[i] = 0;
			m_pWriteCount[i] = 0;
		}
	}

	template<typename T>
	Array<T>::~Array()
	{
		delete[] m_pData;
		delete[] m_pReadCount;
		delete[] m_pWriteCount;
	}



}