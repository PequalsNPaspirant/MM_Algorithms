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

namespace mm {

	template <class T, std::size_t bufferSize = 1024>
	class MemoryPool_v1
	{
		struct Block
		{
			Block *next;
		};

		class Buffer
		{
			static const std::size_t blockSize = sizeof(T) > sizeof(Block) ? sizeof(T) : sizeof(Block);
			uint8_t data[blockSize * bufferSize];

		public:

			Buffer * const next;

			Buffer(Buffer *next) :
				next(next)
			{
			}

			T *getBlock(std::size_t index)
			{
				return reinterpret_cast<T *>(&data[blockSize * index]);
			}
		};

		Block *firstFreeBlock = nullptr;
		Buffer *firstBuffer = nullptr;
		std::size_t bufferedBlocks = bufferSize;


	public:

		MemoryPool_v1() = default;
		MemoryPool_v1(MemoryPool_v1 &&memoryPool) = delete;
		MemoryPool_v1(const MemoryPool_v1 &memoryPool) = delete;
		MemoryPool_v1 operator =(MemoryPool_v1 &&memoryPool) = delete;
		MemoryPool_v1 operator =(const MemoryPool_v1 &memoryPool) = delete;

		~MemoryPool_v1()
		{
			while (firstBuffer) {
				Buffer *buffer = firstBuffer;
				firstBuffer = buffer->next;
				delete buffer;
			}
		}

		T *allocate()
		{
			if (firstFreeBlock) {
				Block *block = firstFreeBlock;
				firstFreeBlock = block->next;
				return reinterpret_cast<T*>(block);
			}

			if (bufferedBlocks >= bufferSize) {
				firstBuffer = new Buffer(firstBuffer);
				bufferedBlocks = 0;
			}

			return firstBuffer->getBlock(bufferedBlocks++);
		}

		void deallocate(T *pointer)
		{
			Block *block = reinterpret_cast<Block *>(pointer);
			block->next = firstFreeBlock;
			firstFreeBlock = block;
		}
	};

	template <class T, std::size_t bufferSize = 1024>
	class Allocator_v1 : private MemoryPool_v1<T, bufferSize>
	{
		Allocator_v1 *copyAllocator = nullptr;
		std::allocator<T> *rebindAllocator = nullptr;

	public:

		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T *pointer;
		typedef const T *const_pointer;
		typedef T &reference;
		typedef const T &const_reference;
		typedef T value_type;

		template <class U>
		struct rebind
		{
			typedef Allocator_v1<U, bufferSize> other;
		};

		Allocator_v1() = default;
		Allocator_v1(Allocator_v1 &allocator) : copyAllocator(&allocator) {}

		template <class U>
		Allocator_v1(const Allocator_v1<U, bufferSize> &other)
		{
			if (!std::is_same<T, U>::value)
				rebindAllocator = new std::allocator<T>();
		}

		~Allocator_v1()
		{
			delete rebindAllocator;
		}

		pointer allocate(size_type n, const void *hint = 0)
		{
			if (copyAllocator)
				return copyAllocator->allocate(n, hint);

			if (rebindAllocator)
				return rebindAllocator->allocate(n, hint);

			if (n != 1 || hint)
				throw std::bad_alloc();

			return MemoryPool_v1<T, bufferSize>::allocate();
		}

		void deallocate(pointer p, size_type n)
		{
			if (copyAllocator) {
				copyAllocator->deallocate(p, n);
				return;
			}

			if (rebindAllocator) {
				rebindAllocator->deallocate(p, n);
				return;
			}

			MemoryPool_v1<T, bufferSize>::deallocate(p);
		}

		void construct(pointer p, const_reference val)
		{
			new (p) T(val);
		}

		void destroy(pointer p)
		{
			p->~T();
		}
	};

}


