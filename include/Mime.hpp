/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:51:31 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/29 16:58:27 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
# define MIME_HPP

# include "general.hpp"

class Mime
{
	private:
		std::string _extension;
		std::map<std::string, std::string> _mimes;
	
	public:
		Mime();
		~Mime();

		std::string parseExtension(std::string path);
		std::string getMime() const;

		void testPrint(string path);
	
};

#endif