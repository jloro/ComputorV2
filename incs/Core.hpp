#ifndef CORE_HPP
# define CORE_HPP

# include "Historic.hpp"
# include <string>
# include <map>
# include "Type.hpp"

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
		std::map<std::string, Type*>	_map;

		void Exec();
		void ReadLine(Historic & historic);
		void Assignation();
		void Calcul();
		void PrintMap();
		void ReplaceVar();
		void Checker();

};

#endif
