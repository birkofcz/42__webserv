/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/06 17:10:46 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"
#include "../include/Parser.hpp"
#include "../include/Server.hpp"
#include "../include/ServerManager.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		print("Usage: ./webserv <config_file>", RED, 2);
		exit(EXIT_FAILURE);
	}
	Parser	parser(argv[1]);
	parser.parseFile(argv[1]);
	parser.printServers();

	ServerManager	manager;
	manager.initServers(parser.getServers());
	manager.runServers();
	return 0;
}