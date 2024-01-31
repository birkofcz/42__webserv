/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 16:20:36 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/30 16:26:09 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Error.hpp"

/* 
buildErrorPage
This function is responsible for building error page. It will check if there 
is custom error page defined in location, if not, it will check if there is a custom error page
defined in server, if not, it will build [TS]erver default error page based on error code.*/
string
Error::buildErrorPage(short error_code, string location_key,  Server &server)
{

	Log::Msg(DEBUG, FUNC + "location_key: " + location_key);
	Log::Msg(DEBUG, FUNC + "error_code: " + toString(error_code));
	
	string l_dep_path(""); 										//location error page path
	string s_dep_path("");										//server error page path
	std::vector<Location> locations = server.getLocations();	//locations on server
	Location *location = NULL;									//location to be used
	std::map<int, string> loc_error_pages;						//location error pages
	std::map<int, string> ser_error_pages;						//server error pages

	//check if location exists
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == location_key)
		{
			location = &(*it);
			Log::Msg(DEBUG, FUNC + "location found on a server");
			break;
		}
	}

	//if location exists, check if error page exists in location, if not, check if is defined in server
	if (location)
	{
		loc_error_pages = location->getErrorPages();
		if (!loc_error_pages.empty() && loc_error_pages.find(error_code) != loc_error_pages.end())
		{
			l_dep_path = loc_error_pages[error_code];
			if (l_dep_path[0] == '/')
				l_dep_path = l_dep_path.substr(1);
			Log::Msg(DEBUG, FUNC + "location error page: " + l_dep_path);
		}
		else
		{
			Log::Msg(DEBUG, FUNC + "location found, location error pages not found - > switching to server check");
			ser_error_pages = server.getErrorPages();
			if (!ser_error_pages.empty() && ser_error_pages.find(error_code) != ser_error_pages.end())
			{
				s_dep_path = ser_error_pages[error_code];
				if (s_dep_path[0] == '/')
					s_dep_path = s_dep_path.substr(1);
				Log::Msg(DEBUG, FUNC + "server error page: " + s_dep_path);
			}
			else
				Log::Msg(DEBUG, FUNC + "server error pages not found - > switching to [TS]erver default error page");
		}
	}
	//if location does not exist, check if there is error page defined in server
	else
	{
		Log::Msg(DEBUG, FUNC + "location not found -> switching to server check");
		ser_error_pages = server.getErrorPages();
		if (!ser_error_pages.empty() && ser_error_pages.find(error_code) != ser_error_pages.end())
		{
			s_dep_path = ser_error_pages[error_code];
			if (s_dep_path[0] == '/')
				s_dep_path = s_dep_path.substr(1);
			Log::Msg(DEBUG, FUNC + "server error page: " + s_dep_path);
		}
		//if there is no error page defined in server, opt for default error page
		else
			Log::Msg(DEBUG, FUNC + "server error pages not found - > switching to [TS]erver default error page");
			
	}



	/* 	This part is checking if there is path defined for error page in location or server, if not, 
	it will build default error page */
	if (!l_dep_path.empty())
	{
		Log::Msg(DEBUG, FUNC + "inside location error page reader");

		//open file on the path, read it and write it to the string to return
		std::ifstream file(l_dep_path.c_str());
		string line;
		if (file.fail())
		{
			std::cerr << "Error opening file for location error pages: " << l_dep_path << std::endl;
			return ("");
		}
		Log::Msg(DEBUG, FUNC + "file opened");
		string default_error_page("");
		while (std::getline(file, line))
		{
			default_error_page += line;
			default_error_page += "\r\n";
		}
		file.close();

		Log::Msg(DEBUG, FUNC + "file closed, response build");
		
		return (default_error_page);
	}
	else if (l_dep_path.empty() && !s_dep_path.empty())
	{
		Log::Msg(DEBUG, FUNC + "inside server error page reader");
		//open file on the path, read it and write it to the string to return
		std::ifstream file(s_dep_path.c_str());
		string line;
		if (file.fail())
		{
			std::cerr << "Error opening file for server error pages: " << s_dep_path << std::endl;
			return ("");
		}
		Log::Msg(DEBUG, FUNC + "file opened");
		string default_error_page("");
		while (std::getline(file, line) != NULL)
		{
			default_error_page += line;
			default_error_page += "\r\n";
		}
		file.close();
		
		Log::Msg(DEBUG, FUNC + "file closed, response build");
		
		return (default_error_page);
	}
	else if (s_dep_path.empty() && s_dep_path.empty())
	{
		Log::Msg(DEBUG, FUNC + "inside [TS]erver default error page creator");
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

		Log::Msg(DEBUG, FUNC + "[TS]erver default error page created");
		return error_page;
	}
	return ("");
}

/* getErrorDescription
This function is responsible for returning error description based on error code.
It is used mostly to fill header in response with error description.*/
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

/* instantErrorPage
This function is responsible for building instant error page. It is used for quick error page build, without
checking if there is custom error page defined in location or server.*/

string
Error::instantErrorPage(short error_code)
{
	Log::Msg(DEBUG, FUNC + "Creating instant error page");
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

		error_page += "HTTP/1.1 ";
		error_page += ecode;
		error_page += " ";
		error_page += description;
		error_page += "\r\n";
		error_page += "Content-Type: text/html\r\n";
		error_page += "Content-Length: 165\r\n";
		error_page += "\r\n";
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

		Log::Msg(DEBUG, FUNC + "Instant error page created");
		return error_page;
}
