/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:25:51 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/10 16:41:49 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"

/* print - will print message with optional args - color and output stream (cout or cerr = 1 or 2) */
void print(string message, string color, int output)
{
	switch (output)
	{
		case 2:
			cerr << color << message << RESET << endl;
			break;
		default:
			cout << color << message << RESET << endl;
			break;
	}
}


/* CppSplit - splits on delimiter, puts string in vector */
std::vector<string>	CppSplit(std::string str, char delimiter)
{
	std::vector<string>		result;
	std::stringstream		ss(str);
	string					token;

	while (std::getline(ss, token, delimiter))
		if (!token.empty())
			result.push_back(token);
	return (result);
}

/* isNumeric - checks if input is numeric */
bool isNumeric(string str)
{
	for (unsigned int i = 0; i < str.length(); i++)
		if (!isdigit(str[i]))
			return (false);
	return (true);
}

/* void debugPrint(string message, string color)
{
	if (debug)
		print(message, color);
} */

void debugPrint(string message, string color)
{
	if (debugFile)
	{
        // Write the message to a log file
        std::ofstream logFile;
        logFile.open("debugLog.txt", std::ios::app); // Open in append mode
        if (logFile.is_open())
        {
            logFile << message << endl;
            logFile.close();
        }
        else
        {
            cerr << "Unable to open log file." << endl;
        }
	}
    else if (debug)
    {
        // Print the message with the specified color, or no color if NONECOLOR is used
        cout << color << message << "\033[0m" << endl; // Reset color after message if used
    }
}