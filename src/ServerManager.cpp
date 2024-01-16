/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:42:21 by tkajanek          #+#    #+#             */
/*   Updated: 2024/01/06 16:31:49 by tkajanek         ###   ########.fr       */
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
#define MESSAGE_BUFFER 5000000

ServerManager::ServerManager()
{
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
}
ServerManager::~ServerManager() {}

void ServerManager::initServers(vector<Server> servers)
{
	cout << endl;
	print("ServerManager -- Initializing  Servers...", GREEN);
	_servers = servers;
	// char buf[INET_ADDRSTRLEN];
	// This constant represents the maximum length, in characters, of the string representation of an IPv4 address
	// "xxx.xxx.xxx.xxx" plus the null terminator.
	bool serverDouble; // to track whether a server is a duplicate.
	for (vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		serverDouble = false;
		for (vector<Server>::iterator it2 = _servers.begin(); it2 != it; ++it2)
		{
			if (it2->getHost() == it->getHost() && it2->getPorts()[0] == it->getPorts()[0])
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
		// print( "Server Created: ServerName[%s] Host[%s] Port[%d]",it->getServerName().c_str(),
		//         inet_ntop(AF_INET, &it->getHost(), buf, INET_ADDRSTRLEN), it->getPort());
	}
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
	struct epoll_event serverEvent;
	serverEvent.events = EPOLLIN | EPOLLOUT; // Edge-triggered mode
	for (vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (listen(it->getFd(), 512) == -1)
		{
			print("webserv: listen error:", RED);
			exit(EXIT_FAILURE);
		}
		if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0)
		// F_SETFL: This is a command to fcntl indicating that you want to set the file status flags.
		// O_NONBLOCK: This flag specifies non-blocking mode.
		{
			print("webserv: fclt error:", RED);
			exit(EXIT_FAILURE);
		}
		serverEvent.data.fd = it->getFd();
		if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, it->getFd(), &serverEvent) == -1)
		{
			perror("epoll_ctl");
			exit(EXIT_FAILURE);
		}
		_servers_map.insert(std::make_pair(it->getFd(), *it));
	}

	while (true)
	{
		struct epoll_event triggeredEvents[MAX_EVENTS];
		int numEvents = epoll_wait(_epoll_fd, triggeredEvents, MAX_EVENTS, 1000); // 1 second timeout
		if (numEvents == -1)
		{
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		// cout << "epoll wait iteration" << endl;
		for (int i = 0; i < numEvents; ++i)
		{
			// There is incoming data from a client.
			// Client sends an HTTP request (e.g., GET, POST)for example.
			if (triggeredEvents[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR))
			{
				int fd = triggeredEvents[i].data.fd;
				if (_servers_map.count(fd))
					acceptNewConnection(_servers_map.find(fd)->second);
				else if (_clients_map.count(fd))
					readRequest(fd, _clients_map[fd]);
			}

			// The socket's send buffer is ready to accept data, and we can write to it without blocking.
			// Example: After processing the request, the server may generate an HTTP response.
			if (triggeredEvents[i].events & (EPOLLOUT | EPOLLHUP | EPOLLERR))
			{
				int fd = triggeredEvents[i].data.fd;
				if (_clients_map.count(fd) && !_clients_map[fd].response._response_content.empty())
				{
					// int cgi_state = _clients_map[fd].response.getCgiState();
					// if (cgi_state == 1 && FD_ISSET(_clients_map[fd].response._cgi_obj.pipe_in[1], &_write_fd_pool))
					// 	sendCgiBody(_clients_map[fd], _clients_map[fd].response._cgi_obj);
					// else if (cgi_state == 1 && FD_ISSET(_clients_map[fd].response._cgi_obj.pipe_out[0], &_recv_fd_pool))
					// 	readCgiResponse(_clients_map[fd], _clients_map[fd].response._cgi_obj);
					// else if ((cgi_state == 0 || cgi_state == 2) && FD_ISSET(fd, &_write_fd_pool))
					// 	sendResponse(fd, _clients_map[fd]);
					sendResponse(fd, _clients_map[fd]);
				}
			}
		}
		//checkTimeout();
	}
}

void ServerManager::sendResponse(const int& fd, Client& c)
{
	cout << endl << "SENDING RESPONSE data: \n" << c.response._response_content << endl;

	std::ofstream debugFile("debug_output.txt", std::ios::app); // Opens the file in append mode
    debugFile << "SENDING RESPONSE data: \n" << c.response._response_content << "\n";
    debugFile.close();

    ssize_t bytes_written = send(fd, c.response._response_content.c_str(), c.response._response_content.size(), MSG_DONTWAIT);
    if (bytes_written < 0)
    {
        perror("write");
        // Handle error as needed
    }
    else
    {
		c.clearClient();
        std::cout << "Successful SEND RESPONSE \n";
    }
}



void ServerManager::acceptNewConnection(Server &serv)
{
	struct sockaddr_in client_address;
	socklen_t client_address_size = sizeof(client_address);
	int client_sock = accept(serv.getFd(), (struct sockaddr *)&client_address, &client_address_size);
	if (client_sock == -1)
	{
		perror("accept");
		return;
	}

	// Set the accepted socket to non-blocking
	if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl");
		close(client_sock);
		return;
	}

	// Create and manage the client
	Client new_client(serv);
	new_client.setSocket(client_sock);

	// add client to epoll structure
	struct epoll_event client_event;
    client_event.events = EPOLLIN | EPOLLOUT; // Edge-triggered mode
    client_event.data.fd = client_sock;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_sock, &client_event) == -1)
    {
        perror("client_epoll_ctl");
        exit(EXIT_FAILURE);
    }

	// Add the client to the clients map
	if (_clients_map.count(client_sock) != 0)
		_clients_map.erase(client_sock);
	// to handle scenarios where a client disconnects and then reconnects
	// with the same socket. If the socket is reused, it ensures that the old entry
	// in the _clients_map is removed before adding a new entry for the reconnected client.
	_clients_map.insert(std::pair<int, Client>(client_sock, new_client));
	// Output information about the new connection
	char buf[INET_ADDRSTRLEN];
	
	fprintf(stderr, "New Connection From %s, Assigned Socket %d\n",
			inet_ntop(AF_INET, &client_address.sin_addr, buf, INET_ADDRSTRLEN), client_sock);
}

void    ServerManager::_closeConnection(const int fd)
{
    struct epoll_event event;
    event.events = 0; // Clear all events
    event.data.fd = fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &event) == -1)
	{
        // Handle error if epoll_ctl fails
        perror("Error removing fd from epoll");
    }
	close(fd);
    _clients_map.erase(fd);
}

void ServerManager::readRequest(const int& fd, Client& c)
{
	char buffer[MESSAGE_BUFFER];
	int bytes_read = 0;
	bytes_read = recv(fd, buffer, MESSAGE_BUFFER, MSG_DONTWAIT);
	cout << endl << "BUFFER BEFORE feed: "<< endl << buffer << endl;

	std::ofstream debugFile("debug_output.txt", std::ios::app); // Opens the file in append mode
    debugFile << "\nBUFFER BEFORE feed: \n" << buffer << "\n";
	if (bytes_read == 0)
	{
		// Client closed the connection
		// Logger::logMsg(YELLOW, CONSOLE_OUTPUT, "webserv: Client %d Closed Connection", fd);
		_closeConnection(fd);
		return;
	}
	else if (bytes_read < 0)
	{
		// Read error occurred
		// Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: fd %d read error %s", fd, strerror(errno));
		_closeConnection(fd);
		return;
	}
	else if (bytes_read != 0)
	{
		// data were successfully read.
		// c.updateTime();
		c.request.feed(buffer, bytes_read);
		memset(buffer, 0, sizeof(buffer));
	}

    debugFile << "PRESENTING REQUEST data: \n" << c.request << "\n";
    debugFile.close();

	// assignServer(c);
	c.clientBuildResponse();


	// if (c.request.parsingCompleted() || c.request.errorCode()) {
	// 		assignServer(c);
	// 		Logger::logMsg(CYAN, CONSOLE_OUTPUT, "Request Received From Socket %d, Method=<%s>  URI=<%s>",
	// 					fd, c.request.getMethodStr().c_str(), c.request.getPath().c_str());
	// 		c.buildResponse();
	// 		if (c.response.getCgiState()) {
	// 			handleReqBody(c);
	// 			// Add logic to wait for pipe events if needed
	// 		}
	// 	}
	
}



/* Assigen server_block configuration to a client based on Host Header in request and server_name*/
// void	ServerManager::assignServer(Client& c)
// {
//     for (vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
//     {
//         if (c.server.getHost() == it->getHost() &&
//             c.server.getPort() == it->getPort() &&
//             c.request.getServerName() == it->getServerName())
//         {
//             c.setServer(*it);
//             return ;
//         }
//     }
// }

	//