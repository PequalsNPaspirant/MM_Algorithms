//Goal
//To check behavior of std:Smart_Pointers

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

#include "STL/STL_shared_ptr.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

/*

References:
https://stackoverflow.com/questions/29972563/how-to-construct-a-vector-with-unique-pointers
https://stackoverflow.com/questions/2802953/how-do-shared-pointers-work
https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful
https://stackoverflow.com/questions/27085782/how-to-break-shared-ptr-cyclic-reference-using-weak-ptr
https://stackoverflow.com/questions/27377868/make-shared-own-implementation
http://www.boost.org/doc/libs/1_57_0/libs/smart_ptr/shared_ptr.htm
https://eli.thegreenplace.net/2012/06/20/c11-using-unique_ptr-with-standard-library-containers/
https://eli.thegreenplace.net/2012/06/20/c11-using-unique_ptr-with-standard-library-containers/
https://stackoverflow.com/questions/21377360/proper-way-to-create-unique-ptr-that-holds-an-allocated-array/21377424
https://stackoverflow.com/questions/13061979/shared-ptr-to-an-array-should-it-be-used
https://stackoverflow.com/questions/40447401/why-allow-shared-ptrtn

Smart pointer links on Herb Suter blog page:
https://herbsutter.com/gotw/

*/


namespace mm {

	//========= shared_ptr to Base class object pointing to shared_ptr to Derived class object

	class Base
	{
	public:
		Base()
		{
			++Base::numConstructionCalls;
		}
		~Base() // Destructor is not virtual
		{
			++Base::numDestructionCalls;
		}

		static int numConstructionCalls;
		static int numDestructionCalls;
	};

	class Derived : public Base
	{
	public:
		Derived()
		{
			++Base::numConstructionCalls;
		}
		~Derived() // Destructor is not virtual
		{
			++Base::numDestructionCalls;
		}
	};

	int Base::numConstructionCalls = 0;
	int Base::numDestructionCalls = 0;

	MM_DECLARE_FLAG(STL_SharedPtrToBaseDerived);

	MM_UNIT_TEST(STL_SharedPtrToBaseDerived1_UnitTest_sanity, STL_SharedPtrToBaseDerived)
	{
		{
			std::shared_ptr<Base> pb = shared_ptr<Derived>(new Derived());
		}

		MM_EXPECT_TRUE(Base::numConstructionCalls == 2 && Base::numDestructionCalls == 2);
	}

	//========= shared_ptr assignment between two irrelevant classes

	class MyType2;
	class MyType1
	{
	public:
		int x;

		MyType1& operator=(const MyType2& rhs);
		void swap(const MyType2& rhs);
	};

	class MyType2
	{
	public:
		int y;

		MyType2& operator=(const MyType1& rhs);
		void swap(const MyType1& rhs);
	};

	MyType1& MyType1::operator=(const MyType2& rhs)
	{
		x = rhs.y;
		return *this;
	}

	void MyType1::swap(const MyType2& rhs)
	{

	}

	MyType2& MyType2::operator=(const MyType1& rhs)
	{
		y = rhs.x;
		return *this;
	}

	void MyType2::swap(const MyType1& rhs)
	{

	}


	MM_UNIT_TEST(STL_SharedPtrToBaseDerived2_UnitTest_sanity, STL_SharedPtrToBaseDerived)
	{
		MyType1 m1;
		MyType2 m2;
		m2 = m1;

		shared_ptr<MyType1> pm1 = make_shared<MyType1>();
		//std::shared_ptr<MyType2> pm2 = pm1; // error C2440: 'initializing': cannot convert from 'std::shared_ptr<mm::MyType1>' to 'std::shared_ptr<mm::MyType2>'
		// The following shared_ptr assignment works only when U* can be implicitly converted into T*
		// template<typename U>
		// shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<U>& rhs)

		MM_EXPECT_TRUE(true);
	}


	//========= use of shared_ptr<T>::


}