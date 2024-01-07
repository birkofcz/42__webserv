/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/07 16:37:00 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"
#include "../include/Parser.hpp"
#include "../include/Server.hpp"
#include "../include/ServerManager.hpp"
#include "../include/Mime.hpp"
#include <iostream>
#include <cstdlib>

bool debug = false;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		print("Usage: ./webserv <config_file>", RED, 2);
		exit(EXIT_FAILURE);
	}
	if (argv[2] && !strcmp(argv[2], "-d"))
		debug = true;
	Parser	parser(argv[1]);
	parser.parseFile(argv[1]);
	parser.printServers();

	// Mime mime;
	// mime.testPrint("test/anotherdir/test.jpeg");
	

	ServerManager	manager;
	manager.initServers(parser.getServers());
	manager.runServers();
	
	return 0;
}