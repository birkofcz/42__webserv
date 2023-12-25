/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:04:21 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/25 19:22:24 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
	// _last_msg_time = time(NULL);
}

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
		// this->server = src.server;
		// this->_last_msg_time = src._last_msg_time;
	}
	return;
}

/* Assinment operator */
Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		this->_client_socket = rhs._client_socket;
		this->_client_address = rhs._client_address;
		this->request = rhs.request;
		this->response = rhs.response;
		// this->server = rhs.server;
		// this->_last_msg_time = rhs._last_msg_time;
	}
	return (*this);
}

Client::Client(Server &server)
{
	setServer(server);
	// // request.setMaxBodySize(server.getClientMaxBodySize());
	// _last_msg_time = time(NULL);
}

void Client::setSocket(int &sock)
{
	_client_socket = sock;
}

void Client::setAddress(sockaddr_in &addr)
{
	_client_address = addr;
}

void Client::setServer(Server &server)
{
	response.setServer(server);
}

const int &Client::getSocket() const
{
	return (_client_socket);
}

// const HttpRequest   &Client::getRequest() const
// {
//     return (request);
// }

const struct sockaddr_in &Client::getAddress() const
{
	return (_client_address);
}

// const time_t     &Client::getLastTime() const
// {
//     return (_last_msg_time);
// }

// // void	Client::clientBuildResponse()
// // {
// // 	response.setRequest(this->request);
// // 	response.buildResponse();
// // }

// void             Client::updateTime()
// {
//     _last_msg_time = time(NULL);
// }

// void             Client::clearClient()
// {
//     response.clear();
//     request.clear();
// }