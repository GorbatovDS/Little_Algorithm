#pragma once

#include <vector>
#include <iomanip>
#include "CostValue.h"

using namespace std;

class CostMatrix
{
	friend istream& operator >> (istream& is, CostMatrix& mtx)
	{
		mtx.clear();

		size_t mSize;
		is >> mSize;
		if (mSize < 3)
		{
			throw exception("CostMatrix::read: matrix size < 3");
		}

		mtx.matrix.resize(mSize);
		for (size_t i = 0; i < mSize; i++)
		{
			mtx.matrix[i].resize(mSize);
		}
		for (size_t i = 0; i < mSize; i++)
		{
			for (size_t j = 0; j < mSize; j++)
			{
				is >> mtx.matrix[i][j];
			}
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (!mtx.matrix[i][i].getInf())
			{
				throw exception("CostMatrix::read: non-infinite main diagonal");
			}
		}

		return is;
	}
	friend ostream& operator << (ostream& os, CostMatrix& mtx)
	{
		for (size_t i = 0; i < mtx.mSize(); i++)
		{
			for (size_t j = 0; j < mtx.mSize(); j++)
				os << setw(3) << " " << mtx.matrix[i][j];

			os << endl;
		}

		return os;
	}

public:
	CostMatrix() { clear(); }
	CostMatrix(const CostMatrix& rval)
		: matrix(rval.matrix)
	{}

	void clear() { matrix.clear(); }
	size_t mSize() { return matrix.size(); }


	CostMatrix& operator =(const CostMatrix& rval)
	{
		matrix = rval.matrix;
		return *this;
	}

protected:
	vector < vector< CostValue > > matrix;
};