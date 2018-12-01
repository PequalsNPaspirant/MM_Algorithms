#include <iostream>
#include <string>
#include <unordered_set>
#include <memory>
#include <cassert> //for assert()
using namespace std;

#include "Others/Others_CarRentalSystem.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//Abstract class Car
	class Car
	{
	public:
		//A car can not exist without a licence plate number, brand and some default number of seats
		Car(const string& licencePlateNumber, const string& brand, int numDefaultSeats) 
			: defaultSeats_(numDefaultSeats), 
			licencePlateNumber_(licencePlateNumber),
			brand_(brand),			
			isRented_(false)
		{

		}
		virtual ~Car() = 0
		{}

		virtual int getMaxPassangers() const = 0;
		const string& getLicencePlateNumber() const
		{
			return licencePlateNumber_;
		}

		const string& getBrand() const
		{
			return brand_;
		}

		void setIsRented(bool flag)
		{
			isRented_ = flag;
		}

		bool getIsRented() const
		{
			return isRented_;
		}

	protected:
		int defaultSeats_;

	private:
		string licencePlateNumber_; //licence plate number may be alphanumeric
		string brand_;
		bool isRented_;
	};

	class SUV : public Car
	{
	public:
		SUV(const string& licencePlateNumber, const string& brand, int numDefaultSeats)
			: Car(licencePlateNumber, brand, numDefaultSeats),
			hasThirdRow_(false)
		{}

		int getMaxPassangers() const override
		{
			if (hasThirdRow_)
				return defaultSeats_ + 3; //third row has additional 3 seats
			else
				return defaultSeats_;
		}

		void addThirdRow()
		{
			hasThirdRow_ = true;
		}

		void removeThirdRow()
		{
			hasThirdRow_ = false;
		}

	private:
		bool hasThirdRow_;
	};

	class Sedan : public Car
	{
	public:
		Sedan(const string& licencePlateNumber, const string& brand, int numDefaultSeats)
			: Car(licencePlateNumber, brand, numDefaultSeats),
			hasSportsPackage_(false)
		{}

		int getMaxPassangers() const override
		{
			return defaultSeats_;
		}

		void addSportsPackage()
		{
			hasSportsPackage_ = true;
		}

		void removeSportsPackage()
		{
			hasSportsPackage_ = false;
		}

	private:
		bool hasSportsPackage_;
	};

	class CarRental
	{
	public:
		CarRental()
			: totalCars_(0),
			carsRentedOut_(0)
		{}
		bool addCar(unique_ptr<Car>&& car)
		{
			std::pair<unordered_set<unique_ptr<Car>>::iterator, bool> result
				= carCollection_.insert(std::move(car));
			if (result.second) //true is car is successfully added, false if car is already present in collection
			{
				++totalCars_;
				return true;
			}

			return false;
		}
		bool removeCar(const string& licencePlateNumber)
		{
			unique_ptr<Car> dummyCarPtr = make_unique<SUV>(licencePlateNumber, "", 0);
			unordered_set<unique_ptr<Car>>::size_type numCarsErased = carCollection_.erase(dummyCarPtr);
			totalCars_ -= numCarsErased;

			if (numCarsErased != 0)
				return true;
			else
				return false;
		}

		bool isCarAvailableForRent(const string& licencePlateNumber) const
		{
			unique_ptr<Car> dummyCarPtr = make_unique<SUV>(licencePlateNumber, "", 0);
			auto it = carCollection_.find(dummyCarPtr);
			if (it != carCollection_.end())
				return true;
			else
				return false;
		}

		bool rentOutCarIfAvailable(const string& licencePlateNumber)
		{
			unique_ptr<Car> dummyCarPtr = make_unique<SUV>(licencePlateNumber, "", 0);
			auto it = carCollection_.find(dummyCarPtr);
			if (it != carCollection_.end())
			{
				++carsRentedOut_;
				(*it)->setIsRented(true);
				return true;
			}
			else
				return false;
		}

		const unique_ptr<Car>& getCar(const string& licencePlateNumber)
		{
			unique_ptr<Car> dummyCarPtr = make_unique<SUV>(licencePlateNumber, "", 0);
			auto it = carCollection_.find(dummyCarPtr);
			if (it != carCollection_.end())
				return *it;
			else
				return nullptr;
		}

		int getTotalCars()
		{
			return totalCars_;
		}
		int getTotalCarsRentedOut()
		{
			return carsRentedOut_;
		}
		int getCarsAvailableForRent()
		{
			return totalCars_ - carsRentedOut_;
		}

	private:
		struct carHasher
		{
			size_t operator()(const unique_ptr<Car>& carPtr) const noexcept
			{
				return std::hash<string>{}(carPtr->getLicencePlateNumber());
			}
		};
		struct checkCarEquality
		{
			bool operator()(const unique_ptr<Car>& carPtrLhs, const unique_ptr<Car>& carPtrRhs) const noexcept
			{
				return carPtrLhs->getLicencePlateNumber() == carPtrRhs->getLicencePlateNumber();
			}
		};

		//store cars in a hash set hashing done based on Licence Plate Number
		unordered_set<unique_ptr<Car>, carHasher, checkCarEquality> carCollection_; 

		int totalCars_;
		int carsRentedOut_;
	};

	//Testing:

	void testCarRentalSystem()
	{
		SUV suv("ABC123", "BMW", 5);
		assert(suv.getMaxPassangers() == 5);
		suv.addThirdRow();
		assert(suv.getMaxPassangers() == 8);
		suv.removeThirdRow();
		assert(suv.getMaxPassangers() == 5);

		Sedan sedan("ABC123", "BMW", 5);
		assert(sedan.getMaxPassangers() == 5);
		sedan.addSportsPackage();
		assert(sedan.getMaxPassangers() == 5);

		CarRental carRentalShop;
		assert(carRentalShop.addCar(make_unique<SUV>("ABC123", "BMW", 5)) == true); //Car added successfully
		assert(carRentalShop.addCar(make_unique<Sedan>("PQR456", "Honda", 6)) == true); //Car added successfully
		assert(carRentalShop.addCar(make_unique<SUV>("XYZ789", "Toyota", 6)) == true); //Car added successfully
		assert(carRentalShop.addCar(make_unique<Sedan>("abc111", "Lexus", 5)) == true); //Car added successfully
		assert(carRentalShop.addCar(make_unique<SUV>("XYZ789", "Toyota", 6)) == false); //Car already exists in collection
		assert(carRentalShop.addCar(make_unique<Sedan>("PQR456", "Honda", 6)) == false); //Car already exists  in collection
		assert(carRentalShop.getTotalCars() == 4);
		assert(carRentalShop.getCarsAvailableForRent() == 4);
		assert(carRentalShop.getTotalCarsRentedOut() == 0);

		assert(carRentalShop.isCarAvailableForRent("PQR456") == true);
		assert(carRentalShop.rentOutCarIfAvailable("PQR456") == true);
		assert(carRentalShop.getCar("PQR456")->getIsRented() == true);

		assert(carRentalShop.getTotalCars() == 4);
		assert(carRentalShop.getCarsAvailableForRent() == 3);
		assert(carRentalShop.getTotalCarsRentedOut() == 1);
	}

	MM_DECLARE_FLAG(Others_CarRentalSystem);

	MM_UNIT_TEST(Others_CarRentalSystem_sanity, Others_CarRentalSystem)
	{
		testCarRentalSystem();
	}
}