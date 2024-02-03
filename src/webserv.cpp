/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/01 16:31:02 by sbenes           ###   ########.fr       */
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
setLogInterface function to set the flags for the log output = options for output to console and/or file
Also sets the flags for the log type = options for output of info, warning, error, debug
Also returns the config file and interface flags

Current version only supports the following flags:
--if - interface mode (runs the server in the background thread and the user can interact with it via the interface)
--debug - debug mode (prints the debug logs)

In the default case, the server is run in the main thread and the user can only interact with it via the UNIX signals.
Logging is set to console and file, debug logs are not printed, only runtime logs are printed (Info, Warning, Error)
If the config file is not found, the default config file is used.
*/
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

/* 
printStartInfo function to print the header graphics and start info of the server, based on the flags set
 */
void	printStartInfo(std::vector<string> config_and_interface)
{
	//std::system("clear"); //clears the terminal

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

/* 
main function to start the server

Runs the server in two modes:
1. Interface mode - if the --if flag is set (optional). In interface mode, the server is run in the background thread
and the user can interact with it via the interface. The server is started and stopped by the user via commands, there is 
also a command to print the servers configuration or the log file from the parser.

2. Server mode - if the --if flag is not set (default). In server mode, the server is run in the main thread and
the user can only interact with it via the UNIX signals. The server is started and stopped by sending a signal to it.

 */
int main(int argc, char **argv)
{
	std::vector<string> config_and_interface = setLogInterface(argc, argv);
	
	// INTERFACE ON CASE
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

//pri ukonceni nutno close fd_epoll a fd of servers.
//Asi iterovat skrze server map...