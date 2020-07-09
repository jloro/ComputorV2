#include "Function.hpp"
#include "Matrix.hpp"
#include "Real.hpp"
#include "Complex.hpp"

Function::Function(const std::string & str)
{
	if (str.find("[") != std::string::npos)
		_type = eType::Matrix;
	else if (str.find("i") != std::string::npos)
		_type = eType::Complex;
	else
		_type = eType::Real;

	std::string tmp = str.substr(0, str.find('='));
	_var = tmp.substr(tmp.find('(') + 1, tmp.find(')') - (tmp.find('(') + 1));
	_fct = str.substr(str.find('=') + 1);
}

Function::~Function() {}
/*
Function &	Function::operator=(const Function & rhs)
{

}
*/

std::string Function::ToPrint() const
{
	std::string ret = _fct;
	for (auto it = ret.begin(); it != ret.end(); it++)
	{
		if ((*it == '*' || *it == '+' || *it == '-' || *it == '/' || *it == '%') && *(it + 1) != ' ')
		{
			ret.insert(it + 1, ' ');
			it = ret.begin();
		}
		else if ((*it == '*' || *it == '+' || *it == '-' || *it == '/' || *it == '%') && it != ret.begin() && *(it - 1) != ' ')
		{
			ret.insert(it, ' ');
			it = ret.begin();
		}
	}
	return ret;
}
#include "ncurses.h"
std::string Function::Solve(std::string var) const
{
	std::string::size_type pos;
	std::string fct = _fct;

	printw("fct:%s varl:%s  varn:%s\n", fct.c_str(), _var.c_str(), var.c_str());
	while ((pos = fct.find(_var)) != std::string::npos)
		fct.replace(pos, _var.length(), var);

	if (_type == eType::Real)
	{
		if (var.find("[") != std::string::npos)
			return Matrix::EvalExpr(fct).ToPrint();
		else if (var.find("i") != std::string::npos)
			return Complex::EvalExpr(fct).ToPrint();
		else
			return Real::EvalExpr(fct).ToPrint();
	}
	else if (_type == eType::Matrix)
		return Matrix::EvalExpr(fct).ToPrint();
	else
		return Complex::EvalExpr(fct).ToPrint();
}
