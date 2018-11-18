/*
Problem categories:

case 1: When we know the ball is heavier (or it is lighter):

a. Exactly 1 ball  heavier among n balls. Find 1 heavier ball.		=> if there are 3^n balls, we need n weighings.
b. Exactly 2 balls heavier among n balls. Find 2 heavier balls.	=>
c. Exactly x balls heavier among n balls. Find all x heavier balls.=>
d. Exactly x balls heavier among n balls. Find any 1 heavier ball.	=>
e. Exactly x balls heavier among n balls. Find any t heavier balls.=>

case 2: When we know the ball is odd but do not know whether heavier or lighter:

a. Exactly 1 ball  odd among n balls. Find 1 heavier ball.		=> if there are (3^n-1)/2 balls, we need n weighings.
b. Exactly 2 balls odd among n balls. Find 2 heavier balls.	=>
c. Exactly x balls odd among n balls. Find all x heavier balls.=>
d. Exactly x balls odd among n balls. Find any 1 heavier ball.	=>
e. Exactly x balls odd among n balls. Find any t heavier balls.=>


Algorithms for each case:

Case 1-a: Exactly 1 ball  heavier among n balls. Find 1 heavier ball.		=> if there are 3^n balls, we need n weighings.


	

Case 2-a: Exactly 1 ball  odd among n balls. Find 1 heavier ball.		=> if there are (3^n-1)/2 balls, we need n weighings.

Find odd ball from a group of 9 balls. You dont know if the odd ball is heavier or lighter.  
Solution:

if (1+2+3) != (4+5+6),
	if (1+2+3) != (7+8+9)
		you know the odd ball's in (1+2+3), and also you know whether it is heavier or lighter,
		so compare two of the (1+2+3) and you will have the answer in the third comparison.
		if(1 != 2)
			odd is 1 or 2
		else 
			odd is 3
	else
		the odd ball is in (4+5+6) and again you can have the answer in the third comparison.
else //(1+2+3) & (4+5+6) are same
	if (1+2+3) != (7+8+9)
		the odd ball's in (7+8+9), and again you have it in the third weighing.
	else //(1+2+3) & (7+8+9) are same
		the odd ball must be in d, 
		and without knowing whether it is heavier or lighter you cannot get it in only one more weighing.



Find odd ball from a group of 12 balls. You dont know if the odd ball is heavier or lighter.
Solution:

1. COMPARE (1, 2, 3, 4) & (5, 6, 7, 8)
IF (1, 2, 3, 4) = (5, 6, 7, 8)
	2. COMPARE (9, 10) & (11, 8)
	IF (9, 10) = (11, 8)
		12 = Odd
		3. COMPARE 12 & any other
		IF 12 < any other
			12 = light
		ELSE (12 > any other)
			12 = heavy
	ELSE IF (9, 10) < (11, 8)
		implies 9 = light / 10 = light / 11 = heavy
		3. COMPARE 9 & 10
		IF 9 = 10
			11 = heavy
		ELSE IF 9 < 10
			9 = light
		ELSE (9 > 10)
			10 = light
	ELSE ( (9, 10) > (11, 8) )
		implies 9 = heavy / 10 = heavy / 11 = light
		...same as above
ELSE IF (1, 2, 3, 4) < (5, 6, 7, 8)
	2. COMPARE (1, 2, 5) & (3, 6, 9)
	IF (1, 2, 5) = (3, 6, 9)
		implies 4 = light / 7 = heavy / 8 = heavy
		3. COMPARE 7 & 8
		IF 7 = 8
			4 = light
		ELSE IF 7 < 8
			8 = heavy
		ELSE (7 > 8)
			7 = heavy
	ELSE IF (1, 2, 5) < (3, 6, 9)
		implies 1 = light / 2 = light / 6 = heavy
		3. COMPARE 1 & 2
		IF 1 = 2
			6 = heavy
		ELSE IF 1 < 2
			1 = light
		ELSE (1 > 2)
			2 = light
	ELSE ( (1, 2, 5) > (3, 6, 9) )
		implies 1 = heavy / 2 = heavy / 6 = light
		...same as above
ELSE ( (1, 2, 3, 4) > (5, 6, 7, 8) )
	2. COMPARE (1, 2, 5) & (3, 6, 9)
	...same as above

------- OR ----------

1. COMPARE (1, 2, 3, 4) & (5, 6, 7, 8)
IF (1, 2, 3, 4) = (5, 6, 7, 8) //none of these is odd
	2. COMPARE (1, 2, 3) & (9, 10, 11)
	IF (1, 2, 3) = (9, 10, 11)
		12 is Odd
		3. COMPARE 12 & any other
		IF 12 < any other
			12 = light
		ELSE (12 > any other)
			12 = heavy
	ELSE IF (1, 2, 3) < (9, 10, 11)
		implies 9 or 10 or 11 is heavy
		3. COMPARE 9 & 10
		IF 9 = 10
			11 = heavy
		ELSE IF 9 < 10
			10 is heavy
		ELSE (9 > 10)
			9 is heavy
	ELSE // (1, 2, 3) > (9, 10, 11)
		implies 9 or 10 or 11 is light
		...same as above
ELSE IF (1, 2, 3, 4) < (5, 6, 7, 8) // (9, 10, 11, 12) all are good
	implies one of (1, 2, 3, 4) is light or one of (5, 6, 7, 8) is heavy
	2. COMPARE (1, 2, 8) & (5, 6, 4)
	IF (1, 2, 8) = (5, 6, 4)
		implies 3 is light or 7 is heavy
		3. COMPARE 3 & any other good one
		IF 3 < any other good one
			3 = light
		ELSE
			3 = good and 7 is heavy
	ELSE IF (1, 2, 8) < (5, 6, 4)
		implies 3 is light or 7 is heavy
		3. COMPARE 3 & any other good one
		IF 3 < any other good one
			3 = light
		ELSE
			3 = good and 7 is heavy
*/