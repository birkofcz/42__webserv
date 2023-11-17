/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:34:46 by sbenes            #+#    #+#             */
/*   Updated: 2023/11/17 11:58:33 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* Basic header file with basic level definitions - 
I would include this in all adjacent code */

#ifndef GENERAL_HPP
# define GENERAL_HPP

#include <iostream>
#include <string>
// containers and webstuff

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
#define RESET	"\033[0m"
#define NONE	""

/* Standalone utilities */

void print(string message, string color = NONE, int output = 1); //print with optional arguments set to default values

#endif