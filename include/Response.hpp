/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 19:43:11 by tkajanek          #+#    #+#             */
/*   Updated: 2024/01/29 17:26:28 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "HttpRequest.hpp"
# include "Server.hpp"
# include "Mime.hpp"
# include "Error.hpp"
# include "Log.hpp"
# include "Cgi.hpp"

# include <sys/stat.h>
# include <algorithm>
# include <ctime>    // for time, struct tm, and gmtime
# include <cstring>  // for memset
# include <fstream>


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
		void clear();

		
		// void handleCgi(HttpRequest &);
		// void cutRes(size_t);
		// int getCgiState();
		// void setCgiState(int);
		// void setErrorResponse(short code);
		bool	getCgiFlag ()
		{
			return (this->_cgi_flag);
		}
		string	getStatusLineCgi();
		void setStatusCode(short code);
		
		Cgi cgi_object;

		std::string removeBoundary(std::string& body, std::string& boundary, std::string& filename); //proc public
	
		HttpRequest request;
		std::string _response_content; //zmenit oznaceni bez _
		bool _cgi_flag;

		friend std::ostream& operator<<(std::ostream& os, const Response& response);

	private:
		std::string	_location_key;
		std::string _mime;
		Server _server;
		std::string _target_file; //ex: "/var/www/html/index.html"
		std::vector<uint8_t> _body_bytes; //A vector of bytes representing the body of the response. may represent non-textual data, such as images, executables, or any raw byte sequence.
		size_t _body_length;
		std::string _response_body_str; //to store text-based data, such as HTML content, plain text, or other character sequences.
		std::string _location; //for Header
		short _status_code; //  200(for a successful response).
		// // // char *_res; co to je?
		
		// int _cgi_fd[2]; smazat
		size_t _cgi_response_length;
		bool _auto_index; //automatic generation of directory listings when a client requests a directory that does not contain an index file 

		// private methods
		int 	_buildBody();
		int 	_handleTarget();
		void	_getLocationMatch(std::string&, std::vector<Location>, std::string&);
		bool	_isAllowedMethod(HttpMethod&, Location&, short&);
		void 	_appendRoot(Location&, HttpRequest&);
		std::string _combinePaths(std::string, std::string, std::string);
		bool _isDirectory(std::string);
		// size_t file_size();
		void _setStatusLine();
		void _setHeaders();
		// void setServerDefaultErrorPages();
		int _readFile();
		bool _fileExists(const std::string&);
		void _contentType();
		void _contentLength();
		void _connection();
		void _serverHeader();
		void _locationHeader();
		void _date();
		
		// void buildErrorBody();
		bool _reqError();
		int handleCgi(std::string &);
		// int handleCgiTemp(std::string &);

		string _buildAutoindex(string &path);
};

inline std::ostream& operator<<(std::ostream& os, const Response& response) {
    // Output private attributes of the Response object
    os << "Location Key: " << response._location_key << std::endl;
    os << "MIME: " << response._mime << std::endl;
    os << "Target File: " << response._target_file << std::endl;
    os << "Body Length: " << response._body_length << std::endl;
    os << "Response Body String: " << response._response_body_str << std::endl;
    os << "Location: " << response._location << std::endl;
    os << "Status Code: " << response._status_code << std::endl;
    os << "CGI Response Length: " << response._cgi_response_length << std::endl;
    os << "Auto Index: " << response._auto_index << std::endl;
	os << "Response Content: " << response._response_content << std::endl; // Added
    os << "CGI Flag: " << response._cgi_flag << std::endl;
    // Output other private attributes as needed...

    return os;
    
    return os;
}

#endif