/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:34:46 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/29 17:26:11 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* Basic header file with basic level definitions - 
I would include this in all adjacent code */

#ifndef GENERAL_HPP
# define GENERAL_HPP

#include <iostream>
#include <string>
#include <sstream>

#include <fstream>
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

//namespace snippets to use
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

enum HttpMethod
{
	GET,
	POST,
	DELETE,
	// PUT,
	HEAD,
	NONE
};

template <typename T>
std::string toString(const T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

/* Standalone utilities */

void print(string message, string color = NONECOLOR, int output = 1); //print with optional arguments set to default values
void parseFile(const string& path); //parse file with path
std::vector<string>	CppSplit(std::string str, char delimiter);
bool isNumeric(string str);


#endif