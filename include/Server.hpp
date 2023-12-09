/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 15:58:39 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/03 15:46:51 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "general.hpp"
# include <cstring>

class Server
{
	private:
		string				_name;
		std::vector<int>	_ports;
		std::vector<string>	_server_names;
		string				_root;
		std::vector<string> _index;

		// string				_error_page;
		// string				_location;
		// bool	autoindex;
		// int		client_max_body_size;
		// string	cgi;
		// string	upload;
		// string	auth;
		// string	methods;
		// string	redirect;
		// string	return_;
		int					_socket_fd;
		struct sockaddr_in 	_server_address;
		in_addr_t			_host; //needs to extract from config

	public:
		Server();
		~Server();


		//methods
		void	setupServer(void);
  
		//setters
		void	setName(string name);
		void	setPorts(std::vector<int> ports);
		void	setServerNames(std::vector<string> server_names);
		void	setRoot(string root);
		void	setIndex(std::vector<string> index);
		void	setFd(int fd);
		
		//getters
		string				getName() const;
		std::vector<int>	getPorts() const;
		std::vector<string>	getServerNames() const;
		string				getRoot() const;
		std::vector<string>	getIndex() const;
		const in_addr_t&	getHost() const;
		int					getFd() const;
		
    friend std::ostream& operator<<(std::ostream& os, const Server& server);
};

inline std::ostream& operator<<(std::ostream& os, const Server& server) {
    os << "Server Information:\n"
       << "Name: " << server.getName() << "\n"
       << "Ports: ";
    std::vector<int> ports = server.getPorts();
    for (size_t i = 0; i < ports.size(); ++i) {
        os << ports[i] << " ";
    }
    os << "\nServer Names: ";
    std::vector<std::string> serverNames = server.getServerNames();
    for (size_t i = 0; i < serverNames.size(); ++i) {
        os << serverNames[i] << " ";
    }
    os << "\nRoot: " << server.getRoot() << "\nIndex: ";
    std::vector<std::string> index = server.getIndex();
    for (size_t i = 0; i < index.size(); ++i) {
        os << index[i] << " ";
    }
    os << "\nHost: " << server.getHost() << "\nSocket FD: " << server.getFd() << "\n";

    // You can add more variables as needed

    return os;
}
#endif