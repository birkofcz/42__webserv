/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:58:07 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/02 15:58:13 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"

class ConfigParser
{
	private:
		int		port;
		string	server_name;
		string	error_page;
		string	location;
		string	root;
		string	index;
		bool	autoindex;
		int		client_max_body_size;
		string	cgi;
		string	upload;
		string	auth;
		string	methods;
		string	redirect;
		string	return_;

	public:
		ConfigParser(const string& path);
		~ConfigParser();

		void	parseFile(const string& path);
};


//////// --- METHODS --- ////////

/* Get port number from a line */
int getPort(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	if (split[1].empty())
	{
		print("getPort: port not specified", RED, 2);
		return -1;
	}
	if (split[1].find(';') != string::npos)
		split[1].erase(split[1].find(';'));
	for (size_t i = 0; i < split[1].size(); i++)
	{
		if (!isdigit(split[1][i]))
		{
			print("getPort: port not specified", RED, 2);
			return -1;
		}
	}
	return (atoi(split[1].c_str()));
}

/* Get server names from a line */
std::vector<string> getServerNames(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	std::vector<string> server_names;
	if (split[1].empty())
	{
		print("getServerNames: server name not specified", RED, 2);
		return server_names;
	}
	//cleans the string(s) from the semicolon and pushes them to the vector
	for (size_t i = 1; i < split.size(); i++)
	{
		if (split[i].find(';') != string::npos)
			split[i].erase(split[i].find(';'));
		server_names.push_back(split[i]);
	}
	return server_names;
}

/* Get root from a line */
string getRoot(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	if (split[1].empty())
	{
		print("getRoot: root not specified", RED, 2);
		return "";
	}
	if (split[1].find(';') != string::npos)
		split[1].erase(split[1].find(';'));
	return split[1];
}

/* Get index from a line */
std::vector<string> getIndex(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	std::vector<string> index;
	if (split[1].empty())
	{
		print("getIndex: index not specified", RED, 2);
		return index;
	}
	//cleans the string(s) from the semicolon and pushes them to the vector
	for (size_t i = 1; i < split.size(); i++)
	{
		if (split[i].find(';') != string::npos)
			split[i].erase(split[i].find(';'));
		index.push_back(split[i]);
	}
	return index;
}


void parseFile(const string& path)
{
	std::ifstream file(path.c_str());
	string line;

	if (!file.good())
	{
		print("Error opening file", RED, 2);
		file.close();
		return;
	}
	//testprint
	/* while (std::getline(file, line))
			std::cout << line << std::endl; */

	while(std::getline(file, line))
	{
		if (line[0] == '#' || line.empty()) //skips comments and empty lines
			continue;
		else if (line.find("server") != string::npos)
		{
			print("config: Found server directive", GREEN);
			//create new server object inside the vector and then passes 
			//the rest of the data to it. If one alreadz exists, create a new one
		}
		else if (line.find("listen") != string::npos)
		{
			print("config: Found listen directive", GREEN);
			int port = getPort(line);
			//if (port != -1) - here we check and write the listen port to the structure?
			// control print
			cout << "port: " << port << endl;
		}
 		else if (line.find("server_name") != string::npos)
		{
			print("config: Found server_name directive", GREEN);
			std::vector<string> server_names = getServerNames(line);
			cout << "server_names: ";
			for (size_t i = 0; i < server_names.size(); i++)
				cout << (i+1) << ": " << server_names[i] << " ";
			cout << endl;
		}
		else if (line.find("root") != string::npos)
		{
			print("config: Found root directive", GREEN);
			string root = getRoot(line);
			cout << "root: " << root << endl;
		}
		else if (line.find("index") != string::npos)
		{
			print("Found index directive", GREEN);
			std::vector<string> index = getIndex(line);
			cout << "index: ";
			for (size_t i = 0; i < index.size(); i++)
				cout << (i+1) << ": " << index[i] << " ";
			cout << endl;
		}

/*
		else if (line.find("error_page") != string::npos)
		{
			print("Found error_page directive", GREEN);
			//parse error_page directive
		}
		else if (line.find("location") != string::npos)
		{
			print("Found location directive", GREEN);
			//parse location directive
		}
		else if (line.find("autoindex") != string::npos)
		{
			print("Found autoindex directive", GREEN);
			//parse autoindex directive
		}
		else if (line.find("client_max_body_size") != string::npos)
		{
			print("Found client_max_body_size directive", GREEN);
			//parse client_max_body_size directive
		}
		else if (line.find("cgi") != string::npos)
		{
			print("Found cgi directive", GREEN);
			//parse cgi directive
		}
		else if (line.find("upload") != string::npos)
		{
			print("Found upload directive", GREEN);
			//parse upload directive
		}
		else if (line.find("auth") != string::npos)
		{
			print("Found auth directive", GREEN);
			//parse auth directive
		}
		else if (line.find("methods") != string::npos)
		{
			print("Found methods directive", GREEN);
			//parse methods directive
		}
		else if (line.find("redirect") != string::npos)
		{
			print("Found redirect directive", GREEN);
			//parse redirect directive
		}
		else if (line.find("return") != string::npos)
		{
			print("Found return directive", GREEN);
			//parse */
		//}
	}
	file.close();
}
