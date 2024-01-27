/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 14:33:11 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/21 14:42:01 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

# include "general.hpp"

class HttpRequest;

class CGI
{
	private:
		std::map<std::string, std::string>	_environment;
		string 								_cgi_path;
		char**								_char_environment;
		char**								_arguments_for_execve;	
		int									_exit_code;
		pid_t								_cgi_pid;

	public:
		int									pipe_in[2];
		int									pipe_out[2];
	
		CGI();
		CGI(string cgi_path);
		~CGI();

		void								setEnvironment(HttpRequest &request);
		///to be continued...14.1.2024


};

#endif