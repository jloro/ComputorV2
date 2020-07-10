#include "Core.hpp"
#include <curses.h>
#include <cctype>
#include <regex>
#include <stdexcept>
#include "Real.hpp"
#include "Matrix.hpp"
#include <sstream>
#include "Complex.hpp"
#include <stack>

std::string	Core::Dtoa(double n)
{
	std::ostringstream ss;

	ss << std::fixed;
	ss << std::setprecision(10);
	ss << n;
	std::string str = ss.str();
	while ((str.back() == '0' || str.back() == '.')  && str.find('.') != std::string::npos)
		str.erase(str.length() - 1, 1);
	if (str.compare("-0") == 0)
		str = "0";
	return str;
}

std::string Core::ToLower(std::string str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		*it = tolower(*it);
	return str;
}

bool Core::CheckChar(int c)
{
	if (c == ' ' || c == '%' || (c >= '(' && c <= '9') || c == ';' || c == '=' || c == '?' || c == '^' || c == '[' || c == ']')
		return true;
	return false;
}

Core::Core() {}

Core::~Core() {}

void Core::Calcul()
{
	if (std::regex_search(_cmd, std::regex("(?:[a-z]{2,})|(?:[a-h]|[j-z])")))
	{
		printw("%s\n", _cmd.c_str());
	}
	else if (_cmd.find("[") != std::string::npos)
	{
		printw("%s\n", Matrix::EvalExpr(_cmd.substr(0, _cmd.find('='))).ToPrint().c_str());
	}
	else if (_cmd.substr(0, _cmd.find("=")).find("i") != std::string::npos)
	{
		printw("%s\n", Complex::EvalExpr(_cmd.substr(0, _cmd.find('='))).ToPrint().c_str());
	}
	else
	{
		printw("%s\n", Core::Dtoa(Real::EvalExpr(_cmd.substr(0, _cmd.find('='))).GetValue()).c_str());
	}
}

void Core::Assignation()
{
	if (_cmd.rfind("(", _cmd.find("=")) != std::string::npos)
	{
		_mapFun[Core::ToLower(_cmd.substr(0, _cmd.find("(")))] = new Function(_cmd);
		printw("%s\n", _mapFun[Core::ToLower(_cmd.substr(0, _cmd.find("(")))]->ToPrint().c_str());
	}
	else if (_cmd.find("[") != std::string::npos)
	{
		Matrix value = Matrix::EvalExpr(_cmd.substr(_cmd.find("=") + 1));
		printw("%s\n", value.ToPrint().c_str());
		_map[Core::ToLower(_cmd.substr(0, _cmd.find("=")))] = new Matrix(value.ToString());
	}
	else if (_cmd.substr(_cmd.find("=") + 1).find("i") != std::string::npos)
	{
		Complex value = Complex::EvalExpr(_cmd.substr(_cmd.find("=") + 1));
		printw("%s\n", value.ToPrint().c_str());
		Complex *ret = new Complex(value.GetReal(), value.GetIma());
		_map[Core::ToLower(_cmd.substr(0, _cmd.find("=")))] = ret;
	}
	else
	{
		Real value = Real::EvalExpr(_cmd.substr(_cmd.find("=") + 1));
		printw("%s\n", Core::Dtoa(value.GetValue()).c_str());
		_map[Core::ToLower(_cmd.substr(0, _cmd.find("=")))] = new Real(value.GetValue());
	}
}

void Core::ReplaceVar()
{
	//checker
	if (_cmd.find('=') == std::string::npos)
		throw std::runtime_error("Syntax error: no equals sign found.");
	if (std::count(_cmd.begin(), _cmd.end(), '=') != 1)
		throw std::runtime_error("Syntax error: multiple equals sign found.");

	std::smatch m;

	std::string str, value, varStr, tmp;
	int pos;
	bool fct = false;

	//calcul
	if (std::regex_match(_cmd, std::regex(".*\\?$")))
		str = _cmd.substr(0, _cmd.find("="));
	//assignation
	else
	{
		if (_cmd.substr(0, _cmd.find('=')).find('(') != std::string::npos)
		{
			fct = true;
			varStr = _cmd.substr(0, _cmd.find('=')).substr(_cmd.find('(') + 1, _cmd.find(')') - _cmd.find('(') - 1);
			if (std::any_of(varStr.begin(), varStr.end(), [](int c) { return isalpha(c) == 0; }) || varStr.compare("") == 0 || varStr.compare("i") == 0)
				throw std::runtime_error("Syntax error: variable of function incorrect.");
		}
		str = _cmd.substr(_cmd.find("=") + 1);
	}

	tmp = str;
	std::stack<std::string> stack;
	while (std::regex_search(tmp, m, std::regex("(?:[a-z]{2,})|(?:[a-h]|[j-z])")))
	{
		if (_map.find(m.str()) == _map.end() && m.str().compare("i") != 0 && m.str().compare(varStr) != 0 && _mapFun.find(m.str()) == _mapFun.end()) 
			throw std::runtime_error("Syntax error: variable "+m.str()+" unknow.");

		/*
		if (_mapFun.find(m.str()) != _mapFun.end() && _mapFun[m.str()]->GetVar().compare(tmp.substr(tmp.find('(') + 1, tmp.find(')') - tmp.find('(') - 1)) == 0 && _map.find(tmp.substr(tmp.find('(') + 1, tmp.find(')') - tmp.find('(') - 1)) == _map.end())
		{
			stack.push(m.str());
			tmp.replace(tmp.find('(') + 1, tmp.find(')') - tmp.find('(') - 1, "0");
		}
		else if (m.str().compare("i") != 0 && !(fct && m.str().compare(varStr) == 0))
			stack.push(m.str());
*/
		//2t => 2*, while because if 2 times same var
		std::string::size_type pos = -1;
		while ((pos = _cmd.find(m.str(), pos + 1)) != std::string::npos)
		{
			if (isdigit(_cmd[pos - 1]) != 0)
				_cmd.insert(pos, "*");
			if (isdigit(_cmd[pos + m.length()]) != 0)
				_cmd.insert(pos + m.length(), "*");
		}

		tmp.replace(m.position(), m.length(), "0");

	}

	int len = 0;

	while (std::regex_search(str, m, std::regex("[a-z]+")))
	{
		if (m.str().compare("i") == 0 || (fct && m.str().compare(varStr) == 0))
		{
			str = m.suffix();
			continue;
		}
		stack.push(m.str());
		str = m.suffix();
	}

	while (stack.size() != 0)
	{
		tmp = stack.top();
		stack.pop();

		len = tmp.length();
		pos = _cmd.rfind(tmp);
		Type* var = _map[Core::ToLower(tmp)];
		if (_mapFun.find(tmp) != _mapFun.end())
		{
			str = _cmd.substr(pos);
			varStr = str.substr(str.find('(') + 1, str.find(')') - str.find('(') - 1);
			//printw("cmd:%s\n", _cmd.c_str());
			value = _mapFun[Core::ToLower(tmp)]->Solve(varStr);
			len += 2 + varStr.length();
		}
		else if (var->GetType() == eType::Real)
			value = Core::Dtoa(static_cast<Real*>(var)->GetValue());
		else if (var->GetType() == eType::Matrix)
			value = static_cast<Matrix*>(var)->ToString();
		else if (var->GetType() == eType::Complex)
			value = "("+static_cast<Complex*>(var)->ToString()+")";

		_cmd.replace(pos, len, value);
	}
}

void Core::Checker()
{
	std::string calc;
	std::smatch m;

	if (std::regex_match(_cmd, std::regex(".*\\?$")))//calcul
	{
		calc = _cmd.substr(0, _cmd.find('='));
	}
	else//assignation
	{
		calc = _cmd.substr(_cmd.find("=") + 1);
		std::string name = _cmd.substr(0, _cmd.find('='));
		if (name.find('(') != std::string::npos)
		{
			std::string var = name.substr(_cmd.find('(') + 1, _cmd.find(')') - _cmd.find('(') - 1);
			std::string::size_type pos;
			while ((pos = calc.find(var)) != std::string::npos)
				calc.replace(pos, var.length(), "0");
			name = name.substr(0, name.find('('));
		}
		if (name.compare("i") == 0)
			throw std::runtime_error("Syntax error: variable name can't be 'i'.");
		if (std::any_of(name.begin(), name.end(), [](int c) { return isalpha(c) == 0; }) || name.compare("") == 0)
			throw std::runtime_error("Syntax error: variable name must be only letters.");
	}

	
	if (std::count(_cmd.begin(), _cmd.end(), '(') != std::count(_cmd.begin(), _cmd.end(), ')'))
		throw std::runtime_error("Syntax error: wrong number of parenthesis.");

	while (_cmd.find(")(") != std::string::npos)
		_cmd.insert(_cmd.find(")(") + 1, "*");

	std::string tmp = _cmd;
	if (std::regex_search(tmp, m, std::regex("\\d\\(")))
		throw std::runtime_error("Syntax error.");

	while (std::regex_search(tmp, m, std::regex("\\.")))
	{
		if (isdigit(tmp[m.position() - 1]) == 0 || isdigit(tmp[m.position() + 1]) == 0)
			throw std::runtime_error("Syntax error.");
		tmp = m.suffix();
	}

	void (*check)(std::string&);

	if (calc.find(']') != std::string::npos)
		check = Matrix::Check;
	else if (calc.find('i') != std::string::npos)
		check = Complex::Check;
	else if (!std::regex_search(calc, std::regex("(?:[a-z]{2,})|(?:[a-h]|[j-z])")))
		check = Real::Check;
	else//POLYNOM ou pas
		return;

	while (calc.find(")") != std::string::npos)
	{
		int posFirstP = calc.find_last_of("(");
		int posLastP = calc.find(")", calc.rfind("("));
		std::string subcalc = calc.substr(posFirstP + 1, posLastP - posFirstP - 1);
		check(subcalc);
		calc.replace(posFirstP, posLastP - posFirstP + 1, "0");
	}
	check(calc);
}

void Core::Exec()
{
	_cmd = Core::ToLower(_cmd);
	int i = -1; 
	_cmd.erase(std::remove_if(_cmd.begin(), _cmd.end(), [&i, this](int c) {
		i++;
		if (c == ' ')
		{
			if (_cmd[i - 1] && !Core::CheckChar(_cmd[i - 1]) && _cmd[i + 1] && !Core::CheckChar(_cmd[i + 1]))
				return false;
			else
				return true;
		}
		return false;
	}), _cmd.end());
	ReplaceVar();
	Checker();
	if (std::regex_match(_cmd, std::regex(".*\\?$")))
		Calcul();
	else
		Assignation();
}

void Core::PrintMap()
{
	for (std::map<std::string, Type*>::iterator it = _map.begin(); it != _map.end(); it++)
	{
		printw(it->first.c_str());
		printw(" = ");
		printw(it->second->ToPrint().c_str());
		printw("\n");
	}
}

void Core::Loop()
{
	while (1)
	{
		printw("> ");
		ReadLine(_historic);
		if (_cmd.compare("exit") == 0 || _cmd.compare("q") == 0)
			break;
		_historic.Add(_cmd);
		printw("\n");
		if (_cmd.compare("print") == 0)
			PrintMap();
		else
		{
			try
			{
				Exec();
			}
			catch (std::exception & e)
			{
				printw(e.what());
				printw("\n");
			}
		}
		_cmd.clear();
	}
}

void	Core::ReadLine(Historic & historic)
{
	int old_curs = curs_set(1);
	size_t pos = 0;
	int x, y, c;
	getyx(stdscr, y, x);

	while (1)
	{
		mvaddnstr(y, x, _cmd.c_str(), _cmd.size());
		move(y, x+pos);
		c = getch();

		if (c == KEY_ENTER || c == '\n' || c == '\r')
			break;
		else if (isprint(c)) 
		{
			if (_cmd.size() < 100)
				_cmd.insert(_cmd.begin() + pos++, c);
		} 
		else if (c == 260) 
		{
			if (pos > 0)
				pos -= 1;
		}
		else if (c == KEY_RIGHT) 
		{
			if (pos < _cmd.size())
				pos += 1;
		} 
		else if (c == KEY_UP || c == KEY_DOWN) 
		{
			if (c == KEY_UP)
				_cmd = historic.GetPrevCmd();
			else
				_cmd = historic.GetNextCmd();
			if (pos > _cmd.size())
			{
				while (pos > _cmd.size())
				{
					delch();
					pos--;
					move(y, x+pos);
				}
				delch();
			}
			pos = _cmd.size();
		} 
		else if (c == 127) 
		{
			if (pos > 0) 
			{
				historic.ResetIt();
				_cmd.erase(pos - 1, 1);
				mvdelch(y, x + pos - 1);
				pos -= 1;
			}
		}
	}

	mvaddnstr(y, x, _cmd.c_str(), _cmd.size());
	if (old_curs != ERR) 
		curs_set(old_curs);
}
