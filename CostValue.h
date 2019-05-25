#pragma once

#include <exception>
#include <iostream>
#include <sstream>

using namespace std;

class CostValue
{
	friend inline istream& operator >> (istream& is, CostValue& val) { return is >> val.value; }
	friend inline ostream& operator <<(ostream& os, const CostValue& val)
	{
		if (val.getInf())
		{
			return os << "inf";
		}
		return os << val.value;
	}

public:
	CostValue(double val = 0)
		: value(val)
	{}

	void clear() { value = 0; }
	void setInf() { value = -1; }
	bool getInf() const { return (value < 0); }

	bool operator ==(const CostValue& rval)
	{
		if (getInf())
		{
			if (rval.getInf())
			{
				return true;
			}
			return false;
		}
		if (rval.getInf())
		{
			return false;
		}
		return value == rval.value;
	}
	bool operator !=(const CostValue& rval) { return !operator ==(rval); }
	bool operator <(const CostValue& rval)
	{
		if (getInf())
		{
			return false;
		}
		if (rval.getInf())
		{
			return true;
		}
		return value < rval.value;
	}
	bool operator >=(const CostValue& rval) { return !operator <(rval); }
	bool operator >(const CostValue& rval)
	{
		if (rval.getInf())
		{
			return false;
		}
		if (getInf())
		{
			return true;
		}
		return value > rval.value;
	}
	bool operator <=(const CostValue& rval) { return !operator >(rval); }

	CostValue operator +(const CostValue& rval) { return (getInf() || rval.getInf()) ? CostValue(-1) : CostValue(value + rval.value); }
	CostValue operator +=(const CostValue& rval) { return (*this = operator +(rval)); }
	CostValue operator -(const CostValue& rval)
	{
		CostValue result;

		if (getInf())
		{
			result.setInf();
		}
		else
		{
			if (rval.getInf())
			{
				throw exception("CostValue::subtraction: value - infinity");
			}
			else
			{
				result.value = value - rval.value;
				if (result.value < 0)
				{
					throw exception("CostValue::subtraction: negative result");
				}
			}
		}

		return result;
	}
	CostValue operator -=(const CostValue& rval) { return (*this = operator -(rval)); }

private:
	double value;
};
