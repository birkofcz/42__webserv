/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/03 15:45:27 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"
#include "../include/Parser.hpp"
#include "../include/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		print("Usage: ./webserv <config_file>", RED, 2);
		return 1;
	}
	Parser parser(argv[1]);
	parser.parseFile(argv[1]);
	parser.printServers();
	
	return 0;
}