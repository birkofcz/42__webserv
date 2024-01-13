/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:19:58 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/13 14:46:51 by sbenes           ###   ########.fr       */
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

    while (true) 
	{
		if (_isRunning)
    		std::cout << GREEN << "(Servers running) " << RESET << "Enter command (shutdown/config) > ";
		else
			std::cout << RED << "(Servers stopped) " << RESET << "Enter command (run/shutdown/config) > ";
        std::cin >> command;
		if (command == "run")
		{
			if(pthread_create(&_managerThread, NULL, Interface::runServersWrapper, this) != 0)
			{
				cerr << RED << "Error creating thread" << RESET << endl;
				exit(EXIT_FAILURE);
			}
			cout << GREEN << "Server(s) started" << RESET << endl;
			_isRunning = true;
			//some control print of what has been started??
		}
        else if (command == "shutdown") 
		{
            shutdownServers();
            break;
        } 
		else if (command == "config") 
		{
	        printServers();
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
    cout << "Shutting down servers." << endl;
	raise(SIGINT);
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

