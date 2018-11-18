#pragma once

#include <memory>

namespace mm {

	struct charInfo
	{
		charInfo()
			: count(0), index(-1)
		{
		}

		int count;
		int index;
	};

	struct charInfo2
	{
		charInfo2(int countIn = 0, int indexIn = -1)
			: count(countIn), index(indexIn)
		{
		}

		bool operator>(const charInfo2& right)
		{
			return index > right.index;
		}

		int count;
		int index;
	};

	struct CompareCharInfoPtrs
	{
		bool operator()(const std::unique_ptr<charInfo2>& left, const std::unique_ptr<charInfo2>& right)
		{
			return left->index > right->index;
		}
	};

	

}