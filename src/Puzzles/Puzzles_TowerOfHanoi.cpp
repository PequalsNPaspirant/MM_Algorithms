#include <stack>
#include <vector>
#include <cassert>
#include <cmath>
using namespace std;

#include "Puzzles/Puzzles_TowerOfHanoi.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"


/*
There are n disks. disk 1 is smallest and disk n is the largest disk.
There are three poles with index 1, 2 and 3.
The number of moves required is: 2^n – 1
*/

namespace mm
{
	/*
	This recursive function will be called 2^n times where n = numDisks
	Time Complexity: O(2^n)
	Space Complexity: O(n) for function stack
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(int numDisks, int from, int to, int aux)
	{
		if (numDisks == 0)
			return 0;

		unsigned long long numMoves = 0;
		numMoves += Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(numDisks - 1, from, aux, to);
		//Move top disk: from -> to
		//We can display it on screen
		//cout << "Move disk " << numDisks << " from " << from << " to " << to;
		numMoves += 1;
		numMoves += Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(numDisks - 1, aux, to, from);
		return numMoves;
	}

	/*
	This recursive function will be called 2^n times where n = numDisks
	Time Complexity: O(2^n)
	Space Complexity: O(n) for function stack and the stacks used to store disks
	*/
	unsigned long long Puzzles_TowerOfHanoi_recursive_v1(int numDisks, stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		if (numDisks == 0)
			return 0;

		unsigned long long numMoves = 0;
		numMoves += Puzzles_TowerOfHanoi_recursive_v1(numDisks - 1, from, aux, to);
		//Move top disk: from -> to
		//We can display it on screen
		//cout << "Move disk " << numDisks << " from " << from << " to " << to;
		//OR we can store it in stack
		int disk = from.top();
		from.pop();
		assert(to.empty() || to.top() > disk);
		to.push(disk);
		numMoves += 1;
		numMoves += Puzzles_TowerOfHanoi_recursive_v1(numDisks - 1, aux, to, from);
		return numMoves;
	}

	/*
	Source: https://en.wikipedia.org/wiki/Tower_of_Hanoi
	Iterative solution:

	A simple solution for the toy puzzle is to alternate moves between the smallest piece and a non-smallest piece. 
	When moving the smallest piece, always move it to the next position in the same direction (to the right if the 
	starting number of pieces is even, to the left if the starting number of pieces is odd). If there is no tower 
	position in the chosen direction, move the piece to the opposite end, but then continue to move in the correct 
	direction. For example, if you started with three pieces, you would move the smallest piece to the opposite end, 
	then continue in the left direction after that. When the turn is to move the non-smallest piece, there is only 
	one legal move. Doing this will complete the puzzle in the fewest moves.

	Simpler statement of iterative solution
	For an even number of disks:

	make the legal move between pegs A and B (in either direction),
	make the legal move between pegs A and C (in either direction),
	make the legal move between pegs B and C (in either direction),
	repeat until complete.
	For an odd number of disks:

	make the legal move between pegs A and C (in either direction),
	make the legal move between pegs A and B (in either direction),
	make the legal move between pegs B and C (in either direction),
	repeat until complete.
	In each case, a total of 2n − 1 moves are made.

	Time Complexity: O(2^n)
	Space Complexity: O(1)

	NOTE: This approach is same as Puzzles_TowerOfHanoi_binary_v2
	*/
	unsigned long long Puzzles_TowerOfHanoi_iterative_v1(stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		//This approach is same as Puzzles_TowerOfHanoi_binary_v2()
		//return Puzzles_TowerOfHanoi_binary_v2(from, to, aux);
		return 0;
	}

	/*
	Source: https://en.wikipedia.org/wiki/Tower_of_Hanoi

	Equivalent iterative solution
	Another way to generate the unique optimal iterative solution:

	Number the disks 1 through n (largest to smallest).

	If n is odd, the first move is from peg A to peg C.
	If n is even, the first move is from peg A to peg B.
	Now, add these constraints:

	No odd disk may be placed directly on an odd disk.
	No even disk may be placed directly on an even disk.
	There will sometimes be two possible pegs: one will have disks, and the other will be empty. Place the disk on the non-empty peg.
	Never move a disk twice in succession.
	Considering those constraints after the first move, there is only one legal move at every subsequent turn.

	The sequence of these unique moves is an optimal solution to the problem equivalent to the iterative solution described above.

	Time Complexity: O(2^n)
	Space Complexity: O(1)
	*/
	unsigned long long Puzzles_TowerOfHanoi_iterative_v2(stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		size_t numDisks = from.size();
		if (numDisks == 0)
			return 0;

		unsigned long long totalMoves = 0;
		if (numDisks < sizeof(unsigned long long) * 8)
			totalMoves = (1ull << numDisks) - 1;
		else
			totalMoves = numeric_limits<unsigned long long>::max();

		stack<int>* source = &from;
		stack<int>* destination = &to;
		stack<int>* auxiliary = &aux;
		//If number of disks is even, then interchange  
		//destination pole and auxiliary pole 
		if ((numDisks & 1) == 0)
		{
			stack<int>* temp = destination;
			destination = auxiliary;
			auxiliary = temp;
		}



		return totalMoves;
	}

	/*
	This recursive function will be called n times where n = numDisks. It keeps previously calculated values in cache.
	Time Complexity: O(n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(int numDisks, int from, int to, int aux, vector<unsigned long long>& cache)
	{
		if (numDisks == 0)
			return 0;

		if(cache[numDisks] == 0)
		{
			unsigned long long numMoves = 0;
			numMoves += Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(numDisks - 1, from, aux, to, cache);
			//Move top disk: from -> to
			//We can display it on screen. But due to memoization, it does not display all (2^n - 1) steps.
			//cout << "Move disk " << numDisks << " from " << from << " to " << to;
			numMoves += 1;
			numMoves += Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(numDisks - 1, aux, to, from, cache);
			cache[numDisks] = numMoves;
		}

		return cache[numDisks];
	}

	/*
	Time Complexity: O(n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves_DP_bottomup_v1(int numDisks, int from, int to, int aux, vector<unsigned long long>& cache)
	{
		//For 0 disks, the number of moves are 0
		cache[0] = 0;

		//For 1 disk, the number of moves are 1
		//cache[1] = 1;

		for (int i = 1; i <= numDisks; ++i)
		{
			//the pattern is derived from the recursive approach.
			//For every disk, we need twice as many moves as required for (disks - 1) and one extra move to place last disk.
			cache[i] = cache[i-1] * 2 + 1; 
		}

		return cache[numDisks];
	}

	/*
	Time Complexity: O(1)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves(int numDisks)
	{
		//double can not keep big integer part of values precisely
		//return pow(2, numDisks) - 1;

		//This function can calculate num moves for disks upto 64
		assert(numDisks <= sizeof(unsigned long long) * 8);

		if (numDisks < sizeof(unsigned long long) * 8)
			return (1ull << numDisks) - 1;
		else
			return numeric_limits<unsigned long long>::max();
	}

	/*
	Source: https://en.wikipedia.org/wiki/Tower_of_Hanoi
	Furthermore, the disk to be moved is determined by the number of times the move count (m) can be divided by 2 (i.e. the number of 
	zero bits at the right), counting the first move as 1 and identifying the disks by the numbers 0, 1, 2 etc. in order of increasing size. 
	This permits a very fast non-recursive computer implementation to find the positions of the disks after m moves without reference to any 
	previous move or distribution of disks.
	The operation, which counts the number of consecutive zeros at the end of a binary number, gives a simple solution to the problem: 
	the disks are numbered from zero, and at move m, disk number count trailing zeros is moved the minimal possible distance to 
	the right (circling back around to the left as needed).

	0000 - initial condition
	0001 - move disk 1
	0010 - move disk 2
	0011 - move disk 1
	0100 - move disk 3
	0101 - move disk 1
	0110 - move disk 2
	0111 - move disk 1
	1000 - move disk 4
	1001 - move disk 1
	1010 - move disk 2
	1011 - move disk 1
	1100 - move disk 3
	1101 - move disk 1
	1110 - move disk 2
	1111 - move disk 1

	Time Complexity: O(2^n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_binary_v1(stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		size_t numDisks = from.size();
		if (numDisks == 0)
			return 0;

		unsigned long long totalMoves = 0;
		if (numDisks < sizeof(unsigned long long) * 8)
			totalMoves = (1ull << numDisks) - 1;
		else
			totalMoves = numeric_limits<unsigned long long>::max();

		stack<int>* source = &from;
		stack<int>* destination = &to;
		stack<int>* auxiliary = &aux;
		//If number of disks is even, then interchange  
		//destination pole and auxiliary pole 
		if ((numDisks & 1) == 0)
		{
			stack<int>* temp = destination;
			destination = auxiliary;
			auxiliary = temp;
		}

		/*
		Sources: 
		https://en.wikipedia.org/wiki/De_Bruijn_sequence
		https://en.wikipedia.org/wiki/Find_first_set#CTZ
		http://supertech.csail.mit.edu/papers/debruijn.pdf
		http://graphics.stanford.edu/~seander/bithacks.html
		https://stackoverflow.com/questions/757059/position-of-least-significant-bit-that-is-set/757266

		In combinatorial mathematics, a de Bruijn sequence of order n on a size-k alphabet A is a cyclic sequence in which 
		every possible length-n string on A occurs exactly once as a substring (i.e., as a contiguous subsequence). 
		Such a sequence is denoted by B(k, n) and has length k^n, which is also the number of distinct substrings of length n on A; 
		de Bruijn sequences are therefore optimally short. There are [(k!)^(k^n)] / (k^n) distinct de Bruijn sequences B(k, n).

		The sequence can be used to shorten a brute-force attack on a PIN-like code lock that does not have an "enter" key and 
		accepts the last n digits entered. For example, a digital door lock with a 4-digit code would have B (10, 4) solutions, 
		with length 10000. Therefore, only at most 10000 + 3 = 10003 (as the solutions are cyclic) presses are needed to open the lock. 
		Trying all codes separately would require 4 × 10000 = 40000 presses.

		The symbols of a de Bruijn sequence written around a circular object (such as a wheel of a robot) can be used to identify its angle by 
		examining the n consecutive symbols facing a fixed point. This angle-encoding problem is known as the "rotating drum problem". 
		Gray codes can be used as similar rotary positional encoding mechanisms.

		A de Bruijn sequence can be used to quickly find the index of the least significant set bit ("right-most 1") or 
		the most significant set bit ("left-most 1") in a word using bitwise operations. An example of returning the index of 
		the least significant bit from a 32 bit unsigned integer is given below using bit manipulation and multiplication.

		//for i from 0 to 31: MultiplyDeBruijnBitPosition[ (0x077CB531 * (1 << i)) >> 27 ] = i
		//OR MultiplyDeBruijnBitPosition[ (0x077CB531 << i) >> 27 ] = i
		static const int MultiplyDeBruijnBitPosition[32] =
		{
			0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
			31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
		};
		unsigned int v;
		int r = MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27];
		//OR int r = MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27];

		If v has no set bits the operation returns 0.
		The constant, 0x077CB531U, in the expression is the B (2, 5) sequence 0000 0111 0111 1100 1011 0101 0011 0001 (spaces added for clarity).

		B(2, 1) = 01 = {0, 1}
		B(2, 2) = 00110 = {00, 01, 11, 10}
		B(2, 3) = 0001110100 = {000, 001, 011, 111, 110, 101, 010, 100}

		v & -v gives the number with only the least significant bit ON.
		-v is same as ~v + 1....refer 2's compliment technique to store negative numbers
		Source: https://en.wikipedia.org/wiki/Two%27s_complement
		The two's complement of an N-bit number is defined as its complement with respect to 2N. 
		For instance, for the three-bit number 010, the two's complement is 110, because 010 + 110 = 1000.
		Compared to other systems for representing signed numbers (e.g., ones' complement), two's complement has the advantage that 
		the fundamental arithmetic operations of addition, subtraction, and multiplication are identical to those for unsigned 
		binary numbers (as long as the inputs are represented in the same number of bits - as the output, and any overflow beyond 
		those bits is discarded from the result). This property makes the system simpler to implement, especially for higher-precision arithmetic. 
		Unlike ones' complement systems, two's complement has no representation for negative zero, and thus does not suffer from its 
		associated difficulties.
		Conveniently, another way of finding the two's complement of a number is to take its ones' complement and add one: the sum of a number and 
		its ones' complement is all '1' bits, or 2^N − 1; and by definition, the sum of a number and its two's complement is 2^N.

		The ones' complement of a binary number is defined as the value obtained by inverting all the bits in the binary representation 
		of the number (swapping 0s for 1s and vice versa). The ones' complement behaves like the negative of the original number with binary addition.

		An N-bit ones' complement numeral system can only represent integers in the range −(2^(N−1) − 1) to 2^(N−1) − 1 while two's complement can 
		express −2^(N−1) to 2^(N−1) − 1.

		Examples of some 8 bit numbers:
		----------------------------------------------------
		Number     One's compliment      Two's compliment
		----------------------------------------------------
		-128       can not represent     1000 0000
		-127       1000 0000             1000 0001
		-126       1000 0001             1000 0010
		-125       1000 0010             1000 0011

		-3         1111 1100             1111 1101
		-2         1111 1101             1111 1110
		-1         1111 1110             1111 1111
		-0         1111 1111             can not represent

		+0         0000 0000             0000 0000

		+1         0000 0001             0000 0001
		+2         0000 0010             0000 0010
		+3         0000 0011             0000 0011

		+125       0111 1101             0111 1101
		+126       0111 1110             0111 1110
		+127       0111 1111             0111 1111
		+128       can not represent     can not represent
		----------------------------------------------------

		*/

		static unsigned int MultiplyDeBruijnBitPosition[32];
		static unsigned int MultiplyDeBruijnBitPosition2[32];
		static bool initialized = false;
		if (!initialized)
		{
			for (int i = 0; i < 32; ++i)
			{
				MultiplyDeBruijnBitPosition[(0x077CB531u * (1 << i)) >> 27] = i;
				MultiplyDeBruijnBitPosition2[(0x077CB531u << i) >> 27] = i;
			}

			initialized = true;
		}

		stack<int>* stacks[3] = { source, destination, auxiliary };
		for (int v = 1; v <= totalMoves; ++v)
		{
			//Find the Least Significant Bit position from right. Rightmost bit position is 0.
			int bitPosition = MultiplyDeBruijnBitPosition2[((uint32_t)((v & -v) * 0x077CB531U)) >> 27];
			int disk = bitPosition + 1; //bit position starts from 0, but disks start from 1
			
			for (int i = 0; i < 3; ++i)
			{
				if (!stacks[i]->empty() && stacks[i]->top() == disk)
				{
					stack<int>* target = nullptr;
					int targetIndex = (i + 1) % 3;
					if (stacks[targetIndex]->empty() || stacks[targetIndex]->top() > disk)
						target = stacks[targetIndex];
					else
						target = stacks[(i + 2) % 3];

					assert(target->empty() || target->top() > stacks[i]->top());
					//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
					//cout << "Move disk " << disk << " from " << stacks[i]->getID() << " to " << target->getID();
					target->push(disk);
					stacks[i]->pop();
					
					break;
				}
			}
		}

		return totalMoves;
	}

	/*
	Time Complexity: O(2^n)
	Space Complexity: O(n)

	NOTE: Please check the comments for Puzzles_TowerOfHanoi_iterative_v1 above.
	*/
	unsigned long long Puzzles_TowerOfHanoi_binary_v2(stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		size_t numDisks = from.size();
		if (numDisks == 0)
			return 0;

		stack<int>* source = &from;
		stack<int>* destination = &to;
		stack<int>* auxiliary = &aux;
		//If number of disks is even, then interchange  
		//destination pole and auxiliary pole 
		if ((numDisks & 1) == 0)
		{
			stack<int>* temp = destination;
			destination = auxiliary;
			auxiliary = temp;
		}

		unsigned long long totalMoves = 0;
		if (numDisks < sizeof(unsigned long long) * 8)
			totalMoves = (1ull << numDisks) - 1;
		else
			totalMoves = numeric_limits<unsigned long long>::max();

		auto moveDisk = [](stack<int>* fromStack, stack<int>* toStack) {
			if (fromStack->empty() && toStack->empty())
				assert(false);
			else if (fromStack->empty())
			{
				//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
				//cout << "Move disk " << toStack->top() << " from " << toStack->getID() << " to " << fromStack->getID();
				fromStack->push(toStack->top());
				toStack->pop();
			}
			else if (toStack->empty())
			{
				//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
				//cout << "Move disk " << fromStack->top() << " from " << fromStack->getID() << " to " << toStack->getID();
				toStack->push(fromStack->top());
				fromStack->pop();
			}
			else if (fromStack->top() < toStack->top())
			{
				//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
				//cout << "Move disk " << fromStack->top() << " from " << fromStack->getID() << " to " << toStack->getID();
				toStack->push(fromStack->top());
				fromStack->pop();
			}
			else if (fromStack->top() > toStack->top())
			{
				//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
				//cout << "Move disk " << toStack->top() << " from " << toStack->getID() << " to " << fromStack->getID();
				fromStack->push(toStack->top());
				toStack->pop();
			}
		};

		for (int i = 1; i <= totalMoves; ++i)
		{
			if (i % 3 == 1)
				moveDisk(source, destination);
			else if (i % 3 == 2)
				moveDisk(source, auxiliary);
			else if (i % 3 == 0)
				moveDisk(auxiliary, destination);
		}

		return totalMoves;
	}


	/*
	Source: https://en.wikipedia.org/wiki/Tower_of_Hanoi
	The source and destination pegs for the mth move can also be found elegantly from the binary representation of m using bitwise operations.
	To use the syntax of the C programming language, move m is from peg (m & m - 1) % 3 to peg ((m | m - 1) + 1) % 3, where the disks begin
	on peg 0 and finish on peg 1 or 2 according as whether the number of disks is even or odd. 

	Time Complexity: O(2^n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_binary_v3(stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		size_t numDisks = from.size();
		if (numDisks == 0)
			return 0;

		stack<int>* source = &from;
		stack<int>* destination = &to;
		stack<int>* auxiliary = &aux;
		//If number of disks is even, then interchange  
		//destination pole and auxiliary pole 
		if ((numDisks & 1) == 0)
		{
			stack<int>* temp = destination;
			destination = auxiliary;
			auxiliary = temp;
		}

		unsigned long long totalMoves = 0;
		if (numDisks < sizeof(unsigned long long) * 8)
			totalMoves = (1ull << numDisks) - 1;
		else
			totalMoves = numeric_limits<unsigned long long>::max();

		stack<int>* stacks[3] = { source, auxiliary, destination }; //Please note the sequence, the destination stack is at last
		for (int m = 1; m <= totalMoves; ++m)
		{
			//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
			//cout << "Move disk " << stacks[(m & m - 1) % 3]->top() << " from " << stacks[(m & m - 1) % 3]->getID() << " to " << stacks[((m | m - 1) + 1) % 3]->getID();
			stacks[((m | m - 1) + 1) % 3]->push(stacks[(m & m - 1) % 3]->top());
			stacks[(m & m - 1) % 3]->pop();
		}

		return totalMoves;
	}

	/*
	Source: https://en.wikipedia.org/wiki/Tower_of_Hanoi
	Another formulation is from peg (m - (m & -m)) % 3 to peg (m + (m & -m)) % 3.

	Time Complexity: O(2^n)
	Space Complexity: O(n)
	*/

	unsigned long long Puzzles_TowerOfHanoi_binary_v4(stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		size_t numDisks = from.size();
		if (numDisks == 0)
			return 0;

		stack<int>* source = &from;
		stack<int>* destination = &to;
		stack<int>* auxiliary = &aux;
		//If number of disks is even, then interchange  
		//destination pole and auxiliary pole 
		if ((numDisks & 1) == 0)
		{
			stack<int>* temp = destination;
			destination = auxiliary;
			auxiliary = temp;
		}

		unsigned long long totalMoves = 0;
		if (numDisks < sizeof(unsigned long long) * 8)
			totalMoves = (1ull << numDisks) - 1;
		else
			totalMoves = numeric_limits<unsigned long long>::max();

		stack<int>* stacks[3] = { source, auxiliary, destination }; //Please note the sequence, the destination stack is at last
		for (int m = 1; m <= totalMoves; ++m)
		{
			//We can display the move on screen. Assuming, we have ID with each stack OR we can also display address of stack.
			//cout << "Move disk " << stacks[(m - (m & -m)) % 3]->top() << " from " << stacks[(m - (m & -m)) % 3]->getID() << " to " << stacks[(m + (m & -m)) % 3]->getID();
			stacks[(m + (m & -m)) % 3]->push(stacks[(m - (m & -m)) % 3]->top());
			stacks[(m - (m & -m)) % 3]->pop();
		}

		return totalMoves;
	}

	// Test
	MM_DECLARE_FLAG(Puzzles_TowerOfHanoi_test);

	MM_UNIT_TEST(Puzzles_TowerOfHanoi_test_1, Puzzles_TowerOfHanoi_test)
	{
		//MM_PRINT_TEST_RESULTS_ON_SINGLE_LINE(true);
		MM_PRINT_TEST_RESULTS_ON_SINGLE_LINE(false);

		struct testData
		{
			int numDisks;
			unsigned long long expectedMoves;

		};

		vector<testData> data{
		{ 0,  0 },
		{ 1,  2 - 1 },
		{ 2,  4 - 1 },
		{ 3,  8 - 1 },
		{ 4,  16 - 1 },
		{ 5,  32 - 1 },
		{ 6,  64 - 1 },
		{ 7,  128 - 1 },
		{ 8,  256 - 1 },
		{ 9,  512 - 1 },
		{ 10, 1024 - 1 },
		{ 11, 2048 - 1 },
		{ 12, 4096 - 1 },
		{ 13, 8192 - 1 },
		{ 14, 16384 - 1 },
		{ 15, 32768 - 1 },
		{ 16, 65536 - 1 },
		{ 17, 131072 - 1 },
		{ 18, 262144 - 1 },
		{ 19, 524288 - 1 },
		{ 20, 1048576 - 1 },
		{ 21, 2097152 - 1 },
		{ 22, 4194304 - 1 },
		{ 23, 8388608 - 1 },
		{ 24, 16777216 - 1 },
		{ 25, 33554432 - 1 },
		{ 26, 67108864 - 1 },
		{ 27, 134217728 - 1 },
		{ 28, 268435456 - 1 },
		{ 29, 536870912 - 1 },
		{ 30, 1073741824 - 1 },
		{ 31, 2147483648 - 1 },

		{ 50, 1125899906842624 - 1 },
		{ 60, 1152921504606846976 - 1 },
		{ 64, numeric_limits<unsigned long long>::max() },
		};

		auto validate = [](stack<int>& st) -> bool {
			for (int j = 1; j <= st.size(); ++j)
				if (st.top() == j)
					st.pop();
				else
					return false;

			return true;
		};

		for (int i = 0; i < data.size(); ++i)
		{
			unsigned long long actualMoves = 0;
			if (data[i].numDisks <= 25)
			{
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(data[i].numDisks, 1, 2, 3)) == data[i].expectedMoves,
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			if(data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_recursive_v1(data[i].numDisks, from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			//NOTE: Puzzles_TowerOfHanoi_iterative_v1 is same as Puzzles_TowerOfHanoi_binary_v2

			//{
			//	stack<int> from;
			//	stack<int> to;
			//	stack<int> aux;
			//	for (int j = data[i].numDisks; j > 0; --j)
			//		from.push(j);
			//	MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_iterative_v2(from, to, aux)) == data[i].expectedMoves
			//		&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
			//		data[i].numDisks, actualMoves, data[i].expectedMoves);
			//}

			{
				vector<unsigned long long> cache(data[i].numDisks + 1, 0);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(data[i].numDisks, 1, 2, 3, cache)) == data[i].expectedMoves,
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			{
				vector<unsigned long long> cache(data[i].numDisks + 1, 0);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves_DP_bottomup_v1(data[i].numDisks, 1, 2, 3, cache)) == data[i].expectedMoves,
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves(data[i].numDisks)) == data[i].expectedMoves,
				data[i].numDisks, actualMoves, data[i].expectedMoves);

			if (data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_binary_v1(from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			if (data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_binary_v2(from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			if (data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_binary_v3(from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			if (data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_binary_v4(from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

		}
	}
}