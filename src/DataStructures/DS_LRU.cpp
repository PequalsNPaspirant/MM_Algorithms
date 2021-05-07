//Goal
//Create a task container and executor identified by string/enum

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "DataStructures/DS_LRU.h"

namespace mm {

	namespace LRU_v1 {


		void test()
		{

		}

	}

	MM_DECLARE_FLAG(LRU_v1);

	MM_UNIT_TEST(LRU_v1_test1, LRU_v1)
	{
		LRU_v1::test();
	}
}

