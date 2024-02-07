/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:58:07 by sbenes            #+#    #+#             */
/*   Updated: 2024/02/06 16:45:18 by tkajanek         ###   ########.fr       */
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


//////// --- METHODS - parsing the lines one by one helper parsers (2. level)--- ////////

/* Get port numbers from a line */
int
Parser::parsePort(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	//int port = -1;
	if (split[1].empty() || split[1] == ";")
	{
		//print("getPort: port not specified", RED, 2);
		return (-1);
	}
	//cleans the string(s) from the semicolon and pushes them to the vector
	if (split[1].find(';') != string::npos)
		split[1].erase(split[1].find(';'));
	///test if everything is number
	if (isNumeric(split[1]))
		return atoi(split[1].c_str());
	else
	{
		//print("getPort: port not numeric", RED, 2);
		return (-1);
	}
}

/* Helper function to check the formal validity of IPv4 address format - uses inet_pton designed
for the occasion */
static int checkIPv4(const string& ip)
{
	struct sockaddr_in sa;
	return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
}

/* Get a host name in in_addr_t format. Using above helper function to check if the ip address 
is in a right format */
in_addr_t 
Parser::parseHost(const std::string& line) 
{
	std::vector<std::string> split = CppSplit(line, ' ');

	if (split.size() < 2) 
	{
		Log::Msg(DEBUG, FUNC + "Host not specified.");
		//print("parseHost: Invalid configuration line. Host not specified.", RED, 2);
		return INADDR_NONE; // INDDR_NONE is -1, indicating an error
	}

	std::string host = split[1];
	//check the format of the host
	if (host.find(';') != std::string::npos)
		host.erase(host.find(';'));
	Log::Msg(DEBUG, FUNC + "Parsed host = " + host);

	//check if the host is localhost
	if (host == "localhost")
		host = "127.0.0.1";
		
	//check if the host is an ip address
	if (checkIPv4(host) == 1)
	{	
		Log::Msg(DEBUG, FUNC + "inet_pton: Host is an ip address");	
		return inet_addr(host.c_str());
	}
	else
	{
		Log::Msg(DEBUG, FUNC + "inet_pton: Host is not an ip address");
		//print("parseHost: Invalid IP address or hostname.", RED, 2);
		return INADDR_NONE;
	}
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

/* Get location path from a line */
string	
Parser::parseLocationPath(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	if (split[1].empty())
	{
		print("getLocationPath: path not specified", RED, 2);
		return "";
	}
	return split[1];
}

/* Get allowed methods from a line */
std::vector<int>
Parser::parseAllowedMethods(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	std::vector<int> allowed_methods;
	if (split[1].empty())
	{
		print("parseAllowedMethods: allowed methods not specified", RED, 2);
		return allowed_methods;
	}
	//cleans the string(s) from the semicolon and pushes them to the vector
	for (size_t i = 1; i < split.size(); i++)
	{
		if (split[i].find(';') != string::npos)
			split[i].erase(split[i].find(';'));
		if (split[i] == "GET")
			allowed_methods.push_back(0);
		else if (split[i] == "POST")
			allowed_methods.push_back(1);
		else if (split[i] == "DELETE")
			allowed_methods.push_back(2);
		else if (split[i] == "NONE")
			allowed_methods.push_back(3);
		else
		{
			print("parseAllowedMethods: unknown method", RED, 2);
			return allowed_methods;
		}
	}
	return allowed_methods;
}

/* Get error pages from a line - in format ex. 404 path 503 path;*/
std::map<int, string>
Parser::parseErrorPages(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	std::map<int, string> error_pages;
	if (split[1].empty())
	{
		print("getErrorPages: error pages not specified", RED, 2);
		return error_pages;
	}
	//cleans the string(s) from the semicolon and pushes them to the map
	for (size_t i = 1; i < split.size(); i++)
	{
		if (split[i].find(';') != string::npos)
			split[i].erase(split[i].find(';'));
		if (isNumeric(split[i]))
		{
			int error_code = atoi(split[i].c_str());
			if (error_code < 100 || error_code > 599)
			{
				print("getErrorPages: error code out of range", RED, 2);
				return error_pages;
			}
			if (split[i + 1].empty())
			{
				print("getErrorPages: error page path not specified", RED, 2);
				return error_pages;
			}
			if (split[i + 1].find(';') != string::npos)
				split[i + 1].erase(split[i + 1].find(';'));
			error_pages[error_code] = split[i + 1];
			i++;
		}
		else
		{
			print("getErrorPages: error code not numeric", RED, 2);
			return error_pages;
		}	
	}
	return error_pages;
	
}

/* Get client_max_body_size from a line */
size_t
Parser::parseClientMaxBodySize(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	if (split[1].empty())
	{
		print("parseClientMaxBodySize: client_max_body_size not specified", RED, 2);
		return CLIENT_MAX_BODY_SIZE_LIMIT;
	}
	if (atoi(split[1].c_str()) < 0)
	{
		print("parseClientMaxBodySize: client_max_body_size out of range", RED, 2);
		return CLIENT_MAX_BODY_SIZE_LIMIT;
	}
	if (split[1].find(';') != string::npos)
		split[1].erase(split[1].find(';'));
	if (isNumeric(split[1]))
		return atoi(split[1].c_str());
	else
	{
		print("parseClientMaxBodySize: client_max_body_size not numeric", RED, 2);
		return CLIENT_MAX_BODY_SIZE_LIMIT;
	}
}

/* Get cgi from a line - in format ex. cgi .php /usr/bin/php-cgi;*/
std::map<string, string>
Parser::parseCgi(const string& line)
{
	std::vector<string> split = CppSplit(line, ' ');
	std::map<string, string> cgi;
	if (split[1].empty())
	{
		print("getCgi: cgi not specified", RED, 2);
		return cgi;
	}
	//cleans the string(s) from the semicolon and pushes them to the map
	for (size_t i = 1; i < split.size(); i++)
	{
		if (split[i].find(';') != string::npos)
			split[i].erase(split[i].find(';'));
		if (split[i].find('.') != string::npos)
		{
			string extension = split[i];
			if (split[i + 1].empty())
				return cgi;
			if (split[i + 1].find(';') != string::npos)
				split[i + 1].erase(split[i + 1].find(';'));
			string path = split[i + 1];
			cgi[extension] = path;
			i++;
		}
		else
			return cgi;
	}
	return cgi;
}




/////////// --- PARSER --- ///////////
void Parser::parseFile(const string& path)
{
	std::ifstream file(path.c_str());
	string line;

	if (!file.good())
	{
		Log::Msg(ERROR, "Error opening config file: " + path);
		file.close();
		return;
	}

	int server_count = 0;
	bool inServerBlock = false;
	bool inLocationBlock = false;
	int blockDepth = 0;

	Server currentServer;
	Location currentLocation;

	//open file for parsing log
	std::fstream plog;

	plog.open("logs/parser_log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	if (!plog.is_open())
	{
		Log::Msg(ERROR, "Error opening parser log file");
		return;
	}


	//cout << YELLOW << "\n[PARSING CONFIG FILE ... ]" << RESET << endl << endl;
	plog << endl << Log::TimeStamp() << endl;
	plog << "PARSING CONFIG FILE: " + path << endl << endl;

	std::stack<string> blockStack; 
	while (std::getline(file, line))
	{
		// Trim leading white spaces
		line.erase(0, line.find_first_not_of(" \t"));

		if (line.empty() || line[0] == '#') // Skips comments and empty lines
			continue;

		if (line.find("{") != string::npos)
		{
			//cout << "Found { at depth " << blockDepth << endl;
			blockDepth++;
			if (line.find("server {") != string::npos)
			{
				blockStack.push("server");
				//cout << YELLOW << "Found server { at depth " << blockDepth << RESET << endl;
				plog << "Found server { at depth " << blockDepth << endl;
				inServerBlock = true;
				server_count++;
				std::stringstream ss;
				ss << "server" << server_count;
				currentServer.setName(ss.str());
			}
			else if (line.find("location") != string::npos && inServerBlock)
			{
				blockStack.push("location");
				//cout << YELLOW << "\tFound location { at depth " << blockDepth << RESET << endl;
				plog << "\tFound location { at depth " << blockDepth << endl;
				inLocationBlock = true;
				currentLocation = Location();
				currentLocation.setPath(parseLocationPath(line));
			}
		}
		else if (line.find("}") != string::npos)
		{
			if (!blockStack.empty())
			{
				string currentBlock = blockStack.top();
				blockStack.pop();
			
				if (currentBlock == "location")
				{
					//cout << RED << "\tFound } at depth " << blockDepth << " - ending location block ^" << RESET << endl;
					plog << "\tFound } at depth " << blockDepth << " - ending location block ^" << endl;
					inLocationBlock = false;
					currentServer.addLocation(currentLocation);
				}
				else if (currentBlock == "server")
				{
					//cout << RED << "Found } at depth " << blockDepth << " - ending server block ^" << RESET << endl;
					plog << "Found } at depth " << blockDepth << " - ending server block ^" << endl;
					inServerBlock = false;
					_servers.push_back(currentServer);
					currentServer = Server(); // Reset for the next server block
				}
			}
			blockDepth--;
		}

		if (inServerBlock && !inLocationBlock)
		{
			if (line.find("listen") != string::npos)
				{
					//print("config[server]: Found listen directive", GREEN);
					plog << "config[server]: Found listen directive" << endl;
					int port = parsePort(line);
					if (port == -1)
					{
						Log::Msg(ERROR, "Invalid port number in config file");
						exit(1);
					}
					currentServer.setPort(port);
				}
				else if (line.find("host") != string::npos)
				{
					//print("config[server]: Found host directive", GREEN);
					plog << "config[server]: Found host directive" << endl;
					in_addr_t host = parseHost(line);
					if (host == INADDR_NONE)
					{
						Log::Msg(ERROR, "Invalid host in config file");
						exit(1);
					}	
					currentServer.setHost(host);
				}
				else if (line.find("server_name") != string::npos)
				{
					//print("config[server]: Found server_name directive", GREEN);
					plog << "config[server]: Found server_name directive" << endl;
					std::vector<string> server_names = parseServerNames(line);
					currentServer.setServerNames(server_names);
				}
				else if (line.find("root") != string::npos)
				{
					//print("config[server]: Found root directive", GREEN);
					plog << "config[server]: Found root directive" << endl;
					string root = parseRoot(line);
					currentServer.setRoot(root);
				}
				else if (line.find("autoindex") != string::npos)
				{
					//print("config[server]: Found autoindex directive", GREEN);
					plog << "config[server]: Found autoindex directive" << endl;
					string autoindex = parseRoot(line);
					currentServer.setAutoindex(autoindex);
				}
				else if (line.find("index") != string::npos)
				{
					//print("config[server]: Found index directive", GREEN);
					plog << "config[server]: Found index directive" << endl;
					std::vector<string> index = parseIndex(line);
					currentServer.setIndex(index);
				}
				else if (line.find("error_page") != string::npos)
				{
					//print("config[server]: Found error_page directive", GREEN);
					plog << "config[server]: Found error_page directive" << endl;
					std::map<int, string> error_pages = parseErrorPages(line);
					currentServer.setErrorPages(error_pages);
				}
				else if (line.find("client_max_body_size") != string::npos)
				{
					//print("config[server]: Found client_max_body_size directive", GREEN);
					plog << "config[server]: Found client_max_body_size directive" << endl;
					size_t client_max_body_size = parseClientMaxBodySize(line);
					currentServer.setClientMaxBodySize(client_max_body_size);
				}
				else if (line.find("allowed_methods") != string::npos)
				{
					//print("config[server]: Found allowed_methods directive", GREEN);
					plog << "config[server]: Found allowed_methods directive" << endl;
					std::vector<int> allowed_methods = parseAllowedMethods(line);
					currentServer.setAllowedMethods(allowed_methods);
				}
				else if (line.find("upload_path") != string::npos)
				{
					//print("config[server]: Found upload_path directive", GREEN);
					plog << "config[server]: Found upload_path directive" << endl;
					string upload_path = parseRoot(line);
					currentServer.setUploadPath(upload_path);
				}
				else if (line.find("cgi") != string::npos)
				{
					//print("config[server]: Found cgi directive", GREEN);
					plog << "config[server]: Found cgi directive" << endl;
					std::map<string, string> cgi = parseCgi(line);
					currentServer.setCgi(cgi);
				}
		}
		else if (inLocationBlock && inServerBlock)
		{
			if (line.find("root") != string::npos)
			{
				//print("\tconfig[location]: Found root directive", GREEN);
				plog << "\tconfig[location]: Found root directive" << endl;
				string root = parseRoot(line);
				currentLocation.setRoot(root);
			}
			else if (line.find("autoindex") != string::npos)
			{
				//print("\tconfig[location]: Found autoindex directive", GREEN);
				plog << "\tconfig[location]: Found autoindex directive" << endl;
				string autoindex = parseRoot(line);
				currentLocation.setAutoindex(autoindex);
			}
			else if (line.find("index") != string::npos)
			{
				//print("\tconfig[location]: Found index directive", GREEN);
				plog << "\tconfig[location]: Found index directive" << endl;
				std::vector<string> index = parseIndex(line);
				currentLocation.setIndex(index);
			}
			else if (line.find("allowed_methods") != string::npos)
			{
				// print("\tconfig[location]: Found allowed_methods directive", GREEN);
				plog << "\tconfig[location]: Found allowed_methods directive" << endl;
				std::vector<int> allowed_methods = parseAllowedMethods(line);
				currentLocation.setAllowedMethods(allowed_methods);
			}
			else if (line.find("error_page") != string::npos)
			{
				// print("\tconfig[location]: Found error_page directive", GREEN);
				plog << "\tconfig[location]: Found error_page directive" << endl;
				std::map<int, string> error_pages = parseErrorPages(line);
				currentLocation.setErrorPages(error_pages);
			}
			else if (line.find("client_max_body_size") != string::npos)
			{
				// print("\tconfig[location]: Found client_max_body_size directive", GREEN);
				plog << "\tconfig[location]: Found client_max_body_size directive" << endl;
				size_t client_max_body_size = parseClientMaxBodySize(line);
				currentLocation.setClientMaxBodySize(client_max_body_size);
			}
			else if (line.find("upload_path") != string::npos)
			{
				// print("\tconfig[location]: Found upload_path directive", GREEN);
				plog << "\tconfig[location]: Found upload_path directive" << endl;
				string upload_path = parseRoot(line);
				currentLocation.setUploadPath(upload_path);
			}
			else if (line.find("cgi") != string::npos)
			{
				// print("\tconfig[location]: Found cgi directive", GREEN);
				plog << "\tconfig[location]: Found cgi directive" << endl;
				std::map<string, string> cgi = parseCgi(line);
				currentLocation.setCgi(cgi);
			}
		}
	/* 	else
		{
			// Handling directives outside of any blocks
			print("config: Directive outside server block", RED, 2);
			print(line, RED, 2);
		} */
	}

	file.close();
	Log::Msg(INFO, "Config file parsed successfully");
}

//////// --- GETTERS --- ////////
std::vector<Server>
Parser::getServers() const
{
	return _servers;
}




//////// --- PRINTERS --- ////////

/* Prints the parsed servers and their location configurations */
void
Parser::printServers()
{
cout << endl << YELLOW << "[ CONFIGURATION SUMMARY ]" << RESET << endl << endl;

	cout << "(std::vector<Server> _servers) SIZE: " << _servers.size() << endl << endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		cout << BOLD << "-------------------------------------" << endl;
		cout << GREEN << "Name: " << _servers[i].getName() << RESET << endl;
		cout << BOLD << "-------------------------------------" << RESET << endl << endl;
		cout << "Port: ";
		if (_servers[i].getPort() == -1)
			cout << "not set or error format";
		else
			cout << _servers[i].getPort();

		cout << endl;
		cout << "Server names: ";
		if (_servers[i].getServerNames().empty())
		{
			cout << "not set";
		}
		else
		{
			for (size_t j = 0; j < _servers[i].getServerNames().size(); j++)
				cout << _servers[i].getServerNames()[j] << " ";
		}
		cout << endl;
		cout << "Root: " << (_servers[i].getRoot().empty() ? "not set" : _servers[i].getRoot()) << endl;
		cout << "Index: ";
		if (_servers[i].getIndex().empty())
		{
			cout << "not set";
		}
		else
		{
			for (size_t j = 0; j < _servers[i].getIndex().size(); j++)
				cout << _servers[i].getIndex()[j] << " ";
		}
		cout << endl;
		string autoindex = _servers[i].getAutoindex() ? "on" : "off";
		cout << "Autoindex: " << autoindex << endl;
		if (_servers[i].getClientMaxBodySize() != CLIENT_MAX_BODY_SIZE_LIMIT)
			cout << "Client max body size: " << _servers[i].getClientMaxBodySize() << endl;
		else
			cout << "Client max body size: not set, using limit of 10 MB" << endl;
		cout << "Upload path: " << (_servers[i].getUploadPath().empty() ? "not set" : _servers[i].getUploadPath()) << endl;
		cout << "Allowed methods: ";
		if (_servers[i].getAllowedMethods().empty())
		{
			cout << "not set";
		}
		else
		{
			for (size_t j = 0; j < _servers[i].getAllowedMethods().size(); j++)
			{
				string allowed_methods = "";
				switch (_servers[i].getAllowedMethods()[j])
				{
					case 0:
						allowed_methods = "GET";
						break;
					case 1:
						allowed_methods = "POST";
						break;
					case 2:
						allowed_methods = "DELETE";
						break;
					case 3:
						allowed_methods = "NONE";
						break;
					default:
						allowed_methods = "UNKNOWN";
						break;
				}
				cout << allowed_methods << " ";
			}
		}
		cout << endl;
		cout << "Cgi: " << endl;
		std::map<string, string> cgi = _servers[i].getCgi();
		if (cgi.empty())
		{
			cout << "\tNot specified" << endl;
		}
		else
		{
			for (std::map<string, string>::iterator it = cgi.begin(); it != cgi.end(); ++it)
				cout << "\t" << it->first << " " << it->second << endl;
		}
		cout << "Error pages: " << endl;
		std::map<int, string> error_pages = _servers[i].getErrorPages();
		if (error_pages.empty())
		{
			cout << "\tNot specified" << endl;
		}
		else
		{
			for (std::map<int, string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
				cout << "\t" << it->first << " " << it->second << endl;
		}
		cout << endl;
		if (_servers[i].getLocations().size() > 0)
		{
			cout << endl << endl;
			cout << "Locations: " << endl;
			for (size_t j = 0; j < _servers[i].getLocations().size(); j++)
			{
				cout << endl;
				cout << "\t" << _servers[i].getLocations()[j].getPath() << endl;
				cout << "\tRoot: " << (_servers[i].getLocations()[j].getRoot().empty() ? "not set" : _servers[i].getLocations()[j].getRoot()) << endl;
				cout << "\tIndex: ";
				if (_servers[i].getLocations()[j].getIndex().empty())
				{
					cout << "not set";
				}
				else
				{
					for (size_t k = 0; k < _servers[i].getLocations()[j].getIndex().size(); k++)
						cout << _servers[i].getLocations()[j].getIndex()[k] << " ";
				}
				cout << endl;
				cout << "\tAllowed methods: ";
				if (_servers[i].getLocations()[j].getAllowedMethods().empty())
				{
					cout << "not set";
				}
				else
				{
					for (size_t k = 0; k < _servers[i].getLocations()[j].getAllowedMethods().size(); k++)
					{
						string allowed_methods = "";
						switch (_servers[i].getLocations()[j].getAllowedMethods()[k])
						{
							case 0:
								allowed_methods = "GET";
								break;
							case 1:
								allowed_methods = "POST";
								break;
							case 2:
								allowed_methods = "DELETE";
								break;
							case 3:
								allowed_methods = "NONE";
								break;
							default:
								allowed_methods = "UNKNOWN";
								break;
						}
						cout << allowed_methods << " ";
					}
				}
				cout << endl;
				string autoindex = _servers[i].getLocations()[j].getAutoindex() ? "on" : "off";
				cout << "\tAutoindex: " << autoindex << endl;
				if (_servers[i].getLocations()[j].getClientMaxBodySize() != CLIENT_MAX_BODY_SIZE_LIMIT)
					cout << "\tClient max body size: " << _servers[i].getLocations()[j].getClientMaxBodySize() << endl;
				else if (_servers[i].getClientMaxBodySize() != CLIENT_MAX_BODY_SIZE_LIMIT)
					cout << "\tClient max body size: not set, using server limit of " << _servers[i].getClientMaxBodySize() << " bytes." << endl;
				else
					cout << "\tClient max body size: not set, using limit of 10 MB" << endl;
				cout << "\tUpload path: " << (_servers[i].getLocations()[j].getUploadPath().empty() ? "not set" : _servers[i].getLocations()[j].getUploadPath()) << endl;
				cout << "\tCgi: " << endl;
				if (_servers[i].getLocations()[j].getCgi().empty())
				{
					cout << "\t\tNot specified" << endl;
				}
				else
				{
					std::map<string, string> cgi = _servers[i].getLocations()[j].getCgi();
					for (std::map<string, string>::iterator it = cgi.begin(); it != cgi.end(); ++it)
						cout << "\t\t" << it->first << " " << it->second << endl;
				}
				cout << "\tError pages: " << endl;
				if (_servers[i].getLocations()[j].getErrorPages().empty())
				{
					cout << "\t\tNot specified" << endl;
				}
				else
				{
					std::map<int, string> error_pages = _servers[i].getLocations()[j].getErrorPages();
					for (std::map<int, string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
						cout << "\t\t" << it->first << " " << it->second << endl;
				}
				cout << endl;
			}
		}
		cout << endl << endl;
	}
}