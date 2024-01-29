/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:49:47 by tkajanek          #+#    #+#             */
/*   Updated: 2024/01/24 20:04:38 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "general.hpp"
# include <cstddef> // size_t
# include <stdint.h> // uint8_t
# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include <vector>


#define MAX_URI_LENGTH 2048 //domyslet

using std::map;

enum ParsingState
{
    Request_Line,
    Request_Line_Post_Put,
    Request_Line_Method,	
    Request_Line_First_Space,
    Request_Line_URI_Path_Slash,
    Request_Line_URI_Path,
    Request_Line_URI_Query,
    // Request_Line_URI_Fragment,
    Request_Line_Version,
    Request_Line_HT,
    Request_Line_HTT,
    Request_Line_HTTP,
    Request_Line_HTTP_Slash,
    Request_Line_Major,
    Request_Line_Dot,
    Request_Line_Minor,
    Request_Line_CR,
    Request_Line_LF,
    Field_Name_Start,
    Fields_End,
    Field_Name,
    Field_Value,
    Field_Value_End,
    Chunked_Length_Begin,
    Chunked_Length,
    Chunked_Ignore,
    Chunked_Length_CR,
    Chunked_Length_LF,
    Chunked_Data,
    Chunked_Data_CR,
    Chunked_Data_LF,
    Chunked_End_CR,
    Chunked_End_LF,
    Message_Body,
    Parsing_Done
};

class HttpRequest
{
	public:
		HttpRequest();
		~HttpRequest();

		HttpMethod&									getMethod();
		std::string&								getPath();
		std::string&								getQuery();
		// std::string&								getFragment();
		std::string									getHeader(const std::string&);
		const std::map<std::string, std::string>&	getHeaders() const;
		std::string                                 getMethodStr();
		std::string&								getBody();
		std::string                                 getServerName();
		std::string									getServerPort();
		std::string&								getBoundary();
		size_t 										getBodyLen()
		{
			return (_body_length);
		}
		bool										getMultiformFlag();

		void setMethod(HttpMethod&);
		void setMaxBodySize(size_t);
		void setBody(std::string name);
		void setHeader(std::string&, std::string&);

		void feed(char* data, size_t size);
		// bool parsingCompleted();
		// void printMessage();
		void clear();
		short getErrorCode();
		// bool keepAlive();
		// void cutReqBody(int bytes);
		bool complete_flag;
		
		int client_socket;

	private:
		std::string _path;
		std::string _query;
		// // std::string _fragment; //on the client side
		map<std::string, std::string> _request_headers;
		std::vector<uint8_t> _body; //message stored in bytes, usually not part of GET
		std::string _boundary; //associated with multipart/form-data requests. MIMe, POST method
		HttpMethod _method;
		map<uint8_t, std::string> _method_str;
		ParsingState _state;
		size_t _max_body_size;
		size_t _body_length;
		short _error_code;
		size_t _chunk_length;
		std::string _storage; // temporary buffer
		std::string _key_storage; // temp buffer for key in the map
		short _method_index;
		uint8_t _ver_major; //HTTP/(1).1
		uint8_t _ver_minor; //HTTP/1.(1)
		std::string _server_name;
		std::string _server_port;
		std::string _body_str;

		/* flags */
		bool _fields_done_flag; //all headers done
		bool _body_flag; //the request contains body
		bool _body_done_flag;
		
		bool _chunked_flag;
		bool _multiform_flag;

		void _handle_headers();

		friend std::ostream& operator<<(std::ostream& os, const HttpRequest& request);
};

inline std::ostream& operator<<(std::ostream& os, const HttpMethod& method) {
    switch (method) {
        case GET:
            os << "GET";
            break;
        case POST:
            os << "POST";
            break;
        case DELETE:
            os << "DELETE";
            break;
        default:
            os << "NONE";
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const HttpRequest& request) {
    os << "Method: " << request._method << std::endl;
    os << "Path: " << request._path << std::endl;
    os << "Query: " << request._query << std::endl;
	os << "_ver_major: " << request._ver_major << std::endl;
	os << "_ver_minor: " << request._ver_minor << std::endl;
    os << "Headers:" << std::endl;
    const map<std::string, std::string>& headers = request._request_headers;
    for (map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        os << it->first << ": " << it->second << std::endl;
    }
	os << "_boundary:" << request._boundary << std::endl;
    os << "Body str: " << request._body_str << std::endl;
	os << "Body vector: ";
  	for (std::vector<uint8_t>::const_iterator it = request._body.begin(); it != request._body.end(); ++it) {
    uint8_t byte = *it;
    os << static_cast<char>(byte) << " ";
}
    os << std::endl;

    return os;
}

#endif