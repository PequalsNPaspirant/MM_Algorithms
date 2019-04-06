//Goal:
//Converting Derived* → Base* works okay; why doesn’t Derived** → Base** work? (From C++ FAQ section)

#include <iostream>
using namespace std;

namespace mm {

	class Vehicle {
	public:
		virtual ~Vehicle() { }
		virtual void startEngine() = 0;
	};
	class Car : public Vehicle {
	public:
		virtual void startEngine() {};
		virtual void openGasCap() {};
	};
	class NuclearSubmarine : public Vehicle {
	public:
		virtual void startEngine() {};
		virtual void fireNuclearMissile() {};
	};

	void CastDerivedPtrToPtr()
	{
		Car*  carPtr = new Car;
		NuclearSubmarine* nuclearSubmarinePtr = new NuclearSubmarine;

		Vehicle* vehiclePtr = carPtr;
		vehiclePtr = nuclearSubmarinePtr;

		Vehicle** vehiclePtrPtr = nullptr;
		//	Vehicle** vehiclePtrPtr = &carPtr;  // This is an error in C++
		*vehiclePtrPtr = nuclearSubmarinePtr;
		// This last line would have caused carPtr to point to sub !
		carPtr->openGasCap();  // This might call fireNuclearMissile()!
							   // ...
	}

}

