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

Complex operator-(const double & lhs, const Complex & rhs)
{
	Complex l(lhs, 0);
	return l - rhs;
}

Complex operator/(const double & lhs, const Complex & rhs)
{
	Complex l(lhs, 0);
	return l / rhs;
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

/*
Complex Complex::CalcBrackets(std::string & str, std::string brackets, int posFirstP, int posLastP)
{
	int posOp = 0, first, last;
	Complex right, left, res;
	printw("a\n");
	printw("str:%s\nb:%s\n", str.c_str(), brackets.c_str());
	if (str[posLastP + 1] == '*' || str[posLastP + 1] == '/')
	{
		first = posFirstP;
		left = Complex::EvalExpr(brackets);
		posOp = posLastP + 1;
		if (str[posOp + 1] == '(')
		{
			right = Complex::EvalExpr(str.substr(posOp + 1, str.find(')', posOp + 1)));
			last = str.find(')', posOp + 1) - first;
		}
		else if (str[posOp + 1] == '-')
		{
			right = Complex::EvalExpr(str.substr(posOp + 1, str.find_first_of("+-/*", posOp + 2) - posOp - 1));
			if (str.find_first_of("+-/*", posOp + 2) != std::string::npos)
				last = str.find_first_of("+-/*", posOp + 2) - first;
			else
				last = str.length() - first;
		}
		else
		{
			right = Complex::EvalExpr(str.substr(posOp + 1, str.find_first_of("+-/*", posOp + 1) - posOp - 1));
			if (str.find_first_of("+-/*", posOp + 1) != std::string::npos)
				last = str.find_first_of("+-/*", posOp + 1) - first;
			else
				last = str.length() - first;
		}
	}
	else if (str[posFirstP - 1] == '*' || str[posFirstP - 1] == '/')
	{
		right = Complex::EvalExpr(brackets);
		posOp = posFirstP - 1;
		if (str[posOp - 1] == ')')
		{
			left = Complex::EvalExpr(str.substr(str.rfind('(', posOp - 1) + 1, posOp - 2 - str.rfind('(', posOp - 1)));
		}
		else if (str.find_last_of("+-/*", posOp - 1) != std::string::npos)
		{
			left = Complex::EvalExpr(str.substr(str.find_last_of("+-/*", posOp - 1), posOp - str.find_last_of("+-/*", posOp - 1)));
		}
		else
		{
			left = Complex::EvalExpr(str.substr(0, posOp));
		}
	}
	else if (str[posFirstP - 1] == '-' || str[posFirstP - 1] == '+')
	{
		right = Complex::EvalExpr(brackets);
		posOp = posFirstP - 1;
		if (str[posOp - 1] == ')')
		{
			left = Complex::EvalExpr(str.substr(str.rfind('(', posOp - 1) + 1, posOp - 1 - str.rfind('(', posOp - 1) - 1));
		}
		else if (posOp != 0 && str.find_last_of("+-/*", posOp - 1) != std::string::npos)
		{
			left = Complex::EvalExpr(str.substr(str.find_last_of("+-/*", posOp - 1), posOp - str.find_last_of("+-/*", posOp - 1)));
		}
		else if (posOp == 0)
		{
			left = Complex::EvalExpr("0");
		}
		else
		{
			left = Complex::EvalExpr(str.substr(0, posOp));
		}
	}
	else if (str[posLastP + 1] == '-' || str[posLastP + 1] == '+')
	{
		left = Complex::EvalExpr(brackets);
		posOp = posLastP + 1;
		if (str[posOp + 1] == '(')
			right = Complex::EvalExpr(str.substr(posOp + 1, str.find(')', posOp + 1)));
		else if (str[posOp + 1] == '-')
			right = Complex::EvalExpr(str.substr(posOp + 1, str.find_first_of("+-/*", posOp + 2) - posOp - 1));
		else
			right = Complex::EvalExpr(str.substr(posOp + 1, str.find_first_of("+-/*", posOp + 1) - posOp - 1));
	}
	printw("avant calc\n");
	if (str[posOp] == '-')
		res = left - right;
	else if (str[posOp] == '+')
		res = left + right;
	else if (str[posOp] == '*')
		res = left * right;
	else if (str[posOp] == '/')
	{
		if (right.ToString().compare("0") == 0)
			throw std::runtime_error("Can't divide by 0.");
		res = left / right;
	}
	printw("left:%s\nright:%s\n", left.ToPrint().c_str(), right.ToPrint().c_str());
	printw("first:%d\nlast:%d\n", first, last);
	str.erase(first, last);
	printw("strafter:%s\n", str.c_str());
	//if (first != 0 && str[first - 1] != '-' && str[first - 1] != '+')
//		str.insert(first, "+" + res.ToString());
//	else
	if (str[first - 1] == '*' || str[first - 1] == '/' || str[first + 1] == '*' || str[first + 1] == '/')
		str.insert(first, "("+res.ToString()+")");
	else
		str.insert(first, res.ToString());
	return res;
}
*/
Complex Complex::CalcBrackets(std::string & str, std::string brackets, int posFirstP, int posLastP)
{
	posFirstP++;
	posLastP++;
	printw("str:%s\nb:%s\n", str.c_str(), brackets.c_str());
	return Complex(0, 0);
}

Complex Complex::EvalExpr(std::string str)
{
	std::smatch m;
	Complex x;

	while (1)
	{
		if (str.find_last_of("(") != std::string::npos)
		{
			int posFirstP = str.find_last_of("(");
			int posLastP = str.find(")", str.find_last_of("("));
			std::string subcalc = str.substr(posFirstP + 1, posLastP - posFirstP - 1);
			Complex::CalcBrackets(str, subcalc, posFirstP, posLastP);
			printw("ret:%s\nstr:%s\n", x.ToPrint().c_str(), str.c_str());
		}
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
			return Complex::CalcComplex(str);
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
