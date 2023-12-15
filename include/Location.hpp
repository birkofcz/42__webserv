/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:44 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/15 15:47:22 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "general.hpp"

class Location
{
	private:
		std::vector<int>	_allowed_methods; // GET, POST, DELETE, NONE
		string				_root;
		std::vector<string>	_index;
		string				_cgi;
		string				_autoindex;


	public:
		Location();
		~Location();

	// Setters & getters

		void				setAllowedMethods(std::vector<int> allowed_methods);
		void				setRoot(string root);
		void				setIndex(std::vector<string> index);
		void				setCgi(string cgi);
		void				setAutoindex(string autoindex);

		std::vector<int>	getAllowedMethods();
		string				getRoot();
		std::vector<string>	getIndex();
		string				getCgi();
		string				getAutoindex();
};	


#endif