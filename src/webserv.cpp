/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/27 16:40:07 by sbenes           ###   ########.fr       */
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

/*
setLogs - sets the logging options based on the flags passed, prints the logo and
the logging options set
 */
// void setLogs(char **argv)
// {
// 	std::system("clear"); //clears the terminal

// 	cout << "									   " << endl;
// 	cout << GREEN << " __ _____ ___ __                     " << endl;
// 	cout <<  "| _|_   _/ __|_ |___ _ ___ _____ _ _ " << endl;
// 	cout << "| |  | | \\__ \\| / -_| '_\\ V / -_| '_|" << endl;
// 	cout << "| |  |_| |___/| \\___|_|  \\_/\\___|_|  " << endl;
// 	cout << "|__|         |__|                    " << endl;
// 	cout << "									   " << RESET << endl << endl;

// 	if (!argv[2])
// 	{
// 		Log::out_console = true;
// 		Log::type_info = true;
// 		Log::type_warning = true;
// 		Log::type_error = true;
// 		cout << GREEN << "[TS]erver logging: default - CONSOLE logs ON" << RESET << endl;
// 		cout << GREEN << "[TS]erver logging: default - RUNTIME logs ON (no DEBUG)" << RESET << endl;
// 	}
// 	if (argv[2] && !argv[3])
// 	{
// 		if (!strcmp(argv[2], "-log"))
// 		{
// 			Log::out_console = true;
// 			cout << GREEN << "[TS]erver logging: CONSOLE logs ON" << RESET << endl;
// 		}
// 		if (!strcmp(argv[2], "-logfile"))
// 		{
// 			Log::out_console = true;
// 			Log::out_file = true;
// 			cout << GREEN << "[TS]erver logging: CONSOLE & FILE logs ON" << RESET << endl;
// 		}
// 		Log::type_info = true;
// 		Log::type_warning = true;
// 		Log::type_error = true;
// 		cout << GREEN << "[TS]erver logging: default - RUNTIME logs ON (no DEBUG)" << RESET << endl;
// 	}
// 	if (argv[2] && argv[3] && !strcmp(argv[2], "-log"))
// 	{
// 		Log::out_console = true;
// 		cout << GREEN << "[TS]erver logging: CONSOLE logs ON" << RESET << endl;
// 	}
// 	if (argv[2] && argv[3] && !strcmp(argv[2], "-logfile"))
// 	{
// 		Log::out_console = true;
// 		Log::out_file = true;
// 		cout << GREEN << "[TS]erver logging: CONSOLE & FILE logs ON" << RESET << endl;
// 	}
// 	if (argv[3] && !strcmp(argv[3], "--all"))
// 	{
// 		Log::type_info = true;
// 		Log::type_warning = true;
// 		Log::type_error = true;
// 		Log::type_debug = true;
// 		cout << GREEN << "[TS]erver logging: ALL log types ON" << RESET << endl;
// 	}
// 	if (argv[3] && !strcmp(argv[3], "--debug"))
// 	{
// 		Log::type_debug = true;
// 		cout << GREEN << "[TS]erver logging: DEBUG log ON" << RESET << endl;
// 	}
// 	if (argv[3] && !strcmp(argv[3], "--info"))
// 	{
// 		Log::type_info = true;
// 		cout << GREEN << "[TS]erver logging: INFO log ON" << RESET << endl;
// 	}
// 	if (argv[3] && !strcmp(argv[3], "--warning"))
// 	{
// 		Log::type_warning = true;
// 		cout << GREEN << "[TS]erver logging: WARNING log ON" << RESET << endl;
// 	}
// 	if (argv[3] && !strcmp(argv[3], "--error"))
// 	{
// 		Log::type_error = true;
// 		cout << GREEN << "[TS]erver logging: ERROR log ON" << RESET << endl;
// 	}
// 	if (argv[3] && !strcmp(argv[3], "--runtime"))
// 	{
// 		Log::type_info = true;
// 		Log::type_warning = true;
// 		Log::type_error = true;
// 		cout << RED << "Runtime log types on" << RESET << endl;
// 		cout << GREEN << "[TS]erver logging: RUNTIME logs ON (no DEBUG)" << RESET << endl;
// 	}
// 	cout << endl;

// 	// //control if the logging is set correctly
// 	// cout << "out_console: " << Log::out_console << endl;
// 	// cout << "out_file: " << Log::out_file << endl;
// 	// cout << "type_info: " << Log::type_info << endl;
// 	// cout << "type_warning: " << Log::type_warning << endl;
// 	// cout << "type_error: " << Log::type_error << endl;
// 	// cout << "type_debug: " << Log::type_debug << endl;
// 	// cout << endl;

// }

std::vector<string> setLogInterface(int argc, char **argv)
{
	std::vector<string> config_and_interface;
	config_and_interface.push_back("config/default.conf");
	config_and_interface.push_back("no_interface");

	std::vector<string> args_vector;
	for (int i = 0; i < argc; i++)
		args_vector.push_back(argv[i]);	

	if (argc == 1)
		return config_and_interface;
	if (argc == 2)
	{
		config_and_interface[0] = args_vector[1];
		return config_and_interface;
	}
	if (argc == 3)
	{
		if (args_vector[2] == "--if")
			config_and_interface[1] = "interface";
		else if (args_vector[2] == "--debug")
			Log::type_debug = true;
		else
			Log::Msg(ERROR, "Invalid argument: " + args_vector[2]);
		config_and_interface[0] = args_vector[1];
		return config_and_interface;
	}
	if (argc == 4)
	{
		if (args_vector[2] == "--if")
			config_and_interface[1] = "interface";
		else if (args_vector[2] == "--debug")
			Log::type_debug = true;
		else
			Log::Msg(ERROR, "Invalid argument: " + args_vector[2]);
		if (args_vector[3] == "--debug")
			Log::type_debug = true;
		else
			Log::Msg(ERROR, "Invalid argument: " + args_vector[3]);
		config_and_interface[0] = args_vector[1];
		return config_and_interface;
	}
	return config_and_interface;	

}
void	printStartInfo(std::vector<string> config_and_interface)
{
	std::system("clear"); //clears the terminal

	cout << GREEN << endl;
	cout << "    _____________________                    " << endl;
	cout << "   / _/ ___/_  __/ __/  /__ _____  _____ ____ " << endl;
	cout << "  / // (_ / / / _\\ \\ / / -_) __/ |/ / -_) __/ " << endl;
	cout << " / / \\___/ /_/ /___// /\\__/_/  |___/\\__/_/    " << endl;
	cout << "/__/              /__/                        " << endl;
	cout << RESET << endl;

		cout << GREEN << "[GTS]erver logging: CONSOLE && FILE logs ON" << RESET << endl;
	if (Log::type_debug)
		cout << GREEN << "[GTS]erver logging: RUNTIME && DEBUG logs ON" << RESET << endl;
	else
		cout << GREEN << "[GTS]erver logging: RUNTIME logs ON (no DEBUG)" << RESET << endl;
	
	if (config_and_interface[1] == "interface")
		cout << GREEN << "[GTS]erver interface: ON" << RESET << endl;
	else
		cout << GREEN << "[GTS]erver interface: OFF" << RESET << endl;

	if (config_and_interface[0] == "config/default.conf")
		cout << GREEN << "[GTS]erver: Config file not found, using default." << RESET << endl;
	else
		cout << GREEN << "[GTS]erver config file found: " << config_and_interface[0] << RESET << endl;
	
	cout << GREEN << "\n------ " << endl;
	cout << " LOGS: " << endl;
	cout << "------ " << RESET << endl;

	
}

int main(int argc, char **argv)
{
	std::vector<string> config_and_interface = setLogInterface(argc, argv);
	
	// INTERFACE ON
	if (config_and_interface[1] == "interface")
	{
		printStartInfo(config_and_interface);
		
		Parser parser(config_and_interface[0]);
		parser.parseFile(config_and_interface[0]);	

		std::vector<Server> servers = parser.getServers();
		Interface interface(parser, servers);
		interface.start();

		return 0;
	}
	
	printStartInfo(config_and_interface);

	Parser parser(config_and_interface[0]);
	parser.parseFile(config_and_interface[0]);

	ServerManager manager;
	manager.initServers(parser.getServers());
	Log::Msg(INFO, "Server(s) started");
	manager.runServers();

	return 0;
}