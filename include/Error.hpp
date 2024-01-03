/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 16:20:16 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/03 16:44:58 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include "general.hpp"

class Error
{
	public:
		static string	getErrorDescription(short error_code);
		static string	buildErrorPage(short error_code);
};

#endif 