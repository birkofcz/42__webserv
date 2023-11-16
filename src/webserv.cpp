/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:11:06 by sbenes            #+#    #+#             */
/*   Updated: 2023/11/16 17:30:33 by sbenes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/general.hpp"

int main(int /* argc */, char **/* argv */)
{
	string message = "This is where the webserv will hopefully be in a few weeks :) !";
	
	cout << endl;
	print(message, GREEN);
	print(message, RED, 2);
	print(message);
	print(message, NONE, 2);
	cout << endl;
	
	return 0;
}