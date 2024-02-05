/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 16:03:12 by tkajanek          #+#    #+#             */
/*   Updated: 2024/02/05 16:49:31 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

/* Constructor */
Cgi::Cgi()
{
	this->_cgi_pid = -1;
	this->cgi_pipe_out_read_end = -1;
	this->cgi_pipe_in_write_end = -1;
	this->_client_fd = -1;
	this->_exit_code = 0;
	this->_cgi_path = "";
	this->_char_environment = NULL;
	this->_arguments_for_execve = NULL;
}

Cgi::Cgi(std::string path)
{
	this->_cgi_pid = -1;
	this->cgi_pipe_out_read_end = -1;
	this->cgi_pipe_in_write_end = -1;
	this->_client_fd = -1;
	this->_exit_code = 0;
	this->_cgi_path = path;
	this->_char_environment = NULL;
	this->_arguments_for_execve = NULL;
}

Cgi::~Cgi() {

	if (this->_char_environment)
	{
		for (int i = 0; this->_char_environment[i]; i++)
			free(this->_char_environment[i]);
		free(this->_char_environment);
	}
	if (this->_arguments_for_execve)
	{
		for (int i = 0; this->_arguments_for_execve[i]; i++)
			free(_arguments_for_execve[i]);
		free(_arguments_for_execve);
	}
	this->_environment.clear();
}

Cgi::Cgi(const Cgi& other)
{
		this->_environment = other._environment;
		this->_char_environment = other._char_environment;
		this->_arguments_for_execve = other._arguments_for_execve;
		this->_cgi_path = other._cgi_path;
		this->_cgi_pid = other._cgi_pid;
		this->_exit_code = other._exit_code;
		this->cgi_pipe_out_read_end = other.cgi_pipe_out_read_end;
		this->cgi_pipe_in_write_end = other.cgi_pipe_in_write_end;
		this->_client_fd = other._client_fd; 
}

Cgi& Cgi::operator=(const Cgi &rhs)
{
    if (this != &rhs)
	{
		this->_environment = rhs._environment;
		this->_char_environment = rhs._char_environment;
		this->_arguments_for_execve = rhs._arguments_for_execve;
		this->_cgi_path = rhs._cgi_path;
		this->_cgi_pid = rhs._cgi_pid;
		this->_exit_code = rhs._exit_code;
		this->cgi_pipe_out_read_end = rhs.cgi_pipe_out_read_end;
		this->_client_fd = rhs._client_fd;
	}
	return (*this);
}

// /*Set functions */
// void Cgi::setCgiPid(pid_t cgi_pid)
// {
//     this->_cgi_pid = cgi_pid;
// }

void	Cgi::setCgiPath(const string& cgi_path) {this->_cgi_path = cgi_path;}
void	Cgi::setExtension(const string& ext) {this->_cgi_extension = ext;}

// /* Get functions */
// const std::map<std::string, std::string> &Cgi::getEnv() const
// {
//     return (this->_environment);
// }

const pid_t &Cgi::getCgiPid() const
{
	return (this->_cgi_pid);
}

// const std::string &Cgi::getCgiPath() const
// {
//     return (this->_cgi_path);
// }

// static int findStart(const std::string path, const std::string delim)
// {
// 	if (path.empty())
// 		return (-1);
// 	size_t poz = path.find(delim);
// 	if (poz != std::string::npos)
// 		return (poz);
// 	else
// 		return (-1);
// }


// /* initialization environment variable */
void Cgi::initEnv(HttpRequest& req, const std::vector<Location>::iterator it_loc)
{
	size_t			poz;
	std::string executable_path = it_loc->getCgiPath(_cgi_extension);
	Log::Msg(DEBUG, FUNC + "_cgi_path: " + _cgi_path);
	Log::Msg(DEBUG, FUNC + "executable_path: " + executable_path);
	if (executable_path.empty())
		return; //??
	// extension = this->_cgi_path.substr(this->_cgi_path.find("."));
	// std::map<std::string, std::string>::iterator it_path = it_loc->_executable_path.find(extension);
    // if (it_path == it_loc->_executable_path.end())
    //     return ;
    
	this->_environment["AUTH_TYPE"] = "Basic"; //podle requestu ..
	this->_environment["CONTENT_LENGTH"] = req.getBodyLen();
	this->_environment["CONTENT_TYPE"] = req.getHeader("content-type");
    this->_environment["GATEWAY_INTERFACE"] = "CGI/1.1";

	poz = (this->_cgi_path.empty()) ? std::string::npos : this->_cgi_path.find("cgi-bin/");
	/*
	_environment["SCRIPT_NAME"] = "/cgi-bin/my_script.cgi";
	_environment["SCRIPT_FILENAME"] = "my_script.cgi";
	*/
	this->_environment["SCRIPT_NAME"] = this->_cgi_path;
    this->_environment["SCRIPT_FILENAME"] = this->_cgi_path.substr(poz + 8, this->_cgi_path.size());
    this->_environment["PATH_INFO"] = getPathInfo();
    this->_environment["PATH_TRANSLATED"] = it_loc->getRoot() + (this->_environment["PATH_INFO"] == "" ? "/" : this->_environment["PATH_INFO"]);
    // this->_environment["QUERY_STRING"] = decode(req.getQuery());
    this->_environment["REMOTE_ADDR"] = req.getHeader("host");
    this->_environment["SERVER_NAME"] = req.getServerName();
    this->_environment["SERVER_PORT"] = req.getServerPort();
    this->_environment["REQUEST_METHOD"] = req.getMethodStr();
    // this->_environment["HTTP_COOKIE"] = req.getHeader("cookie");
    this->_environment["DOCUMENT_ROOT"] = it_loc->getRoot();
	this->_environment["REQUEST_URI"] = req.getPath() + req.getQuery();
    this->_environment["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_environment["REDIRECT_STATUS"] = "200";
	this->_environment["SERVER_SOFTWARE"] = "[GTS]erver";

	this->_char_environment = (char **)calloc(sizeof(char *), this->_environment.size() + 1);
	std::map<std::string, std::string>::const_iterator it = this->_environment.begin();
	for (int i = 0; it != this->_environment.end(); it++, i++)
	{
		std::string tmp = it->first + "=" + it->second;
		Log::Msg(DEBUG, FUNC + "during allocating loop ENV: " + tmp);
		this->_char_environment[i] = strdup(tmp.c_str());
	}
	this->_arguments_for_execve = (char **)malloc(sizeof(char *) * 3);
	this->_arguments_for_execve[0] = strdup(executable_path.c_str());
	this->_arguments_for_execve[1] = strdup(this->_cgi_path.c_str());
	this->_arguments_for_execve[2] = NULL;
}


static void handle_timeout(int signal)
{
	(void) signal;
	exit(1);
   	// kill(getpid(), SIGTERM);
}

/* Pipe and execute CGI */
int Cgi::execute(short& error_code)
{
	int cgi_stdout[2];
	int cgi_stdin[2];
	if (this->_arguments_for_execve[0] == NULL || this->_arguments_for_execve[1] == NULL)
	{
		Log::Msg(ERROR, "No executable or arguments for CGI script");
		error_code = 500;
		return 1;
	}

    if (pipe(cgi_stdin) == -1)
    {
		Log::Msg(ERROR, "Failed pipe: " + toString(strerror(errno)));
        error_code = 500;
        return 1;
    }
	
	if (pipe(cgi_stdout) < 0)
	{
       	Log::Msg(ERROR, "Failed pipe: " + toString(strerror(errno)));
		close(cgi_stdin[0]);
		close(cgi_stdin[1]);
		error_code = 500;
		return 1;
	}

	this->_cgi_pid = fork();
	if (this->_cgi_pid == 0)
	{
		// Set up a timeout of 5 seconds
		signal(SIGALRM, handle_timeout);
		alarm(5);

		dup2(cgi_stdin[0], STDIN_FILENO);
		dup2(cgi_stdout[1], STDOUT_FILENO);
		close(cgi_stdin[0]);
		close(cgi_stdin[1]);
		close(cgi_stdout[0]);
		close(cgi_stdout[1]);

		this->_exit_code = execve(this->_arguments_for_execve[0], this->_arguments_for_execve, this->_char_environment);
		exit(this->_exit_code);
	}
	else if (this->_cgi_pid > 0)
	{
		cgi_pipe_in_write_end = cgi_stdin[1];	// write end for pipe to input a data to script	
		close (cgi_stdin[0]);
		cgi_pipe_out_read_end = cgi_stdout[0]; // the output of script to be read
		close(cgi_stdout[1]);
		return 0;
	}
	else
	{
		close (cgi_stdin[0]);
		close(cgi_stdout[1]);
        Log::Msg(ERROR, "Fork failed: " + toString(strerror(errno)));
		error_code = 500;
		return 1;
	}
}


// /* Translation of parameters for QUERY_STRING environment variable */
// std::string Cgi::decode(std::string &path)
// {
// 	size_t token = path.find("%");
// 	while (token != std::string::npos)
// 	{
// 		if (path.length() < token + 2)
// 			break ;
// 		char decimal = fromHexToDec(path.substr(token + 1, 2));
// 		path.replace(token, 3, toString(decimal));
// 		token = path.find("%");
// 	}
// 	return (path);
// }

std::string Cgi::getPathInfo() const
{
	// Assuming that the script name is everything after the last '/'
	size_t lastSlashPos = _cgi_path.rfind('/');
	if (lastSlashPos != std::string::npos && lastSlashPos + 1 < _cgi_path.length())
	{
		// Extract the path info following the script name
		return _cgi_path.substr(lastSlashPos + 1);
	}
	else // If no additional path info, return an empty string
		return "";
}

void	Cgi::setCgiClientFd(int client_fd)
{
	this->_client_fd = client_fd;
}

void	Cgi::clear()
{
	this->_cgi_pid = -1;
	this->cgi_pipe_out_read_end = -1;
	this->cgi_pipe_in_write_end = -1;
	this->_exit_code = 0;
	this->_cgi_path = "";
	this->_char_environment = NULL;
	this->_arguments_for_execve = NULL;
	this->_environment.clear();
	this->_cgi_extension.clear();
}
