/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:58:07 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/03 16:22:35 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"
#include "../include/Parser.hpp"

Parser::Parser(string path)
{
	_path = path;
}

Parser::~Parser()
{
}


//////// --- METHODS --- ////////

/* Get port number from a line */
std::vector<int> 
Parser::parsePorts(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	std::vector<int> port;
	if (split[1].empty())
	{
		print("getPort: port not specified", RED, 2);
		return port;
	}
	//cleans the string(s) from the semicolon and pushes them to the vector
	for (size_t i = 1; i < split.size(); i++)
	{
		if (split[i].find(';') != string::npos)
			split[i].erase(split[i].find(';'));
		port.push_back(atoi(split[i].c_str()));
	}
	return port;
}

/* Get server names from a line */
std::vector<string> 
Parser::parseServerNames(const string& line)
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
string
Parser::parseRoot(const string& line)
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
std::vector<string> 
Parser::parseIndex(const string& line)
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

// Parser for a config file
/* void 
Parser::parseFile(const string& path)
{
	std::ifstream file(path.c_str());
	string line;

	if (!file.good())
	{
		print("Error opening file", RED, 2);
		file.close();
		return;
	}
	int server_count = 0;
	while(std::getline(file, line))
	{
		if (line[0] == '#' || line.empty()) //skips comments and empty lines
			continue;
		else if (line.find("server") != string::npos)
		{
			print("config: Found server", GREEN);
			server_count++;
			Server server;
			// cout << "server_count: " << server_count << endl;
			std::stringstream ss;
			ss << "server" << server_count;
			server.setName(ss.str());
			_servers.push_back(server);
		}
		else if (line.find("listen") != string::npos)
		{
			print("config: Found listen directive", GREEN);
			std::vector<int> ports = parsePorts(line);
			// for (size_t i = 0; i < ports.size(); i++)
			// 	cout << (i+1) << ": " << ports[i] << " ";
			if (!_servers.empty()) {
            _servers.back().setPorts(ports);
        }
		}
 		else if (line.find("server_name") != string::npos)
		{
			print("config: Found server_name directive", GREEN);
			std::vector<string> server_names = parseServerNames(line);
			cout << "server_names: ";
			// for (size_t i = 0; i < server_names.size(); i++)
			// 	cout << (i+1) << ": " << server_names[i] << " ";
			// cout << endl;
			if (!_servers.empty()) {
			_servers.back().setServerNames(server_names);
		}
		}
		else if (line.find("root") != string::npos)
		{
			print("config: Found root directive", GREEN);
			string root = parseRoot(line);
			// cout << "root: " << root << endl;
			if (!_servers.empty())
			_servers.back().setRoot(root);
		}
		else if (line.find("index") != string::npos)
		{
			print("Found index directive", GREEN);
			std::vector<string> index = parseIndex(line);
			cout << "index: ";
			// for (size_t i = 0; i < index.size(); i++)
			// 	cout << (i+1) << ": " << index[i] << " ";
			// cout << endl;
			if (!_servers.empty())
				_servers.back().setIndex(index);
		}
		else
		{
			print("config: Unknown directive", RED, 2);
			print(line, RED, 2);
		}

	}
	file.close();
} */

//working version using bool for in-server block indication
void
Parser::parseFile(const string& path) 
{
	std::ifstream file(path.c_str());
	string line;

	if (!file.good())
	{
		print("Error opening file", RED, 2);
		file.close();
		return;
	}

	int server_count = 0;
	bool inServerBlock = false;
	Server currentServer;
	std::cout << YELLOW << "\n[PARSING FILE ... ]" << RESET << std::endl << std::endl;

	while(std::getline(file, line))
	{
		// Trim leading white spaces
		line.erase(0, line.find_first_not_of(" \t"));

		if (line.empty() || line[0] == '#') // Skips comments and empty lines
			continue;

		// Start of a server block
		if (line.find("server {") != string::npos)
		{
			inServerBlock = true;
			server_count++;
			std::stringstream ss;
			ss << "server" << server_count;
			currentServer.setName(ss.str());
			continue;
		}

		// End of a server block
		if (line.find("}") != string::npos && inServerBlock)
		{
			inServerBlock = false;
			_servers.push_back(currentServer);
			currentServer = Server(); // Reset for the next server block
			continue;
		}

		// Process directives within a server block
		if (inServerBlock)
		{
			if (line.find("listen") != string::npos)
			{
				print("config: Found listen directive", GREEN);
				std::vector<int> ports = parsePorts(line);
				currentServer.setPorts(ports);
			}
			else if (line.find("server_name") != string::npos)
			{
				print("config: Found server_name directive", GREEN);
				std::vector<string> server_names = parseServerNames(line);
				currentServer.setServerNames(server_names);
			}
			else if (line.find("root") != string::npos)
			{
				print("config: Found root directive", GREEN);
				string root = parseRoot(line);
				currentServer.setRoot(root);
			}
			else if (line.find("index") != string::npos)
			{
				print("config: Found index directive", GREEN);
				std::vector<string> index = parseIndex(line);
				currentServer.setIndex(index);
			}
			else
			{
				print("config: Unknown directive inside server block", RED, 2);
				print(line, RED, 2);
			}
        } 
		else 
		{
			print("config: Directive outside server block", RED, 2);
			print(line, RED, 2);
		}
	}
	file.close();
}


//////// --- GETTERS --- ////////
std::vector<Server>
Parser::getServers() const
{
	return _servers;
}

//////// --- PRINTERS --- ////////
void
Parser::printServers()
{
	std::cout << std::endl;
	std::cout << YELLOW << "[ CONFIGURATION SUMMARY ]" << RESET << std::endl;
	std::cout << std::endl;
	std::cout << "(std::vector<Server> _servers) SIZE: " << _servers.size() << std::endl << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		cout << "Name: " << _servers[i].getName() << endl;
		cout << "Ports: ";
		for (size_t j = 0; j < _servers[i].getPorts().size(); j++)
			cout << _servers[i].getPorts()[j] << " ";
		cout << endl;
		cout << "Server names: ";
		for (size_t j = 0; j < _servers[i].getServerNames().size(); j++)
			cout << _servers[i].getServerNames()[j] << " ";
		cout << endl;
		cout << "Root: " << _servers[i].getRoot() << endl;
		cout << "Index: ";
		for (size_t j = 0; j < _servers[i].getIndex().size(); j++)
			cout << _servers[i].getIndex()[j] << " ";
		cout << endl;
		cout << endl;
		cout << "-------------------------------------" << endl;
		cout << endl;

	}
}