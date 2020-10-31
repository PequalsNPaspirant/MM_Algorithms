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

namespace mm {

	template <std::size_t N>
	class arena_v1
	{
		static const std::size_t alignment = 16;
		alignas(alignment) char buf_[N];
		char* ptr_;

		std::size_t	align_up(std::size_t n) { return n + (alignment - 1) & ~(alignment - 1); }

	public:
		arena_v1() : ptr_(buf_) {}
		arena_v1(const arena_v1&) = delete;
		arena_v1& operator=(const arena_v1&) = delete;

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
	class stack_allocator_v1
	{
		arena_v1<N>& a_;

	public:
		typedef T value_type;

	public:
		template <class U> struct rebind { typedef stack_allocator_v1<U, N> other; };

		explicit stack_allocator_v1(arena_v1<N>& a) : a_(a) {}
		template <class U>
		stack_allocator_v1(const stack_allocator_v1<U, N>& a) : a_(a.a_) {}
		stack_allocator_v1(const stack_allocator_v1&) = default;
		stack_allocator_v1& operator=(const stack_allocator_v1&) = delete;

		T* allocate(std::size_t n)
		{
			return reinterpret_cast<T*>(a_.allocate(n * sizeof(T)));
		}

		void deallocate(T* p, std::size_t n)
		{
			a_.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
		}

		template <class T1, std::size_t N1, class U, std::size_t M>
		friend bool	operator==(const stack_allocator_v1<T1, N1>& x, const stack_allocator_v1<U, M>& y);

		template <class U, std::size_t M> 
		friend class stack_allocator_v1;
	};

	template <class T, std::size_t N, class U, std::size_t M>
	bool operator==(const stack_allocator_v1<T, N>& x, const stack_allocator_v1<U, M>& y)
	{
		return N == M && &x.a_ == &y.a_;
	}

	template <class T, std::size_t N, class U, std::size_t M>
	bool operator!=(const stack_allocator_v1<T, N>& x, const stack_allocator_v1<U, M>& y)
	{
		return !(x == y);
	}

}


