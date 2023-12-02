/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/02 16:05:28 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		print("Usage: ./webserv [configuration file]\n", RED);
		exit(EXIT_FAILURE);
	}

	const char*	configFilePath = argv[1];
	(void) configFilePath;
	manager.setupServers(cluster.getServers());
	manager.runServers();





	string	message = "This is where the webserv will hopefully be in a few weeks :) !";
	cout << endl;
	print(message, GREEN);
	print(message, RED, 2);
	print(message);
	print(message, NONE, 2);
	cout << endl;
	
	return 0;
}