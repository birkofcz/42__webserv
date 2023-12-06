/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:09:12 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/06 16:55:17 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <map>
# include <vector>
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
			
	private:
		vector<Server>		_servers;
		map<int, Server>	_servers_map; // every server fd connected with Server
		map<int, Client>	_clients_map;
		int 				_epoll_fd;

		// fd_set     _recv_fd_pool;
		// fd_set     _write_fd_pool;
		// int        _biggest_fd;

		void acceptNewConnection(Server &);
		//void checkTimeout();
		//void initializeSets();
		void readRequest(const int &, Client &);
		//void handleReqBody(Client &);
		//void sendResponse(const int &, Client &);
		//void sendCgiBody(Client &, CgiHandler &);
		//void readCgiResponse(Client &, CgiHandler &);
		//void closeConnection(const int);
		//void assignServer(Client &);
		//void addToSet(const int , fd_set &);
		//void removeFromSet(const int , fd_set &);
};

#endif
