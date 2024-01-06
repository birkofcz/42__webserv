/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 16:20:16 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/06 16:05:07 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include "general.hpp"
# include "Server.hpp"
# include "Location.hpp"

class Error
{
	public:
		static string	getErrorDescription(short error_code);
		static string	buildErrorPage(short error_code, string location_key, Server &server, Location &location);
};

#endif 