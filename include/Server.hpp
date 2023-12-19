/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 15:58:39 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/19 16:29:16 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* 
 --- Directives from the config file to handle: ---

server_name (server, location)
listen (server)
root (server, location)
index (server, location)
error_page (server, location)
location (server)
autoindex (server, location)
client_max_body_size (server, location)
cgi (server, location)
upload_path (server, location)
allowed_methods (server, location)

cgi directive example:
cgi .php=/usr/bin/php-cgi (file extension, path to executable)

eror_page directive example:
error_page 404 /404.html (error code, path to error page)

location directive example:
location /(path) { 
	allowed_methods GET POST (methods)
	root /var/www/html (root)
	index index.html (index)
	autoindex on (autoindex)
	cgi .php=/usr/bin/php-cgi (cgi)
	upload_path /var/www/html/uploads (upload)
}

 */


#ifndef SERVER_HPP
# define SERVER_HPP

# include "general.hpp"
# include "Location.hpp"
# include <cstring>

class Location;

class Server
{
	private:
		string				_name;
		std::vector<int>	_ports;
		std::vector<string>	_server_names;
		string				_root;
		std::vector<string> _index;

		std::map<int, string>	_error_page; //to be done 19/12/202
		bool				_autoindex; //to be done 19/12/2023
		int					_client_max_body_size; //to be done 19/12/2023
		std::map<string, string>	cgi; //to be done 19/12/2023
		string				_upload_path; //to be done 19/12/2023
		std::vector<int>	_allowed_methods; //to be done 19/12/2023

		int					_socket_fd;
		struct sockaddr_in 	_server_address;
		in_addr_t			_host; //needs to extract from config

		std::vector<Location>	_locations;	//vector of locations as per config file for a particular server
		
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
		void	addLocation(Location location);
		void	setErrorPage(std::map<int, string> error_page); //to be done 19/12/2023
		void	setAutoindex(bool autoindex); //to be done 19/12/2023
		void	setClientMaxBodySize(int client_max_body_size); //to be done 19/12/2023
		void	setCgi(std::map<string, string> cgi); //to be done 19/12/2023
		void	setUploadPath(string upload_path);	//to be done 19/12/2023
		void	setAllowedMethods(std::vector<int> allowed_methods); //to be done 19/12/2023
		void	setHost(in_addr_t host); //needs to extract from config
		
		//getters
		string				getName() const;
		std::vector<int>	getPorts() const;
		std::vector<string>	getServerNames() const;
		string				getRoot() const;
		std::vector<string>	getIndex() const;
		const in_addr_t&	getHost() const;
		int					getFd() const;
		std::map<int, string>	getErrorPage() const; //to be done 19/12/2023
		bool				getAutoindex() const; //to be done 19/12/2023
		int					getClientMaxBodySize() const; //to be done 19/12/2023
		std::map<string, string>	getCgi() const; //to be done 19/12/2023
		string				getUploadPath() const; //to be done 19/12/2023
		std::vector<int>	getAllowedMethods() const; //to be done 19/12/2023
		

		std::vector<Location>	getLocations() const;
		
    friend std::ostream& operator<<(std::ostream& os, const Server& server);
};

inline std::ostream& operator<<(std::ostream& os, const Server& server) {
    os << "\nServer Information:\n"
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
	os << "Locations: \n";
	std::vector<Location> locations = server.getLocations();
	for (size_t i = 0; i < locations.size(); ++i) {
		//print location
		os << "Location " << i << ":\n";
		os << "Path: " << locations[i].getPath() << "\n";
		os << "Allowed Methods: ";
		std::vector<int> allowedMethods = locations[i].getAllowedMethods();
		for (size_t j = 0; j < allowedMethods.size(); ++j) {
			os << allowedMethods[j] << " ";
		}
		os << "\nRoot: " << locations[i].getRoot() << "\n";
		os << "Index: ";
		std::vector<std::string> index = locations[i].getIndex();
		for (size_t j = 0; j < index.size(); ++j) {
			os << index[j] << " ";
		}
		os << "\nCgi: " << locations[i].getCgi() << "\n";
		os << "Autoindex: " << locations[i].getAutoindex() << "\n";
		
	}
	return os;
}

#endif