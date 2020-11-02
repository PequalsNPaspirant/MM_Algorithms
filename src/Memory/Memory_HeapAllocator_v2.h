#pragma once

/*
This is a fixed size custom memory allocator based on heap memory pool

*/

#include <iostream>
#include <iomanip>  //for setw
#include <cmath>
#include <memory>
using namespace std;

#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Assert.h"

// This is fixed size Heap Allocator

//#define MM_DEBUG

namespace mm {

	template <class T, size_t bufferSize>
	class MemoryPool_v2
	{
	public:
		MemoryPool_v2() = default;
		MemoryPool_v2(MemoryPool_v2&& memoryPool) = delete;
		MemoryPool_v2(const MemoryPool_v2& memoryPool) = delete;
		MemoryPool_v2 operator =(MemoryPool_v2&& memoryPool) = delete;
		MemoryPool_v2 operator =(const MemoryPool_v2& memoryPool) = delete;

		~MemoryPool_v2()
		{
			while (bufferHead_) {
				Buffer* buffer = bufferHead_;
				bufferHead_ = buffer->nextBuffer_;
				delete buffer;
			}
		}

		T* allocate()
		{
#ifdef MM_DEBUG
			cout << "\nMemoryPool_v2: Allocate for type: " << typeid(T).name()
				<< " Size: " << sizeof(T);
#endif

			if (freeBlockHead_) {
				FreeBlock* block = freeBlockHead_;
				freeBlockHead_ = block->next_;
				return reinterpret_cast<T*>(block);
			}

			if (!bufferHead_ || bufferHead_->isFull())
			{
#ifdef MM_DEBUG
				if (bufferHead_)
					cout << "\nbuffer full, reallocating memory";
#endif
				bufferHead_ = new Buffer(bufferHead_);
			}

			return bufferHead_->getNextBlock();
		}

		void deallocate(T* pointer)
		{
#ifdef MM_DEBUG
			cout << "\nMemoryPool_v2: Deallocate for type: " << typeid(T).name()
				<< " Size: " << sizeof(T);
#endif

			FreeBlock* block = reinterpret_cast<FreeBlock*>(pointer);
			block->next_ = freeBlockHead_;
			freeBlockHead_ = block;
		}

	private:
		struct FreeBlock
		{
			FreeBlock* next_;
		};

		class Buffer
		{
		public:
			static constexpr const size_t blockSize = sizeof(T) > sizeof(FreeBlock) ? sizeof(T) : sizeof(FreeBlock);
			Buffer* const nextBuffer_;

			Buffer(Buffer* nextBuffer)
				: nextBuffer_{ nextBuffer }, index_{ 0 }				
			{
			}

			T* getNextBlock()
			{
				return reinterpret_cast<T*>(&data[blockSize * index_++]);
			}

			bool isFull()
			{
				return index_ >= bufferSize;
			}

		private:
			uint8_t data[blockSize * bufferSize];
			size_t index_;
		};

		FreeBlock* freeBlockHead_ = nullptr;
		Buffer* bufferHead_ = nullptr;
	};

	template <class T, size_t bufferSize = 1024>
	class HeapAllocator_v2
	{
		std::shared_ptr< MemoryPool_v2<T, bufferSize> > pool_;
		std::allocator<T>* rebindAllocator = nullptr;

	public:
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;

		template <class U> struct rebind { typedef HeapAllocator_v2<U, bufferSize> other; };

		HeapAllocator_v2()
			: pool_{ std::make_shared< MemoryPool_v2<T, bufferSize> >() }
		{}
		HeapAllocator_v2(HeapAllocator_v2&& allocator) = delete;
		HeapAllocator_v2(const HeapAllocator_v2& allocator) : pool_{ allocator.pool_ } {
#ifdef MM_DEBUG
			cout << "\nHeapAllocator_v2: Copy Ctr";
#endif
		}
		template <class U>
		HeapAllocator_v2(const HeapAllocator_v2<U, bufferSize>& other)
		{
			//This constructor is never called for current test cases
#ifdef MM_DEBUG
			cout << "\nHeapAllocator_v2: Rebind Copy Ctr";
#endif
			if (!std::is_same<T, U>::value)
				rebindAllocator = new std::allocator<T>();
		}
		HeapAllocator_v2& operator=(HeapAllocator_v2&&) = delete;
		HeapAllocator_v2& operator=(const HeapAllocator_v2&) = delete;

		~HeapAllocator_v2()
		{
			delete rebindAllocator;
		}

		pointer allocate(size_type n, const void* hint = 0)
		{
#ifdef MM_DEBUG
			cout << "\nHeapAllocator_v2: Allocate for type: " << typeid(T).name()
				<< " Size: " << sizeof(T) 
				<< " Num Elements: " << n;
#endif

			if (rebindAllocator)
				return rebindAllocator->allocate(n, hint);

			if (n != 1 || hint)
				throw std::bad_alloc();

			return pool_->allocate();
		}

		void deallocate(pointer p, size_type n)
		{
			if (n != 1)
				throw std::bad_alloc();

#ifdef MM_DEBUG
			cout << "\nHeapAllocator_v2: Deallocate for type: " << typeid(T).name()
				<< " Size: " << sizeof(T)
				<< " Num Elements: " << n;
#endif

			if (rebindAllocator) {
				rebindAllocator->deallocate(p, n);
				return;
			}

			pool_->deallocate(p);
		}

		template <typename... Args>
		void construct(pointer p, Args&&... args)
		{
#ifdef MM_DEBUG
			cout << "\nHeapAllocator_v2: constructing object";
#endif
			new (p) T(std::forward<Args>(args)...);
		}

		void destroy(pointer p)
		{
#ifdef MM_DEBUG
			cout << "\nHeapAllocator_v2: destroying object";
#endif
			p->~T();
		}

		template <class T1, size_t N1, class U, size_t M>
		friend bool	operator==(const HeapAllocator_v2<T1, N1>& x, const HeapAllocator_v2<U, M>& y);

		template <class U, size_t M>
		friend class HeapAllocator_v2;

	};

	/*
	operator== should be:
	1. reflexive i.e. a == a
	2. symmetric i.e if a == b then b == a
	3. transitive i.e. if a = b and b = c, then a = c
	It should not exit via an exception.
	*/
	template <class T, size_t N, class U, size_t M>
	bool operator==(const HeapAllocator_v2<T, N>& x, const HeapAllocator_v2<U, M>& y)
	{
		return N == M 
			&& x.pool_ == y.pool_;
	}

	template <class T, size_t N, class U, size_t M>
	bool operator!=(const HeapAllocator_v2<T, N>& x, const HeapAllocator_v2<U, M>& y)
	{
		return !(x == y);
	}
}


