/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:48:25 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/31 16:19:54 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include "general.hpp"

enum LogType
{
	INFO,
	WARNING,
	ERROR,
	DEBUG
};

/* 
Class to handle the logging of the server, defined as static, so it can be used anywhere
in the code without the need to create an instance of the class
 */
class Log
{
	private:
		static string			_DetermineColor(LogType type);

	public:
		//switchers - what output is activated
		static bool		out_console;
		static bool		out_file;

		//switchers - what type of messages are activated
		static bool		type_info;
		static bool		type_warning;
		static bool		type_error;
		static bool		type_debug;
		
		static void		Msg(LogType type, string msg);
		static string	TimeStamp();

};

#endif
