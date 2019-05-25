#include <iostream>
#include <iomanip>
#include "Branch.h"

Branch::Branch()
{
	clear();
}

Branch::Branch(const Branch& rval)
	: CostMatrix(rval)
	, rowLabels(rval.rowLabels)
	, colLabels(rval.colLabels)
	, path(rval.path)
	, H(rval.H)
{}

Branch::Branch(const CostMatrix& cm)
	: CostMatrix(cm)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		rowLabels.push_back(i + 1);
		colLabels.push_back(i + 1);
	}

	path.clear();
	H.clear();
}

Branch& Branch::operator =(const Branch& rval)
{
	CostMatrix::operator =(rval);
	rowLabels = rval.rowLabels;
	colLabels = rval.colLabels;
	path = rval.path;
	H = rval.H;

	return *this;
}

void Branch::clear()
{
	CostMatrix::clear();

	rowLabels.clear();
	colLabels.clear();
	path.clear();
	H.clear();
}

void Branch::reduce()
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		reduceRow(i);
	}
	for (size_t i = 0; i < matrix.size(); i++)
	{
		reduceCol(i);
	}
}

CostValue Branch::minInRow(size_t idx)
{
	CostValue min = matrix[idx][0];

	for (size_t i = 1; i < matrix.size(); i++)
	{
		if (matrix[idx][i] < min)
		{
			min = matrix[idx][i];
		}
	}

	return min;
}

void Branch::reduceRow(size_t idx)
{
	CostValue min = minInRow(idx);

	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[idx][i] -= min;
	}

	H += min;
}

CostValue Branch::minInCol(size_t idx)
{
	CostValue min = matrix[0][idx];

	for (size_t i = 1; i < matrix.size(); i++)
	{
		if (matrix[i][idx] < min)
		{
			min = matrix[i][idx];
		}
	}

	return min;
}

void Branch::reduceCol(size_t idx)
{
	CostValue min = minInCol(idx);

	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i][idx] -= min;
	}

	H += min;
}

Arc Branch::findHighestZero(size_t& row, size_t& col)
{
	const CostValue zero(0);
	CostValue power(0);

	row = col = matrix.size();
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix.size(); j++)
		{
			if (matrix[i][j] == zero)
			{
				matrix[i][j].setInf();

				CostValue p = minInRow(i) + minInCol(j);
				if ((p > power) || (row == matrix.size()))
				{
					power = p;
					row = i;
					col = j;
				}

				matrix[i][j] = zero;
			}
		}
	}

	return Arc(rowLabels[row], colLabels[col]);
}

void Branch::setInf(size_t row, size_t col)
{
	matrix[row][col].setInf();
}
///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Branch::addToPath(Arc a)
{
	Path oldPath = path;

	path.clear();
	path.push_back(a);

	Arc seg;
	while (oldPath.getArcStart(path.back().second, seg))
	{
		path.push_back(seg);
	}
	while (oldPath.getArcEnd(path.front().first, seg))
	{
		path.push_front(seg);
	}

	for (size_t i = 0; i < matrix.size(); i++)
	{
		if (rowLabels[i] == path.back().second)
		{
			for (size_t j = 0; j < matrix.size(); j++)
			{
				if (colLabels[j] == path.front().first)
				{
					matrix[i][j].setInf();
					break;
				}
			}
			break;
		}
	}

	while (!oldPath.empty())
	{
		path.push_back(oldPath.front());
		oldPath.pop_front();
	}
}

void Branch::scratchRowCol(size_t row, size_t col)
{
	rowLabels.erase(rowLabels.begin() + row);
	matrix.erase(matrix.begin() + row);

	colLabels.erase(colLabels.begin() + col);
	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i].erase(matrix[i].begin() + col);
	}
}

void Branch::handle2by2()
{
	CostValue c1 = matrix[0][0] + matrix[1][1];
	CostValue c2 = matrix[0][1] + matrix[1][0];

	if (c1 < c2)
	{
		addToPath(Arc(rowLabels[0], colLabels[0]));
		addToPath(Arc(rowLabels[1], colLabels[1]));
		H += c1;
	}
	else
	{
		addToPath(Arc(rowLabels[0], colLabels[1]));
		addToPath(Arc(rowLabels[1], colLabels[0]));
		H += c2;
	}
}