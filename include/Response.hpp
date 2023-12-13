/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 19:43:11 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/13 18:31:14 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "HttpRequest.hpp"
# include "Server.hpp"

/*
	Responsible for building and storing the response. when response is ready,
	it will be stored on _response_content and can be used by func getRes().
*/
class Response
{
	public:
		// static Mime _mime;
		Response();
		Response(HttpRequest&);
		~Response();

		// std::string getRes();
		// size_t getLen() const;
		// int getCode() const;

		void setRequest(HttpRequest&);
		void setServer(Server&);

		void buildResponse();
		// void clear();
		// void handleCgi(HttpRequest &);
		// void cutRes(size_t);
		// int getCgiState();
		// void setCgiState(int);
		// void setErrorResponse(short code);

		// CgiHandler _cgi_obj;

		// std::string removeBoundary(std::string &body, std::string &boundary);
	
		HttpRequest request;

	private:
		Server _server;
		std::string _target_file; //ex: "/var/www/html/index.html"
		std::vector<uint8_t> _body_bytes; //A vector of bytes representing the body of the response. may represent non-textual data, such as images, executables, or any raw byte sequence.
		size_t _body_length;
		std::string _response_body_str; //to store text-based data, such as HTML content, plain text, or other character sequences.
		std::string _location;
		short _status_code; //  200(for a successful response).
		char *_res;
		std::string _response_content; //proc byl public and ucel
		int _cgi;
		// int _cgi_fd[2];
		// size_t _cgi_response_length;
		// bool _auto_index; //automatic generation of directory listings when a client requests a directory that does not contain an index file 

		// int buildBody();
		// size_t file_size();
		// void setStatusLine();
		// void setHeaders();
		// void setServerDefaultErrorPages();
		// int readFile();
		// void contentType();
		// void contentLength();
		// void connection();
		// void server();
		// void location();
		// void date();
		// int handleTarget();
		// void buildErrorBody();
		// bool reqError();
		// int handleCgi(std::string &);
		// int handleCgiTemp(std::string &);
};

#endif