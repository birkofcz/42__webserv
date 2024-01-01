/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 14:35:12 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/01 14:53:06 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

using std::string;

/* string _buildAutoindex(string &path)
{
	struct dirent	*entry;
	DIR 			*directory;	
	string			auto_index("");

	directory = opendir(path.c_str());
	if (directory == NULL)
	{
		std::cerr << "Error opening directory for autoindex" << std::endl;
		return ("");
	}
	auto_index += "<html><body><h1>Directory Listing</h1><ul>";

	
	while ((entry = readdir(directory)) != NULL)
	{
		string dir_name = entry->d_name; //must convert to string
		if (dir_name == "." || dir_name == "..")
			continue ;
		auto_index += "<a href=\"" + dir_name + "\">" + dir_name + "</a><br>";
	}
	auto_index += "</ul></body></html>";
	closedir(directory);
	return (auto_index);
} */


#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string _buildAutoindex(string &path)
{
    struct dirent *entry;
    DIR *directory;
    struct stat fileInfo;
    string auto_index("");
    vector<string> dirList, fileList;

    directory = opendir(path.c_str());
    if (directory == NULL)
    {
        cerr << "Error opening directory for autoindex" << endl;
        return "";
    }

    while ((entry = readdir(directory)) != NULL)
    {
        string dir_name = entry->d_name;
        if (dir_name == "." || dir_name == "..")
            continue;

        string fullPath = path + "/" + dir_name;
        if (stat(fullPath.c_str(), &fileInfo) != 0)
        {
            cerr << "Error getting file info for " << dir_name << endl;
            continue;
        }

        if (S_ISDIR(fileInfo.st_mode))
        {
            dirList.push_back("<a href=\"" + dir_name + "/\">" + dir_name + "/</a><br>");
        }
        else
        {
            fileList.push_back("<a href=\"" + dir_name + "\">" + dir_name + "</a><br>");
        }
    }
    closedir(directory);

    auto_index += "<html><body><h1>Directory Listing</h1><ul>";
	for (vector<string>::const_iterator it = dirList.begin(); it != dirList.end(); ++it)
    	auto_index += *it;
	for (vector<string>::const_iterator it = fileList.begin(); it != fileList.end(); ++it)
    	auto_index += *it;

    auto_index += "</ul></body></html>";

    return auto_index;
}

int main()
{
	string path = "../../0_tests";
	string auto_index = _buildAutoindex(path);

	//write into html file//
	std::ofstream file;
	file.open("autoindex.html");
	file << auto_index;
	file.close();
	return (0);
}