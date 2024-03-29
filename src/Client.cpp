/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabtan <gabtan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:04:21 by tkajanek          #+#    #+#             */
/*   Updated: 2024/02/09 15:34:17 by gabtan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client() {}

Client::~Client() {}

/* Copy constructor */
Client::Client(const Client &src)
{
	if (this != &src)
	{
		this->_client_socket = src._client_socket;
		this->_client_address = src._client_address;
		this->request = src.request;
		this->response = src.response;

	}
	return;
}

/* Assinment operator */
Client &Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		this->_client_socket = rhs._client_socket;
		this->_client_address = rhs._client_address;
		this->request = rhs.request;
		this->response = rhs.response;
	}
	return (*this);
}

Client::Client(Server& server)
{
	setServer(server);
	request.setMaxBodySize(server.getClientMaxBodySize());
}

void Client::setSocket(int& sock)
{
	_client_socket = sock;
	request.client_socket = sock;
}

void Client::setAddress(sockaddr_in& addr)
{
	_client_address = addr;
}

void Client::setServer(Server& server)
{
	response.setServer(server);
}

const int &Client::getSocket() const
{
	return (_client_socket);
}

const struct sockaddr_in&	Client::getAddress() const
{
	return (_client_address);
}


void	Client::clientBuildResponse()
{
	response.setRequest(this->request);
	Log::Msg(INFO, request.getMethodStr() + " recieved");
	response.buildResponse();
}

void             Client::clearClient()
{
    response.clear();
    request.clear();
}
