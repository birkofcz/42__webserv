/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 15:41:53 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/06 17:15:53 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/* Constructor with default values initialization */
Server::Server()
{
	_name = "";
	_port = -1;
	_server_names = std::vector<string>();
	_root = "";
	_index = std::vector<string>();
	_autoindex = false;
	_client_max_body_size = CLIENT_MAX_BODY_SIZE_LIMIT;
	_allowed_methods = std::vector<int>();
	_cgi = std::map<string, string>();
	_upload_path = "";
}

Server::~Server()
{
}

/* SETTERS --------------------------------------------------------------------------------------------------------*/

void Server::setName(string name)
{
	_name = name;
}

void Server::setPort(int port)

{
	_port = port;
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

void
Server::setErrorPages(std::map<int, string> error_pages)
{
	_error_pages = error_pages;
}

void
Server::setAutoindex(string autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else
		_autoindex = false;
}

void
Server::setClientMaxBodySize(size_t client_max_body_size)
{
	_client_max_body_size = client_max_body_size;
}

void
Server::setAllowedMethods(std::vector<int> allowed_methods)
{
	_allowed_methods = allowed_methods;
}

void 
Server::setCgi(std::map<string, string> cgi)
{
	_cgi = cgi;
}

void
Server::setUploadPath(string upload_path)
{
	_upload_path = upload_path;
}

void
Server::setHost(in_addr_t host)
{
	_host = host;
}


/* GETTERS --------------------------------------------------------------------------------------------------------*/

string
Server::getName() const
{
	return _name;
}

int
Server::getPort() const
{
	return _port;
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

std::map<int, string>
Server::getErrorPages() const {return _error_pages;}

bool
Server::getAutoindex() const
{
	return _autoindex;
}

size_t
Server::getClientMaxBodySize() const
{
	return _client_max_body_size;
}

std::vector<int>
Server::getAllowedMethods() const
{
	return _allowed_methods;
}

std::map<string, string>	Server::getCgi() const {return _cgi;}
string						Server::getUploadPath() const {return _upload_path;}

const std::vector<Location>::iterator	Server::getLocationKey(std::string key)
{
	std::vector<Location>::iterator it;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		if (it->getPath() == key)
			return (it);
	}
	return (it);
	//throw ErrorException("Error: path to location not found");
}

// method
void Server::setupServer(void)
{
	if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		Log::Msg(ERROR, FUNC + "socket: " + toString(strerror(errno)));
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
	
	memset(&_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = _host;
	//print the host in readable format..
	Log::Msg(DEBUG, FUNC + "Host: " + toString(inet_ntoa(_server_address.sin_addr)));
	_server_address.sin_port = htons(_port);
	Log::Msg(DEBUG, FUNC + "Port " + toString(_port));

	if (bind(_socket_fd, (struct sockaddr *)&_server_address, sizeof(_server_address)) == -1)
	{
		Log::Msg(ERROR, FUNC + "bind: " + toString(strerror(errno)));
		exit(EXIT_FAILURE);
	}
}
