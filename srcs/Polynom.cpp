/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynom.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jloro <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 10:17:11 by jloro             #+#    #+#             */
/*   Updated: 2020/07/28 09:42:54 by jules            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <regex>
#include <map>
#include <vector>
#include <ncurses.h>
#include "Core.hpp"

double abs(double n)
{
	if (n < 0)
		return -n;
	return n;
}

double sqrtd(double n, double epsilon = 1e-10)
{
	if (n < 0)
		return -1;
	double res = 0;
	double d = 1;
	double tmp = 0;

	while (1)
	{
		tmp = res;
		while (tmp * tmp < n)
			tmp += d;

		if (tmp * tmp - n < epsilon)
			return tmp;
		tmp -= d;
		res = tmp;
		d /= 10;
	}
}


void normEquation(std::string & equa)
{
	if (std::regex_search(equa, std::regex("x\\^(?:[^0-2]|[0-2](?:\\d|\\.\\d))")))
		throw std::runtime_error("Polynomial degree unknown.");
}

void	getCoefs(std::string equa, std::map<std::string, double> & coefs, bool rhs)
{
	std::regex rgx("((?:\\+|-|^)\\d+(?:\\.\\d+)?)\\*x\\^([0-2])");
	std::regex_iterator<std::string::iterator> rit(equa.begin(), equa.end(), rgx);
	std::regex_iterator<std::string::iterator> rend;
	for (; rit != rend;rit++)
	{
		if (coefs.find((*rit)[2]) == coefs.end())
			coefs[(*rit)[2]] = rhs ? -std::stod((*rit)[1]) : std::stod((*rit)[1]);
		else
			coefs[(*rit)[2]] += rhs ? -std::stod((*rit)[1]) : std::stod((*rit)[1]);

		if (coefs[(*rit)[2]] == -0)
			coefs[(*rit)[2]] = 0;
	}
}

void	printReducedForm(std::map<std::string, double> & coefs)
{
	printw("Reduced Form: ");
	for (std::map<std::string, double>::iterator it=coefs.begin(); it!=coefs.end(); ++it)
	{
		if (it->second < 0)
		{
			if (it == coefs.begin())
				printw("-%s * x^%s ", Core::Dtoa(abs(it->second)).c_str(), it->first.c_str());
			else
				printw("- %s * x^%s ", Core::Dtoa(abs(it->second)).c_str(), it->first.c_str());
		}
		else	
		{
			if (it == coefs.begin())
				printw("%s * x^%s ", Core::Dtoa(it->second).c_str(), it->first.c_str());
			else
				printw("+ %s * x^%s ", Core::Dtoa(it->second).c_str(), it->first.c_str());
		}
	}
	printw("= 0\n");
}

void	quadratic(double a, double b, double c)
{
	double d = b*b - 4*a*c;
	printw("Polynomial degree : 2\n");
	if (d > 0)
	{
		printw("Discriminant strictly positive, the two solutions are : \n");
		printw("%s\n", Core::Dtoa((-b - sqrtd(d)) / (2 * a)).c_str());
		printw("%s\n", Core::Dtoa((-b + sqrtd(d)) / (2 * a)).c_str());
	}
	else if (d == 0)
	{
		printw("Discriminant is null, the solution is : \n");
		printw("%s\n", Core::Dtoa(-b / (2 * a)).c_str());
	}
	else
	{
		printw("Discriminant strictly negative, there 2 complex solutions : \n");
		printw("%s\n", Core::Dtoa(-b / (2 * a)).c_str());
		if (-sqrtd(abs(d)) / (2 * a) >= 0)
			printw(" + %si\n", Core::Dtoa(abs(-sqrtd(abs(d)) / (2 * a))).c_str());
		else
			printw(" - %si\n", Core::Dtoa(abs(-sqrtd(abs(d)) / (2 * a))).c_str());

		printw("%s\n", Core::Dtoa(-b / (2 * a)).c_str());
		if (sqrtd(abs(d)) / (2 * a) >= 0)
			printw(" + %si\n", Core::Dtoa(abs(sqrtd(abs(d)) / (2 * a))).c_str());
		else
			printw(" - %si\n", Core::Dtoa(abs(sqrtd(abs(d)) / (2 * a))).c_str());
	}
}

void	degreeOne(double a, double b)
{
	printw("Polynomial degree: 1\n");
	printw("The solution is:\n");
	printw("%s\n", Core::Dtoa(-b / a).c_str());
}

void Polynom(std::string equa)
{
	printw("%s\n", equa.c_str());
	std::map<std::string, double>	coefs;

	getCoefs(equa, coefs, false);

	std::vector<std::string> zeros;

	for (std::map<std::string, double>::iterator it=coefs.begin(); it!=coefs.end(); it++)
		if (it->second == 0.0)
			zeros.push_back(it->first);

	printReducedForm(coefs);	

	for (std::vector<std::string>::iterator it=zeros.begin(); it!=zeros.end(); it++)
			coefs.erase(*it);

	if (coefs.find("2") != coefs.end())
		quadratic(coefs["2"], coefs["1"], coefs["0"]);
	else if (coefs.find("1") != coefs.end())
		degreeOne(coefs["1"], coefs["0"]);
	else
	{
		if (coefs["0"] == 0)
			printw("All reals are solutions.\n");
		else
			printw("There is no solutions.\n");
	}
}
