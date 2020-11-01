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

namespace mm {

	template <class T, std::size_t bufferSize = 1024>
	class MemoryPool_v2
	{
		struct Block
		{
			Block* next_;
		};

		class Buffer
		{
			static constexpr const std::size_t blockSize = sizeof(T) > sizeof(Block) ? sizeof(T) : sizeof(Block);
			uint8_t data[blockSize * bufferSize];

		public:
			Buffer* const nextBuffer_;

			Buffer(Buffer* nextBuffer) :
				nextBuffer_(nextBuffer)
			{
			}

			T* getBlock(std::size_t index)
			{
				return reinterpret_cast<T*>(&data[blockSize * index]);
			}
		};

		Block* freeBlockHead_ = nullptr;
		Buffer* bufferHead_ = nullptr;
		std::size_t bufferedBlocks = bufferSize;

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
			//cout << "\nMemoryPool_v2: Allocate for type: " << typeid(T).name();
			if (freeBlockHead_) {
				Block* block = freeBlockHead_;
				freeBlockHead_ = block->next_;
				return reinterpret_cast<T*>(block);
			}

			if (bufferedBlocks >= bufferSize) {
				bufferHead_ = new Buffer(bufferHead_);
				bufferedBlocks = 0;
			}

			return bufferHead_->getBlock(bufferedBlocks++);
		}

		void deallocate(T* pointer)
		{
			//cout << "\nMemoryPool_v2: Deallocate for type: " << typeid(T).name();
			Block* block = reinterpret_cast<Block*>(pointer);
			block->next_ = freeBlockHead_;
			freeBlockHead_ = block;
		}
	};

	template <class T, std::size_t bufferSize = 1024>
	class HeapAllocator_v2
	{
		//MemoryPool_v2<T, bufferSize>* pool_;
		std::shared_ptr< MemoryPool_v2<T, bufferSize> > pool_;
		//HeapAllocator_v2* copyAllocator = nullptr;
		std::allocator<T>* rebindAllocator = nullptr;

	public:
		typedef std::size_t size_type;
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
			//cout << "\nHeapAllocator_v2: Copy Ctr";
		}
		//template <class U>
		//HeapAllocator_v2(const HeapAllocator_v2<U, bufferSize>& other)
		//{
		//	if (!std::is_same<T, U>::value)
		//		rebindAllocator = new std::allocator<T>();
		//}
		HeapAllocator_v2& operator=(HeapAllocator_v2&&) = delete;
		HeapAllocator_v2& operator=(const HeapAllocator_v2&) = delete;

		~HeapAllocator_v2()
		{
			delete rebindAllocator;
		}

		pointer allocate(size_type n, const void* hint = 0)
		{
			//if (copyAllocator)
			//	return copyAllocator->allocate(n, hint);

			//cout << "\nHeapAllocator_v2: Allocate for type: " << typeid(T).name() << " Size: " << n;

			if (rebindAllocator)
				return rebindAllocator->allocate(n, hint);

			if (n != 1 || hint)
				throw std::bad_alloc();

			return pool_->allocate();
		}

		void deallocate(pointer p, size_type n)
		{
			//if (copyAllocator) {
			//	copyAllocator->deallocate(p, n);
			//	return;
			//}

			if (n != 1)
				throw std::bad_alloc();

			//cout << "\nHeapAllocator_v2: Deallocate for type: " << typeid(T).name() << " Size: " << n;

			if (rebindAllocator) {
				rebindAllocator->deallocate(p, n);
				return;
			}

			pool_->deallocate(p);
		}

		void construct(pointer p, const_reference val)
		{
			new (p) T(val);
		}

		void destroy(pointer p)
		{
			p->~T();
		}

		template <class T1, std::size_t N1, class U, std::size_t M>
		friend bool	operator==(const HeapAllocator_v2<T1, N1>& x, const HeapAllocator_v2<U, M>& y);

		template <class U, std::size_t M>
		friend class HeapAllocator_v2;

	};

	template <class T, std::size_t N, class U, std::size_t M>
	bool operator==(const HeapAllocator_v2<T, N>& x, const HeapAllocator_v2<U, M>& y)
	{
		return N == M 
			&& x.pool_ == y.pool_;
	}

	template <class T, std::size_t N, class U, std::size_t M>
	bool operator!=(const HeapAllocator_v2<T, N>& x, const HeapAllocator_v2<U, M>& y)
	{
		return !(x == y);
	}
}


