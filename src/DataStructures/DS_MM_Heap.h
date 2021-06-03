#pragma once

#include <vector>
#include <utility>
#include <functional>
using namespace std;

namespace mm {

	template<typename T, typename C = std::less<T>>
	class MM_Heap
	{
	public:
		MM_Heap(size_t maxSize = 0, C&& comparatorIn = C()) // by default it is max heap
			:
			index_(-1),
			comparator_(std::move(comparatorIn))
		{
			data_.reserve(maxSize);
		}

		inline void push(const T& obj)
		{
			if (data_.size() == size())
			{
				size_t newSize = static_cast<size_t>(size() * 1.5);
				data_.reserve(newSize);
			}
			//MM_Assert::mmRunTimeAssert(index_ < int(data_.size() - 1));
			++index_;
			if(index_ < data_.size())
				data_[index_] = obj;
			else
				data_.push_back(obj);

			//std::push_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
			siftUp(index_);
		}

		inline void pop(bool updateIndex = true)
		{
			//MM_Assert::mmRunTimeAssert(index_ > -1 && index_ < data_.size());
			//std::pop_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);

			//If only one element
			if (index_ == 0)
			{
				--index_;
				return;
			}

			std::swap(data_[0], data_[index_]);
			//if(updateIndex)
			//data_[0]->indexInMinHeap = 0;
			--index_;
			siftDown(0, updateIndex);
		}

		inline const T& top()
		{
			//MM_Assert::mmRunTimeAssert(index_ > -1);
			return data_[0];
		}

		//inline void addNext(const T& obj)
		//{
		//	data_[++index_] = obj;
		//}

		//inline void addNext(T&& obj)
		//{
		//	data_[++index_] = std::move(obj);
		//}

		inline size_t size()
		{
			return index_ + 1;
		}

		inline bool empty()
		{
			return index_ == -1;
		}

		inline void clear()
		{
			index_ = -1; //TODO: Do we need to destroy all the elements?
		}

		inline const T& get(int index)
		{
			//MM_Assert::mmRunTimeAssert(index_ > -1 && index <= index_);
			return data_[index];
		}

		inline void remove(int index)
		{
			std::swap(data_[index], data_[index_]);

			--index_;
			if (comparator_(data_[index], data_[index_ + 1]))
				siftDown(index);
			else
				siftUp(index);
		}

	private:
		vector<T> data_;
		int index_;
		const C comparator_;

		inline size_t getLeftChild(size_t index) {
			return ((index << 1) + 1);
		}

		inline size_t getRightChild(size_t index) {
			return ((index << 1) + 2);
		}

		inline size_t getParent(size_t index) {
			return ((index - 1) >> 1);
		}

		inline void heapify()
		{
			//std::make_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
			//Start from the parent of last element
			for (int i = getParent(index_); i >= 0; --i)
				siftDown(i);
		}

		inline void siftDown(size_t subTreeRootIndex, bool updateIndex = true)
		{
			size_t parentOfLastElement = getParent(index_);
			T buffer = data_[subTreeRootIndex];
			while (subTreeRootIndex <= parentOfLastElement)
			{
				size_t largestIndex = getLeftChild(subTreeRootIndex); // Assume left child is largest
				if (largestIndex >= size())
					break;

				size_t rightChildIndex = largestIndex + 1;
				if (rightChildIndex <= index_ && comparator_(data_[largestIndex], data_[rightChildIndex]))
					largestIndex = rightChildIndex;

				if (comparator_(buffer, data_[largestIndex]))
				{
					data_[subTreeRootIndex] = data_[largestIndex];
					//if(updateIndex)
					//data_[subTreeRootIndex]->indexInMinHeap = subTreeRootIndex;
					subTreeRootIndex = largestIndex;
				}
				else
					break;
			}
			data_[subTreeRootIndex] = buffer;
			//if(updateIndex)
			//data_[subTreeRootIndex]->indexInMinHeap = subTreeRootIndex;
		}
		inline void siftUp(size_t childIndex)
		{
			T buffer = data_[childIndex];
			while (childIndex > 0)
			{
				size_t parentIndex = getParent(childIndex);
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
	};

}