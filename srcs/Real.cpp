#include "Real.hpp"
#include <cmath>
#include <regex>
#include "Core.hpp"
#include "ncurses.h"

Real::Real(void) : Type(eType::Real), _v(0)
{
}

Real::Real(double v) : Type(eType::Real), _v(v)
{
}

Real::~Real() {}

Real &	Real::operator=(const Real & rhs)
{
	_v = rhs.GetValue();
	return *this;
}

Real &	Real::operator=(const double & rhs)
{
	_v = rhs;
	return *this;
}

Real &	Real::operator+(const Real & rhs)
{
	_v += rhs.GetValue();
	return *this;
}

Real &	Real::operator+(const double & rhs)
{
	_v += rhs;
	return *this;
}

Real &	Real::operator-(const Real & rhs)
{
	_v -= rhs.GetValue();
	return *this;
}

Real &	Real::operator-(const double & rhs)
{
	_v -= rhs;
	return *this;
}

Real &	Real::operator*(const Real & rhs)
{
	_v *= rhs.GetValue();
	return *this;
}

Real &	Real::operator*(const double & rhs)
{
	_v *= rhs;
	return *this;
}

Real &	Real::operator/(const Real & rhs)
{
	_v /= rhs.GetValue();
	return *this;
}

Real &	Real::operator/(const double & rhs)
{
	_v /= rhs;
	return *this;
}

Real operator-(const double & lhs, const Real & rhs)
{
	Real l(lhs);
	return l - rhs;
}

double	Real::GetValue() const
{
	return _v;
}

std::string Real::ToPrint() const
{
	return Core::Dtoa(_v);
}
std::string Real::ToString() const
{
	return Core::Dtoa(_v);
}

void Real::Calc(std::string & str, int firstSub, int endSub, int posSubcalc, int firstEra, int endEra)
{
	double ret = 0, left, right;
	std::string subcalc = str.substr(firstSub, endSub);
	str.erase(firstEra, endEra);
	std::string::size_type sz;
	std::string::size_type tmp;
	left = std::stod(subcalc, &tmp);
	if (subcalc[tmp] == '^')
	{
		if (subcalc.rfind('-', tmp) != std::string::npos)
		{
			str.insert(posSubcalc, "-");
			posSubcalc++;
		}
		ret = std::stod(subcalc.substr(tmp + 1), &sz);
		left = pow(left, ret);
		sz += tmp + 1;
	}
	else
		sz = tmp;
	right = std::stod(subcalc.substr(sz + 1),&tmp);
	if (subcalc[sz + 1 + tmp] == '^')
	{
		ret = std::stod(subcalc.substr(sz + tmp + 2));
		right = pow(right, ret);
	}
	if (subcalc[sz] == '*')
		ret = left * right;
	else if (subcalc[sz] == '-')
		ret = left - right;
	else if (subcalc[sz] == '+')
		ret = left + right;
	else if (subcalc[sz] == '/')
	{
		if (right == 0)
			throw std::runtime_error("Can't divide by 0.");
		ret = left / right;
	}
	else if (subcalc[sz] == '%')
	{
		if (right == 0)
			throw std::runtime_error("Can't modulo by 0.");
		ret = fmod(left, right);
	}

	if (posSubcalc != 0 && Core::Dtoa(ret)[0] != '-' && str[posSubcalc - 1] != '+')
		str.insert(posSubcalc, "+" + Core::Dtoa(ret));
	else
		str.insert(posSubcalc, Core::Dtoa(ret));
}

Real Real::EvalExpr(std::string str)
{
	Real ret = 0;
	std::smatch m;

	while (1)
	{
		if (str.find_last_of("(") != std::string::npos)
		{
			int posFirstP = str.find_last_of("(");
			int posLastP = str.find(")", str.find_last_of("("));
			std::string subcalc = str.substr(posFirstP + 1, posLastP - posFirstP - 1);
			int posSubcalc = posFirstP;
			str.erase(posFirstP, posLastP - posFirstP + 1);
			ret = EvalExpr(subcalc);
			str.insert(posSubcalc, Core::Dtoa(ret.GetValue()));
		}
		else if (std::regex_search(str, m, std::regex("-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?(?:\\*|/|%)-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?")))
		{
				for (auto x:m)
					Calc(str, str.find(x.str()), x.str().length(), str.find(x.str()), str.find(x.str()), x.str().length());
		}
		else if (std::regex_search(str, m, std::regex("-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?(?:\\+|-)-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?")))
		{
				for (auto x:m)
					Calc(str, str.find(x.str()), x.str().length(), str.find(x.str()), str.find(x.str()), x.str().length());
		}
		else
		{
			if (str[0] == '-' && str[1] == '-')
				str.erase(str.begin(), str.begin() + 2);
			if (str.find("^") != std::string::npos)
			{
				std::string::size_type tmp;
				double nb = std::stod(str, &tmp);
				double power = std::stod(str.substr(tmp + 1));
				ret = pow(nb, power);
				if (str[0] == '-')
					ret = 0.0-ret;
				str = Core::Dtoa(ret.GetValue());
			}
			break;
		}
	}
	return Real(std::stod(str));
}

void Real::Check(std::string & str)
{
	std::smatch m;

	while (std::regex_search(str, m, std::regex("-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?(?:\\*|/|\\+|-|%)-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?")))
	{
		str.replace(m.position(), m.length(), "0");
	}
	if (str.compare("") == 0)
		throw std::runtime_error("Syntax error.");

	if (str.find("^") != std::string::npos)
	{
		std::string tmp = str.substr(str.find("^") + 1);
		if (std::regex_search(tmp, m, std::regex("-?\\d+(?:\\.\\d+)?")))
			str.erase(str.find("^"), m.length() + 1);
		else
			throw std::runtime_error("Syntax error: power not valid.");
	}

	if (std::regex_search(str, m, std::regex("-?\\d+(?:\\.\\d+)?")))
		str.erase(m.position(), m.length());

	if (str.compare("") != 0)
		throw std::runtime_error("Syntax error.");
}
