/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:42:21 by tkajanek          #+#    #+#             */
/*   Updated: 2024/02/08 16:47:11 by sbenes           ###   ########.fr       */
=======
/*   By: gabtan <gabtan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:42:21 by tkajanek          #+#    #+#             */
/*   Updated: 2024/02/09 15:37:41 by gabtan           ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerManager.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstddef> //for size_t

using std::vector;
#define MAX_EVENTS 64
#define MESSAGE_BUFFER 1000000

ServerManager::ServerManager()
{
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
	{
		Log::Msg(ERROR, FUNC + "epoll_create1: " + toString(strerror(errno)));
		exit(EXIT_FAILURE);
	}
}
ServerManager::~ServerManager() {}

int		ServerManager::getEpollFd() {return this->_epoll_fd;};

void	ServerManager::initServers(vector<Server> servers)
{	
	_servers = servers;
	bool serverDouble; // to track whether a server is a duplicate.
	for (vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		serverDouble = false;
		for (vector<Server>::iterator it2 = _servers.begin(); it2 != it; ++it2)
		{
			if (it2->getHost() == it->getHost() && it2->getPort() == it->getPort())
			{
				it->setFd(it2->getFd());
				serverDouble = true;
				// If a duplicate is found, it sets the file descriptor (Fd)
				// of the current server to that of the duplicate server.
			}
		}
		if (!serverDouble)
		{
			it->setupServer(); // creates a socket and binds it with servers address
			// std::cout << *it;
		}
		
		char buf[INET_ADDRSTRLEN] = {0};
		// This constant represents the maximum length, in characters, of the string representation of an IPv4 address
		// "xxx.xxx.xxx.xxx" plus the null terminator.
		Log::Msg(INFO, "Server Created: ServerName[" + it->getName() + "] Host[" + 
			inet_ntop(AF_INET, &it->getHost(), buf, INET_ADDRSTRLEN) + "] Port[" + 
			toString(it->getPort()) + "]");
	}
}

void	ServerManager::_addToEpoll(int fd, int event_flag)
{
	struct epoll_event event;
	event.events = event_flag;  // Set the events flag (EPOLLIN, EPOLLOUT ...)
	event.data.fd = fd;     // The file descriptor to monitor

	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		Log::Msg(ERROR, FUNC + "epoll_ctl: " + toString(strerror(errno)));
		// Handle error if needed
		return;
	}
	Log::Msg(DEBUG, FUNC + "fd added to epoll: " + toString(fd));
}

/*
Traditional mechanisms like select and poll suffer from performance
degradation as the number of file descriptors increases.
This is because these mechanisms require iterating through
the entire set of file descriptors to check for events.

epoll employs a more efficient data structure that allows it
to scale better as the number of file descriptors grows.
It uses a red-black tree and hash table internally
to keep track of file descriptors and their associated events.
It maintains a data structure (the epoll instance) that stores
information about the registered file descriptors
and the events the program is interested in.

An epoll instance is a data structure maintained by the
operating system to keep track of file descriptors that
the application is interested in monitoring for events.

The epoll_event structure is used to specify the events of interest
for a particular file descriptor in the epoll instance.
struct epoll_event
{
	uint32_t events;  // Bitmask of events to monitor (EPOLLIN, EPOLLOUT, EPOLLET, etc.)
	epoll_data_t data; // User data variable (can be a file descriptor or a pointer)
};

When calling epoll_wait, the program is blocked until
one or more of the monitored events occur.
The function returns information about the events that have occurred,
allowing the program to take appropriate actions.

*/
void ServerManager::runServers()
{
	// let servers listen and put them to epoll structure.
	for (vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (listen(it->getFd(), 512) == -1) //512 pending connections before rejecting new connection attempts.
		{
			Log::Msg(ERROR, FUNC + "listen error: " + toString(strerror(errno)));
			exit(EXIT_FAILURE);
		}
		if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0)
		// F_SETFL: This is a command to fcntl indicating that you want to set the file status flags.
		// O_NONBLOCK: This flag specifies non-blocking mode.
		{
			Log::Msg(ERROR, FUNC + "fclt error: " + toString(strerror(errno)));
			exit(EXIT_FAILURE);
		}
		_addToEpoll(it->getFd(), EPOLLIN | EPOLLOUT);
		_servers_map.insert(std::make_pair(it->getFd(), *it));
	}

	// run epoll_wait to recognize events on epoll structure.
	while (!terminateFlag)
	{
		struct epoll_event triggeredEvents[MAX_EVENTS];
		int numEvents = epoll_wait(_epoll_fd, triggeredEvents, MAX_EVENTS, 1000); // 1 second timeout
		if (numEvents == -1)
		{
			Log::Msg(ERROR, FUNC + "epoll_wait. " + toString(strerror(errno)));
			terminateFlag = true;
			// exit(EXIT_FAILURE);
		}
		for (int i = 0; i < numEvents; ++i)
		{
			// There is incoming data from a client or a pipe.
			// Client sends an HTTP request (e.g., GET, POST)for example.
			if (triggeredEvents[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR))
			{
				int fd = triggeredEvents[i].data.fd;
				Log::Msg(DEBUG, FUNC + "EPOLLIN fd triggered : " + toString(fd));
				if (_servers_map.count(fd))
					_acceptNewConnection(_servers_map.find(fd)->second);
				else if (_clients_map.count(fd))
				{
					_readRequest(fd, _clients_map[fd]);
					if (_clients_map[fd].response.getCgiFlag())
					{
						// Add cgi pipe_in[1] (write end of the pipe) to the epoll interest list
						_addToEpoll(_clients_map[fd].response.cgi_object.cgi_pipe_in_write_end, EPOLLOUT);
						// Add cgi pipe_out[0] (read end of the pipe) to the epoll interest list
						_addToEpoll(_clients_map[fd].response.cgi_object.cgi_pipe_out_read_end, EPOLLIN);
				
						Log::Msg(DEBUG, FUNC + "write end of CGI pipe in added to epoll interest list: " + toString(_clients_map[fd].response.cgi_object.cgi_pipe_in_write_end));
						Log::Msg(DEBUG, FUNC + "read end of CGI pipe in added to epoll interest list: " + toString(_clients_map[fd].response.cgi_object.cgi_pipe_out_read_end));
						Log::Msg(DEBUG, FUNC + "pipe fd added to client fd map : " + toString(_clients_map[fd].response.cgi_object.cgi_pipe_out_read_end));
						Log::Msg(DEBUG, FUNC + "pipe fd added to client fd map : " + toString(_clients_map[fd].response.cgi_object.cgi_pipe_in_write_end));

						_cgi_pipe_to_client_map.insert(std::make_pair(_clients_map[fd].response.cgi_object.cgi_pipe_out_read_end, fd));
						_cgi_pipe_to_client_map.insert(std::make_pair(_clients_map[fd].response.cgi_object.cgi_pipe_in_write_end, fd));
					
						// std::map<int, int>::const_iterator it;
						// for (it = _cgi_pipe_to_client_map.begin(); it != _cgi_pipe_to_client_map.end(); ++it)
						// {
						// int key = it->first;
						// int value = it->second;
						// std::cout << "Key: " << toString(key) << ", Value: " << toString(value) << std::endl;
   					}
				}
				else if (_cgi_pipe_to_client_map.count(fd)) // Handle EPOLLIN on CGI pipe. Reading CGI script output and appending it to the response.
				{
					int fd_client = _cgi_pipe_to_client_map[fd];
					Log::Msg(DEBUG, FUNC + "EPOLLIN on cgi out pipe triggered. Fd_client: " + toString(fd_client));
					int status;
					pid_t pid = waitpid(_clients_map[fd_client].response.cgi_object.getCgiPid(), &status, 0); // Wait for child process to terminate
					if (pid == -1)
					{
						Log::Msg(ERROR, FUNC + "waitpid = -1. " + toString(strerror(errno)));
						_clients_map[fd_client].response.response_content.append(Error::instantErrorPage(500));
						close(_clients_map[fd_client].response.cgi_object.cgi_pipe_out_read_end);
						_cgi_pipe_to_client_map.erase(fd);
						// exit(EXIT_FAILURE);
					}
					if(WEXITSTATUS(status) != 0)
					{
						Log::Msg(ERROR, FUNC + "CGI script terminated with an error status: " + toString(WEXITSTATUS(status)));
						_clients_map[fd_client].response.response_content.append(Error::instantErrorPage(502));
						close(_clients_map[fd_client].response.cgi_object.cgi_pipe_out_read_end);
						_cgi_pipe_to_client_map.erase(fd);
					}
					else if (WIFEXITED(status))
					{
						_readCgiResponse(_clients_map[fd_client]);
						_clients_map[fd_client].response.setStatusCode(200);
						_clients_map[fd_client].response.response_content.insert(0, _clients_map[fd_client].response.getStatusLineCgi());
						_cgi_pipe_to_client_map.erase(fd);
					}
					else if (WIFSIGNALED(status))
					{
						Log::Msg(ERROR, FUNC + "Child process running CGI script terminated by signal: " + toString(WTERMSIG(status)));
						_clients_map[fd_client].response.response_content.append(Error::instantErrorPage(502));						
						close(_clients_map[fd_client].response.cgi_object.cgi_pipe_out_read_end);
						_cgi_pipe_to_client_map.erase(fd);
					}
				}
			}
			// The socket's send buffer is ready to accept data, and we can write to it without blocking.
			// Example: After processing the request, the server may generate an HTTP response.
			if (triggeredEvents[i].events & (EPOLLOUT | EPOLLHUP | EPOLLERR))
			{
				int fd = triggeredEvents[i].data.fd;
				if (_clients_map.count(fd) && !_clients_map[fd].response.response_content.empty())
				{
					Log::Msg(DEBUG, FUNC + "EPOLLOUT fd of client triggered : " + toString(fd));
					_sendResponse(fd, _clients_map[fd]);
				}
				else if (_cgi_pipe_to_client_map.count(fd))
				{
					int fd_client = _cgi_pipe_to_client_map[fd];
					Log::Msg(DEBUG, FUNC + "EPOLLOUT cgi_in pipe triggered. fd_client: " + toString(fd_client));
					_sendCgiBody(_clients_map[fd_client]);
					_cgi_pipe_to_client_map.erase(fd);
				}	
			}
		}
	}
	this->clear();
}

void ServerManager::clear()
{
 	if ( ServerManager::getEpollFd() != -1)
		close (getEpollFd());
	for (std::map<int, Server>::iterator it = _servers_map.begin(); it != _servers_map.end();)
	{
		int fd = it->first;
		if (close(fd) == -1)
			Log::Msg(ERROR, "Failed to close file descriptor " + toString(fd));
		_servers_map.erase(it++);
	}
}

void ServerManager::_sendResponse(const int& fd, Client& c)
{
	//----
	//write the response to the text file in data/response_temp.txt - in truncate mode - for debugging and showcase purposes
	std::ofstream responseFile("data/response_temp.txt", std::ios::out | std::ios::trunc); // Opens the file in write/truncate mode
	responseFile << c.response.response_content << "\n";
	responseFile.close();
	//----
	
	// std::ofstream debugFile("debug_output.txt", std::ios::app); // Opens the file in append mode
    // debugFile << "SENDING RESPONSE data: \n" << c.response.response_content << "\n";
    // debugFile.close();

    ssize_t bytes_written = send(fd, c.response.response_content.c_str(), c.response.response_content.size(), MSG_DONTWAIT);
    if (bytes_written < 0)
    {
        Log::Msg(ERROR, FUNC + "Send error");
        // Handle error as needed
    }
    else
    {
		// if (c.request.keepAlive() == false)
		// 	_closeConnection(c.getSocket());
		c.clearClient();
		Log::Msg(DEBUG, FUNC + "Succesfully sent response to fd: " + toString(c.getSocket()));	
	}
}

void ServerManager::_acceptNewConnection(Server &serv)
{
	struct sockaddr_in client_address;
	socklen_t client_address_size = sizeof(client_address);
	int client_sock = accept(serv.getFd(), (struct sockaddr *)&client_address, &client_address_size);
	if (client_sock == -1)
	{
		Log::Msg(ERROR, FUNC + "accept: " + toString(strerror(errno)));
		return;
	}
	
	// Set the accepted socket to non-blocking
	if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0)
	{
		Log::Msg(ERROR, FUNC + "fcntl: " + toString(strerror(errno)));
		close(client_sock);
		return;
	}

	// Create and manage the client
	Client new_client(serv);
	new_client.setSocket(client_sock);


	// add client to epoll structure
	_addToEpoll(client_sock, EPOLLIN | EPOLLOUT);

	// Add the client to the clients map
	if (_clients_map.count(client_sock) != 0)
		_clients_map.erase(client_sock);
		
	// to handle scenarios where a client disconnects and then reconnects
	// with the same socket. If the socket is reused, it ensures that the old entry
	// in the _clients_map is removed before adding a new entry for the reconnected client.
	_clients_map.insert(std::pair<int, Client>(client_sock, new_client));
	// Output information about the new connection
	char buf[INET_ADDRSTRLEN];
	Log::Msg(INFO, "New Connection From " + toString(inet_ntop(AF_INET, &client_address.sin_addr, buf, INET_ADDRSTRLEN)) + ", Assigned Socket " + toString(client_sock));
}

void    ServerManager::_closeConnection(const int fd)
{
	struct epoll_event event;
	event.events = 0; // Clear all events
	event.data.fd = fd;

	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &event) == -1)
		Log::Msg(ERROR, FUNC + "epol_ctl: " + toString(strerror(errno)));
	close(fd);
	_clients_map.erase(fd);
}

void ServerManager::_readRequest(const int& fd, Client& c)
{
	char buffer[MESSAGE_BUFFER];
	int bytes_read = 0;
	bytes_read = recv(fd, buffer, MESSAGE_BUFFER, MSG_DONTWAIT);

	// std::ofstream debugFile("debug_output.txt", std::ios::app); // Opens the file in append mode
    // debugFile << "\nBUFFER BEFORE feed: \n" << buffer << "\n";
	
	//writes the request to the text file in data/request_temp.txt - in truncate mode - for debugging and showcase purposes
	//----
	std::ofstream requestFile("data/request_temp.txt", std::ios::out | std::ios::trunc); // Opens the file in write/truncate mode
	requestFile << buffer<< "\n";
	requestFile.close();
	//---

	if (bytes_read == 0)
	{
		Log::Msg(DEBUG, FUNC + "Bytes read = 0. Client closed the connection.");
		_closeConnection(fd);
		return;
	}
	else if (bytes_read < 0)
	{
		Log::Msg(ERROR, FUNC + "Read error occurred.");
		_closeConnection(fd);
		return;
	}
	else if (bytes_read != 0) // data were successfully read.
	{
		c.request.feed(buffer, bytes_read);
		memset(buffer, 0, sizeof(buffer));
	}

    // debugFile << "PRESENTING REQUEST data: \n" << c.request << "\n";
    // debugFile.close();

	if (c.request.complete_flag || c.request.getErrorCode())
	{
		Log::Msg(DEBUG, FUNC + "Parsing completed or Error code setted");
		c.clientBuildResponse();
	}
	// 		Msg(DEBUG, "Request Received From Socket %d, Method=<%s>  URI=<%s>",
}

void    ServerManager::_sendCgiBody(Client &c)
{
		ssize_t bytes_written = write(c.response.cgi_object.cgi_pipe_in_write_end, c.request.getBody().c_str(), c.request.getBodyLen());
		if (bytes_written == -1)
		{
			Log::Msg(ERROR, FUNC + "bytes_written = -1");
			close(c.response.cgi_object.cgi_pipe_in_write_end);
			// c.response._status_code = 500; upravit pak na setter
			return ;
			// Handle the error (e.g., log it, return an error code, etc.)
		}
		else if (bytes_written < static_cast<ssize_t>(c.request.getBodyLen()))
		{
			Log::Msg(ERROR, FUNC + "pipe for stdin CGI full");
				// This may happen if the pipe is full, and you need to handle it accordingly
		}
		close(c.response.cgi_object.cgi_pipe_in_write_end);
}

void    ServerManager::_readCgiResponse(Client &c)
{
	char buffer[MESSAGE_BUFFER];
	int bytes_read = 0;

	bytes_read = read(c.response.cgi_object.cgi_pipe_out_read_end, buffer, MESSAGE_BUFFER);
	Log::Msg(DEBUG, FUNC + "bytes read from cgi pipe: " + toString(bytes_read));
	if (bytes_read == 0)
    {
		Log::Msg(DEBUG, FUNC + "Read 0 bytes from CGI pipe indicating the end of the stream has been reached"); 
		close(c.response.cgi_object.cgi_pipe_out_read_end);
        return ;
    }
    else if (bytes_read < 0)
    {
		Log::Msg(ERROR, FUNC + "Error reading from CGI pipe");
		c.response.response_content.append(Error::instantErrorPage(500)); //?
		close(c.response.cgi_object.cgi_pipe_out_read_end);
        return ;
    }
    else
    {
		close(c.response.cgi_object.cgi_pipe_out_read_end);
		buffer[bytes_read] = '\0';
		bool has_content_length = strstr(buffer, "Content-Length:") != 0;
		if (!has_content_length)
		{
			size_t cont_len = _calcContLenCgi(buffer, bytes_read);
			c.response.response_content.append("Content-Length: ");
			c.response.response_content.append(toString(cont_len));
			c.response.response_content.append("\r\n");
		}
		c.response.response_content.append(buffer, bytes_read);
		Log::Msg(DEBUG, FUNC + "CGI buffer:\n" + buffer);
		memset(buffer, 0, sizeof(buffer));
	}
}

size_t ServerManager::_calcContLenCgi(const char* buffer, size_t size)
{
	std::string buffer_str(buffer, size);
	size_t contentLength = 0;
    size_t contentStart = buffer_str.find("\n\n");

    if (contentStart != std::string::npos)
    {
        contentStart += 4; // Move to the start of content after "\r\n\r\n"
        contentLength = buffer_str.size() - contentStart;
    }
	
    // std::cout << "contentLength = " << contentLength << " and content: " << endl;
    // if (contentStart != std::string::npos)
    // {
    //     std::cout << buffer_str.substr(contentStart, contentLength);
    // }
    // std::cout << std::endl;

    return contentLength;
}