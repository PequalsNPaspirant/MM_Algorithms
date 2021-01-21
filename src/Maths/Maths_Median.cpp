#include <iostream>
using namespace std;

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_PrintUtils.h"
#include "Utils/Utils_TimerUtils.h"

/*
References:
https://www.geeksforgeeks.org/median-of-an-unsorted-array-in-liner-time-on/
https://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array/
https://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-2-expected-linear-time/
https://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-3-worst-case-linear-time/
https://www.geeksforgeeks.org/kth-smallest-or-largest-element-in-unsorted-array-set-4/?ref=rp
*/

namespace mm {

	//Find median is a special case of finding kth smallest or largest element in an unsorted array

	/*
	Sort the array and return array[k]
		Time Complexity: O(n log n)
		Space Complexity: O(n) to store sorted array if we can not modify original array
	*/
	int findKthSmallest_v1(const vector<int>& array, int k)
	{

	}

	int findMedian_v1(const vector<int>& array)
	{
		return 0;
	}

	/*
	Using mean/max heap
	n = array.size()
	If k <= n / 2, create min heap of all elements and then extract min element from heap k times. 
		Time Complexity: O(n) to construct heap + k log n to extract min
		Space Complexity: O(n) to construct heap 
	If k > n / 2, find kth largest element. create max heap of all elements and then extract max element from heap k times. 
		Time Complexity: O(n) to construct heap + k log n to extract max
		Space Complexity: O(n) to construct heap 
	*/
	int findKthSmallest_v2(const vector<int>& array, int k)
	{

	}
	int findMedian_v2(const vector<int>& array)
	{
		return 0;
	}

	/*
	Using mean/max heap
	If k <= array.size() / 2, create max heap of size k from first k elements and push remaining (n - k) elements from array 
	into heap if array[i] < heap.top() and pop top. The top of max heap at end is kth smallest element.
		Time Complexity: O(k) to construct max heap + (n - k) log k to push all remaining elements into max heap.
		Space Complexity: O(k) to construct heap 
	If k > array.size() / 2, find kth largest element. create min heap of size k from first k elements and push remaining (n - k) 
	elements from array into heap if array[i] > heap.top() and pop top. The top of max heap at end is kth largest element.
		Time Complexity: O(k) to construct min heap + (n - k) log k to push all remaining elements into min heap.
		Space Complexity: O(k) to construct heap 
	*/
	int findKthSmallest_v3(const vector<int>& array, int k)
	{

	}
	int findMedian_v3(const vector<int>& array)
	{
		return 0;
	}

	/*
	Using quick select
		Time Complexity: average case: O(n log n)  worst case: O(n^2)
		Space Complexity: O(n) if we can not modify original array
	*/
	int findKthSmallest_v4(const vector<int>& array, int k)
	{

	}
	int findMedian_v4(const vector<int>& array)
	{
		return 0;
	}

	/*
	Using quick select with random pivot
	Time Complexity: Expected linear time
	Space Complexity: O(n) if we can not modify original array
	Refer: https://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-2-expected-linear-time/
	https://www.youtube.com/watch?v=EzeYI7p9MjU
	http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-046j-introduction-to-algorithms-sma-5503-fall-2005/video-lectures/lecture-6-order-statistics-median/
	Book: Introduction to Algorithms - Clifford Stein, Thomas H. Cormen, Charles E. Leiserson, Ronald L.
	*/
	int findKthSmallest_v5(const vector<int>& array, int k)
	{

	}
	int findMedian_v5(const vector<int>& array)
	{
		return 0;
	}

	/*
	Using quick select with pivot as median of medians
	Time Complexity: Worst case linear time
	if we select the group size = p, then
	Step 1:
	To find median of n/p groups each of size p = T(ceiling(n/p)) * O(1) ...finding median of array of fixed size p is 
	done in fixed number of operations and so it is not dependent upon n, so its constant time operation.
	Lets say its done by comparing all elements with each other i.e. it will take p*(p-1) comparisons.
	If p = 5, it will take 5*4 = 20 comparisons, but it will always take 20 comparisons, irrespective of size of n, so its O(20) = O(1)
	Then we need to find a median of all ceiling(n/p) medians recursively so it can be done in T(ceiling(n/p))
	Step 2:
	Once we find the median in step#1, we need to create left (having all elements less than median), right
	(having all elements greater than median) and middle partition (having all elements equal to median). This can be done in (n)
	Step 3:
	The median found in step#1 is median of ceiling(n/p) medians, but its not median of n numbers. Because its median of ceiling(n/p) medians,
	this median is greater than at least some numbers from the 1/2 of the ceiling(n/p) groups. But we need to exclude the current group
	in which median lie. Also there can be a group whose size is less than p, we need to exclude that group as well.
	So approximately, median of median is greater than at least ceiling(p/2) * (1/2 * ceiling(n/p) groups - current group - incomplete group)
	i.e. median of median is greater than at least ceiling(p/2) * (1/2 * ceiling(n/p) - 2)
	Also, median of median is less than at least ceiling(p/2) * (1/2 * ceiling(n/p) - 2) i.e. the elements in the right side groups
	In worst case, the remaining elements can lie on the same side of the median of medians. Thus in worst case,
	one of the partitions created in step#2 may have maximum (n - ceiling(p/2) * (1/2 * ceiling(n/p) - 2)) elements. So, because this is unequal
	partition, we need to call median finding procedure recursively on bigger partition. SO this can be done in
	T(n - ceiling(p/2) * (1/2 * ceiling(n/p) - 2))

	So together, it will take (step#1 + step#2 + step#3)
	T(n) = T(ceiling(n/p)) + O(n) + T(n - ceiling(p/2) * (1/2 * ceiling(n/p) - 2))

	When p = 5
	T(n) = T(ceiling(n/5)) + O(n) + T(n - ceiling(5/2) * (1/2 * ceiling(n/5) - 2)) 
	     = T(ceiling(n/5)) + O(n) + T(n - 3 * ceiling(n/10) - 2)        ...since 1/2 * ceiling(n/5) = ceiling(n/10)
		 = T(ceiling(n/5)) + O(n) + T(n - 3 * ceiling(n/10))
		 = T(n/5) + O(n) + T(n - 3 * n/10)
		 = T(n/5) + O(n) + T(7n/10)                     ....thus we have to recurse on total n/5 + 7n/10 = 9n/10 = 0.9n elements at each recursion
		 ~ O(n)

	When p = 7
		 T(n) = T(ceiling(n/7)) + O(n) + T(n - ceiling(7/2) * (1/2 * ceiling(n/7) - 2))
		 = T(ceiling(n/7)) + O(n) + T(n - 4 * ceiling(n/14) - 2)        ...since 1/2 * ceiling(n/7) = ceiling(n/14)
		 = T(ceiling(n/7)) + O(n) + T(n - 4 * ceiling(n/14))
		 = T(n/7) + O(n) + T(n - 4 * n/14)
		 = T(n/7) + O(n) + T(10n/14)                    ....thus we have to recurse on total n/7 + 10n/14 = 12n/14 = 0.8571n elements at each recursion
		 ~ O(n)

	When p = 9
		 T(n) = T(ceiling(n/9)) + O(n) + T(n - ceiling(9/2) * (1/2 * ceiling(n/9) - 2))
		 = T(ceiling(n/9)) + O(n) + T(n - 5 * ceiling(n/18) - 2)        ...since 1/2 * ceiling(n/9) = ceiling(n/18)
		 = T(ceiling(n/9)) + O(n) + T(n - 5 * ceiling(n/18))
		 = T(n/9) + O(n) + T(n - 5 * n/18)
		 = T(n/9) + O(n) + T(13n/18)                    ....thus we have to recurse on total n/9 + 13n/18 = 15n/18 = 0.8333n elements at each recursion
		 ~ O(n)

	When p = 3
		 T(n) = T(ceiling(n/3)) + O(n) + T(n - ceiling(3/2) * (1/2 * ceiling(n/3) - 2))
		 = T(ceiling(n/3)) + O(n) + T(n - 2 * ceiling(n/6) - 2)        ...since 1/2 * ceiling(n/3) = ceiling(n/6)
		 = T(ceiling(n/3)) + O(n) + T(n - 2 * ceiling(n/6))
		 = T(n/3) + O(n) + T(n - 2 * n/6)
		 = T(n/3) + O(n) + T(2n/3)                     ....thus we have to recurse on total n/3 + 2n/3 = n elements at each recursion
		 ~ O(n log n)

	In short, we need to recurse on 
		 = n/p + (n - ceiling(p/2) * 1/2 * n/p)
		 = n/p + (n - ceiling(p/2) * n/2p)
		 = (2/2p + (2p - ceiling(p/2))/2p) * n
		 = (2 + (2p - ceiling(p/2))/2p * n

	 for p = 3
		= (2 + (2p - ceiling(p/2))/2p * n
		= (2 + (6 - ceiling(3/2))/6 * n
		= (2 + (6 - 2)/6 * n
		= n
	for p = 5
		= (2 + (2p - ceiling(p/2))/2p * n
		= (2 + (10 - ceiling(5/2))/10 * n
		= (2 + (10 - 3)/10 * n
		= 9n/10
		= 0.9000n
	for p = 7
		= (2 + (2p - ceiling(p/2))/2p * n
		= (2 + (14 - ceiling(7/2))/14 * n
		= (2 + (14 - 4)/14 * n
		= 12n/14
		= 0.8571n
	for p = 9
		= (2 + (2p - ceiling(p/2))/2p * n
		= (2 + (18 - ceiling(9/2))/18 * n
		= (2 + (18 - 5)/18 * n
		= 15n/18
		= 0.8333n
	for p = 11
		= (2 + (2p - ceiling(p/2))/2p * n
		= (2 + (22 - ceiling(11/2))/22 * n
		= (2 + (22 - 6)/22 * n
		= 18n/22
		= 0.8181n
	for p = 13
		= (2 + (2p - ceiling(p/2))/2p * n
		= (2 + (26 - ceiling(13/2))/26 * n
		= (2 + (26 - 7)/26 * n
		= 21n/26
		= 0.8077n

	Space Complexity: O(n) if we can not modify original array
	Refer: https://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-3-worst-case-linear-time/
	*/
	int findKthSmallest_v6(const vector<int>& array, int k)
	{

	}
	int findMedian_v6(const vector<int>& array)
	{
		return 0;
	}


	MM_DECLARE_FLAG(Maths_FindMedian);

	MM_UNIT_TEST(Maths_FindMedian_test_1, Maths_FindMedian)
	{
	}
}