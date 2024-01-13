/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/13 16:29:47 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"
#include "../include/Parser.hpp"
#include "../include/Server.hpp"
#include "../include/ServerManager.hpp"
#include "../include/Mime.hpp"
#include "../include/Interface.hpp"
#include "../include/Log.hpp"

#include <iostream>
#include <cstdlib>

bool debug = false;
bool debugFile = false;

/* int main(int argc, char **argv)
{
	if (argc < 2)
	{
		print("Usage: ./webserv <config_file>", RED, 2);
		exit(EXIT_FAILURE);
	}
	if (argv[2] && !strcmp(argv[2], "-d"))
	{	
		debug = true;
		cout << RED << "Debug mode on" << RESET << endl;
	}

	if (argv[2] && !strcmp(argv[2], "-df"))
	{
		debugFile = true;
		cout << RED << "Debug file mode on" << RESET << endl;
	}
	Parser	parser(argv[1]);
	parser.parseFile(argv[1]);
	parser.printServers();

	// Mime mime;
	// mime.testPrint("test/anotherdir/test.jpeg");
	
	std::vector<Server> servers = parser.getServers();
	Interface interface(parser, servers);
	interface.start();

	// ServerManager	manager;
	// manager.initServers(parser.getServers());
	// manager.runServers();
	

	
	return 0;
} */

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		print("Usage: ./webserv <config_file>", RED, 2);
		exit(EXIT_FAILURE);
	}
	if (argv[2] && !strcmp(argv[2], "-log"))
	{	
		Log::out_console = true;
		cout << RED << "Log to console mode on" << RESET << endl;
	}

	if (argv[2] && !strcmp(argv[2], "-logfile"))
	{
		Log::out_file = true;
		cout << RED << "Log to file mode on" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--all"))
	{	
		Log::type_info = true;
		Log::type_warning = true;
		Log::type_error = true;
		cout << RED << "All log types on" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--debug"))
	{	
		Log::type_debug = true;
		cout << RED << "Debug only log type on" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--info"))
	{	
		Log::type_info = true;
		cout << RED << "Info log type on" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--warning"))
	{	
		Log::type_warning = true;
		cout << RED << "Warning log type on" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--error"))
	{	
		Log::type_error = true;
		cout << RED << "Error log type on" << RESET << endl;
	}

	Parser	parser(argv[1]);
	parser.parseFile(argv[1]);
	parser.printServers();
	
	std::vector<Server> servers = parser.getServers();
	Interface interface(parser, servers);
	interface.start();

	return 0;
}