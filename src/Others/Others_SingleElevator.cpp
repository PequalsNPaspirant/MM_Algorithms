#include <vector>
#include <queue>
#include <thread>
using namespace std;

#include "Others/Others_SingleElevator.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*


	*/

	class Elevator
	{

	public:
		enum class Direction { notMoving, up, down };

		Elevator(int totalFloors)
			: currentFloor(0),
			currentDirection(Direction::notMoving),
			floorsToVisit(totalFloors, 0)
		{
		}

		// This function can be called from many different threads that represents people inside/outside Elevator
		// The reuests are added by people inside as well as outside Elevator
		void receiveRequest(int destinationFloor)
		{
			allRequests.push(destinationFloor);
			floorsToVisit[destinationFloor] = 1; // Elevator needs to go to this floor, so set flag 1
			if(currentDirection == Direction::notMoving)
				startMoving();
		}

		// This function starts Elevator moving
		void startMoving()
		{
			while (!allRequests.empty())
			{
				int destination = allRequests.front();
				allRequests.pop();
				if (floorsToVisit[destination] == 1)
				{
					currentDirection = currentFloor < destination ? Direction::up : Direction::down;
					int floorNumber = currentFloor < destination ? currentFloor + 1 : currentFloor - 1;
					for (; floorNumber <= destination; ++floorNumber)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Elevator takes 200 ms to travel one level up/down
						checkIfElevatorShouldStopHere(floorNumber);
					}
				}
			}

			// When all requests are processed and there is no request in queue, stop Elevator
			currentDirection = Direction::notMoving;
		}

		// The current position is continuously updated by some other external component when Elevator is moving
		void checkIfElevatorShouldStopHere(int floorNumber)
		{
			currentFloor = floorNumber;
			if (floorsToVisit[floorNumber] == 1)
			{
				floorsToVisit[floorNumber] = 0; // Elevator reached here, so set flag 0
				openDoors();
				std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Let people come in OR go out
				closeDoors();
			}
		}

		void openDoors()
		{
		}
		void closeDoors()
		{
		}

		int getCurrentFloor()
		{
			return currentFloor;
		}

	private:
		vector<int> floorsToVisit;
		queue<int> allRequests;
		int currentFloor;
		Direction currentDirection;
	};
	
	
	MM_DECLARE_FLAG(simpleElevatorUnitTest);

	MM_UNIT_TEST(simpleElevatorUnitTest_sanity, simpleElevatorUnitTest)
	{
		Elevator ele(5);
		ele.receiveRequest(3);
		int currentFloor = ele.getCurrentFloor();
		MM_EXPECT_TRUE(currentFloor == 3, currentFloor);

	}

}