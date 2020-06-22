#ifndef HISTORIC_HPP
# define HISTORIC_HPP

# include <fstream>
# include <string>
# include <list>
# define PATH ".historic.txt"
# define HISTORIC_MAX 30

class Historic
{
	public:
		Historic(void);
		virtual	~Historic();

		void Add(const std::string cmd);
		int GetSize() const;
		std::string GetPrevCmd();
		std::string GetNextCmd();
		void ResetIt();
	private:
		std::list<std::string>	_cmds;
		std::list<std::string>::iterator _it;
};

#endif
