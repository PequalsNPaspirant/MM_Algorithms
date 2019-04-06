#include <iostream>
#include <memory> // and others
using namespace std;

#include "STL/STL_shared_ptr_cyclic_ref.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace MemoryLeak
	{
		class B; // forward declaration 
				 // for clarity, add destructor to see that they are not called
		class A
		{
		public:
			A(int& a, int& b)
				: numCtorCallsA(a), numDtorCallsA(b)
			{
				++numCtorCallsA;
			}
			~A()
			{
				++numDtorCallsA;
			}
			//private:
			shared_ptr<B> b;
			int& numCtorCallsA;
			int& numDtorCallsA;
		};

		class B
		{
		public:
			B(int& a, int& b)
				: numCtorCallsB(a), numDtorCallsB(b)
			{
				++numCtorCallsB;
			}
			~B()
			{
				++numDtorCallsB;
			}
			//private:
			shared_ptr<A> a;
			int& numCtorCallsB;
			int& numDtorCallsB;
		};
	}

	MM_DECLARE_FLAG(SharedPtrCyclicReferenceMemoryLeak);

	MM_UNIT_TEST(testSharedPtrCyclicReferenceMemoryLeak_v1_UnitTest_sanity, SharedPtrCyclicReferenceMemoryLeak)
	{
		using namespace MemoryLeak;

		for (int testCaseNum = 0; testCaseNum < 2; ++testCaseNum)
		{
			int numCtorCallsA = 0;
			int numDtorCallsA = 0;
			int numCtorCallsB = 0;
			int numDtorCallsB = 0;

			weak_ptr<A> wA;
			weak_ptr<B> wB;
			bool isAExpired = false;
			int useCountA = -1;
			bool isBExpired = false;
			int useCountB = -1;

			switch (testCaseNum)
			{
			case 0:
			{
				//Memory leak
				shared_ptr<A> x(new A(numCtorCallsA, numDtorCallsA));  //x->b share_ptr is default initialized. x.use_count() == 1
				x->b = make_shared<B>(numCtorCallsB, numDtorCallsB); // you can't do "= new B" on shared_ptr  x->b.use_count() == 1                   
				x->b->a = x; // x.use_count() == 2
				wA = x;
				wB = x->b;
				useCountA = x.use_count();
				useCountB = x->b.use_count();
				MM_EXPECT_TRUE(useCountA == 2 && useCountB == 1, useCountA, useCountB);
			}
			break;
			case 1:
			{
				//Memory leak
				shared_ptr<A> x(new A(numCtorCallsA, numDtorCallsA));  //x->b share_ptr is default initialized
				shared_ptr<B> y(new B(numCtorCallsB, numDtorCallsB));
				x->b = y;
				y->a = x;

				wA = x;
				wB = y;
				useCountA = x.use_count();
				useCountB = x->b.use_count();
				MM_EXPECT_TRUE(useCountA == 2 && useCountB == 2, useCountA, useCountB);
			}
			break;
			}

			isAExpired = wA.expired();
			useCountA = wA.use_count();
			isBExpired = wB.expired();
			useCountB = wB.use_count();
			MM_EXPECT_TRUE(isAExpired == false && useCountA == 1 && isBExpired == false && useCountB == 1,
				isAExpired, useCountA, isBExpired, useCountB);

			MM_EXPECT_TRUE(numCtorCallsA == 1 && numDtorCallsA == 0 && numCtorCallsB == 1 && numDtorCallsB == 0,
				numCtorCallsA, numDtorCallsA, numCtorCallsB, numDtorCallsB);
		}
	}

	namespace NoMemoryLeak
	{
		class B; // forward declaration 
					// for clarity, add destructor to see that they are not called
		class A
		{
		public:
			A(int& a, int& b)
				: numCtorCallsA(a), numDtorCallsA(b)
			{
				++numCtorCallsA;
			}
			~A()
			{
				++numDtorCallsA;
			}
			//private:
			shared_ptr<B> b;
			int& numCtorCallsA;
			int& numDtorCallsA;
		};

		class B
		{
		public:
			B(int& a, int& b)
				: numCtorCallsB(a), numDtorCallsB(b)
			{
				++numCtorCallsB;
			}
			~B()
			{
				++numDtorCallsB;
			}
			//private:
			weak_ptr<A> a;
			int& numCtorCallsB;
			int& numDtorCallsB;
		};
	}

	MM_DECLARE_FLAG(SharedPtrCyclicReferenceNoMemoryLeak);

	MM_UNIT_TEST(testSharedPtrCyclicReferenceNoMemoryLeak_v2_UnitTest_sanity, SharedPtrCyclicReferenceNoMemoryLeak)
	{
		using namespace NoMemoryLeak;

		for (int testCaseNum = 0; testCaseNum < 2; ++testCaseNum)
		{
			int numCtorCallsA = 0;
			int numDtorCallsA = 0;
			int numCtorCallsB = 0;
			int numDtorCallsB = 0;

			weak_ptr<A> wA;
			weak_ptr<B> wB;
			bool isAExpired = false;
			int useCountA = -1;
			bool isBExpired = false;
			int useCountB = -1;

			switch (testCaseNum)
			{
			case 0:
			{
				//No Memory leak
				shared_ptr<A> x(new A(numCtorCallsA, numDtorCallsA));  //x->b share_ptr is default initialized. x.use_count() == 1
				x->b = make_shared<B>(numCtorCallsB, numDtorCallsB); // you can't do "= new B" on shared_ptr  x->b.use_count() == 1                   
				x->b->a = x; // x.use_count() == 1
				wA = x;
				wB = x->b;
				useCountA = x.use_count();
				useCountB = x->b.use_count();
				MM_EXPECT_TRUE(useCountA == 1 && useCountB == 1, useCountA, useCountB);
			}
			break;
			case 1:
			{
				//No Memory leak
				shared_ptr<A> x(new A(numCtorCallsA, numDtorCallsA));  //x->b share_ptr is default initialized
				shared_ptr<B> y(new B(numCtorCallsB, numDtorCallsB));
				x->b = y;
				y->a = x;

				wA = x;
				wB = y;
				useCountA = x.use_count();
				useCountB = x->b.use_count();
				MM_EXPECT_TRUE(useCountA == 1 && useCountB == 2, useCountA, useCountB);
			}
			break;
			}

			isAExpired = wA.expired();
			useCountA = wA.use_count();
			isBExpired = wB.expired();
			useCountB = wB.use_count();
			MM_EXPECT_TRUE(isAExpired == true && useCountA == 0 && isBExpired == true && useCountB == 0,
				isAExpired, useCountA, isBExpired, useCountB);

			MM_EXPECT_TRUE(numCtorCallsA == 1 && numDtorCallsA == 1 && numCtorCallsB == 1 && numDtorCallsB == 1,
				numCtorCallsA, numDtorCallsA, numCtorCallsB, numDtorCallsB);
		}
	}

}