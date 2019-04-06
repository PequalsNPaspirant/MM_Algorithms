
//#include <iostream>

//#include "WindowsUtils/WindowsUtils.h"
//#include "Timer/Timer_Timer.h"
//#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

//using namespace mm;

/*
Linear Recurrence Relation:
In mathematics, a recurrence relation is an equation that recursively defines a sequence
or multidimensional array of values, once one or more initial terms are given:
each further term of the sequence or array is defined as a function of the preceding terms.

A linear recurrence relation is a function or a sequence such that each term is a linear combination
of previous terms (function of degree 1). Each term can be described as a function of the previous terms.
A famous example is the Fibonacci sequence: f(n) = f(n-1) + f(n-2).
sequence: f(n) = f(n-1) * f(n-2) is not a linear recurrence.

Typically these problems can easily be broken down into small subproblems, so these can be
solved by divide and conquer or by dynamic programming if subproblems are overlapping (using memoization).
But when 'n' is very large, the above algorithms O(n) needs to go through many iterations (alogn with lot of
memoization in case of top-down dynamic programming approach or other bookkeeping/tabulation in case of
bottom-up dynamic programming) which may be practically expensive in terms of memory and CPU execution time.
We can solve it easily in O(log n) using following mathematical technique.

if	f(n) = G( f(n-1), f(n-2), ..., f(n-k))
    OR f(n) = a1 * f(n-1) + a2 * f(n-2) + ... + ak * f(n-k)
    OR f(n) = SUM_OF(ai * f(n-i)) where 1 <= i <= k
    where G is a linear function and a1, a2, ..., ak are integers including zero.
It is mandatory to provide k initial conditions, otherwise the whole sequence is undefined.
Let's say we are provided with initial values: f(1), f(2), ..., f(k)

f(k+1) = a1 * f(k) + a2 * f(k-1) + ... + ak * f(1)

We can write this in matrix form as:
f(k+1) = [a1 a2 a3 ... ak] | f(k)   |
                           | f(k-1) |
                           .
                           .
                           .
                           | f(1)   |

OR it can be written by rearranging the terms:

f(k+1) = a1 * f(k) + a2 * f(k-1) + ... + ak * f(1)

matrix form:
f(k+1) = [ak ak-1 ... a1]  | f(1)   |
                           | f(2)   |
                           .
                           .
                           .
                           | f(k)   |

Similarly,
f(k+2) = [ak ak-1 ... a1]  | f(2)   |
                           | f(3)   |
                           .
                           .
                           .
                           | f(k+1) |

If we generalize this further, we can write:

if f(k+1) = a1 * f(k) + a2 * f(k-1) + ... + ak * f(1)
then
    | f(2)     | = | 0  1  0  0  ... 0  | | f(1) |
    | f(3)     |   | 0  0  1  0  ... 0  | | f(2) |
    |    .     |   | 0  0  0  1  ... 0  | | f(3) |
    |	 .     |   |                    | |      |
    |	 .     |   |                    | |      |
    | f(k-1)   |   |                    | |      |
    | f(k)     |   |                    | |      |
    | f(k+1)   |   | ak ak-1 ak-2 ...a1 | | f(k) |
    
    OR
    
    if f(k+1) = a1 * f(1) + a2 * f(2) + ... + ak * f(k)
    then
    | f(2)     | = | 0  1  0  0  ... 0  | | f(1) |
    | f(3)     |   | 0  0  1  0  ... 0  | | f(2) |
    |    .     |   | 0  0  0  1  ... 0  | | f(3) |
    |	 .     |   |                    | |      |
    |	 .     |   |                    | |      |
    | f(k-1)   |   |                    | |      |
    | f(k)     |   |                    | |      |
    | f(k+1)   |   | a1 a2 ...   ... ak | | f(k) |

So if we have a square matrix of dimension k x k (Lets call it transformation matrix T)
    whose bottom row is [ak ak-1 ... a1]
    and top-right (k-1)x(k-1) matrix is unit matrix
    and left column has all zeros (except bottom element in this row),
We can get the row matric of dimension k x 1 as a result of matxic multiplication.
(Lets call it resultant matrix R, and its bottom most term is f(k+1), so lets call it as R(k+1))
The row matrix on RHS is also of similar format, lets call it R(k)

So, we can write it as: R(k+1) = T * R(k)
and its obvious that R(k+2) = T * R(k+1)
                            = T * T * R(k)
                            = T^2 * R(k)
                            = T^(k+2 - k) * R(k)
and finally          R(n)   = T^(n - k) * R(k)

Please note that:
R(k) has initial k terms of the sequence arranged from top to bottom in ascending order
R(n) will have last k terms of the sequence arranged from top to bottom in ascending order
T has all coefficients of first k terms in the linear recurrence function arranged from
left to right in ascending order.

T^(n - k) can be solved by exponentiation by squaring in log(n-k) multiplications i.e. in O(log n)

Examples:

Fibonacci sequence:
f(n) = 1 * f(n-1) + 1 * f(n-2)
     = 1 * f(n-2) + 1 * f(n-1)  ... first k terms arranged in ascending order (k = 2)
OR we can say
f(3) = 1 * f(1) + 1 * f(2)
First two terms: f(1) = 0, f(2) = 1
Thus,
    | f(n-1) | = | 0  1 | ^ (n - 2) * | f(1) |
    | f(n)   |   | 1  1 |             | f(2) |
    
    | f(n-1) | = | 0  1 | ^ (n - 2) * |  0   |
    | f(n)   |   | 1  1 |             |  1   |

*/

