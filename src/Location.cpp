/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:57:02 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/08 16:45:15 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Location.hpp"

/* Constructor with defaut values initialized */
Location::Location()
{
	_path = "";
	_allowed_methods = std::vector<int>();
	_root = "";
	_index = std::vector<string>();
	_autoindex = false;
	_error_pages = std::map<int, string>();
	_client_max_body_size = CLIENT_MAX_BODY_SIZE_LIMIT;
	_cgi_map = std::map<string, string>();
	_upload_path = "";

}

Location::~Location() {}


/// -- SETTERS -- /// ---------------------------------------------------------------------

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
Location::setCgi(std::map<string, string> cgi)
{
	_cgi_map = cgi;
}

void
Location::setAutoindex(string autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else
		_autoindex = false;
}

void
Location::setErrorPages(std::map<int, string> error_pages)
{
	_error_pages = error_pages;
}

void
Location::setClientMaxBodySize(size_t client_max_body_size)
{
	_client_max_body_size = client_max_body_size;
}

void
Location::setUploadPath(string upload_path)
{
	_upload_path = upload_path;
}




/// -- GETTERS -- /// ---------------------------------------------------------------------

string
Location::getPath() const
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

std::map<string, string>
Location::getCgi()
{
	return (_cgi_map);
}

bool
Location::getAutoindex()
{
	return (_autoindex);
}

std::map<int, string>
Location::getErrorPages()
{
	return (_error_pages);
}

size_t
Location::getClientMaxBodySize()
{
	return (_client_max_body_size);
}

string
Location::getUploadPath()
{
	return (_upload_path);
}


string	Location::getCgiPath(string key)
{
	for (std::map<std::string, std::string>::iterator it = _cgi_map.begin(); it != _cgi_map.end(); ++it)
	{
		if (it->first == key)
		{
			return it->second; // Return the value associated with the key
		}
	}

	// std::map<std::string, std::string>::iterator it = _cgi_map.find(key);
    // if (it != _cgi_map.end())
    // {
    //     return it->second; // Return the value associated with the key
    // }
	// If key is not found, you might want to return a default value or handle it appropriately
	return "";
};