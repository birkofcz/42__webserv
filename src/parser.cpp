/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:58:07 by sbenes            #+#    #+#             */
/*   Updated: 2023/11/27 16:57:31 by sbenes           ###   ########.fr       */
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
		else if (line.find("listen") != string::npos)
		{
			print("config: Found listen directive", GREEN);
			//parse listen directive
			int port = getPort(line);
			//if (port != -1) - here we check and write the listen port to the structure?
		

			cout << "port: " << port << endl;
		}
/* 		else if (line.find("server_name") != string::npos)
		{
			print("Found server_name directive", GREEN);
			//parse server_name directive
		}
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
		else if (line.find("root") != string::npos)
		{
			print("Found root directive", GREEN);
			//parse root directive
		}
		else if (line.find("index") != string::npos)
		{
			print("Found index directive", GREEN);
			//parse index directive
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
		else
		{
			print("Unknown directive", RED, 2);
			print(line, RED, 2);
		}
	}
	file.close();
}
