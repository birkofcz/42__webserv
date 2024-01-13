/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:13:55 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/13 16:27:48 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_HPP
# define INTERFACE_HPP

# include "general.hpp"
# include "Parser.hpp"
# include "ServerManager.hpp"

class Interface
{
	private:
		Parser&					_parser;
		std::vector<Server>&	_servers;
		ServerManager			_manager;
		pthread_t				_managerThread;

		static void				signalHandler(int signum);
		void					shutdownServers();
		void					printServers();
		
		static void*			runServersWrapper(void* context);

		bool					_isRunning;

	public:
		Interface(Parser &parser, std::vector<Server> &servers);
		~Interface();

		void					start();
		void					runServers();
};

#endif