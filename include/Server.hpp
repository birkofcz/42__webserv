/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:12:27 by sbenes            #+#    #+#             */
/*   Updated: 2023/11/17 12:59:51 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "general.hpp"

class Server
{
	private:
		int					server_fd;
		struct sockaddr_in	address;
		int 				port;
		
	public:
		Server(int port);
		~Server();

		void initialize();
		void acceptConnection();
		void handleRequest(int client_socket);
		void sendResponse(int client_socket);
};


#endif