/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/14 11:13:24 by sbenes           ###   ########.fr       */
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
void setLogFlags(char **argv)
{
	cout << "									   " << endl;
	cout << GREEN << " __ _____ ___ __                     " << endl;
	cout <<  "| _|_   _/ __|_ |___ _ ___ _____ _ _ " << endl;
	cout << "| |  | | \\__ \\| / -_| '_\\ V / -_| '_|" << endl;
	cout << "| |  |_| |___/| \\___|_|  \\_/\\___|_|  " << endl;
	cout << "|__|         |__|                    " << endl;
	cout << "									   " << RESET << endl << endl;

	if (!argv[2])
	{
		Log::out_console = true;
		Log::type_info = true;
		Log::type_warning = true;
		Log::type_error = true;
		cout << GREEN << "[TS]erver logging: default - CONSOLE logs ON" << RESET << endl;
		cout << GREEN << "[TS]erver logging: default - RUNTIME logs ON (no DEBUG)" << RESET << endl;
	}
	if (argv[2] && !argv[3])
	{
		if (!strcmp(argv[2], "-log"))
		{	
			Log::out_console = true;
			cout << GREEN << "[TS]erver logging: CONSOLE logs ON" << RESET << endl;
		}
		if (!strcmp(argv[2], "-logfile"))
		{
			Log::out_file = true;
			cout << GREEN << "[TS]erver logging: FILE logs ON" << RESET << endl;
		}
		Log::type_info = true;
		Log::type_warning = true;
		Log::type_error = true;
		cout << GREEN << "[TS]erver logging: default - RUNTIME logs ON (no DEBUG)" << RESET << endl;
	}
	if (argv[2] && argv[3] && !strcmp(argv[2], "-log"))
	{	
		Log::out_console = true;
		cout << GREEN << "[TS]erver logging: CONSOLE logs ON" << RESET << endl;
	}
	if (argv[2] && argv[3] && !strcmp(argv[2], "-logfile"))
	{
		Log::out_file = true;
		cout << GREEN << "[TS]erver logging: FILE logs ON" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--all"))
	{	
		Log::type_info = true;
		Log::type_warning = true;
		Log::type_error = true;
		Log::type_debug = true;
		cout << GREEN << "[TS]erver logging: ALL log types ON" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--debug"))
	{	
		Log::type_debug = true;
		cout << GREEN << "[TS]erver logging: DEBUG log ON" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--info"))
	{	
		Log::type_info = true;
		cout << GREEN << "[TS]erver logging: INFO log ON" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--warning"))
	{	
		Log::type_warning = true;
		cout << GREEN << "[TS]erver logging: WARNING log ON" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--error"))
	{	
		Log::type_error = true;
		cout << GREEN << "[TS]erver logging: ERROR log ON" << RESET << endl;
	}
	if (argv[3] && !strcmp(argv[3], "--runtime"))
	{	
		Log::type_info = true;
		Log::type_warning = true;
		Log::type_error = true;
		cout << RED << "Runtime log types on" << RESET << endl;
		cout << GREEN << "[TS]erver logging: RUNTIME logs ON (no DEBUG)" << RESET << endl;
	}

}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		print("Usage: ./webserv <config_file>", RED, 2);
		exit(EXIT_FAILURE);
	}
	setLogFlags(argv);

	Parser	parser(argv[1]);
	parser.parseFile(argv[1]);
	//parser.printServers();
	
	std::vector<Server> servers = parser.getServers();
	Interface interface(parser, servers);
	interface.start();

	return 0;
}