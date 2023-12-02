/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:09:12 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/02 19:57:28 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class ServerManager
{
	public:                 
		ServerManager();
		~ServerManager();

		void	initServers(std::vector<ServerConfig>);
		void	runServers();
			
	private:
		std::vector<ServerConfig> _servers;
		std::map<int, ServerConfig> _servers_map; // every server fd connected with ServerConfig
		std::map<int, Client> _clients_map;
		// fd_set     _recv_fd_pool;
		// fd_set     _write_fd_pool;
		// int        _biggest_fd;

		void acceptNewConnection(ServerConfig &);
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
