/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 14:33:11 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/07 17:38:42 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

# include "general.hpp"
# include "Location.hpp"
# include "HttpRequest.hpp"
# include <string.h>

class HttpRequest;

class Cgi
{
	private:
		std::map<std::string, std::string>	_environment;
		string 								_cgi_path;
		string 								_cgi_extension;
		char**								_char_environment;
		char**								_arguments_for_execve;	
		int									_exit_code;
		pid_t								_cgi_pid;
		int									_client_fd;

	public:
		Cgi();
		Cgi(string cgi_path);
		~Cgi();
		Cgi(const Cgi &other);
		Cgi& operator=(Cgi const &rhs);

		void	setEnvironment(HttpRequest& request);
		void	setExtension(const string& ext);
		void	setCgiPath(const string& cgi_path);
		void	setCgiClientFd(int client_fd);

		int		execute(short &error_code);
		void	initEnv(HttpRequest& req, const std::vector<Location>::iterator it_loc);
		void	clear();
		// std::string setCookie(const std::string& str);

		// const std::map<std::string, std::string> &getEnv() const;
		const pid_t &getCgiPid() const;
		// const std::string &getCgiPath() const;
		std::string getPathInfo() const;
		// std::string decode(std::string &path);

		int									cgi_pipe_out_read_end;
		int									cgi_pipe_in_write_end;
};

#endif