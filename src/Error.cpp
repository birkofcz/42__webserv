/* ************************************************************************** */
/*		                                                                    */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 16:20:36 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/01 16:41:00 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Error.hpp"

string
Error::buildErrorPage(short error_code, string location_key,  Server &server, Location &location)
{
	//check for error page in location and server
	string l_dep_path("");
	string s_dep_path("");

	cout << YELLOW << "Error::buildErrorPage" << RESET << endl;
	cout << YELLOW << "\tlocation_key: " << location_key << endl;


	if (location_key != "")
	{
		if (location.getErrorPages().find(error_code) != location.getErrorPages().end())
		{
			l_dep_path = location.getErrorPages()[error_code];
			if (l_dep_path[0] == '/')
				l_dep_path = l_dep_path.substr(1); 
			cout << YELLOW << "\tlocation error page: " << l_dep_path << RESET << endl;
		}
	}
	else if (location_key == "")
	{
		if (server.getErrorPages().find(error_code) != server.getErrorPages().end())
		{
			s_dep_path = server.getErrorPages()[error_code];
			if (s_dep_path[0] == '/')
				s_dep_path = s_dep_path.substr(1);
			cout << YELLOW << "\tserver error page: " << s_dep_path << RESET << endl;
		}
	}
	if (l_dep_path != "")
	{
		cout << YELLOW << "\tinside location error page reader " << RESET << endl;
		//open file on the path, read it and write it to the string to return
		std::ifstream file(l_dep_path.c_str());
		string line;
		if (file.fail())
		{
			std::cerr << "Error opening file for location error pages: " << l_dep_path << std::endl;
			return ("");
		}
		cout << YELLOW << "\tfile opened" << RESET << endl;
		string default_error_page("");
		while (std::getline(file, line))
		{
			default_error_page += line;
			default_error_page += "\r\n";
		}
		file.close();
		cout << YELLOW << "\tfile closed and response build" << RESET << endl;
		return (default_error_page);
	}
	else if (l_dep_path == "" && s_dep_path != "")
	{
		cout << YELLOW << "\tinside server error page reader " << RESET << endl;
		//open file on the path, read it and write it to the string to return
		std::ifstream file(s_dep_path.c_str());
		string line;
		if (file.fail())
		{
			std::cerr << "Error opening file for server error pages: " << s_dep_path << std::endl;
			return ("");
		}
		cout << YELLOW << "\tfile opened" << RESET << endl;
		string default_error_page("");
		while (std::getline(file, line) != NULL)
		{
			default_error_page += line;
			default_error_page += "\r\n";
		}
		file.close();
		cout << YELLOW << "\tfile closed and response build" << RESET << endl;
		return (default_error_page);
	}
	else if (l_dep_path == "" && s_dep_path == "")
	{
		cout << YELLOW << "\tinside default error page creator " << RESET << endl;
		//Build default error page
		std::stringstream ss;
		ss << error_code;
		string ecode = ss.str();
		string description("");
		string error_page("");

		std::map<short, string> error_description;

		error_description[100] = "Continue";
		error_description[101] = "Switching Protocol";
		error_description[200] = "OK";
		error_description[201] = "Created";
		error_description[202] = "Accepted";
		error_description[203] = "Non-Authoritative Information";
		error_description[204] = "No Content";
		error_description[205] = "Reset Content";
		error_description[206] = "Partial Content";
		error_description[300] = "Multiple Choice";
		error_description[301] = "Moved Permanently";
		error_description[302] = "Moved Temporarily";
		error_description[303] = "See Other";
		error_description[304] = "Not Modified";
		error_description[307] = "Temporary Redirect";
		error_description[308] = "Permanent Redirect";
		error_description[400] = "Bad Request";
		error_description[401] = "Unauthorized";
		error_description[403] = "Forbidden";
		error_description[404] = "Not Found";
		error_description[405] = "Method Not Allowed";
		error_description[406] = "Not Acceptable";
		error_description[407] = "Proxy Authentication Required";
		error_description[408] = "Request Timeout";
		error_description[409] = "Conflict";
		error_description[410] = "Gone";
		error_description[411] = "Length Required";
		error_description[412] = "Precondition Failed";
		error_description[413] = "Payload Too Large";
		error_description[414] = "URI Too Long";
		error_description[415] = "Unsupported Media Type";
		error_description[416] = "Requested Range Not Satisfiable";
		error_description[417] = "Expectation Failed";
		error_description[418] = "I'm a teapot";
		error_description[421] = "Misdirected Request";
		error_description[425] = "Too Early";
		error_description[426] = "Upgrade Required";
		error_description[428] = "Precondition Required";
		error_description[429] = "Too Many Requests";
		error_description[431] = "Request Header Fields Too Large";
		error_description[451] = "Unavailable for Legal Reasons";
		error_description[500] = "Internal Server Error";
		error_description[501] = "Not Implemented";
		error_description[502] = "Bad Gateway";
		error_description[503] = "Service Unavailable";
		error_description[504] = "Gateway Timeout";
		error_description[505] = "HTTP Version Not Supported";

		if (error_description.find(error_code) == error_description.end())
			description = "Undefined";
		else
			description = error_description[error_code];

		error_page += "<html>\r\n<head><title>";
		error_page += ecode;
		error_page += " ";
		error_page += description;
		error_page += " </title></head>\r\n";
		error_page += "<body>\r\n";
		error_page += "<center><h1>";
		error_page += ecode;
		error_page += " ";
		error_page += description;
		error_page += "</h1></center>\r\n";

		cout << YELLOW << "\tdefault error page created" << RESET << endl;
		return error_page;
	}
	return ("");
}

string
Error::getErrorDescription(short error_code)
{
	string description("");
	std::map<short, string> error_description;

	error_description[100] = "Continue";
	error_description[101] = "Switching Protocol";
	error_description[200] = "OK";
	error_description[201] = "Created";
	error_description[202] = "Accepted";
	error_description[203] = "Non-Authoritative Information";
	error_description[204] = "No Content";
	error_description[205] = "Reset Content";
	error_description[206] = "Partial Content";
	error_description[300] = "Multiple Choice";
	error_description[301] = "Moved Permanently";
	error_description[302] = "Moved Temporarily";
	error_description[303] = "See Other";
	error_description[304] = "Not Modified";
	error_description[307] = "Temporary Redirect";
	error_description[308] = "Permanent Redirect";
	error_description[400] = "Bad Request";
	error_description[401] = "Unauthorized";
	error_description[403] = "Forbidden";
	error_description[404] = "Not Found";
	error_description[405] = "Method Not Allowed";
	error_description[406] = "Not Acceptable";
	error_description[407] = "Proxy Authentication Required";
	error_description[408] = "Request Timeout";
	error_description[409] = "Conflict";
	error_description[410] = "Gone";
	error_description[411] = "Length Required";
	error_description[412] = "Precondition Failed";
	error_description[413] = "Payload Too Large";
	error_description[414] = "URI Too Long";
	error_description[415] = "Unsupported Media Type";
	error_description[416] = "Requested Range Not Satisfiable";
	error_description[417] = "Expectation Failed";
	error_description[418] = "I'm a teapot";
	error_description[421] = "Misdirected Request";
	error_description[425] = "Too Early";
	error_description[426] = "Upgrade Required";
	error_description[428] = "Precondition Required";
	error_description[429] = "Too Many Requests";
	error_description[431] = "Request Header Fields Too Large";
	error_description[451] = "Unavailable for Legal Reasons";
	error_description[500] = "Internal Server Error";
	error_description[501] = "Not Implemented";
	error_description[502] = "Bad Gateway";
	error_description[503] = "Service Unavailable";
	error_description[504] = "Gateway Timeout";
	error_description[505] = "HTTP Version Not Supported";

	if (error_description.find(error_code) == error_description.end())
		description = "Undefined";
	else
		description = error_description[error_code];

	return description;
}
