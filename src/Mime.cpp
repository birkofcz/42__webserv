/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:55:07 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/30 11:33:04 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Mime.hpp"

/* Constructor - loads the mime type data from a file */
Mime::Mime()
{
	string path = "./data/mime_types.csv";
	std::ifstream file(path.c_str());
	string line;
	if (file.fail())
	{
		std::cerr << "Error opening file: " << path << std::endl;
		exit(EXIT_FAILURE);
	}
	cout << "Testprint here before reading file" << endl;
	while (std::getline(file, line))
	{
		std::vector<string> split = CppSplit(line, ',');//split line by ',' and add to map
		_mimes[split[0]] = split[1];

	}
	file.close();
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
	for (std::map<string, string>::const_iterator it = _mimes.begin(); it != _mimes.end(); ++it)
	{
		if (it->first == _extension)
			return it->second;
	}
	/* if no match, return text/plain - this is considered safest option,
	as it will be displayed as text in browser, but not executed. 
	It prevets browser from misinterpreting the content*/
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


