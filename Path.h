#pragma once

#include <utility>
#include <list>
#include <iostream>

using namespace std;

typedef pair<int, int> Arc;

class Path : public list < Arc >
{
public:
	bool getArcStart(int arcStart, Arc &a)
	{
		for (auto iter = begin(); iter != end(); iter++)
		{
			if (iter->first == arcStart)
			{
				a = *iter;
				erase(iter);
				return true;
			}
		}
		return false;
	}
	bool getArcEnd(int arcEnd, Arc &a)
	{
		for (auto iter = begin(); iter != end(); iter++)
		{
			if (iter->second == arcEnd)
			{
				a = *iter;
				erase(iter);
				return true;
			}
		}
		return false;
	}
};

inline ostream& operator <<(ostream& os, const Path& path)
{
	for (auto & iter : path)
	{
		os << " (" << iter.first << "," << iter.second << ")";
	}

	return os;
}