//Goal
//Explore function overloading rules

#include "C++/C++_FunctionOverloading.h"

namespace mm {

	/*
	Summary:
	Function overloading based on:
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Sr.	| OK?	| Overloading based on								| Examples
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	1	| Yes	| signed & unsigned									| fun(int a)
																	  fun(unsigned int a)
	1	| No	| value & reference									| fun(int a)
																	  fun(int& a)
	2	| No	| const object & non-const object					| fun(const int a) 
																	  fun(int a)
	3	| No	| const pointer & non-const pointer					| fun(int* const a) 
																	  fun(int* a)
	4	| Yes	| value & pointer									| fun(int a) 
																	  fun(int* a)
	5	| Yes	| pointer to const & pointer to non-const			| fun(const int* a) 
																	  fun(int* a)
	6	| Yes	| const reference & non-const reference				| fun(const int& a) 
																	  fun(int& a)
	7	| Yes	| const member function & non-const member function	| A::fun() const 
																	  A::fun()     // this pointer is implicitly passed to member function as A const* this
		 															  A::fun(const A const* this) const 
																	  A::fun(A const* this)
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	*/

	//No	| signed & unsigned	
	void fun_0(int value) {}
	void fun_0(unsigned int value) {}
	void test_0()
	{
		int a = 10;
		unsigned int b = a;
		fun_0(a);
		fun_0(b);
	}

	//No	| value & reference	
	void fun_1(int value) {}
	void fun_1(int& value) {}
	void test_1()
	{
		int a = 10;
		int& b = a;
		//fun_1(a); // error C2668: 'fun_1': ambiguous call to overloaded function OR //More than one instances of overloaded function matches the argument list
		//fun_1(b); // Same as above
	}

	//No	| const object & non-const object
	void fun_2(int value) {}
	//void fun_2(const int value) {} // error C2084: function 'void fun_2(int)' already has a body
	void test_2()
	{
		int a = 10;
		const int b = 20;
		fun_2(a);
		fun_2(b);
	}

	//No	| const pointer & non-const pointer
	void fun_3(int* value) {}
	//void fun_3(int* const value) {} //error C2084: function 'void fun_5(int *)' already has a body
	void test_3()
	{
		int a = 10;
		int* pa = &a;
		int* const cpa = &a;
		fun_3(pa);
		fun_3(cpa);
	}

	//Yes: value & pointer
	void fun_4(int value) {}
	void fun_4(int* value) {}
	void test_4()
	{
		int a = 10;
		fun_4(a);
		fun_4(&a);
	}

	//Yes	| pointer to const object & pointer to non-const object
	void fun_5(int* value) {}
	void fun_5(const int* value) {}
	void test_5()
	{
		int a = 10;
		const int b = 10;
		int* pa = &a;
		const int* pcb = &b;
		fun_5(pa);
		fun_5(pcb);
	}

	//Yes	| reference to const object & reference to non-const object
	void fun_6(int& value) {}
	void fun_6(const int& value) {}
	void test_6()
	{
		int a = 10;
		const int b = 10;
		int& ra = a;
		const int& rcb = b;
		fun_6(ra);
		fun_6(rcb);
	}

	//Yes	| const member function & non-const member function
	class A
	{
	public:
		void fun_6() {}
		void fun_6() const {}
	};

	void test_7()
	{
		A a;
		const A ca;
		a.fun_6(); // this pointer is implicitly passed to this function as A* const this
		ca.fun_6(); // this pointer is implicitly passed to this function as const A const* this
	}

	//fun_5() above i.e. the function overloading based on const pointer and non-const pointer is same as overloading 
	// member function of any class with const


	//TODO:

	//based on l-value reference and r-value reference

	//based on base and derived class objects

}

