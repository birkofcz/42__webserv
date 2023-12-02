/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:04:21 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/02 20:20:15 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
    _last_msg_time = time(NULL);
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
		this->server = src.server;
		this->_last_msg_time = src._last_msg_time;

	}
	return ;
}

/* Assinment operator */
Client&	Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		this->_client_socket = rhs._client_socket;
		this->_client_address = rhs._client_address;
		this->request = rhs.request;
		this->response = rhs.response;
		this->server = rhs.server;
		this->_last_msg_time = rhs._last_msg_time;
	}
	return (*this);
}

Client::Client(ServerConfig& server)
{
    setServer(server);
    request.setMaxBodySize(server.getClientMaxBodySize());
    _last_msg_time = time(NULL);
}

void    Client::setServer(ServerConfig &server)
{
    response.setServer(server);
}