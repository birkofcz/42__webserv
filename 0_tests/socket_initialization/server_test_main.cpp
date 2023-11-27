/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_test_main.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:21:54 by tkajanek          #+#    #+#             */
/*   Updated: 2023/11/27 18:53:19 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring> //memset
#include <vector>


#include <unistd.h> //close
#include <arpa/inet.h> //Types used: struct sockaddr_in. htons(), inet_addr().
#include <sys/socket.h> // AF_INET, SOCK_STREAM, SOMAXCONN. socket(), bind(), listen().
#include <poll.h>

void printUsage()
{
    std::cerr << "Usage: ./webserv [configuration file]\n";
    exit(EXIT_FAILURE);
}

// Function to generate a simple HTML page
std::string generateHtmlPage()
{
    return "<html><head><title>Hello Page</title></head><body><h1>Hello</h1></body></html>";
}

int main(int argc, char *argv[])
{
    // Check for the correct number of command-line arguments
    if (argc != 2)
	{
        printUsage();
    }

    // Read the configuration file path
    const char *configFilePath = argv[1];
	(void) configFilePath;

    // Basic socket initialization and configuration
    int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketFd == -1)
	{
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
	/*
	int socket(int domain, int type, int protocol);

    domain: the socket’s protocol family, generally AF_INET or AF_INET6.
    type: the type of socket, generally SOCK_STREAM or SOCK_DGRAM.
    protocol: the protocol to use with the socket. In general, there is only one valid protocol by socket type,
	TCP for a stream socket and UDP for a datagram socket, which means we can safely put 0 here.
	AF_INET: stands for Address Family IPv4.
	SOCK_STREAM indicates that the socket will provide a reliable, connection-oriented stream, commonly used with TCP.
	0: This parameter is called the protocol. When 0 is passed, the system selects the default protocol
	for the specified socket type and address family. For SOCK_STREAM (TCP), it will use the default TCP protocol.
	*/

    // Set up server address struct to store information about the server's address.
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; //Sets address family of the socket. AF_INET indicates tthe socket will use IPv4 addrs.
    serverAddr.sin_addr.s_addr = INADDR_ANY;	// Specifies the IP address to bind the socket to. INADDR_ANY represents all available interfaces on the machine. 
												// It means the socket will accept connections from any network interface.
    serverAddr.sin_port = htons(8080); 	// htons is a function that converts the port number from host byte order to network byte order.
										// This is necessary because different computer architectures may use different byte orders,
										// and network protocols often use a standardized byte order (network byte order).

    if (bind(serverSocketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
        perror("Error binding socket");
        close(serverSocketFd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocketFd, SOMAXCONN) == -1)
	{
        perror("Error listening for connections");
        close(serverSocketFd);
        exit(EXIT_FAILURE);
    }
	/*
	int listen(int sockfd, int backlog);

    backlong: an integer representing the number of connection requests allowed in the queue.
	Incoming connections will be placed in this queue until we accept them.
	*/

    // Now you can proceed with the rest of your server implementation
    std::cout << "Server initialized and listening on port 8080...\n";
	
	// while (true)
	// {
    //     // Accept a connection
    //     struct sockaddr_in clientAddr;
    //     socklen_t clientAddrLen = sizeof(clientAddr);
    //     int clientsocketFd = accept(serverSocketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    //     if (clientsocketFd == -1)
	// 	{
    //         perror("Error accepting connection");
    //         continue;  // Continue to the next iteration to accept another connection
	// 	}
	// }
	/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	*/


    // Set up pollfd structures for server socket and client sockets
    std::vector<pollfd> fds;
    fds.push_back({serverSocketFd, POLLIN, 0});

	while (true)
	{
        // Use poll to wait for events on the sockets
        if (poll(fds.data(), fds.size(), -1) == -1)
		{
            perror("Error in poll");
            break; // Exit the loop on poll error
        }

        // Check for events on the server socket
        if (fds[0].revents & POLLIN)
		{
            // Accept a connection
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientsocketFd = accept(serverSocketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
            if (clientsocketFd == -1)
			{
                perror("Error accepting connection");
            }
			else
			{
                std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";
                fds.push_back({clientsocketFd, POLLIN, 0}); // Add the new client socket to the pollfd vector
				// Send a simple "Hello" HTML page to the client
                std::string response = "HTTP/1.1 200 OK\r\n"
                                       "Content-Type: text/html\r\n"
                                       "Connection: close\r\n"
                                       "\r\n" +
                                       generateHtmlPage();
				 send(clientsocketFd, response.c_str(), response.size(), 0);

                // Close the client socket after sending the response
                close(clientsocketFd);
                fds.pop_back(); // Remove the closed client socket from the pollfd vector
            }	   
		    }
     }
		// Check for events on client sockets
        for (size_t i = 1; i < fds.size(); ++i)
		{
            if (fds[i].revents & POLLIN)
			{
                // Handle data on the client socket
                char buffer[1024];
                ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytesRead == -1)
				{
                    perror("Error reading from client socket");
                }
				else if (bytesRead == 0)
				{
                    std::cout << "Connection closed by client\n";
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i); // Remove the closed client socket from the pollfd vector
                }
				else
				{
                    // Process the received data
                    // ...

                    // Example: Echo back the received data
                    send(fds[i].fd, buffer, bytesRead, 0);
                }
            }
        }
    



	/*
	int poll(struct pollfd fds[], nfds_t nfds, int timeout);

	struct pollfd
	{
    int   fd;         // File descriptor
    short events;     // Requested events
    short revents;    // Detected events
	};

	fds: an array of pollfd structures, which contain the file descriptors to poll
	and the events for which we’d like to be notified. We will take a closer look at this structure below.
	nfds: an integer representing the number of elements in the previous array.
	timeout: an integer representing a temporal value in milliseconds,
	during which poll() will be able to block the execution of our program in order to poll the sockets.
	Once this deadline has been reached, poll() will terminate its execution. Setting this timeout to -1 allows poll()
	to block indefinitely as long as no file descriptor has changed states.
	On the other hand, if we put 0 here, poll() will immediately return,
	even if none of the polled file descriptors are ready.

	*/


	/*
	SEND AND RECIEVE
	ssize_t recv(int client_socket, void *buf, ssize_t len, int flags);
	ssize_t send(int client_socket, const void *buf, size_t len, int flags);
	*/

      // Close all client sockets when done
    for (size_t i = 1; i < fds.size(); ++i) {
        close(fds[i].fd);
    }

    // Close the server socket when done
    close(serverSocketFd);
    return 0;
}
