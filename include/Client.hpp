/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:00:09 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/06 15:53:28 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

// #include "Webserv.hpp"
#include "HttpRequest.hpp"
//  #include "Response.hpp"
# include <netinet/in.h>

/**
	 Client
   - Stores all information related to the client such as socket and address
	 along with request and response objects. each Client also have an object of the server it's conntected to.
**/
class Client
{
	public:
		Client();
		Client(const Client& other);
		//Client(ServerConfig& );
		Client&	operator=(const Client& rhs);
		~Client();

		const int&					getSocket() const;
		const struct sockaddr_in&	getAddress() const;
		//const HttpRequest&			getRequest() const;
		//const time_t&				getLastTime() const;

		void	setSocket(int& sock);
		void	setAddress(sockaddr_in& Add);
		// void	setServer(ServerConfig &);
		//void	buildResponse();
		//void	updateTime();

		//void	clearClient();
		// Response response;
		HttpRequest request;
		// ServerConfig server;

	private:
		int		_client_socket;
		struct	sockaddr_in _client_address;
	//	time_t	_last_msg_time;
};

#endif