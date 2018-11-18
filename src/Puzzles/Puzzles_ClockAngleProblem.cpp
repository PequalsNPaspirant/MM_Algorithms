#include <cmath>
using namespace std;

#include "Puzzles/Puzzles_ClockAngleProblem.h"

namespace mm {

	double calculateAngleBetweenHrAndMinHands(const unsigned int hr, const unsigned int min)
	{
		double hrHandSpeed = 360.0 / (12 * 60);  //in degrees per min
		double minHandSpeed = 360.0 / 60;   //in degrees per min

		double angle = fabs(min * minHandSpeed - (hr * 60 + min) * hrHandSpeed);

		while (angle > 360)
			angle -= 360;

		if (angle > 180)
			angle = 360 - angle;

		return angle;
	}

	void q_puzzles_01_ClockAngleProblem()
	{
		double d1 = calculateAngleBetweenHrAndMinHands(0, 0);
		double d2 = calculateAngleBetweenHrAndMinHands(5, 30);
		double d3 = calculateAngleBetweenHrAndMinHands(9, 0);
		double d4 = calculateAngleBetweenHrAndMinHands(12, 0);
		double d5 = calculateAngleBetweenHrAndMinHands(8, 20);
		double d6 = calculateAngleBetweenHrAndMinHands(20, 20);
		double d7 = calculateAngleBetweenHrAndMinHands(0, 0);
		double d8 = calculateAngleBetweenHrAndMinHands(0, 0);
		double d9 = calculateAngleBetweenHrAndMinHands(0, 0);
		double d10 = calculateAngleBetweenHrAndMinHands(0, 0);
		double d11 = calculateAngleBetweenHrAndMinHands(0, 0);
	}

}
