/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 15:58:39 by sbenes            #+#    #+#             */
/*   Updated: 2023/12/02 16:27:24 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "general.hpp"

class Server
{
    private:
        std::vector<int>    port;
        std::vector<string>	server_name;
        string              error_page;
        string              location;
        string              root;
        std::vector<string> index;
        // bool	autoindex;
        // int		client_max_body_size;
        // string	cgi;
        // string	upload;
        // string	auth;
        // string	methods;
        // string	redirect;
        // string	return_;

    public:
        Server();
        ~Server();



        
};

#endif