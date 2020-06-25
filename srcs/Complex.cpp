#include "Complex.hpp"
#include <cmath>
#include "ncurses.h"
#include "Core.hpp"
#include "Real.hpp"

Complex::Complex(void) : Type(eType::Complex), _r(0), _i(0) {}

Complex::Complex(double r, double i) : Type(eType::Complex), _r(r), _i(i) {}

Complex::~Complex(void) {}

std::ostream & operator<<(std::ostream & o, Complex & rhs)
{
	o << rhs.GetReal();
	if (rhs.GetIma() < 0)
		o << " - " << abs(rhs.GetIma());
	else
		o << " + " << rhs.GetIma();
	o << "i";
	return o;
}

Complex Complex::operator+(const Complex & rhs)
{
	Complex ret(GetReal() + rhs.GetReal(), GetIma() + rhs.GetIma());
	return ret;
}

Complex Complex::operator+(const double & rhs)
{
	Complex r(rhs, 0);
	return *this + r;
}

Complex Complex::operator-(const Complex & rhs)
{
	Complex ret(GetReal() - rhs.GetReal(), GetIma() - rhs.GetIma());
	return ret;
}

Complex Complex::operator-(const double & rhs)
{
	Complex r(rhs, 0);
	return *this - r;
}

Complex Complex::operator/(const Complex & rhs)
{
	double a = GetReal();
	double ap = rhs.GetReal();
	double b = GetIma();
	double bp = rhs.GetIma();

	Complex ret((a * ap + b * bp) / (ap * ap + bp * bp), (b * ap - a * bp) / (ap * ap + bp * bp));

	return ret;
}

Complex Complex::operator/(const double & rhs)
{
	Complex ret(GetReal() / rhs, GetIma() / rhs);
	return ret;
}

Complex Complex::operator*(const Complex & rhs)
{
	double a = GetReal();
	double ap = rhs.GetReal();
	double b = GetIma();
	double bp = rhs.GetIma();

	Complex ret(a * ap - b * bp, a * bp + b * ap);

	return ret;
}

Complex Complex::operator*(const double & rhs)
{
	Complex ret(GetReal() * rhs, GetIma() * rhs);
	return ret;
}

double Complex::GetReal(void) const { return _r; }
double Complex::GetIma(void) const { return _i; }

Complex & Complex::operator=(const Complex & rhs)
{
	_r = rhs.GetReal();
	_i = rhs.GetIma();
	return *this;
}

Complex & Complex::operator+=(const Complex & rhs)
{
	*this = *this + rhs;
	return *this;
}

Complex & Complex::operator+=(const double & rhs)
{
	*this = *this + rhs;
	return *this;
}

std::string Complex::ToPrint() const
{ 
	std::string ret;

	if (_r != 0)
		ret += Core::Dtoa(_r);
	if (_i < 0)
	{
		if (_r != 0)
		{
			ret += " - ";
			ret += Core::Dtoa(fabs(_i));
		}
		else if (_i != -1)
			ret += Core::Dtoa(_i);
		else
			ret += "-";
	}
	else
	{
		if (_r != 0)
			ret += " + ";
		if (_i != 1)
			ret += Core::Dtoa(_i);
	}
	ret += "i";
	return ret;
}

std::string Complex::ToString() const
{ 
	std::string ret;

	if (_r != 0)
		ret += Core::Dtoa(_r);
	if (_i < 0)
	{
		if (_r != 0)
		{
			ret += "-";
			ret += Core::Dtoa(fabs(_i));
		}
		else if (_i != -1)
			ret += Core::Dtoa(_i);
		else
			ret += "-";
	}
	else
	{
		if (_r != 0)
			ret += "+";
		if (_i != 1)
			ret += Core::Dtoa(_i);
	}
	ret += "i";
	return ret;
}

void Complex::Check(std::string & str)
{
	(void)str;
}

Complex Complex::CalcComplex(std::string str)
{
	std::string i = "0", tmp;
	std::smatch m;

	tmp = str;
	while (std::regex_search(tmp, m, std::regex("((?:-?\\d+(?:\\.\\d+)?))?i(?:$|(?:\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?))?")))
	{
		if (m.str()[0] != '-' && i.length() != 0)
			i += "+";
		if (m.str().compare("i") == 0)
			i += "1";
		else if (m.str().compare("-i") == 0)
			i += "-1";
		else
			i += m.str();
		if (m.str()[0] != '-' && str.find(m.str()) != 0)
			str.erase(str.find(m.str()) - 1, m.length() + 1);
		else
			str.erase(str.find(m.str()), m.length());
		tmp = m.suffix();
	}
	i.erase(std::remove(i.begin(), i.end(), 'i'), i.end());
	if (str.length() == 0)
		str = "0";
	if (str[0] == '+')
		str.erase(str.begin());
	return Complex(Real::EvalExpr(str), Real::EvalExpr(i));
}

Complex Complex::EvalExpr(std::string str)
{
	std::smatch m;
	Complex x;
	while (1)
	{
		/*
		if (str.find_last_of("(") != std::string::npos)
		{
			int posFirstP = str.find_last_of("(");
			int posLastP = str.find(")", str.find_last_of("("));
			std::string subcalc = str.substr(posFirstP + 1, posLastP - posFirstP - 1);
			int posSubcalc = posFirstP;
			str.erase(posFirstP, posLastP - posFirstP + 1);
			//ret = EvalExpr(subcalc);
			//str.insert(posSubcalc, Core::Dtoa(ret));
		}
		else if (std::regex_search(str, m, std::regex("-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?(?:\\*|/|%)-?\\d+(?:\\.\\d+)?(?:\\^-?\\d+(?:\\.\\d+)?)?")))
		{
				for (auto x:m)
					Calc(str, str.find(x.str()), x.str().length(), str.find(x.str()), str.find(x.str()), x.str().length());
		}
		*/
		if (std::regex_search(str, m, std::regex("(?:^|\\+|\\-)[^\\+\\-\\*\\/]*(?:\\*|\\/)[^\\+\\-\\*\\/]*")))
		{
			std::string match = m.str();
			Complex res;
			int split;
			if (match[0] == '+')
				match.erase(0, 1);
			if (match.find("*") != std::string::npos)
				split = match.find("*");
			else
				split = match.find("/");
			Complex left = Complex::CalcComplex(match.substr(0, split));
			Complex right = Complex::CalcComplex(match.substr(split + 1));
			if (match.find("*") != std::string::npos)
				res = left * right;
			else if (match.find("/") != std::string::npos)
			{
				if (right.ToPrint().length() == 0)
					throw std::runtime_error("Can't divide by 0.");
				res = left / right;
			}
			str.erase(m.position(), m.length());
			if (m.position() != 0 && res.ToString()[0] != '-')
				str.insert(m.position(), "+" + res.ToString());
			else
				str.insert(m.position(), res.ToString());
		}
		else
		{
			printw("%s\n", Complex::CalcComplex(str).ToPrint().c_str());
			/*
			if (str[0] == '-' && str[1] == '-')
				str.erase(str.begin(), str.begin() + 2);
			if (str.find("^") != std::string::npos)
			{
				std::string::size_type tmp;
				double nb = std::stod(str, &tmp);
				double power = std::stod(str.substr(tmp + 1));
				ret = pow(nb, power);
				str = Core::Dtoa(ret);
			}
			*/
			break;
		}
	}
	return Complex(0, 0);
}
