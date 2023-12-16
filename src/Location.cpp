/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:57:02 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/16 14:37:28 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Location.hpp"

Location::Location()
{
	//_allowed_methods = std::vector<int>(); //is this necessary?
	_root = "";
	//_index = ""; //is this necessary?
	_cgi = "";
	_autoindex = "";

}

Location::~Location() {}


/// -- SETTERS -- ///

void
Location::setPath(string path)
{
	_path = path;
}

void
Location::setAllowedMethods(std::vector<int> allowed_methods)
{
	_allowed_methods = allowed_methods;
}

void
Location::setRoot(string root)
{
	_root = root;
}

void
Location::setIndex(std::vector<string> index)
{
	_index = index;
}

void
Location::setCgi(string cgi)
{
	_cgi = cgi;
}

void
Location::setAutoindex(string autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else
		_autoindex = false;
}



/// -- GETTERS -- ///

string
Location::getPath()
{
	return (_path);
}

std::vector<int>
Location::getAllowedMethods()
{
	return (_allowed_methods);
}

string
Location::getRoot()
{
	return (_root);
}

std::vector<string>
Location::getIndex()
{
	return (_index);
}

string
Location::getCgi()
{
	return (_cgi);
}

bool
Location::getAutoindex()
{
	return (_autoindex);
}
