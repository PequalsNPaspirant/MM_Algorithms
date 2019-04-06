

#include "DataStructures/DS_heap.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"


/*
Binary Heap generally provides following operations:
1. insert() [e.g. void std::priority_queue<T>::push(T obj). complexity: O(log n)]
2. extractMax/Min() [e.g. void std::priority_queue<T>::pop(). complexity: O(log n)]
3. peepMax() [e.g. T std::priority_queue<T>::top(). complexity: O(1)]

Binary Heap can also provide following operations, but its costly:
1. int find(T key). [OR Check if T key exists in heap.] [returns index of element if heap is implemented as array. complexity: O(n)]
2. ChangePriority(int index, T newPriority) [complexity: O(log n)]
3. ChangePriority(T key, T newPriority/Key) [Find index and changePriority using above overloaded function#2. complexity: O(n)]
4. delete(int index, T newPriority/Key) [complexity: O(log n)]
5. delete(T key, T newPriority/Key) [Find index and delete using above overloaded function#4. complexity: O(n)]

Functions #2 and #4 above break the abstraction of heap. The indexes are internal representation and is not
supposed to be exposed outside heap data structure. Same is true for pointer to or reference to heap node if
heap is implemented as binary tree.
*/


namespace mm {



}