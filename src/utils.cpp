/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:25:51 by sbenes            #+#    #+#             */
/*   Updated: 2023/11/27 16:50:39 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"

/* print - will print message with optional args - color and output stream (cout or cerr = 1 or 2) */
void print(string message, string color, int output)
{
	switch (output)
	{
		case 2:
			cerr << color << message << RESET << endl;
			break;
		default:
			cout << color << message << RESET << endl;
			break;
	}
}


/* CppSplit - splits on delimiter, puts string in vector */
std::vector<string>	CppSplit(std::string str, char delimiter)
{
	std::vector<string>		result;
	std::stringstream		ss(str);
	string					token;

	while (std::getline(ss, token, delimiter))
		if (!token.empty())
			result.push_back(token);
	return (result);
}
