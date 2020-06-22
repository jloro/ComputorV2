#ifndef COMPLEX_HPP
# define COMPLEX_HPP

# include <iostream>
# include <string>
# include "Type.hpp"

class Complex : public Type
{
	public:
		Complex(void);
		Complex(double r, double i);

		virtual	~Complex();

		double GetReal(void) const;
		double GetIma(void) const;
		std::string ToPrint() const;
		static void Check(std::string & str);
	private:
		double	_r;
		double	_i;
};

std::ostream & operator<<(std::ostream & o, Complex & rhs);
Complex operator+(Complex lhs, Complex & rhs);
Complex operator+(Complex lhs, double & rhs);
Complex operator-(Complex lhs, Complex & rhs);
Complex operator-(Complex lhs, double & rhs);
Complex operator/(Complex lhs, Complex & rhs);
Complex operator/(Complex lhs, double & rhs);
Complex operator*(Complex lhs, Complex & rhs);
Complex operator*(Complex lhs, double & rhs);

#endif
