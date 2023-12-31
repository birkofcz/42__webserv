/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:04:21 by tkajanek          #+#    #+#             */
/*   Updated: 2024/01/01 15:49:50 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
	// _last_msg_time = time(NULL);
	std::cout << "CLIENT was constructed" << std::endl;
}

Client::~Client() {}

/* Copy constructor */
Client::Client(const Client &src)
{
	if (this != &src)
	{
		std::cout << "CLIENT was copied" << std::endl;
		this->_client_socket = src._client_socket;
		this->_client_address = src._client_address;
		this->request = src.request;
		this->response = src.response;
		// this->server = src.server;
		// this->_last_msg_time = src._last_msg_time;
	}
	return;
}

/* Assinment operator */
Client &Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		std::cout << "CLIENT was = assigned" << std::endl;
		this->_client_socket = rhs._client_socket;
		this->_client_address = rhs._client_address;
		this->request = rhs.request;
		this->response = rhs.response;
		// this->server = rhs.server;
		// this->_last_msg_time = rhs._last_msg_time;
	}
	return (*this);
}

Client::Client(Server& server)
{
	std::cout << "CLIENT was constructed with server parametr: " << server.getName() << std::endl;
	setServer(server);
	request.setMaxBodySize(server.getClientMaxBodySize());
	cout << "request object: Max body size setted during a Client construction using server parameter." << std::endl;
	// _last_msg_time = time(NULL);
}

void Client::setSocket(int& sock)
{
	_client_socket = sock;
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

// const HttpRequest&	Client::getRequest() const
// {
//     return (request);
// }

const struct sockaddr_in&	Client::getAddress() const
{
	return (_client_address);
}

// const time_t     &Client::getLastTime() const
// {
//     return (_last_msg_time);
// }

void	Client::clientBuildResponse()
{
	response.setRequest(this->request);
	response.buildResponse();
}

// void             Client::updateTime()
// {
//     _last_msg_time = time(NULL);
// }

void             Client::clearClient()
{
    response.clear();
    request.clear();
}