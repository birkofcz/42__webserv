/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 16:20:16 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/16 16:49:04 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include "general.hpp"
# include "Server.hpp"
# include "Location.hpp"

/* 
Error class is responsible for building error pages and error descriptions.
It is defined as static class, so it can be used without creating an instance, 
just by calling Error::function()
 */

class Error
{
	public:
		static string	getErrorDescription(short error_code);
		static string	buildErrorPage(short error_code, string location_key, Server &server);
};

#endif 
