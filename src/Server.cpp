/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 15:41:53 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/03 15:42:12 by sbenes           ###   ########.fr       */
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
void 
Server::setName(string name)
{
	_name = name;
}

void
Server::setPorts(std::vector<int> ports)
{
	_ports = ports;
}

void
Server::setServerNames(std::vector<string> server_names)
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

/* GETTERS ------------------------------------------------*/\

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
