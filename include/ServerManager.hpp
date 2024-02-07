/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:09:12 by tkajanek          #+#    #+#             */
/*   Updated: 2024/02/06 16:26:04 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <map>
# include <vector>
#include <sys/wait.h>
# include "Server.hpp"
# include "Client.hpp"

using std::map;
using std::vector;

class ServerManager
{
	public:	
		ServerManager();
		~ServerManager();

		void	initServers(std::vector<Server>);
		void	runServers();
		int		getEpollFd();
		void	clear();
			
	private:
		vector<Server>		_servers;
		map<int, Server>	_servers_map; // every server fd connected with Server
		map<int, Client>	_clients_map;
		map<int, int> 		_cgi_pipe_to_client_map;
		int 				_epoll_fd;

		void	_acceptNewConnection(Server &);
		void	_readRequest(const int&, Client &);
		void	_readCgiResponse(Client &c);
		void	_sendResponse(const int&, Client&);
		void	_sendCgiBody(Client&);
		void	_addToEpoll(int fd, int events_flag);
		void	_closeConnection(const int);
		size_t	_calcContLenCgi(const char*, size_t);
};

#endif
