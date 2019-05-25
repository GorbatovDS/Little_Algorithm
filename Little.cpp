#include <algorithm>
#include "Little.h"
#include "Branch.h"

static void addToList(list < Branch >& bList, Branch& b)
{
	if (!b.getH().getInf())
	{
		for (auto iter = bList.begin(); iter != bList.end(); iter++)
		{
			if (b.getH() <= iter->getH())
			{
				bList.insert(iter, b);
				return;
			}
		}
		bList.push_back(b);
	}
}

CostValue littleSolve(const CostMatrix& m, Path& path)
{
	Branch b(m);
	b.reduce();

	list < Branch > bList;
	addToList(bList, b);

	CostValue pathCost;
	pathCost.setInf();

	while (!bList.empty())
	{
		// bList contains SORTED list of fully reduced branches

		b = bList.front();
		bList.pop_front();

		size_t row, col;
		Arc splitArc = b.findHighestZero(row, col);

		Branch bExc(b);
		bExc.setInf(row, col);
		bExc.reduce();
		addToList(bList, bExc);

		Branch bInc(b);
		bInc.addToPath(splitArc);
		bInc.scratchRowCol(row, col);
		bInc.reduce();
		if (bInc.mSize() == 2)
		{
			bInc.handle2by2();
			if (bInc.getH() < pathCost)
			{
				path = bInc.getPath();
				pathCost = bInc.getH();

				for (auto iter = bList.begin(); iter != bList.end(); )
				{
					if (pathCost <= iter->getH())
				{
						iter = bList.erase(iter);
					}
					else
					{
						iter++;
					}
				}
			}
		}
		else
		{
			addToList(bList, bInc);
		}
	}

	return pathCost;
}