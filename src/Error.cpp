/* ************************************************************************** */
/*		                                                                    */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 16:20:36 by sbenes            #+#    #+#             */
/*   Updated: 2024/01/01 16:41:00 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Error.hpp"

string
Error::buildErrorPage(int error_code)
{
	std::stringstream ss;
	ss << error_code;
	string ecode = ss.str();
	string error_page("");

	string error_description("");

		switch (error_code)
	{
		case 100:
			error_description = "Continue";
		case 101:
			error_description = "Switching Protocol";
		case 200:
			error_description = "OK";
		case 201:
			error_description = "Created";
		case 202:
			error_description = "Accepted";
		case 203:
			error_description = "Non-Authoritative Information";
		case 204:
			error_description = "No Content";
		case 205:
			error_description = "Reset Content";
		case 206:
			error_description = "Partial Content";
		case 300:
			error_description = "Multiple Choice";
		case 301:
			error_description = "Moved Permanently";
		case 302:
			error_description = "Moved Temporarily";
		case 303:
			error_description = "See Other";
		case 304:
			error_description = "Not Modified";
		case 307:
			error_description = "Temporary Redirect";
		case 308:
			error_description = "Permanent Redirect";
		case 400:
			error_description = "Bad Request";
		case 401:
			error_description = "Unauthorized";
		case 403:
			error_description = "Forbidden";
		case 404:
			error_description = "Not Found";
		case 405:
			error_description = "Method Not Allowed";
		case 406:
			error_description = "Not Acceptable";
		case 407:
			error_description = "Proxy Authentication Required";
		case 408:
			error_description = "Request Timeout";
		case 409:
			error_description = "Conflict";
		case 410:
			error_description = "Gone";
		case 411:
			error_description = "Length Required";
		case 412:
			error_description = "Precondition Failed";
		case 413:
			error_description = "Payload Too Large";
		case 414:
			error_description = "URI Too Long";
		case 415:
			error_description = "Unsupported Media Type";
		case 416:
			error_description = "Requested Range Not Satisfiable";
		case 417:
			error_description = "Expectation Failed";
		case 418:
			error_description = "I'm a teapot";
		case 421:
			error_description = "Misdirected Request";
		case 425:
			error_description = "Too Early";
		case 426:
			error_description = "Upgrade Required";
		case 428:
			error_description = "Precondition Required";
		case 429:
			error_description = "Too Many Requests";
		case 431:
			error_description = "Request Header Fields Too Large";
		case 451:
			error_description = "Unavailable for Legal Reasons";
		case 500:
			error_description = "Internal Server Error";
		case 501:
			error_description = "Not Implemented";
		case 502:
			error_description = "Bad Gateway";
		case 503:
			error_description = "Service Unavailable";
		case 504:
			error_description = "Gateway Timeout";
		case 505:
			error_description = "HTTP Version Not Supported";
		case 506:
			error_description = "Variant Also Negotiates";
		case 507:
			error_description = "Insufficient Storage";
		case 510:
			error_description = "Not Extended";
		case 511:
			error_description = "Network Authentication Required";
		default:
			error_description = "Undefined";
	}
	
	error_page += "<html>\r\n<head><title>";
	error_page += ecode;
	error_page += " ";
	error_page += error_description;
	error_page += " </title></head>\r\n";
	error_page += "<body>\r\n";
	error_page += "<center><h1>";
	error_page += ecode;
	error_page += " ";
	error_page += error_description;
	error_page += "</h1></center>\r\n";

	return error_page;

}