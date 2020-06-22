#include "Core.hpp"
#include <curses.h>
#include <cctype>
#include <regex>
#include <stdexcept>
#include "Real.hpp"
#include "Matrix.hpp"
#include <sstream>

#define FCT "(\\w+\\(\\w\\))="

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
	if (_cmd.find("[") != std::string::npos)
	{
		printw("%s\n", Matrix::EvalExpr(_cmd.substr(0, _cmd.find('='))).ToPrint().c_str());
	}
	else
	{
		printw("%s\n", Core::Dtoa(Real::EvalExpr(_cmd.substr(0, _cmd.find('=')))).c_str());
	}
}

void Core::Assignation()
{
	if (_cmd.find("[") != std::string::npos)
	{
		Matrix value = Matrix::EvalExpr(_cmd.substr(_cmd.find("=") + 1));
		printw("%s\n", value.ToPrint().c_str());
		_map[Core::ToLower(_cmd.substr(0, _cmd.find("=")))] = new Matrix(value.ToString());
	}
	else
	{
		double value = Real::EvalExpr(_cmd.substr(_cmd.find("=") + 1));
		printw("%s\n", Core::Dtoa(value).c_str());
		_map[Core::ToLower(_cmd.substr(0, _cmd.find("=")))] = new Real(value);
	}
}

void Core::ReplaceVar()
{
	std::smatch m;
	std::string str, value;
	int pos;
	if (std::regex_match(_cmd, std::regex(".*\\?$")))
		str = _cmd.substr(0, _cmd.find("="));
	else
		str = _cmd.substr(_cmd.find("=") + 1);
	while (std::regex_search(str, m, std::regex("[a-z]+")))
	{
		Type* var = _map[Core::ToLower(m.str())];
		if (var->GetType() == eType::Real)
			value = Core::Dtoa(static_cast<Real*>(var)->GetValue());
		else if (var->GetType() == eType::Matrix)
			value = static_cast<Matrix*>(var)->ToString();
		if (!std::regex_match(_cmd, std::regex(".*\\?$")))
			pos = _cmd.find(m.str(), _cmd.find('='));
		else
			pos = _cmd.find(m.str());
		_cmd.erase(pos, m.length());
		_cmd.insert(pos, value);

		pos = str.find(m.str());
		str.erase(pos, m.length());
		str.insert(pos, value);
	}
}

void Core::Checker()
{
	std::string calc;
	std::smatch m;

	if (_cmd.find('=') == std::string::npos)
		throw std::runtime_error("Syntax error: no equals sign found.");
	if (std::count(_cmd.begin(), _cmd.end(), '=') != 1)
		throw std::runtime_error("Syntax error: multiple equals sign found.");

	if (std::regex_match(_cmd, std::regex(".*\\?$")))//calcul
	{
		calc = _cmd.substr(0, _cmd.find('='));
	}
	else//assignation
	{
		calc = _cmd.substr(_cmd.find("=") + 1);
		std::string name = _cmd.substr(0, _cmd.find('='));
		if (name.compare("i") == 0)
			throw std::runtime_error("Syntax error: variable name can't be 'i'.");
		if (std::any_of(name.begin(), name.end(), [](int c) { return isalpha(c) == 0; }) || name.compare("") == 0)
			throw std::runtime_error("Syntax error: variable name must be only letters.");
	}

	while (std::regex_search(calc, m, std::regex("[a-z]+")))
	{
		if (_map.find(m.str()) == _map.end() && m.str().compare("i") != 0) 
			throw std::runtime_error("Syntax error: variable "+m.str()+" unknow.");
		calc.erase(m.position(), m.length());
		calc.insert(m.position(), "0");
	}
	
	if (std::count(calc.begin(), calc.end(), '(') != std::count(calc.begin(), calc.end(), ')'))
		throw std::runtime_error("Syntax error: wrong number of parenthesis.");

	while (_cmd.find(")(") != std::string::npos)
		_cmd.insert(_cmd.find(")(") + 1, "*");

	if (calc.find(']') != std::string::npos)
	{
		while (calc.find(")") != std::string::npos)
		{
			int posFirstP = calc.find_last_of("(");
			int posLastP = calc.find(")", calc.rfind("("));
			std::string subcalc = calc.substr(posFirstP + 1, posLastP - posFirstP - 1);
			Matrix::Check(subcalc);
			calc.erase(posFirstP, posLastP - posFirstP + 1);
			calc.insert(posFirstP, "0");
		}
		Matrix::Check(calc);
	}
	else if (calc.find('i') != std::string::npos)
	{
	}
	else
	{
		std::string tmp = _cmd;
		while (std::regex_search(tmp, m, std::regex("\\.")))
		{
			if (isdigit(tmp[m.position() - 1]) == 0 || isdigit(tmp[m.position() + 1]) == 0)
				throw std::runtime_error("Syntax error.");
			tmp = m.suffix();
		}
		if (std::regex_search(tmp, m, std::regex("\\d\\(")))
			throw std::runtime_error("Syntax error.");
		while (calc.find(")") != std::string::npos)
		{
			int posFirstP = calc.find_last_of("(");
			int posLastP = calc.find(")", calc.rfind("("));
			std::string subcalc = calc.substr(posFirstP + 1, posLastP - posFirstP - 1);
			Real::Check(subcalc);
			calc.erase(posFirstP, posLastP - posFirstP + 1);
			calc.insert(posFirstP, "0");
		}
		Real::Check(calc);
	}
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
	Checker();
	ReplaceVar();
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
		if (_cmd.compare("exit") == 0)
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
