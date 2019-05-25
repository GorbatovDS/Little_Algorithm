#pragma once

#include <vector>
#include "Path.h"
#include "CostMatrix.h"

using namespace std;

class Branch : public CostMatrix
{
public:
	Branch();
	Branch(const Branch& rval);
	Branch(const CostMatrix& cm);

	Branch& operator =(const Branch& rval);

	void clear();
	void reduce();

	CostValue getH() { return H; }
	//size_t mSize() { return matrix.size(); }

	Arc findHighestZero(size_t& row, size_t& col);
	void setInf(size_t row, size_t col);
	void addToPath(Arc a);
	void scratchRowCol(size_t row, size_t col);
	void handle2by2();
	Path getPath() { return path; }

private:
	vector < int > rowLabels;
	vector < int > colLabels;
	Path path;
	CostValue H;

	CostValue minInRow(size_t idx);
	CostValue minInCol(size_t idx);

	void reduceRow(size_t idx);
	void reduceCol(size_t idx);
};