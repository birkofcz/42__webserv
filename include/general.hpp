/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:34:46 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/06 16:18:26 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* Basic header file with basic level definitions - 
I would include this in all adjacent code */

#ifndef GENERAL_HPP
# define GENERAL_HPP

//basic includes
#include <iostream>
#include <string>
#include <sstream>

#include <fstream>
#include <istream>

//networking
#include <arpa/inet.h> // for inet_addr	

//signals
#include <csignal>

//threads
#include <pthread.h>

//file system - opendir, readdir, closedir
#include <dirent.h>

// containers and webstuff

#include <vector>
#include <stack>
#include <map>

#include <cstdlib>

//sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/*
namespace snippets to use - to avoid writing std:: all the time, 
and is a good practice in C++ because of the possible name collisions
*/
using std::cout;
using std::cerr;
using std::endl;
using std::string;

//colors
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define BOLD	"\033[1m"
#define RESET	"\033[0m"

#define NONECOLOR	""

/* 
function identifier - this is used to print the function name in the debug messages,
so that we know where the message is coming from. __FUNCTION__ is a macro that is
replaced by the name of the function where it is used. It is unconfortable to write
the whole thing every time, so we use this shortened macro. 
*/
#define FUNC "[" + toString(__FUNCTION__) + "]: "

//Logging
#include "Log.hpp"

//default client max body size limit - based on 10 megabytes limit used by NGINX
#define CLIENT_MAX_BODY_SIZE_LIMIT 10485760

//templates
/* toString - converts whatever to string:) */
template <typename T>
string toString(const T& value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

//enums
enum HttpMethod
{
	GET,
	POST,
	DELETE,
	// PUT,
	HEAD,
	NONE
};

# ifndef GLOBAL_VARIABLE_H
#  define GLOBAL_VARIABLE_H
extern bool terminateFlag; // Declare global variable
# endif


/* 
Standalone utilities 
*/

void print(string message, string color = NONECOLOR, int output = 1); //print with optional arguments set to default values
void parseFile(const string& path); //parse file with path
std::vector<string>	CppSplit(std::string str, char delimiter); //split string on delimiter, puts strings in vector
bool isNumeric(string str);	//checks if input is numeric


#endif