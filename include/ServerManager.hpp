/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:09:12 by tkajanek          #+#    #+#             */
/*   Updated: 2024/01/25 15:19:46 by tkajanek         ###   ########.fr       */
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
		map<int, int> 		_cgi_pipe_to_client_map;
		int 				_epoll_fd;

		// fd_set     _recv_fd_pool; spojene se select() -> vymazat
		// fd_set     _write_fd_pool; spojene se select() -> vymazat
		// int        _biggest_fd; spojene se select() -> vymazat

		void acceptNewConnection(Server &);
		//void checkTimeout();
		//void initializeSets(); spojene se select() -> vymazat
		void readRequest(const int&, Client &);
		void _readCgiResponse(Client &c);
		//void handleReqBody(Client &);
		void sendResponse(const int&, Client&);
		//void sendCgiBody(Client &, CgiHandler &);
		//void readCgiResponse(Client &, CgiHandler &);
		void _closeConnection(const int);
		//void assignServer(Client &);
		//void addToSet(const int , fd_set &); spojene se select() -> vymazat
		//void removeFromSet(const int , fd_set &); spojene se select() -> vymazat
};

#endif
