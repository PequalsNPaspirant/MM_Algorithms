//Goal
//Test RvalueReferenceAndMove features in C++11

#include <iostream>
#include <string>
using namespace std;

namespace mm {

	class MyClass
	{
	public:
		MyClass(int a, int b)
			:x(a), y(b)
		{
			cout << "\nMyClass::MyClass()";
		}

		int x, y;
	};


	class Container
	{
	public:
		Container(int a, int b)
			:m_e(a, b)
		{
			cout << "\nContainer::Container()";
		}

		MyClass m_e;
	};

	void testBasicsOfMove()
	{
		cout << "\n\ntestBasicsOfMove";
		{
			string str1("Prashant");
			cout << "\n Address of str1: " << &str1 << " Value of str1: " << str1;
			string str2(std::move(str1));
			cout << "\n Address of str2: " << &str2 << " Value of str2: " << str2;
			cout << "\n Address of str1: " << &str1 << " Value of str1: " << str1;
			//str1[5] = 'X'; //Runtime crash at this line as str1 does not have any memory allocated. Its length is zero.
		}
		{
			string str1("Prashant");
			cout << "\n Address of str1: " << &str1 << " Value of str1: " << str1;
			string str2 = static_cast<string&&>(str1); //Same effect as std::move
			cout << "\n Address of str2: " << &str2 << " Value of str2: " << str2;
			cout << "\n Address of str1: " << &str1 << " Value of str1: " << str1;
			//str1[5] = 'X'; //Runtime crash at this line as str1 does not have any memory allocated. Its length is zero.
		}
		/*Ouput:
		testBasicsOfMove
		 Address of str1: 00AFF9F8 Value of str1: Prashant
		 Address of str2: 00AFF9D4 Value of str2: Prashant
		 Address of str1: 00AFF9F8 Value of str1:
		 Address of str1: 00AFF9B0 Value of str1: Prashant
		 Address of str2: 00AFF98C Value of str2: Prashant
		 Address of str1: 00AFF9B0 Value of str1:
		*/
		{
			cout << "\n\nCheck address of variable before and after move";
			int x = 10;
			cout << "\n Address of x: " << &x << " Value of x: " << x;
			int y = std::move(x);
			cout << "\n Address of y: " << &y << " Value of y: " << y;
			cout << "\n Address of x: " << &x << " Value of x: " << x;

			/*Ouput:
			Check address of variable before and after move
			Address of x : 00CFFA8C Value of x : 10
			Address of y : 00CFFA80 Value of y : 10
			Address of x : 00CFFA8C Value of x : 10
			*/
		}
		{
			cout << "\n\nCheck what happens with data before and after move";
			MyClass obj1(5, 10);
			cout << "\n Address of obj1: " << &obj1 << " Value of obj1: " << obj1.x << " " << obj1.y;
			MyClass obj2 = static_cast<MyClass&&>(obj1); //Same effect as std::move
			cout << "\n Address of obj2: " << &obj2 << " Value of obj2: " << obj2.x << " " << obj2.y;
			cout << "\n Address of obj1: " << &obj1 << " Value of obj1: " << obj1.x << " " << obj1.y;

			/*
			Check what happens with data before and after move
			MyClass::MyClass()
			 Address of obj1: 00AFF964 Value of obj1: 5 10
			 Address of obj2: 00AFF954 Value of obj2: 5 10
			 Address of obj1: 00AFF964 Value of obj1: 5 10
			*/
		}

		//Ensure, move does not call constructor
		{
			cout << "\n\nCheck if std::move calls constructor";
			Container obj1(7, 9);
			cout << "\n Address of obj1: " << &obj1 << " Value of obj1: " << obj1.m_e.x << " " << obj1.m_e.y;
			Container obj2 = static_cast<Container&&>(obj1); //Same effect as std::move
			cout << "\n Address of obj2: " << &obj2 << " Value of obj2: " << obj2.m_e.x << " " << obj2.m_e.y;
			cout << "\n Address of obj1: " << &obj1 << " Value of obj1: " << obj1.m_e.x << " " << obj1.m_e.y;

			/*
			Check if std::move calls constructor
			MyClass::MyClass()
			Container::Container()
			 Address of obj1: 00AFF944 Value of obj1: 7 9
			 Address of obj2: 00AFF934 Value of obj2: 7 9
			 Address of obj1: 00AFF944 Value of obj1: 7 9
			*/
		}

	}

	//TODO:
	class Base
	{

	};

	class Derived : public Base
	{

	};

	void testDerivedClassConstructor()
	{

	}

	void RvalueReferenceAndMove()
	{
		testBasicsOfMove();
	}

}