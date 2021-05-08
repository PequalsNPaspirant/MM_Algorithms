//=======================================================================================================//
//   Copyright (c) 2018 Maruti Mhetre                                                                    //
//   All rights reserved.                                                                                //
//=======================================================================================================//
//   Redistribution and use of this software in source and binary forms, with or without modification,   //
//   are permitted for personal, educational or non-commercial purposes provided that the following      //
//   conditions are met:                                                                                 //
//   1. Redistributions of source code must retain the above copyright notice, this list of conditions   //
//      and the following disclaimer.                                                                    //
//   2. Redistributions in binary form must reproduce the above copyright notice, this list of           //
//      conditions and the following disclaimer in the documentation and/or other materials provided     //
//      with the distribution.                                                                           //
//   3. Neither the name of the copyright holder nor the names of its contributors may be used to        //
//      endorse or promote products derived from this software without specific prior written            //
//      permission.                                                                                      //
//=======================================================================================================//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR      //
//   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND    //
//   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR          //
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL   //
//   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   //
//   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  //
//   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT   //
//   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     //
//=======================================================================================================//

#pragma once

#include <functional> //for std::less
#include <queue>
#include <algorithm>
using namespace std;

namespace mm {

	enum class KnapsackProblemAlgo
	{
		naive_v1 = 0,
		naive_v2,
		naive_tail_recursive,
		naive_non_recursive_v1,
		naive_non_recursive_v2,
		naive_non_recursive_v3,
		top_down_using_vector,
		top_down_using_hashmap,
		top_down_using_map_tail_recursive,
		top_down_using_map_non_recursive,
		bottom_up_v1,
		bottom_up_v2,
		bottom_up_object_count_independent_space_efficient_v1,
		bottom_up_object_count_independent_space_efficient_v2,
		bottom_up_object_count_independent_space_efficient_v3,
		bottom_up_object_count_independent_space_efficient_using_set,
		bottom_up_capacity_independent,
		bottom_up_capacity_independent_space_efficient,
		branch_and_bound_naive_v1,
		branch_and_bound_cumulative_sum_v2,
		branch_and_bound_no_recalc_upperbound_v3,
		branch_and_bound_use_greedylike_maxValue_v3a,
		branch_and_bound_use_greedy_maxValue_v3b,
		branch_and_bound_use_deque_v4,
		branch_and_bound_unique_ptr_heap_v5,
		branch_and_bound_heap_preallocate_v6,
		branch_and_bound_heap_preallocate_MM_Queue_v7,
		branch_and_bound_MM_Heap_v8,
		branch_and_bound_MM_Heap_use_greedy_maxValue_v8b,
		branch_and_bound_MM_Heap_use_greedy_maxValue_sort_inplace_v9b,
		greedy,

		max_approaches
	};

	//Global data types and containers
	struct StackMember
	{
		StackMember()
			: returnValuePlaceHolder(nullptr)
		{}
		StackMember(unsigned long long numItemsIn, unsigned long long knapsackCapacityIn, double* returnValuePlaceHolderIn)
			: numItems(numItemsIn),
			knapsackCapacity(knapsackCapacityIn),
			exclude(numeric_limits<double>::max()),
			include(numeric_limits<double>::max()),
			returnValuePlaceHolder(returnValuePlaceHolderIn)
		{}
		unsigned long long numItems;
		unsigned long long knapsackCapacity;
		double exclude;
		double include;
		double* returnValuePlaceHolder;
	};

	struct KnapsackProblemControlStation
	{
		KnapsackProblemControlStation(bool isImplementedIn, unsigned long long maxNumItemsIn, unsigned long long maxKnapsackCapacityIn, unsigned long long max_nWIn, int maskIn, string algoNameIn)
			: isImplemented(isImplementedIn),
			maxNumItems(maxNumItemsIn),
			maxKnapsackCapacity(maxKnapsackCapacityIn),
			max_nW(max_nWIn),
			mask(maskIn),
			algoName(algoNameIn)
		{}
		bool isImplemented;
		unsigned long long maxNumItems;
		unsigned long long maxKnapsackCapacity;
		unsigned long long max_nW;
		int mask;
		string algoName;
	};

	extern unsigned long long numberOfFunctionCalls;
	extern unsigned long long sizeOfHeap;


	template<typename T, typename C = std::less<T>>
	class MM_Heap
	{
	public:
		MM_Heap(size_t capacity = 0, C&& comparatorIn = C()) // by default it is max heap
			: data_(0),
			count_(0),
			comparator_(std::move(comparatorIn))
		{
			if(capacity != 0)
				data_.reserve(capacity);
		}

		//=================== Standard and non-standard heap operations

		inline void heapify()
		{
			//std::make_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
			//Start from the parent of last element
			for(int i = getParent(count_ - 1); i >= 0; --i)
				siftDown(i);
		}

		inline void push(const T& obj)
		{
			//Grow automatically if capacity() == size() i.e. if data_.capacity() = count_
			if (capacity() == size())
				data_.push_back(obj);
			else
				data_[count_] = obj;
			//std::push_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
			siftUp(count_);
			++count_;
		}
		inline void push(T&& obj)
		{
			//Grow automatically if capacity() == size() i.e. if data_.capacity() = count_
			if (capacity() == size())
				data_.push_back(obj);
			else
				data_[count_] = obj;
			//std::push_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
			siftUp(count_);
			++count_;
		}
		inline const T& top()
		{
			//Client needs to ensure size() > 0
			return data_[0];
		}
		inline void pop()
		{
			//Client needs to ensure that size() > 0
			//std::pop_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);

			--count_;
			std::swap(data_[0], data_[count_]);
			siftDown(0);
		}
		inline void siftDown(int subTreeRootIndex)
		{
			int parentOfLastElement = getParent(count_ - 1);
			T buffer = data_[subTreeRootIndex];
			while (subTreeRootIndex <= parentOfLastElement)
			{
				int largestIndex = getLeftChild(subTreeRootIndex); // Assume left child is largest
				int rightChildIndex = largestIndex + 1;
				if(rightChildIndex < count_ && comparator_(data_[largestIndex], data_[rightChildIndex]))
					largestIndex = rightChildIndex;

				if (comparator_(buffer, data_[largestIndex]))
				{
					data_[subTreeRootIndex] = data_[largestIndex];
					subTreeRootIndex = largestIndex;
				}
				else
					break;
			}
			data_[subTreeRootIndex] = buffer;
		}
		inline void siftUp(int childIndex)
		{
			T buffer = data_[childIndex];
			while (childIndex > 0)
			{
				int parentIndex = getParent(childIndex);
				if (comparator_(data_[parentIndex], buffer))
				{
					data_[childIndex] = data_[parentIndex];
					//data_[childIndex]->indexInMinHeap = childIndex;
				}
				else
					break;

				childIndex = parentIndex;
			}

			data_[childIndex] = buffer;
			//data_[childIndex]->indexInMinHeap = childIndex;
		}
		inline size_t size()
		{
			return count_;
		}
		inline bool empty()
		{
			return count_ == 0;
		}
		inline void clear()
		{
			count_ = 0; //Note: Do we need to destroy all the elements? No, use destroy() in that case!
		}
		inline void destroy()
		{
			count_ = 0;
			data_.clear();
		}
		inline void removeAtIndex(int index) //remove at index
		{
			--count_;
			std::swap(data_[index], data_[count_]);

			//--index_;
			if(comparator_(data_[index], data_[count_]))
				siftDown(index);
			else
				siftUp(index);
		}
		inline const T& getAtIndex(int index)
		{
			return data_[index];
		}
		inline T getNextAvailableElement() //get next available element
		{
			return data_[count_]; //client needs to make sure capacity() > size()
		}
		void reserve(size_t newCapacity)
		{
			data_.reserve(newCapacity);
		}
		inline size_t capacity()
		{
			return data_.capacity();
		}		
		inline void resize(size_t newSize)
		{
			return data_.resize(newSize);
		}

		//=================== Limted operations on internal data structure data_

		inline void addToData(const T& obj) //add new element in underlying data structure
		{
			data_.push_back(obj);
		}
		inline void addToData(T&& obj)
		{
			data_.push_back(std::move(obj));
		}

	private:
		vector<T> data_;
		int count_; //count of heap objects
		const C comparator_;

		inline int getLeftChild(int index) {
			return ((index << 1) + 1);
		}

		inline int getRightChild(int index) {
			return ((index << 1) + 2);
		}

		inline int getParent(int index) {
			return ((index - 1) >> 1);
		}
	};


	template<typename T>
	class MM_Queue
	{
	public:
		MM_Queue(size_t initialSize = 0)
			: data_(0),
			start_(0),
			count_(0)
		{
			if (initialSize != 0)
				//data_.reserve(capacity);
				data_.resize(initialSize);
		}
		void push(const T& obj)
		{
			size_t end = getTargetPosition();
			data_[end] = obj;
			++count_;
		}
		void push(T&& obj)
		{
			size_t end = getTargetPosition();
			data_[end] = std::move(obj);
			++count_;
		}
		T& front()
		{
			//MyAssert::myRunTimeAssert(!empty());
			return data_[start_];
		}
		void pop()
		{
			//MyAssert::myRunTimeAssert(!empty());
			--count_;
			++start_;
			if (start_ >= data_.capacity())
				start_ = start_ - data_.capacity();
		}
		bool empty()
		{
			return count_ == 0;
		}
		unsigned int count()
		{
			return count_;
		}
		unsigned int capacity()
		{
			return data_.capacity();
		}
		void reserve(unsigned int newCapacity)
		{
			data_.reserve(newCapacity);
		}

	private:
		vector<T> data_;
		size_t start_;
		size_t count_;

		size_t getTargetPosition()
		{
			size_t end = 0;
			if (count_ == data_.size())
			{
				//Grow by factor of 1.5
				vector<T> newVector(0);
				int newSize = static_cast<int>(data_.size() * 1.5);
				if (newSize == 0)
					newSize = 10; //default size
				newVector.resize(newSize);
				int index = -1;
				for (size_t i = start_; i < data_.size(); ++i)
					newVector[++index] = std::move(data_[i]);
				for (size_t i = 0; i < start_; ++i)
					newVector[++index] = std::move(data_[i]);

				data_.swap(newVector);
				start_ = 0;
				end = count_;
			}
			else
			{
				end = start_ + count_;
				if (end >= data_.capacity())
					end = end - data_.capacity();
			}

			return end;
		}
	};


	struct decisionTreeNode_v6
	{
		//decisionTreeNode_v6()
		//	: level(0),
		//	maxValue(0.0),
		//	totalWeight(0),
		//	upperbound(0.0)
		//{
		//}

		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}
	};

	struct decisionTreeNode_v7
	{
		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}
	};

	struct decisionTreeNode_v8
	{
		unsigned int level;
		double maxValue;
		unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}
	};

	struct Comparator_v8
	{
		bool operator()(const decisionTreeNode_v8* lhs, const decisionTreeNode_v8* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	struct decisionTreeNode_v8b
	{
		unsigned int level;
		double maxValue;
		unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}
	};

	struct Comparator_v8b
	{
		bool operator()(const decisionTreeNode_v8b* lhs, const decisionTreeNode_v8b* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	struct decisionTreeNode_v9b
	{
		unsigned int level;
		double maxValue;
		unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[level - 1];
				cumulativeValueTillNow = cumulativeValues[level - 1];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned long long>::const_iterator iter = std::upper_bound(cumulativeWeights.begin() + level, cumulativeWeights.end(), targetCumulativeWeight);
				//[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long index = iter - cumulativeWeights.begin();
			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (index != level)
			{
				--index;  //iter does not point to cumulativeWeights.begin() + level, so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[index] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[index] - cumulativeValueTillNow);
			}

			if (++index != values.size() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ index ] / weights[ index ]);
			}
		}
	};

	struct Comparator_v9b
	{
		bool operator()(const decisionTreeNode_v9b* lhs, const decisionTreeNode_v9b* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	size_t getHash(size_t num1, size_t num2) noexcept;

	double DP_KnapsackProblem_0_1_naive_v1(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems);
	double DP_KnapsackProblem_0_1_naive_v2(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems);
	double DP_KnapsackProblem_0_1_naive_tail_recursive(double retValue, const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems);
	double DP_KnapsackProblem_0_1_naive_non_recursive_v1(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<StackMember>& functionStack);
	double DP_KnapsackProblem_0_1_naive_non_recursive_v2(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<StackMember>& functionStack);
	double DP_KnapsackProblem_0_1_naive_non_recursive_v3(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<StackMember>& functionStack);
	double DP_KnapsackProblem_0_1_top_down_using_vector(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<vector<double>>& lookupVector);
	double DP_KnapsackProblem_0_1_top_down_using_hashmap(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		unordered_map<size_t, double>& lookupHashMap);
	double DP_KnapsackProblem_0_1_top_down_using_map_tail_recursive(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		unordered_map<size_t, double>& lookup);
	double DP_KnapsackProblem_0_1_top_down_using_map_non_recursive(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		unordered_map<size_t, double>& lookup, vector<StackMember>& functionStack);
	double DP_KnapsackProblem_0_1_bottom_up_v1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<vector<double>>& cache_bottom_up_2d);
	double DP_KnapsackProblem_0_1_bottom_up_v2(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up);
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up_prev, vector<double>& cache_bottom_up_current);
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v1a(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up_prev, vector<double>& cache_bottom_up_current);
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v2(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up);
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v3(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up);
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v4(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up);
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_using_set(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems);
	double DP_KnapsackProblem_0_1_bottom_up_capacity_independent(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<vector<double>>& cache_bottom_up_2d);
	double DP_KnapsackProblem_0_1_bottom_up_capacity_independent_space_efficient(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up_prev, vector<double>& cache_bottom_up_current);
	double DP_KnapsackProblem_0_1_branch_and_bound_v1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices);
	double DP_KnapsackProblem_0_1_branch_and_bound_v2(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights);
	double DP_KnapsackProblem_0_1_branch_and_bound_v3(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights);
	double DP_KnapsackProblem_0_1_branch_and_bound_v3a(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights);
	double DP_KnapsackProblem_0_1_branch_and_bound_v3b(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights);
	double DP_KnapsackProblem_0_1_branch_and_bound_v4(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights);
	double DP_KnapsackProblem_0_1_branch_and_bound_v5(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights);
	double DP_KnapsackProblem_0_1_branch_and_bound_v6(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		vector<vector<decisionTreeNode_v6>>& heapObjectsGrowingPool, queue<decisionTreeNode_v6*, deque<decisionTreeNode_v6*>>& poolIndices);
	double DP_KnapsackProblem_0_1_branch_and_bound_v7(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		vector<vector<decisionTreeNode_v7>>& heapObjectsGrowingPool, MM_Queue<decisionTreeNode_v7*>& poolIndices);
	double DP_KnapsackProblem_0_1_branch_and_bound_v8(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v8*, Comparator_v8>& maxHeap_v8, vector<vector<decisionTreeNode_v8>>& heapObjectsGrowingPool);
	double DP_KnapsackProblem_0_1_branch_and_bound_v8b(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v8b*, Comparator_v8b>& maxHeap_v8b, vector<vector<decisionTreeNode_v8b>>& heapObjectsGrowingPool);
	double DP_KnapsackProblem_0_1_branch_and_bound_v9b(vector<double>& values, vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v9b*, Comparator_v9b>& maxHeap_v9b, vector<vector<decisionTreeNode_v9b>>& heapObjectsGrowingPool);
	double DP_KnapsackProblem_0_1_greedy(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems);

}