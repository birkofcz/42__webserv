/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 15:41:53 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/16 14:24:25 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

/* SETTERS ------------------------------------------------*/

void Server::setName(string name)
{
	_name = name;
}

void Server::setPorts(std::vector<int> ports)

{
	_ports = ports;
}


void Server::setServerNames(std::vector<string> server_names)
{
	_server_names = server_names;
}


void
Server::setRoot(string root)
{
	_root = root;
}

void
Server::setIndex(std::vector<string> index)
{
	_index = index;
}

void	Server::setFd(int fd)
{
	_socket_fd = fd;
}

void	
Server::addLocation(Location location)
{
	_locations.push_back(location);
}

/* GETTERS ------------------------------------------------*/

string
Server::getName() const
{
	return _name;
}

std::vector<int>
Server::getPorts() const
{
	return _ports;
}

std::vector<string>
Server::getServerNames() const
{
	return _server_names;
}

string
Server::getRoot() const
{
	return _root;
}

std::vector<string>
Server::getIndex() const
{
	return _index;
}


const in_addr_t&	Server::getHost() const 
{
	return (_host);
}

int	Server::getFd() const 
{ 
	return _socket_fd; 
}

std::vector<Location>
Server::getLocations() const
{
	return _locations;
}

// method
void Server::setupServer(void)
{
	if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		print("webserv: socket error, Closing ....", RED, 2);
		exit(EXIT_FAILURE);
	}

	// Set socket options to allow reusing the address
	int option_value = 1;
	setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
	// The SO_REUSEADDR option allows the reuse of a local address
	// (IP address and port number) that is still in the TIME_WAIT state.
	// When a socket is closed, it goes into the TIME_WAIT state for a
	// certain period to ensure that any lingering packets in the network
	// are handled properly. Enabling SO_REUSEADDR allows a new socket
	// to bind to the same address immediately after the old socket
	// is closed, even if it's still in TIME_WAIT.


	_host = INADDR_ANY; // replace with proper initialization bsd on config
	
	memset(&_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = _host;
	_server_address.sin_port = htons(_ports[0]); //!!

	//cout << htons(_ports[0]) << " " << _ports[0] << " "  << _host << " " << _socket_fd << endl; // test

	if (bind(_socket_fd, (struct sockaddr *)&_server_address, sizeof(_server_address)) == -1)
	{
		print("webserv: bind error, Closing ....", RED, 2);
		exit(EXIT_FAILURE);
	}
}
