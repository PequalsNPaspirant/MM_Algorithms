#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <type_traits>
using namespace std;

#include "STL/STL_unique_ptr.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//========= vector of unique_ptr<T>

	//MM_DECLARE_FLAG(STL_VectorUniquePtrToInt);

	MM_UNIT_TEST(STL_VectorUniquePtrToInt0_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{
		vector<unique_ptr<int>> vec1;
		vec1.push_back(unique_ptr<int>(new int(10)));
		vec1.push_back(make_unique<int>(20));

		//unique_ptr<int> upi = vec1[0]; // Error: Trying to call deleted copy cnstructor i.e. unique_ptr<int>::unique_ptr<int>(const unique_ptr<int>&)
		unique_ptr<int>& rupi = vec1[0];
		const unique_ptr<int>& crupi = vec1[0];

		int i1 = *vec1[0];
		int& ri1 = *vec1[0];
		const int& cri1 = *vec1[0];

		int i2 = *vec1[0].get();
		int& ri2 = *vec1[0].get();
		const int& cri2 = *vec1[0].get();

		int* pi = vec1[0].get();
		//int*& rpi = vec1[0].get(); // Error: Initial value of reference to non-const must be an lvalue
		//const int*& crpi = vec1[0].get(); // Error: Initial value of reference to non-const must be an lvalue

		MM_EXPECT_TRUE(i1 == 10 && ri1 == 10 && cri1 == 10
					&& i2 == 10 && ri2 == 10 && cri2 == 10
					&& *pi == 10);
	}

	//========= Method 1: initialization of vector of unique_ptr<T> using initialization_list<T>

	MM_UNIT_TEST(STL_VectorUniquePtrToInt1_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{
		//vector<unique_ptr<int>> vec2({make_unique<int>(10), make_unique<int>(20)});
		//vector<unique_ptr<int>> vec2 = {unique_ptr<int>(new int(10)), unique_ptr<int>(new int(20))}; 
		// Error: std::initializer list does not have move constructor. The behaviour of std::initializer_list is to hold objects by value and not allow moving out, so this is not possible.

		//std::initializer_list<unique_ptr<int>> iList = {unique_ptr<int>(new int(10)), unique_ptr<int>(new int(20))};
		//vector<unique_ptr<int>> vec3 = {std::make_move_iterator(std::begin(iList)), std::make_move_iterator(std::end(iList))}; // Error: std::initializer list does not have move constructor

		// Reference: https://stackoverflow.com/questions/8468774/can-i-list-initialize-a-vector-of-move-only-type/8469002#8469002
		unique_ptr<int> upList[] = {unique_ptr<int>(new int(10)), unique_ptr<int>(new int(20))};
		vector<unique_ptr<int>> vec4 = {std::make_move_iterator(std::begin(upList)), std::make_move_iterator(std::end(upList))}; // Works: The iterators returned by std::make_move_iterator will move the pointed-to element when being dereferenced.

		MM_EXPECT_TRUE(*vec4[0] == 10 && *vec4[1] == 20);
	}

	//========= Method 2: initialization of vector of unique_ptr<T> using initialization_list<T>

	// Reference: https://stackoverflow.com/questions/8468774/can-i-list-initialize-a-vector-of-move-only-type/8469002#8469002
	// The approach in above link does not work. Modified it to make it work as below.
	// rvalue analogue of std::reference_wrapper and std::ref
	// Hope to see std::rref_wrapper and std::rref in next standard :)
	template<class T>
	class rref_wrapper
	{
	public:
		// CAUTION - very volatile, use with care
		explicit rref_wrapper(T&& v)
			: _val(std::move(v)) 
		{
		}

		rref_wrapper(const T& v) = delete;

		explicit operator T() const
		{
			return const_cast<T&&>(_val);
		}

		private:
			T _val;
	};

	// only usable on temporaries
	template<class T>
	typename std::enable_if<!std::is_lvalue_reference<T>::value, rref_wrapper<T>>::type rref(T&& v)
	{
	  return rref_wrapper<T>(std::move(v));
	}

	// lvalue reference can go away
	template<class T>
	void rref(T&) = delete;


	MM_UNIT_TEST(STL_VectorUniquePtrToInt2_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{
		using move_only = std::unique_ptr<int>;
		//std::vector<move_only> v = { rref(move_only(new int(10))), rref(move_only(new int(20))), rref(move_only(new int(30))) };
		/*
		Since the standard, for whatever reason, doesn't define a converting copy constructor in class initializer_list like this:
		template<class V>
		initializer_list<U>::initializer_list(const initializer_list<V>& other);
		NEITHER vector defines:
		template<class U>
		vector<U>::vector(const std::initializer_list<V>& other)
		*/

		std::initializer_list<rref_wrapper<move_only>> il{ rref(make_unique<int>(10)), rref(make_unique<int>(20)), rref(make_unique<int>(30)) };
		std::vector<move_only> v(il.begin(), il.end());
		MM_EXPECT_TRUE(*v[0] == 10 && *v[1] == 20 && *v[2] == 30);
	}

	//========= Method 3: initialization of vector of unique_ptr<T> using initialization_list<T>

	/*
	template<typename T>
	vector<T> initialize_vector(initializer_list<T>&& list)
	{
		vector<T> retVal;
		//for (T t : list)
		for(auto i = list.begin(); i != list.end(); ++i) // initializer_list gives const_iterator
		{
			retVal.push_back(std::move(*std::make_move_iterator(i))); // attempting to reference deleted function
		}

		return retVal;
	}

	MM_UNIT_TEST(STL_VectorUniquePtrToInt3_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{
		std::vector<unique_ptr<int>> v = initialize_vector<unique_ptr<int>>({make_unique<int>(10), make_unique<int>(20), make_unique<int>(30)});
		MM_EXPECT_TRUE(*v[0] == 10 && *v[1] == 20 && *v[2] == 30);
	}
	*/


	//========= Method 4: initialization of vector of unique_ptr<T> using initialization_list<T>

	//template<typename... ZeroOrManyTypes>
	//void initialize_vector(vector<ZeroOrManyTypes...>& vec, ZeroOrManyTypes&&... ZeroOrManyObjs)
	//{
	//}

	template<typename ExactlyOneType>
	void initialize_vector(vector<ExactlyOneType>& vec, ExactlyOneType&& obj)
	{
		vec.push_back(std::move(obj));
	}

	template<typename ExactlyOneType>
	void initialize_vector(vector<ExactlyOneType>& vec, const ExactlyOneType& obj)
	{
		vec.push_back(obj); // Not movable objects (like std::unique_ptr) would throw error here
	}

	template<typename FirstType, typename... RestTypes>
	void initialize_vector(vector<FirstType>& vec, FirstType&& obj, RestTypes&&... remainingObjs)
	{
		vec.push_back(std::forward<FirstType>(obj));
		initialize_vector(vec, std::forward<RestTypes>(remainingObjs)...);
	}

	MM_UNIT_TEST(STL_VectorUniquePtrToInt4_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{
		std::vector<unique_ptr<int>> v;
		//unique_ptr<int> notMovable(new int(40));
		//initialize_vector(v, make_unique<int>(10), make_unique<int>(20), make_unique<int>(30), notMovable);
		//MM_EXPECT_TRUE(*v[0] == 10 && *v[1] == 20 && *v[2] == 30 && *v[3] == 40);
		initialize_vector(v, make_unique<int>(10), make_unique<int>(20), make_unique<int>(30));
		MM_EXPECT_TRUE(*v[0] == 10 && *v[1] == 20 && *v[2] == 30);
	}

	//========= construction of unique_ptr<T> for class type

	class MyType
	{
	public:
		explicit MyType(int nIn)
			: n(nIn)
		{}
		~MyType() = default;

		MyType(const MyType&) = delete;
		MyType(MyType&&) = delete;
		MyType& operator=(const MyType&) = delete;
		MyType& operator=(MyType&&) = delete;

		int n;
	};

	MM_UNIT_TEST(STL_VectorUniquePtrToInt5_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{
		vector<unique_ptr<MyType>> vec1;
		vec1.push_back(make_unique<MyType>(10));
		vec1.push_back(make_unique<MyType>(20));
		vec1.push_back(make_unique<MyType>(30));

		MM_EXPECT_TRUE(vec1[0]->n == 10 && vec1[1]->n == 20 && vec1[2]->n == 30);
	}


	//========= use of unique_ptr<T>::

	MM_UNIT_TEST(STL_UniquePtrOperations1_UnitTest_sanity, STL_VectorUniquePtrToInt)
	{

		MM_EXPECT_TRUE(true);
	}

}