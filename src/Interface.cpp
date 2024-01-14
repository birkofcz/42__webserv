/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:19:58 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/14 14:03:27 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Interface.hpp"

Interface::Interface(Parser &parser, std::vector<Server> &servers)
: _parser(parser), _servers(servers)
{
	_isRunning = false;
	std::signal(SIGINT, Interface::signalHandler);
}

Interface::~Interface()
{
	if (_managerThread) 
	{
		pthread_join(_managerThread, NULL); // Ensure the server thread is joined upon destruction
	}
}

/// METHODS // ------------------------------------------------------------------
void
Interface::start()
{
	// Create a pthread to run the server manager

	string command;

	if (pthread_create(&_managerThread, NULL, Interface::runServersWrapper, this) != 0)
	{
		cerr << RED << "Error creating thread" << RESET << endl;
		Log::Msg(ERROR, "Error creating server thread, exiting...");
		exit(EXIT_FAILURE);
	}
	Log::Msg(INFO, "Server(s) started");
	_isRunning = true;

	while (true) 
	{
		if (_isRunning)
			std::cout << GREEN << "\n(Servers running) " << RESET << "Enter command (shutdown/config/parser_log) > ";
		else
			std::cout << RED << "\n(Servers stopped) " << RESET << "Enter command (run/shutdown/config/parser_log) > ";
		std::cin >> command;
		if (command == "shutdown") 
		{
			shutdownServers();
			break;
		} 
		else if (command == "config") 
		{
			printServers();
		}
		else if (command == "parser_log")
		{
			std::ifstream file("logs/parser_log.txt");
			string line;
			if (file.fail())
			{
				std::cerr << "Error opening file: " << "logs/parser_log.txt" << std::endl;
				Log::Msg(ERROR, "Error opening parser logfile: " + string("logs/parser_log.txt"));
				continue ;
			}
			while (std::getline(file, line))
				cout << line << std::endl;
		}
		else 
		{
			std::cout << "Unknown command." << std::endl;
		}
	}
}

/// PRIVATE METHODS // ----------------------------------------------------------

void*
Interface::runServersWrapper(void* context)
{
	Interface* interface = static_cast<Interface*>(context);
	interface->runServers();
	return NULL;
}

void
Interface::runServers()
{
	_manager.initServers(_servers);
	_manager.runServers();
}

	// Signal handler for gentle shutdown
void 
Interface::signalHandler(int signum)
{
		std::cout << "\nSignal (" << signum << ") received. Shutting down..." << std::endl;
		// Perform gentle shutdown tasks here
		exit(signum);
}

void
Interface::shutdownServers()
{
	Log::Msg(INFO, "Shutting down server(s)...");
	raise(SIGINT);
	// if (_managerThread) 
	// {
	// 	Log::Msg(INFO, "Server(s) stopped");
	// 	pthread_join(_managerThread, NULL); // Ensure the server thread is joined upon destruction
	// }
	
/* 	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		//shutdown server, maybe do it in server manager
	}	 */
}

	// Function to print server configurations
void
Interface::printServers()
{
	_parser.printServers();
}

