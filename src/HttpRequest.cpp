/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:59:05 by tkajanek          #+#    #+#             */
/*   Updated: 2023/12/06 16:55:08 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpRequest.hpp"

HttpRequest::HttpRequest()
{
	_method = NONE;
}

HttpRequest::~HttpRequest() {}

HttpMethod &HttpRequest::getMethod()
{
	return (_method);
}

void    HttpRequest::setMethod(HttpMethod& method)
{
    _method = method;
}

void HttpRequest::testFeed(char *data, size_t size)
{
	(void)size;
	uint8_t character = data[0];
	ParsingState _state = Request_Line;
	switch (_state)
	{
		case Request_Line:
		{
			if (character == 'G')
				_method = GET;
			else if (character == 'P')
			{
				_state = Request_Line_Post_Put;
				break;
			}
			else if (character == 'D')
				_method = DELETE;
			else if (character == 'H')
				_method = HEAD;
			else
			{
				//_error_code = 501;
				std::cout << "Method Error Request_Line and Character is = " << character << std::endl;
				return;
			}
			_state = Request_Line_Method;
			break;
		}
		default:
        	break;		
	}
}