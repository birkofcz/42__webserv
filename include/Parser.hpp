/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:29:08 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/03 16:23:14 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "../include/general.hpp"
# include "../include/Server.hpp"

class Server;

class Parser
{
	private:
		string _path;
		std::vector<Server> _servers;
		
	public:
		Parser(string path);
		~Parser();

		//parse file - first level
		void				parseFile(const string& _path);
		
		//parser helper methods - second level
		std::vector<int>	parsePorts(const string& line);
		std::vector<string>	parseServerNames(const string& line);
		string				parseRoot(const string& line);
		std::vector<string>	parseIndex(const string& line);
		
		//getters
		std::vector<Server> getServers() const;

		//printers
		void				printServers();

};

#endif