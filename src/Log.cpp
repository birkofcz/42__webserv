/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 16:28:05 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/13 16:53:10 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Log.hpp"


bool	Log::out_console = false;
bool	Log::out_file = false;

bool	Log::type_info = false;
bool	Log::type_warning = false;
bool	Log::type_error = false;
bool	Log::type_debug = false;

void	
Log::Msg(LogType type, string msg)
{
	if (out_console)
	{
		switch (type)
		{
			case INFO:
				if (type_info)
					print(TimeStamp() + "  [INFO]  " + msg, GREEN);
				break;
			case WARNING:
				if (type_warning)
					print(TimeStamp() + "  [WARNING]  " + msg, YELLOW);
			case ERROR:
				if (type_error)
					print(TimeStamp() + "  [ERROR]  " + msg, RED);
				break;
			case DEBUG:
				if (type_debug)
					print(TimeStamp() + "  [DEBUG]  " + msg, BLUE);
				break;
		}
	}
	if (out_file)
	{
		string filename = "logs/log" + TimeStamp() + ".txt";
		const char *filepath = filename.c_str();
		std::ofstream logFile;
		logFile.open(filepath, std::ios::app); // Open in append mode
		if (logFile.is_open())
		{
			string message("");
			switch (type)
			{
				case INFO:
					if (type_info)
						message = TimeStamp() + "  [INFO]  " + msg;
					break;
				case WARNING:
					if (type_warning)
						message = TimeStamp() + "  [WARNING]  " + msg;
					break;
				case ERROR:
					if (type_error)
						message = TimeStamp() + "  [ERROR]  " + msg;
					break;
				case DEBUG:
					if (type_debug)
						message = TimeStamp() + "  [DEBUG]  " + msg;
					break;
			}
			logFile << message << endl;
		}
	}
}

string
Log::TimeStamp()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	string month("");
	if (ltm->tm_mon + 1 < 10)
		month = "0" + toString(ltm->tm_mon + 1);
	else
		month = ltm->tm_mon + 1;
	string year = toString(ltm->tm_year + 1900);
	
	std::stringstream ss;
	//add date:
	ss << "[" << ltm->tm_mday << "." << month << "." << year << "] ";
	ss << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ";
	return (ss.str());
	
}
