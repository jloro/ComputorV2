#include "Complex.hpp"
#include <cmath>
#include "ncurses.h"
#include "Core.hpp"
#include "Real.hpp"

Complex::Complex(void) : Type(eType::Complex), _r(0), _i(0) {}

Complex::Complex(double r, double i) : Type(eType::Complex), _r(r), _i(i) {}

Complex::~Complex(void) {}

Complex Complex::fmod(Complex left, Complex right)
{
	Complex x = left / right;
	x = Complex(floor(x.GetReal()), floor(x.GetIma()));
	return left - x * right;
}

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
	std::string::size_type pos;
	while ((pos = str.find("i")) != std::string::npos)
	{
		if (isdigit(str[pos + 1]) != 0)
			throw std::runtime_error("Syntax error.");

		str.replace(pos, 1, "0");
	}
	Real::Check(str);
}

Complex Complex::CalcComplex(std::string str)
{
	std::string i = "0", tmp, r = "0";
	std::smatch m;

	tmp = str;
	while (std::regex_search(tmp, m, std::regex("-?(?:\\d+(?:\\.\\d+)?(?:\\^\\d+)?)?i(?:\\^\\d+)?")))
	{
		std::string match = m.str();
		if (match[0] != '-' && i.length() != 0)
			i += "+";
		if (match.find("^") != std::string::npos)
		{
			int power = stod(match.substr(match.find("^") + 1));
			Complex n(0, 1);
			Complex m = n;
			for (int i = 1; i < power; i++)
				n = m * n;
			Complex tmp = Complex::CalcComplex(match.substr(0, match.find("^")));
			if (n.GetReal() == 0)
				i += Core::Dtoa(tmp.GetIma() * n.GetIma());
			else
				r += Core::Dtoa(tmp.GetIma() * n.GetReal());
		}
		else if (match.compare("i") == 0)
			i += "1";
		else if (match.compare("-i") == 0)
			i += "-1";
		else
			i += m.str();

		if (match[0] != '-' && str.find(match) != 0)
			str.erase(str.find(match) - 1, m.length() + 1);
		else
			str.erase(str.find(match), m.length());
		tmp = m.suffix();
	}
	i.erase(std::remove(i.begin(), i.end(), 'i'), i.end());
	if (str.length() == 0)
		str = "0";
	if (str[0] == '+')
		str.erase(str.begin());
	return Complex(Real::EvalExpr(str+"+"+r).GetValue(), Real::EvalExpr(i).GetValue());
}

Complex Complex::EvalExpr(std::string str, std::map<std::string, Complex> saved)
{
	std::smatch m;
	Complex x;
	std::map<std::string, Complex> var;

	if (saved.size() != 0)
		var = saved;
	while (1)
	{
		if (str.find_last_of("(") != std::string::npos)
		{
			int posFirstP = str.find_last_of("(");
			int posLastP = str.find(")", str.find_last_of("("));
			std::string subcalc = str.substr(posFirstP + 1, posLastP - posFirstP - 1);
			std::string key = "A";
			key[0] += var.size();
			var[key] = Complex::EvalExpr(subcalc, var);
			str.replace(posFirstP, posLastP - posFirstP + 1, key);
		}
		else if (std::regex_search(str, m, std::regex("(?:^|\\+|\\-)[^\\+\\-\\*\\/]*(?:\\*|\\/|%)-?[^\\+\\-\\*\\/]*")))
		{
			std::string match = m.str();
			Complex res, left, right;
			int split, start = 0;

			if (match[0] == '+')
				match.erase(0, 1);
			else if (match[0] == '-')
				start = 1;

			if (match.find("*") != std::string::npos)
				split = match.find("*");
			else if (match.find("/") != std::string::npos)
				split = match.find("/");
			else 
				split = match.find("%");

			bool doPower = false;

			if (var.find(match.substr(start, split - start)) != var.end() && match.substr(start, split - start).find('^') == std::string::npos)
			{
				left = var[match.substr(start, split - start)];
				doPower = true;
			}
			else if (var.find(match.substr(start, match.substr(start, split - start).find('^'))) != var.end())
			{
				left = var[match.substr(start, match.substr(start, split - start).find('^'))];
				doPower = true;
			}
			else
				left = Complex::CalcComplex(match.substr(start, split - start));

			if (match.substr(0, split).find('^') != std::string::npos && doPower)
			{
				int power = stod(match.substr(match.find('^') + 1, split - match.find('^') - 1));
				Complex tmp = left;
				for (int i = 1; i < power; i++)
					left = left * tmp;
			}

			start = 0;
			doPower = false;
			if (match[split + 1] == '-')
				start = 1;
			if (var.find(match.substr(split + start + 1)) != var.end() && match.substr(split).find('^') == std::string::npos)
			{
				doPower = true;
				right = var[match.substr(split + start + 1)];
			}
			else if (var.find(match.substr(split + start + 1, match.length() - match.rfind('^') - 1)) != var.end())
			{
				doPower = true;
				right = var[match.substr(split + start + 1, match.length() - match.rfind('^') - 1)];
			}
			else
				right = Complex::CalcComplex(match.substr(split + start + 1));

			if (match.substr(split + 1).find('^') != std::string::npos && doPower)
			{
				int power = stod(match.substr(match.rfind('^') + 1, match.length() - match.rfind('^') - 1));
				Complex tmp = right;
				for (int i = 1; i < power; i++)
					right = right * tmp;
			}

			switch (match[split])
			{
				case '*':
					res = left * right;
					break;
				case '/':
					if (right.GetReal() == 0 && right.GetIma() == 0)
						throw std::runtime_error("Can't divide by 0.");
					res = left / right;
					break;
				case '%':
					printw("%s\n%s\n", left.ToPrint().c_str(), right.ToPrint().c_str());
					res = fmod(left, right);
					break;
			}

			if (match[0] == '-')
				res = res * -1;
			if (match[split + 1] == '-')
				res = res * -1;

			str.erase(m.position(), m.length());
			if (str.length() == 0)
				str.insert(m.position(), res.ToString());
			else if (m.position() != 0)
				str.insert(m.position(), "+("+res.ToString()+")");
			else 
				str.insert(m.position(), "("+res.ToString()+")");
		}
		else
		{
			while (std::regex_search(str, m, std::regex("[A-Z]")))
			{
				std::string match = m.str();
				if (str[m.position() - 1] == '-')
				{
					Complex n;
					if (str[m.position() + 1] == '^')
					{
						std::string tmp;
						if (str.find_first_of("*/-+%", m.position() + 2) != std::string::npos)
							tmp = str.substr(m.position() + 2, str.find_first_of("*/-+%", m.position() + 2) - m.position() - 2);
						else
							tmp = str.substr(m.position() + 2);
						int power = stod(tmp);
						str.erase(m.position() - 1, tmp.length() + 3);
						n = var[match];
						Complex tmpN = n;
						for (int i = 1; i < power; i++)
							n = n * tmpN;
						n = Complex(0, 0) - n;
					}
					else
					{
						str.erase(m.position() - 1, m.length() + 1);
						n = Complex(0, 0) - var[match];
					}

					if (n.ToString()[0] != '-')
						str.insert(m.position() - 1, "+" + n.ToString());
					else
						str.insert(m.position() - 1, n.ToString());
				}
				else
				{
					if (str[m.position() + 1] == '^')
					{
						std::string tmp;
						if (str.find_first_of("*/-+%", m.position() + 2) != std::string::npos)
							tmp = str.substr(m.position() + 2, str.find_first_of("*/-+%", m.position() + 2) - m.position() - 2);
						else
							tmp = str.substr(m.position() + 2);
						int power = stod(tmp);
						str.erase(m.position(), tmp.length() + 2);
						Complex n = var[match];
						Complex tmpN = n;
						for (int i = 1; i < power; i++)
							n = n * tmpN;
						str.insert(m.position(), n.ToString());
					}
					else
					{
						str.replace(m.position(), m.length(), var[match].ToString());
					}
				}
			}
			break;
		}
	}
	return Complex::CalcComplex(str);
}
