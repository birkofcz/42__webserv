/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:51:31 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/04 16:13:50 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
# define MIME_HPP

# include "general.hpp"

/* MIME class is used to recognize the content type of the returning content.
This is used to determine how to process the response.

MIME stands for Multipurpose Internet Mail Extensions. It is a standard 
that indicates the nature and format of a document, file, or assortment of bytes.
It is maintained by the Internet Assigned Numbers Authority (IANA).
More: https://www.iana.org/assignments/media-types/media-types.xhtml

*/

class Mime
{
	private:
		std::string					_extension;
		std::map<string, string>	_mimes;
		short						_status_code;
	
	public:
		Mime(const short &status_code);
		~Mime();

		string						parseExtension(string path);
		string						getMime() const;
		void						testPrint(string path);
	
};

#endif