#ifndef COMPLEX_HPP
# define COMPLEX_HPP

# include <iostream>
# include <string>
# include "Type.hpp"
# include <regex>

class Complex : public Type
{
	public:
		Complex(void);
		Complex(double r, double i);

		Complex & operator=(const Complex & rhs);
		Complex & operator+=(const Complex & rhs);
		Complex & operator+=(const double & rhs);
		Complex operator+(const Complex & rhs);
		Complex operator+(const double & rhs);
		Complex operator-(const Complex & rhs);
		Complex operator-(const double & rhs);
		Complex operator/(const Complex & rhs);
		Complex operator/(const double & rhs);
		Complex operator*(const Complex & rhs);
		Complex operator*(const double & rhs);
		virtual	~Complex();

		double GetReal(void) const;
		double GetIma(void) const;
		std::string ToString() const;

		static Complex EvalExpr(std::string str);

		std::string ToPrint() const;
		static void Check(std::string & str);
	private:
		double	_r;
		double	_i;
		static Complex CalcComplex(std::string str);
};

std::ostream & operator<<(std::ostream & o, Complex & rhs);

#endif
