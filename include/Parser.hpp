/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:29:08 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/08 16:33:17 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "general.hpp"
# include "Server.hpp"
# include "Location.hpp"


/* Parser class to parse the config file and create a vector of Server objects,
including their location blocks and their configuration parsed */

class Server;

class Parser
{
	private:
		string						_path;
		std::vector<Server>			_servers;		//vector of servers as per config file

	public:
		Parser(string path);
		~Parser();

		//parse file - first level
		void						parseFile(const string& _path);
		
		//parser helper methods - second level
		string						parseLocationPath(const string& line);
		int							parsePort(const string& line);
		in_addr_t					parseHost(const string& line);
		std::vector<string>			parseServerNames(const string& line);
		string						parseRoot(const string& line);
		std::vector<string>			parseIndex(const string& line);
		std::vector<int>			parseAllowedMethods(const string& line);
		std::map<int, string>		parseErrorPages(const string& line);
		size_t						parseClientMaxBodySize(const string& line);
		std::map<string, string>	parseCgi(const string& line);

		//getters
		std::vector<Server>			getServers() const;

		//printers
		void						printServers();

};

#endif