/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 15:58:39 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/03 18:42:24 by tkajanek         ###   ########.fr       */
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
cgi .php /usr/bin/php-cgi (file extension, path to executable, can be multiple)

eror_page directive example:
error_page 404 /404.html (error code, path to error page, can be multiple)

location directive example:
location /(path) { 
	allowed_methods GET POST (methods)
	root /var/www/html (root)
	index index.html (index)
	autoindex on (autoindex)
	cgi .php /usr/bin/php-cgi (cgi) - can be multiple
	upload_path /var/www/html/uploads (upload)
}

 */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "general.hpp"
# include "Location.hpp"
# include <cstring>

/* Server class to include all the directives from config file and a vector of "location"
blocks - of Location class objects, including their configuration parsed */

class Location;

class Server
{
	private:
		string						_name;
		int							_port;
		in_addr_t					_host; 
		std::vector<string>			_server_names;
		string						_root;
		std::vector<string>			_index;
		std::map<int, string>		_error_pages; 
		bool						_autoindex; 
		size_t						_client_max_body_size;
		std::map<string, string>	_cgi; 
		string						_upload_path;
		std::vector<int>			_allowed_methods;

		int							_socket_fd;
		struct sockaddr_in			_server_address;

		std::vector<Location>		_locations;	//vector of locations as per config file for a particular server
		
	public:
		Server();
		~Server();


		//methods
		void						setupServer(void);
  
		//setters
		void						setName(string name);
		void						setPort(int port);
		void						setServerNames(std::vector<string> server_names);
		void						setRoot(string root);
		void						setIndex(std::vector<string> index);
		void						setFd(int fd);
		void						addLocation(Location location);
		void						setErrorPages(std::map<int, string> error_page);
		void						setAutoindex(string autoindex);
		void						setClientMaxBodySize(size_t client_max_body_size);
		void						setCgi(std::map<string, string> cgi); 
		void						setUploadPath(string upload_path);
		void						setAllowedMethods(std::vector<int> allowed_methods);
		void						setHost(in_addr_t host); //needs to extract from config
		
		//getters
		string						getName() const;
		int							getPort() const;
		std::vector<string>			getServerNames() const;
		string						getRoot() const;
		std::vector<string>			getIndex() const;
		const in_addr_t&			getHost() const;
		int							getFd() const;
		std::map<int, string>		getErrorPages() const; 
		bool						getAutoindex() const; 
		size_t						getClientMaxBodySize() const; 
		std::map<string, string>	getCgi() const; 
		string						getUploadPath() const; 
		std::vector<int>			getAllowedMethods() const;

		std::vector<Location>		getLocations() const;
		const std::vector<Location>::iterator	getLocationKey(std::string);

		
    friend std::ostream& operator<<(std::ostream& os, const Server& server); ///is this allowed? TEMP for debugging
};

inline std::ostream& operator<<(std::ostream& os, const Server& server) {
    os << "\nServer Information:\n"
       << "Name: " << server.getName() << "\n"
       << "Ports: ";
   int port = server.getPort();
   os << port << " ";

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
		//os << "\nCgi: " << locations[i].getCgi() << "\n";
		os << "Autoindex: " << locations[i].getAutoindex() << "\n";
		
	}
	return os;
}

#endif