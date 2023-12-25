/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:44 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/25 15:19:46 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "general.hpp"


/* Location class - the vector of those will be made in every Server object,
parsing the directives in each "location" block. Tho work is done by Parser class */

class Location
{
	private:
		string						_path;
		std::vector<int>			_allowed_methods; // GET, POST, DELETE, NONE
		string						_root;
		std::vector<string>			_index;
		std::map<string, string>	_cgi;
		bool						_autoindex;
		std::map<int, string>		_error_pages; // 404, /404.html
		int							_client_max_body_size;
		string						_upload_path;


	public:
		Location();
		~Location();

	// Setters & getters
		void						setPath(string path);
		void						setAllowedMethods(std::vector<int> allowed_methods);
		void						setRoot(string root);
		void						setIndex(std::vector<string> index);
		void						setCgi(std::map<string, string> cgi);
		void						setAutoindex(string autoindex);
		void						setErrorPages(std::map<int, string> error_pages);
		void						setClientMaxBodySize(int client_max_body_size);
		void						setUploadPath(string upload_path);


		string						getPath();
		std::vector<int>			getAllowedMethods();
		string						getRoot();
		std::vector<string>			getIndex();
		std::map<string, string>	getCgi();
		bool						getAutoindex();
		std::map<int, string>		getErrorPages();
		int							getClientMaxBodySize();
		string						getUploadPath();
};	


#endif