#include <iostream>
#include <algorithm> //for std::swap
#include <cassert> //for assert()
using namespace std;

#include "DataStructures/DS_MM_shared_ptr.h"
#include "Timer/Timer_Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {
	
	template<typename T>
	class shared_ptr
	{
	public:
		typedef T element_type;
		typedef T* pointer;

		shared_ptr()
			: data_(nullptr),
			ctrlBlock_(nullptr)
		{
		}

		explicit shared_ptr(pointer p)
			: data_(p)			
		{
			initializeCtrlBlock(p);
		}

		shared_ptr(const shared_ptr<T>& other)
			: data_(other.data_),
			ctrlBlock_(other.ctrlBlock_)
		{
			if (ctrlBlock_)
				ctrlBlock_->addRef();
		}

		shared_ptr(shared_ptr<T>&& other)
			: data_(other.data_),
			ctrlBlock_(other.ctrlBlock_)
		{
			other.data_ = nullptr;
			other.ctrlBlock_ = nullptr;
		}

		shared_ptr& operator=(pointer p)
		{
			//release();
			//data_ = p;
			//initializeCtrlBlock(p);
			shared_ptr<T> copy(p);
			this->swap(copy);

			return *this;
		}

		shared_ptr& operator=(const shared_ptr<T>& other)
		{
			if (this != &other)
			{
				shared_ptr<T> copy(other);
				this->swap(copy);
			}

			return *this;
		}

		shared_ptr& operator=(shared_ptr<T>&& other)
		{
			if (this != &other)
			{
				shared_ptr<T> copy(other);
				this->swap(copy);
			}

			return *this;
		}

		void reset(pointer p = nullptr)
		{
			shared_ptr<T> newObj(p);
			this->swap(newObj);
		}

		~shared_ptr()
		{
			release();
		}

		pointer get() noexcept
		{
			return data_;
		}

		element_type& operator*() noexcept
		{
			return *data_;
		}

		pointer operator->() noexcept
		{
			return data_;
		}

		operator bool() const noexcept
		{
			return (nullptr != data_);
		}

		void swap(shared_ptr<T>& other) noexcept //never throws
		{
			std::swap(data_, other.data_);
			std::swap(ctrlBlock_, other.ctrlBlock_);
		}

		int use_count() const noexcept
		{
			if (ctrlBlock_)
				return ctrlBlock_->use_count();

			return 0;
		}

	private:
		pointer data_;
		
		class control_block
		{
		public:
			control_block()
				: use_count_(0)
			{}

			void addRef()
			{
				++use_count_;
			}

			void release()
			{
				--use_count_;
			}

			int use_count()
			{
				return use_count_;
			}

		private:
			int use_count_;
		};

		control_block* ctrlBlock_;

	private:
		void initializeCtrlBlock(pointer p)
		{
			if (!p)
			{
				ctrlBlock_ = nullptr;
				return;
			}

			try 
			{
				ctrlBlock_ = new control_block{};
			}
			catch (std::bad_alloc& e)
			{
				delete p;
				std::cerr << "ERROR: Unable to allocate enough memory: " << e.what();
				throw e;
			}
			ctrlBlock_->addRef();
		}

		void release()
		{
			if (!ctrlBlock_)
				return;

			ctrlBlock_->release();
			if (ctrlBlock_->use_count() == 0)
			{
				delete data_;
				data_ = nullptr;
				delete ctrlBlock_;
				ctrlBlock_ = nullptr;
			}
		}
	};

	//singleton
	class Counter
	{
	public:
		static Counter& getCounter()
		{
			static Counter counter;
			return counter;
		}

		~Counter()
		{}

		void reset()
		{
			constructorCount_ = 0;
			destructorCount_ = 0;
		}

		void increaseConstructorCount()
		{
			++constructorCount_;
		}

		void increaseDestructorCount()
		{
			++destructorCount_;
		}

		bool verify()
		{
			return constructorCount_ == destructorCount_;
		}

		Counter(const Counter&) = delete;
		Counter(Counter&&) = delete;
		Counter& operator=(const Counter&) = delete;
		Counter& operator=(Counter&&) = delete;

	private:
		Counter()
			: constructorCount_(0),
			destructorCount_(0)
		{
		}
		int constructorCount_;
		int destructorCount_;
	};

	class TestClass
	{
	public:
		TestClass()
		{
			Counter::getCounter().increaseConstructorCount();
		}

		~TestClass()
		{
			Counter::getCounter().increaseDestructorCount();
		}

		TestClass* getAddress()
		{
			return this;
		}
	};

	void test_shared_ptr_default_constructor()
	{
		Counter::getCounter().reset();

		{
			shared_ptr<TestClass> p1;
			assert(p1.use_count() == 0);

			const shared_ptr<TestClass> p2;
			assert(p2.use_count() == 0);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_constructor()
	{
		Counter::getCounter().reset();

		{
			TestClass *p = nullptr;
			shared_ptr<TestClass> p1(p);
			assert(p1.use_count() == 0);

			p = new TestClass{};
			const shared_ptr<TestClass> p2(p);
			assert(p2.use_count() == 1);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_copy_constructor()
	{
		Counter::getCounter().reset();

		{
			TestClass *p = new TestClass{};
			shared_ptr<TestClass> p1(p);
			assert(p1.use_count() == 1);

			const shared_ptr<TestClass> p2(p1);
			assert(p1.use_count() == 2);
			assert(p2.use_count() == 2);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_move_constructor()
	{
		Counter::getCounter().reset();

		{
			TestClass *p = new TestClass{};
			shared_ptr<TestClass> p1(p);
			assert(p1.use_count() == 1);

			const shared_ptr<TestClass> p2(std::move(p1));
			assert(p1.use_count() == 0);
			assert(p2.use_count() == 1);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_assignement_raw_ptr()
	{
		Counter::getCounter().reset();

		{
			TestClass *p = new TestClass{};
			shared_ptr<TestClass> p1(p);
			assert(p1.use_count() == 1);

			p1 = new TestClass;
			assert(p1.use_count() == 1);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_copy_assignment()
	{
		Counter::getCounter().reset();

		{
			shared_ptr<TestClass> p1(new TestClass{});
			assert(p1.use_count() == 1);

			const shared_ptr<TestClass> p2(p1);
			assert(p1.use_count() == 2);
			assert(p2.use_count() == 2);

			p1 = p2;

			assert(p1.use_count() == 2);
			assert(p2.use_count() == 2);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_move_assignment()
	{
		Counter::getCounter().reset();

		{
			shared_ptr<TestClass> p1(new TestClass{});
			assert(p1.use_count() == 1);

			shared_ptr<TestClass> p2(std::move(p1));
			assert(p1.use_count() == 0);
			assert(p2.use_count() == 1);

			p2 = std::move(p2);

			assert(p2.use_count() == 1);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_reset()
	{
		Counter::getCounter().reset();

		{
			shared_ptr<TestClass> p1(new TestClass{});
			assert(p1.use_count() == 1);
			p1.reset();
			assert(p1.use_count() == 0);

			shared_ptr<TestClass> p2(new TestClass{});
			shared_ptr<TestClass> p3(p2);
			assert(p2.use_count() == 2);
			assert(p3.use_count() == 2);

			p2.reset(new TestClass{});

			assert(p2.use_count() == 1);
			assert(p3.use_count() == 1);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr_miscellaneous()
	{
		Counter::getCounter().reset();

		{
			TestClass* p = new TestClass{};
			shared_ptr<TestClass> p1(p);
			assert(p == p1.get());
			
			TestClass& obj = *p1;
			assert(p == &obj);

			assert(p == p1->getAddress());

			assert(p1 == true);
			p1.reset();
			assert(p1 == false);
		}

		assert(Counter::getCounter().verify());
	}

	void test_shared_ptr()
	{
		cout << "\nstarting tests for class shared_ptr";

		test_shared_ptr_default_constructor();
		test_shared_ptr_constructor();
		test_shared_ptr_copy_constructor();
		test_shared_ptr_move_constructor();
		test_shared_ptr_assignement_raw_ptr();
		test_shared_ptr_copy_assignment();
		test_shared_ptr_move_assignment();
		test_shared_ptr_reset();
		test_shared_ptr_miscellaneous();

		cout << "\nfinished tests for class shared_ptr";
		
	}
	
	//MM_EXPECT_TRUE(actualSize == sizeCapacityData[0].size, actualSize, sizeCapacityData[0].size);

	MM_DECLARE_FLAG(MM_shared_ptr_UnitTest);

	MM_UNIT_TEST(MM_shared_ptr_UnitTest_sanity, MM_shared_ptr_UnitTest)
	{
		test_shared_ptr();
	}

}