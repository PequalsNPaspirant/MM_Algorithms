
#include "Utils/Utils_PrintUtils.h"

namespace mm {

	// This causes ambiguity with standard defined operator<< for bool
	// If you want this, set cout << std::boolalpha
	//std::ostream& operator<<(std::ostream& o, bool val)
	//{
	//	if (val)
	//		o << "true";
	//	else
	//		o << "false";

	//	return o;
	//}

}