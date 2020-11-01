#pragma once

/*
This is a fixed size custom memory allocator based on heap memory pool

*/

#include <iostream>
#include <iomanip>  //for setw
#include <cmath>
#include <cstddef>
#include <memory>
using namespace std;

#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Assert.h"

// Work in progress to implement the variable size heap allocator...

namespace mm {

	template <std::size_t N>
	class arena_v3
	{
		static const std::size_t alignment = 32;
		//alignas(alignment) char buf_[N];
		char* buf_;
		char* ptr_;

		std::size_t	align_up(std::size_t n) { return n + (alignment - 1) & ~(alignment - 1); }

	public:
		arena_v3() : ptr_(new char[align_up(N)]) {}
		arena_v3(const arena_v3&) = default;
		arena_v3& operator=(const arena_v3&) = delete;

		char* allocate(std::size_t n)
		{
			n = align_up(n);
			if (buf_ + N - ptr_ >= n)
			{
				char* r = ptr_;
				ptr_ += n;
				return r;
			}
			return static_cast<char*>(::operator new(n));
		}

		void deallocate(char* p, std::size_t n)
		{
			n = align_up(n);
			if (buf_ <= p && p < buf_ + N)
			{
				if (p + n == ptr_)
					ptr_ = p;
			}
			else
				::operator delete(p);
		}
	};

	template <class T, std::size_t N>
	class HeapAllocator_v3
	{
		arena_v3<N> a_;

	public:
		typedef T value_type;

	public:
		template <class U> struct rebind { typedef HeapAllocator_v3<U, N> other; };

		explicit HeapAllocator_v3() = default;
		template <class U>
		HeapAllocator_v3(const HeapAllocator_v3<U, N>& a) {}
		HeapAllocator_v3(const HeapAllocator_v3&) = default;
		HeapAllocator_v3& operator=(const HeapAllocator_v3&) = delete;

		T* allocate(std::size_t n)
		{
			return reinterpret_cast<T*>(a_.allocate(n * sizeof(T)));
		}

		void deallocate(T* p, std::size_t n)
		{
			a_.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
		}

		template <class T1, std::size_t N1, class U, std::size_t M>
		friend bool	operator==(const HeapAllocator_v3<T1, N1>& x, const HeapAllocator_v3<U, M>& y);

		template <class U, std::size_t M> 
		friend class HeapAllocator_v3;
	};

	template <class T, std::size_t N, class U, std::size_t M>
	bool operator==(const HeapAllocator_v3<T, N>& x, const HeapAllocator_v3<U, M>& y)
	{
		return N == M && &x.a_ == &y.a_;
	}

	template <class T, std::size_t N, class U, std::size_t M>
	bool operator!=(const HeapAllocator_v3<T, N>& x, const HeapAllocator_v3<U, M>& y)
	{
		return !(x == y);
	}

}


