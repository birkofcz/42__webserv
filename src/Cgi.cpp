/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 16:03:12 by tkajanek          #+#    #+#             */
/*   Updated: 2024/01/27 18:36:21 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

/* Constructor */
Cgi::Cgi()
{
	this->_cgi_pid = -1;
	this->cgi_pipe_out_read_end = -1;
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

// const pid_t &Cgi::getCgiPid() const
// {
//     return (this->_cgi_pid);
// }

// const std::string &Cgi::getCgiPath() const
// {
//     return (this->_cgi_path);
// }

// void Cgi::initEnvCgi(HttpRequest& req, const std::vector<Location>::iterator it_loc)
// {
// 	string cgi_exec = ("cgi-bin/" + it_loc->getCgiPath(_cgi_extension)); //it_loc->getCgiPath(_cgi_extension)).c_str()
// 	char*	cwd = getcwd(NULL, 0);
// 	if(_cgi_path[0] != '/') //if cgi path is not absolute -> creates absolute path with cwd
// 	{
// 		std::string tmp(cwd);
// 		tmp.append("/");
// 		if(_cgi_path.length() > 0)
// 			_cgi_path.insert(0, tmp);
// 	}
// 	if(req.getMethod() == POST)
// 	{
// 		std::stringstream ss;
// 		ss << req.getBody().length();
// 		this->_environment["CONTENT_LENGTH"] = ss.str();
// 		this->_environment["CONTENT_TYPE"] = req.getHeader("content-type");
// 	}

//     this->_environment["GATEWAY_INTERFACE"] = std::string("CGI/1.1");
// 	this->_environment["SCRIPT_NAME"] = cgi_exec;//
//     this->_environment["SCRIPT_FILENAME"] = this->_cgi_path;
//     this->_environment["PATH_INFO"] = this->_cgi_path;//
//     this->_environment["PATH_TRANSLATED"] = this->_cgi_path;
// 	// The translated version of PATH_INFO into an absolute path on the server's file system.
// 	// It's the result of combining SCRIPT_FILENAME with PATH_INFO.
//     this->_environment["REQUEST_URI"] = this->_cgi_path;
// 	// Represents the full original request URI sent by the client.
// 	// It includes SCRIPT_NAME and QUERY_STRING (if present).
//     this->_environment["SERVER_NAME"] = req.getHeader("host"); //does it contain port?
//     this->_environment["SERVER_PORT"] = reg.getServerPort();
//     this->_environment["REQUEST_METHOD"] = req.getMethodStr();
//     this->_environment["SERVER_PROTOCOL"] = "HTTP/1.1"; //redo with req variables?
//     this->_environment["REDIRECT_STATUS"] = "200"; //??
// 	this->_environment["SERVER_SOFTWARE"] = "[GTS]erver";

// 	std::map<std::string, std::string> request_headers = req.getHeaders();
// 	for(std::map<std::string, std::string>::iterator it = request_headers.begin();
// 		it != request_headers.end(); ++it)
// 	{
// 		std::string name = it->first;
// 		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
// 		std::string key = "HTTP_" + name;
// 		_env[key] = it->second;
// 	}
// 	this->_char_environment = (char **)calloc(sizeof(char *), this->_environment.size() + 1);
// 	std::map<std::string, std::string>::const_iterator it = this->_environment.begin();
// 	for (int i = 0; it != this->_environment.end(); it++, i++)
// 	{
// 		std::string tmp = it->first + "=" + it->second;
// 		Log::Msg(DEBUG, FUNC + "during allocating loop ENV: " + tmp);
// 		this->_char_environment[i] = strdup(tmp.c_str());
// 	}

// 	Log::Msg(DEBUG, FUNC + "cgi_exec: " + cgi_exec);
// 	Log::Msg(DEBUG, FUNC + "cgi_exec: " + _cgi_path);

// 	this->_arguments_for_execve = (char **)malloc(sizeof(char *) * 3);
// 	this->_arguments_for_execve[0] = strdup(cgi_exec.c_str()); // cgi-bin/script.cgi
// 	this->_arguments_for_execve[1] = strdup(this->_cgi_path.c_str()); // /var/www/html/cgi-bin/script.cgi
// 	this->_arguments_for_execve[2] = NULL;
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
	// std::string extension;
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
	this->_environment["CONTENT_LENGTH"] = req.getHeader("content-length");
	this->_environment["CONTENT_TYPE"] = req.getHeader("content-type");
    this->_environment["GATEWAY_INTERFACE"] = "CGI/1.1";

	poz = (this->_cgi_path.empty()) ? std::string::npos : this->_cgi_path.find("cgi-bin/");
	/*
	_environment["SCRIPT_NAME"] = "/cgi-bin/my_script.cgi";
	_environment["SCRIPT_FILENAME"] = "my_script.cgi";
	*/
	this->_environment["SCRIPT_NAME"] = this->_cgi_path;
    this->_environment["SCRIPT_FILENAME"] = this->_cgi_path.substr(poz + 8, this->_cgi_path.size());
	//((poz < 0 || (size_t)(poz + 8) > this->_cgi_path.size()) ? "" : this->_cgi_path.substr(poz + 8, this->_cgi_path.size())); // check dif cases after put right parametr from the response
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

/* Pipe and execute CGI */
void Cgi::execute(short& error_code, int pipe_stdin)
{
	int cgi_stdout[2];
	if (this->_arguments_for_execve[0] == NULL || this->_arguments_for_execve[1] == NULL)
	{
		error_code = 500;
		return ;
	}
	if (pipe(cgi_stdout) < 0)
	{
        // Logger::logMsg(RED, CONSOLE_OUTPUT, "pipe() failed");
		if (pipe_stdin != -1)
			close(pipe_stdin);
		error_code = 500;
		return ;
	}

	Log::Msg(DEBUG, FUNC + "before forking");

	
	this->_cgi_pid = fork();
	if (this->_cgi_pid == 0)
	{
		if (pipe_stdin != -1)
			dup2(pipe_stdin, STDIN_FILENO);
		if (pipe_stdin != -1)
			close(pipe_stdin);
		dup2(cgi_stdout[1], STDOUT_FILENO);

		close(cgi_stdout[0]);
		close(cgi_stdout[1]);
		std::cerr << "child after sleeping\n";
		this->_exit_code = execve(this->_arguments_for_execve[0], this->_arguments_for_execve, this->_char_environment);
		exit(this->_exit_code); //kdyz selze execve?
	}
	else if (this->_cgi_pid > 0)
	{
		cgi_pipe_out_read_end = cgi_stdout[0];
		close(cgi_stdout[1]);
		if (pipe_stdin != -1) //not sure
			close(pipe_stdin);
		Log::Msg(DEBUG, FUNC + "parent after forking");
	}
	else
	{
		if (pipe_stdin != -1)
			close(pipe_stdin);
		close(cgi_stdout[1]);
        std::cout << "Fork failed" << std::endl;
		error_code = 500;
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
	else
	{
		// If no additional path info, return an empty string
		return "";
	}
}

// /* Isolation PATH_INFO environment variable */
// std::string Cgi::getPathInfo(std::string& path, std::vector<std::string> extensions)
// {
// 	std::string tmp;
// 	size_t start, end;

// 	for (std::vector<std::string>::iterator it_ext = extensions.begin(); it_ext != extensions.end(); it_ext++)
// 	{
// 		start = path.find(*it_ext);
// 		if (start != std::string::npos)
// 			break ;
// 	}
// 	if (start == std::string::npos)
// 		return "";
// 	if (start + 3 >= path.size())
// 		return "";
// 	tmp = path.substr(start + 3, path.size());
// 	if (!tmp[0] || tmp[0] != '/')
// 		return "";
// 	end = tmp.find("?");
// 	return (end == std::string::npos ? tmp : tmp.substr(0, end));
// }


// int		Cgi::getPipeOutReadEnd()	const
// {	
// 	return pipe_out[0];
// }

// void	Cgi::setCgiStdin(int stdinWriteEnd)
// {
// 	pipe_in[1] = stdinWriteEnd;
// }

void	Cgi::setCgiClientFd(int client_fd)
{
	this->_client_fd = client_fd;
}
void	Cgi::clear()
{
	this->_cgi_pid = -1;
	this->cgi_pipe_out_read_end = -1;
	this->_exit_code = 0;
	this->_cgi_path = "";
	this->_char_environment = NULL;
	this->_arguments_for_execve = NULL;
	this->_environment.clear();
	this->_cgi_extension.clear();
}





/* looped script!
#include <signal.h>
#include <unistd.h>

// Define a global flag to indicate if the timeout occurred
volatile sig_atomic_t timeout_flag = 0;

// Signal handler for SIGALRM
void handle_alarm(int signum) {
    timeout_flag = 1;  // Set the timeout flag
}

// Function to execute the CGI script with a timeout
void Cgi::executeWithTimeout(short& error_code, int pipe_stdin, int timeout_seconds) {
    // Set the signal handler for SIGALRM
    signal(SIGALRM, handle_alarm);

    // Set the alarm for the specified timeout
    alarm(timeout_seconds);

    // Execute the CGI script as usual
    execute(error_code, pipe_stdin);

    // Check if the timeout flag is set
    if (timeout_flag) {
        // Handle the timeout (e.g., log, cleanup, set error code)
        error_code = 504; // Gateway Timeout
    }

    // Disable the alarm
    alarm(0);
}

// Function to execute the CGI script (original function)
void Cgi::execute(short& error_code, int pipe_stdin) {
    // Your existing code for executing the CGI script goes here
}
*/