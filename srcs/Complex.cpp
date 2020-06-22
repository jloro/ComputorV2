#include "Complex.hpp"
#include <cmath>

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

Complex operator+(Complex lhs, Complex & rhs)
{
	Complex ret(lhs.GetReal() + rhs.GetReal(), lhs.GetIma() + rhs.GetIma());
	return ret;
}

Complex operator+(Complex lhs, double & rhs)
{
	Complex r(rhs, 0);
	return lhs + r;
}

Complex operator-(Complex lhs, Complex & rhs)
{
	Complex ret(lhs.GetReal() - rhs.GetReal(), lhs.GetIma() - rhs.GetIma());
	return ret;
}

Complex operator-(Complex lhs, double & rhs)
{
	Complex r(rhs, 0);
	return lhs - r;
}

Complex operator/(Complex lhs, Complex & rhs)
{
	double a = lhs.GetReal();
	double ap = rhs.GetReal();
	double b = lhs.GetIma();
	double bp = rhs.GetIma();

	Complex ret((a * ap + b * bp) / (ap * ap + bp * bp), (b * ap - a * bp) / (ap * ap + bp * bp));

	return ret;
}

Complex operator/(Complex lhs, double & rhs)
{
	Complex ret(lhs.GetReal() / rhs, lhs.GetIma() / rhs);
	return ret;
}

Complex operator*(Complex lhs, Complex & rhs)
{
	double a = lhs.GetReal();
	double ap = rhs.GetReal();
	double b = lhs.GetIma();
	double bp = rhs.GetIma();

	Complex ret(a * ap - b * bp, a * bp + b * ap);

	return ret;
}

Complex operator*(Complex lhs, double & rhs)
{
	Complex ret(lhs.GetReal() * rhs, lhs.GetIma() * rhs);
	return ret;
}

double Complex::GetReal(void) const { return _r; }
double Complex::GetIma(void) const { return _i; }
std::string Complex::ToPrint() const { return std::string("complex"); }
void Complex::Check(std::string & str) { (void)str; }
