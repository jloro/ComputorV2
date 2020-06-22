#include "Historic.hpp"
#include <cstdio>
#include <iostream>
#include <curses.h>

Historic::Historic()
{
	char tmp[256];
	std::fstream fs;
	fs.open (PATH, std::fstream::in);
	if (fs)
	{
		while (fs.getline(tmp, 256, '\n'))
			_cmds.push_back(std::string(tmp));
	}
	fs.close();
	remove(PATH);
	_it = _cmds.end();
}

Historic::~Historic()
{
	std::fstream fs;
	fs.open (PATH, std::fstream::out | std::fstream::app);
	for (std::string cmd : _cmds)
		fs << cmd << std::endl;
	fs.close();
}

void Historic::Add(const std::string cmd)
{
	_cmds.push_back(cmd);
	if (_cmds.size() > HISTORIC_MAX)
		_cmds.pop_front();
	_it = _cmds.end();
}

int Historic::GetSize() const {return _cmds.size(); }

std::string Historic::GetPrevCmd()
{
	if (_it != _cmds.begin())
		_it--;
	return *_it;
}

std::string Historic::GetNextCmd()
{
	if (_it != _cmds.end())
		_it++;

	if (_it == _cmds.end())
		return std::string("");
	return *_it;
}

void Historic::ResetIt()
{
	_it = _cmds.end();
}
