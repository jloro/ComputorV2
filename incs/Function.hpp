/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Function.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jules <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 09:45:44 by jules             #+#    #+#             */
/*   Updated: 2020/07/10 13:20:09 by jules            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTION_HPP
# define FUNCTION_HPP

# include "Type.hpp"
# include <string>

class Function
{
	public:
		Function(const std::string & str);
		virtual	~Function();

		//Function &	operator=(const Function & rhs);
		std::string Solve(std::string var) const;
		std::string ToPrint() const;

		std::string GetVar() const;
	private:
		std::string	_var;
		eType	_type;
		std::string	_fct;
};

#endif
