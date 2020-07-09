#ifndef CORE_HPP
# define CORE_HPP

# include "Historic.hpp"
# include <string>
# include <map>
# include "Type.hpp"
# include "Function.hpp"

class Core
{
	public:
		Core(void);
		virtual	~Core();

		void Loop();
		static std::string Dtoa(double n);
		static std::string ToLower(std::string str);
		static bool CheckChar(int c);
	private:
		Historic _historic;
		std::string _cmd;
		std::map<std::string, Type*>		_map;
		std::map<std::string, Function*>	_mapFun;

		void Exec();
		void ReadLine(Historic & historic);
		void Assignation();
		void Calcul();
		void PrintMap();
		void ReplaceVar();
		void Checker();
		template<class T>
		std::string SolveFun(std::string var);

};

#endif
