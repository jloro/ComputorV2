#ifndef REAL_HPP
# define REAL_HPP

# include "Type.hpp"
# include <string>

class Real : public Type
{
	public:
		Real(void);
		Real(double v);

		virtual	~Real();

		Real &	operator=(const Real & rhs);
		Real &	operator=(const double & rhs);
		Real &	operator+(const Real & rhs);
		Real &	operator+(const double & rhs);
		Real &	operator-(const Real & rhs);
		Real &	operator-(const double & rhs);
		Real &	operator*(const Real & rhs);
		Real &	operator*(const double & rhs);
		Real &	operator/(const Real & rhs);
		Real &	operator/(const double & rhs);

		double	GetValue() const;
		std::string	ToPrint() const;
		static void Check(std::string & str);

		static double EvalExpr(std::string str);
	private:
		double	_v;

		static void Calc(std::string & str, int firstSub, int endSub, int posSubcalc, int firstEra, int endEra);
};

#endif
