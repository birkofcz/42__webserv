/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:55:07 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/08 16:39:51 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Mime.hpp"

/* Constructor - loads the mime type data from a file */
Mime::Mime(const short &status_code, const bool &autoindex)
{
	string path = "./data/mime_types.csv";
	std::ifstream file(path.c_str());
	string line;
	if (file.fail())
	{
		Log::Msg(ERROR, FUNC + "Error opening file: " + path);
		exit(EXIT_FAILURE);
	}
	Log::Msg(DEBUG, FUNC + "before reading data file for MIME " + path);
	while (std::getline(file, line))
	{
		std::vector<string> split = CppSplit(line, ',');//split line by ',' and add to map
		_mimes[split[0]] = split[1];

	}
	file.close();

	_status_code = status_code;
	_autoindex = autoindex;
}

Mime::~Mime()
{
}

// METHODS // ------------------------------------------------------------------

/* Parses the extension from the "target_file" (path) */
string 
Mime::parseExtension(string path)
{
	std::vector<string> split = CppSplit(path, '.');
	if (split.size() > 1)
	{
		_extension = "." + split[split.size() - 1];
	}
	return "";
}

// GETTERS // ------------------------------------------------------------------

/* getMime - finds the mime type in the map using the file extension
and returns it. If no math 0- returns text/plain */
string
Mime::getMime() const
{

	if (_status_code != 200 || (_status_code == 200 && _autoindex == true))
	{
		Log::Msg(DEBUG, FUNC + "status code is not 200 or autoindex is true, setting text/html.");
		return "text/html";
	}
	else
	{
		for (std::map<string, string>::const_iterator it = _mimes.begin(); it != _mimes.end(); ++it)
		{
			if (it->first == _extension)
			{
				Log::Msg(DEBUG, FUNC + "recognized MIME: " + it->second);
				
				return it->second;
			}
		}
	}
	/* if no match, return text/plain - this is considered safest option,
	as it will be displayed as text in browser, but not executed. 
	It prevets browser from misinterpreting the content. But if the status 
	code is not 200 - so there is an error and we need the content-type to be set to text/html 
	to properly encode errorbody to html format. There is no risk as in case of error, there is no content 
	read, only error response body is internally constructed*/
	Log::Msg(DEBUG, FUNC + "no match, setting text/plain.");
	return "text/plain";
}

// TEST // ----------------------------------------------------------------------

/* testPrint - for debugging purposes - prints out the map of mime types 
and what mime was recognized */
void
Mime::testPrint(string path)
{	
	for (std::map<string, string>::const_iterator it = _mimes.begin(); it != _mimes.end(); ++it)
	{
		std::cout << it->first << " => " << it->second << '\n';
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
	cout << "Mime test:" << endl;
	cout << "Path: " << path << endl;
	parseExtension(path);
	cout << "Extension: " << _extension << endl;
	cout << "Recognized MIME: " << getMime() << endl;
}


