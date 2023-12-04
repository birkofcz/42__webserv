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

	public:
		Server();
		~Server();

		//setters
		void	setName(string name);
		void	setPorts(std::vector<int> ports);
		void	setServerNames(std::vector<string> server_names);
		void	setRoot(string root);
		void	setIndex(std::vector<string> index);

		//getters
		string				getName() const;
		std::vector<int>	getPorts() const;
		std::vector<string>	getServerNames() const;
		string				getRoot() const;
		std::vector<string>	getIndex() const;
};

#endif