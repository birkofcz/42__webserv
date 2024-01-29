/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 14:33:11 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/28 16:40:28 by tkajanek         ###   ########.fr       */
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
		// int									pipe_in[2];
		// int									pipe_out[2];
		int									cgi_pipe_out_read_end;
		int									cgi_pipe_in_write_end;
	
		Cgi();
		Cgi(string cgi_path);
		~Cgi();
		Cgi(const Cgi &other);
		Cgi& operator=(Cgi const &rhs);

		void	setEnvironment(HttpRequest& request);
		void	setExtension(const string& ext);
		void	setCgiPath(const string& cgi_path);
		
		// void	setCgiStdin(int stdinWriteEnd);
		void	setCgiClientFd(int client_fd);

		void	execute(short &error_code);
		// int		getPipeOutReadEnd() const; //pipe out is public so mazbe not necessary
		///to be continued...14.1.2024


		void initEnv(HttpRequest& req, const std::vector<Location>::iterator it_loc);
		// void initEnvCgi(HttpRequest& req, const std::vector<Location>::iterator it_loc);
		// 
		// void sendHeaderBody(int &pipe_out, int &fd, std::string &);
		// void fixHeader(std::string &header);
		void clear();
		// std::string setCookie(const std::string& str);

		// void setCgiPid(pid_t cgi_pid);

		// const std::map<std::string, std::string> &getEnv() const;
		const pid_t &getCgiPid() const;
		// const std::string &getCgiPath() const;

		// std::string	getAfter(const std::string& path, char delim);
		// std::string	getBefore(const std::string& path, char delim);
		// std::string	getPathInfo(std::string& path, std::vector<std::string> extensions);
		std::string getPathInfo() const;
		// int	countCookies(const std::string& str);
		// int findStart(const std::string path, const std::string delim);
		// std::string decode(std::string &path);
};

#endif