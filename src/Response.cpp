/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 20:00:29 by tkajanek          #+#    #+#             */
/*   Updated: 2024/02/07 17:11:48 by tkajanek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"
#include "../include/general.hpp"
#include <unistd.h>

// Mime Response::_mime;

Response::Response()
{
	_location_key = "";
	_target_file = "";
	_body_bytes.clear();
	_body_length = 0;
	_response_content = "";
	_response_body_str = "";
	_location = "";
	_status_code = 0;
	_cgi_flag = 0;
	_cgi_response_length = 0;
	_auto_index = false;
}

Response::~Response() {}

Response::Response(HttpRequest& src) : request(src) //proc initializace na 0
{
	_location_key = "";
    _target_file = "";
    _body_bytes.clear();
    _body_length = 0;
    _response_content = "";
    _response_body_str = "";
    _location = "";
    _status_code = 0;
    _cgi_flag = 0;
    _cgi_response_length = 0;
    _auto_index = false;
}
void   Response::_contentType()
{
	_response_content.append("Content-Type: ");
	
	Log::Msg(DEBUG, FUNC + "building \"Content-type\", _status_cod is: " + toString(_status_code));
	
	Mime mime(_status_code, _auto_index);
	//Mime class handling the content type
	mime.parseExtension(_target_file);
	_mime = mime.getMime();
	_response_content.append(_mime);

	// _response_content.append(_mime.getMime()); //suggested by CoPilot
	_response_content.append("\r\n");
}

void   Response::_contentLength()
{
	_response_content.append("Content-Length: ");
	_response_content.append(toString(_response_body_str.length()));
	_response_content.append("\r\n");
}

void   Response::_connection()
{
	if(request.getHeader("connection") == "keep-alive")
		_response_content.append("Connection: keep-alive\r\n");
}

void   Response::_serverHeader()
{
	_response_content.append("Server: [TS]erver\r\n");
}

/*
	When a server sends an HTTP response with a "Location" header,
	it instructs the client (usually a web browser) to navigate to the specified URL.
	This is commonly used for handling HTTP redirects,
	such as when a resource has moved permanently (301 Moved Permanently) or temporarily (302 Found).
*/
void    Response::_locationHeader()
{
	if (_location.length())
	{
		Log::Msg(DEBUG, FUNC + "_location exists: " + _location);
		_response_content.append("Location: "+ _location +"\r\n");
	}
}

void    Response::_date()
{
    char date[1000];
    time_t now = time(0);

    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    gmtime_r(&now, &tm);

    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
	
    _response_content.append("Date: ");
    _response_content.append(date);
    _response_content.append("\r\n");
}

// uri ecnoding
void    Response::_setHeaders()
{
    _contentType();
    _contentLength();
    _connection();
    _serverHeader();
    _locationHeader();
    _date();

    _response_content.append("\r\n");
}

bool	Response::_fileExists(const std::string& f)
{
    std::ifstream file(f.c_str());
    return (file.good());
}

/*
checks whether a given path corresponds to a directory using
low-level file system information. It uses the stat() from the <sys/stat.h> 
to retrieve information about the file specified by the given path.
*/
bool Response::_isDirectory(std::string path)
{
	Log::Msg(DEBUG, FUNC + "path: " + path);
	
    struct stat file_stat;

    if (stat(path.c_str(), &file_stat) != 0)
		return false; // example: file doesnt exist
    return (S_ISDIR(file_stat.st_mode));
}

std::string Response::_combinePaths(std::string p1, std::string p2, std::string p3) // predelat na const &?
{
	std::string res = p1;

	if (!res.empty() && res[res.size() - 1] != '/' && !p2.empty() && p2[0] != '/')
		res += '/';
	res += p2;

	if (!res.empty() && res[res.size() - 1] == '/' && !p3.empty() && p3[0] == '/')
		res.erase(res.size() - 1);

	if (!res.empty() && res[res.size() - 1] != '/' && !p3.empty() && p3[0] != '/')
		res += '/';
	res += p3;

	return res;
}

void Response::_appendRoot(Location &location, HttpRequest &request)
{
	Log::Msg(DEBUG, FUNC + "location.getRoot(): " + location.getRoot());
	Log::Msg(DEBUG, FUNC + "_server.getRoot(): " + _server.getRoot());
	Log::Msg(DEBUG, FUNC + "request.getPath(): " + request.getPath());
	Log::Msg(DEBUG, FUNC + "_target_file before appendRoot: " + _target_file);

	string root("");
	if (location.getRoot().empty())
		root = _server.getRoot();
	else
		root = location.getRoot();
   _target_file = _combinePaths(root, request.getPath(), "");
}

/* check a file for CGI (the extension is supported, the file exists and is executable) and run the CGI */
int        Response::handleCgi(std::string &location_key)
{
    std::string path;
    std::string exten;
    size_t      pos;

    path = this->request.getPath();
	Log::Msg(DEBUG, FUNC + "path before formatting: " + path);

    if (path[0] && path[0] == '/')
        path.erase(0, 1);
    if (path == "cgi-bin")
        path += "/" + _server.getLocationKey(location_key)->getIndex()[0];
    else if (path == "cgi-bin/")
        path.append(_server.getLocationKey(location_key)->getIndex()[0]);

    pos = path.find(".");
    if (pos == std::string::npos)
    {
        _status_code = 501;
        return (1);
    }
    exten = path.substr(pos);
    if (exten != ".py" && exten != ".sh")
    {
        _status_code = 501;
        return (1);
    }
	// /cgi-bin/time.py
	///////////////////////////////////////
    // if (ConfigFile::getTypePath(path) != 1)
    // {
    //     _code = 404;
    //     return (1);
    // }
    // if (ConfigFile::checkFile(path, 1) == -1 || ConfigFile::checkFile(path, 3) == -1)
    // {
    //     _code = 403;
    //     return (1);
    // }
	if (!_isAllowedMethod(request.getMethod(), *_server.getLocationKey(location_key), _status_code))
		return (1);
	Log::Msg(DEBUG, FUNC + "path after formatting: " + path);
	Log::Msg(DEBUG, FUNC + "extension: " + exten);
    cgi_object.clear();
	cgi_object.setExtension(exten);
    cgi_object.setCgiPath("content/www/" + path); //nutno predelat
    _cgi_flag = true;
    cgi_object.initEnv(request, _server.getLocationKey(location_key)); // + URI
	return(cgi_object.execute(this->_status_code));
}


/*
	if the condition for the given method is satisfied
	and the corresponding method in the methods vector is not allowed (i.e., zero),
 	it proceeds to set the code variable to 405 (Method Not Allowed).
*/
bool Response::_isAllowedMethod(HttpMethod& method, Location& location, short& code)
{
    std::vector<int> methods = location.getAllowedMethods();

 	if (std::find(methods.begin(), methods.end(), method) == methods.end())
    {
        Log::Msg(ERROR, FUNC + "method not allowed");
		code = 405;
        return false;
    }
    return true;
}

/*
    Compares URI with locations from config file and tries to find the best match.
    If match found, then location_key is set to that location, otherwise location_key will be an empty string.
*/
void    Response::_getLocationMatch(std::string& path, std::vector<Location> locations, std::string& location_key)
{
    size_t biggest_match = 0;

    for(std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        if(path.find(it->getPath()) == 0)
        {
               if( it->getPath() == "/" || path.length() == it->getPath().length() || path[it->getPath().length()] == '/')
			   //Last condition may be relevant in scenarios where the Location object represents a directory,
               {
                    if(it->getPath().length() > biggest_match)
                    {
                        biggest_match = it->getPath().length();
                        location_key = it->getPath();
                    }
               }
        }
    }
}

int    Response::_handleTarget()
{
    _getLocationMatch(request.getPath(), _server.getLocations(), _location_key);

	Log::Msg(DEBUG, FUNC + "_location_key: " + _location_key);
	
	string path = request.getPath();

	Log::Msg(DEBUG, FUNC + "request.getPath(): " + path);
	
    if (_location_key.length() > 0)
    {
        Location target_location = *_server.getLocationKey(_location_key);
			Log::Msg(DEBUG, FUNC + "target_location found: " + target_location.getPath());
        if (!_isAllowedMethod(request.getMethod(), target_location, _status_code))
        {
            std::cout << "METHOD NOT ALLOWED \n";
            return (1);
        }
			Log::Msg(DEBUG, FUNC + "method allowed: " + toString(request.getMethod()));
        if (request.getBody().length() > static_cast<size_t>(target_location.getClientMaxBodySize()))
        {
            _status_code = 413;
            return (1);
        }
		Log::Msg(DEBUG, FUNC + "body lenght ok: " + toString(request.getBody().length()));
        // if (checkReturn(target_location, _code, _location))
        //     return (1);

		if (target_location.getPath().find("cgi-bin") != std::string::npos)
		{
			return (handleCgi(_location_key));
		}
		 _appendRoot(target_location, request); //assignes _target_file
        // if (!target_location.getCgiExtension().empty())
        // {

        //     if (_target_file.rfind(target_location.getCgiExtension()[0]) != std::string::npos)
        //     {
        //         return (handleCgiTemp(location_key));
        //     }

        // }
		Log::Msg(DEBUG, FUNC + "_target_file before diectory check: " + _target_file);
        if (_isDirectory(_target_file))
        {
			Log::Msg(DEBUG, FUNC + "_target_file is a directory.");
            if (_target_file[_target_file.length() - 1] != '/')
            {
                Log::Msg(DEBUG, FUNC + "_target file does not end with /");
			    _status_code = 301; //moved permanently
                _location = request.getPath() + "/";
                return (1);
            }
            if (!target_location.getIndex().empty()) //nechybi tam [0] ?
			{
                Log::Msg(DEBUG, FUNC + "target_location has index defined.");
				_target_file += target_location.getIndex()[0]; //hardcoded index index.html
				cout << "TEST: LOCATION _target_file after index: " << _target_file << endl;
			}
            else
            {
				Log::Msg(DEBUG, FUNC + "target_location has no index defined, using server index.");
				_target_file += _server.getIndex()[0]; //hardcoded index index.html 

				Log::Msg(DEBUG, FUNC + "_target_file after index: " + _target_file);
			}
			//Index appended, now check if the file exists
            if (!_fileExists(_target_file))
            {
				Log::Msg(DEBUG, FUNC + "_target_file does not exist.");
                if (target_location.getAutoindex())
                {
					Log::Msg(DEBUG, FUNC + "autoindex is on.");
                    _target_file.erase(_target_file.find_last_of('/') + 1);
                    _auto_index = true;
                    return (0);
                }
                else
                {
                    _status_code = 403;
                    return (1);
                }
            }
            if (_isDirectory(_target_file))
            {
                _status_code = 301; // 301 Moved Permanently
                if (!target_location.getIndex().empty())
                    _location = _combinePaths(request.getPath(), target_location.getIndex()[0], "");
                else
                    _location = _combinePaths(request.getPath(), _server.getIndex()[0], "");
                if (_location[_location.length() - 1] != '/')
                    _location.insert(_location.end(), '/');

                return (1);
            }
        }
    }
    else
    {
        _target_file = _combinePaths(_server.getRoot(), request.getPath(), "");
		Log::Msg(DEBUG, FUNC + "location not found" + _target_file);
        if (_isDirectory(_target_file))
        {
            Log::Msg(DEBUG, FUNC + "_target_file is a directory.");
			if (_target_file[_target_file.length() - 1] != '/')
            {
                _status_code = 301; // 301 Moved Permanently
                _location = request.getPath() + "/";
                return (1);
            }
            _target_file += _server.getIndex()[0]; //upravit vector anebo check
            if (!_fileExists(_target_file))
            {
                _status_code = 403; // 403 Forbidden
                return (1);
            }
			// If the resulting file is still a directory, perform a redirect
			// which I dont understand can happen. example of _target_file at that moment
			// "/var/www/html/example/index.html" .
            if (_isDirectory(_target_file))
            {
				Log::Msg(DEBUG, FUNC + "_target_file is a directory - second test.");
                _status_code = 301;
                _location = _combinePaths(request.getPath(), _server.getIndex()[0], "");
                if(_location[_location.length() - 1] != '/')
                    _location.insert(_location.end(), '/');
                return (1);	
            }
        }
    }
	Log::Msg(DEBUG, FUNC + "RETURN: " + _target_file);
    return (0);
}

bool Response::_reqError()
{
    if(request.getErrorCode())
	{
		_status_code = request.getErrorCode();
		return (1);

	}
	return (0);
}

// void Response::setServerDefaultErrorPages()
// {
//     _response_body = getErrorPage(_code);
// }

// void Response::buildErrorBody()
// {
//         if( !_server.getErrorPages().count(_code) || _server.getErrorPages().at(_code).empty() ||
//          request.getMethod() == DELETE || request.getMethod() == POST)
//         {
//             setServerDefaultErrorPages();
//         }
//         else
//         {
//             if(_code >= 400 && _code < 500)
//             {
//                 _location = _server.getErrorPages().at(_code);
//                 if(_location[0] != '/')
//                     _location.insert(_location.begin(), '/');
//                 _code = 302;
//             }

//             _target_file = _server.getRoot() +_server.getErrorPages().at(_code);
//             short old_code = _code;
//             if(readFile())
//             {
//                 _code = old_code;
//                 _response_body = getErrorPage(_code);
//             }
//         }
// }

/* string checkDefaulErrorPage(short error_code, string location_key) */


/*
_buildAutoindex() function is used to generate the HTML for the autoindex page.
It takes the path of the directory to be listed as an argument and returns 
the HTML string.
It uses the dirent.h header file to read the directory entries and the stat() 
function to get the file information (is directory or file).
It feeds the directory entries into two vectors, one for directories and one for files,
and then lists them, directories first, followed by files.
 */
string 
Response::_buildAutoindex(string &path)
{
	struct dirent *entry;
	DIR *directory;
	struct stat fileInfo;
	string auto_index("");
	std::vector<string> dirList, fileList;

	directory = opendir(path.c_str());
	if (directory == NULL)
	{
		print("Error opening directory for autoindex", RED, 2);
		return ("");
	}
	while ((entry = readdir(directory)) != NULL)
	{
		string dir_name = entry->d_name; //must convert to string, d_name is char d_name[256]
		if (dir_name == "." || dir_name == "..")
			continue;
		string fullPath = path + "/" + dir_name;
		if (stat(fullPath.c_str(), &fileInfo) != 0)
		{
			cerr << "Error getting file info for " << dir_name << endl;
			continue;
		}
		if (S_ISDIR(fileInfo.st_mode))
			dirList.push_back("<a href=\"" + dir_name + "/\">" + dir_name + "/</a><br>");
		else
			fileList.push_back("<a href=\"" + dir_name + "\">" + dir_name + "</a><br>");
	}
	closedir(directory);

	auto_index += "<html><body><h1>Directory Listing</h1><ul>";
	for (std::vector<string>::const_iterator it = dirList.begin(); it != dirList.end(); ++it)
		auto_index += *it;
	for (std::vector<string>::const_iterator it = fileList.begin(); it != fileList.end(); ++it)
		auto_index += *it;
	auto_index += "</ul></body></html>";
	
	_response_body_str += auto_index;

	return auto_index;
}

void	Response::buildResponse()
{
    if (_reqError() || _buildBody())
        _response_body_str = Error::buildErrorPage(_status_code, _location_key, _server);
    if (_cgi_flag)
       return ;
	else if (_auto_index)
    {
		if (_buildAutoindex(_target_file) == "")
        {
        	_status_code = 500;
        	_response_body_str = Error::buildErrorPage(_status_code, _location_key, _server);
        }
        else
            _status_code = 200;
		_response_body_str = _buildAutoindex(_target_file);
		_status_code = 200;
		Log::Msg(DEBUG, FUNC + "Autoindex build as response");
    }
	
	Log::Msg(DEBUG, FUNC + "setting status line and headers.");
    _setStatusLine();
    _setHeaders();
	if (request.getMethod() != HEAD && (request.getMethod() == GET || _status_code != 200))
		_response_content.append(_response_body_str);
	Log::Msg(DEBUG, FUNC + "_response_content: " + _response_content);
}

// void Response::setErrorResponse(short code)
// {
//     _response_content = "";
//     _code = code;
//     _response_body = "";
//     buildErrorBody();
//     setStatusLine();
//     setHeaders();
//     _response_content.append(_response_body);
// }

// // Returns the entire reponse ( Headers + Body )
// std::string Response::getRes()
// {
//     return (_response_content);
// }

// // Returns the length of entire reponse ( Headers + Body) 
// size_t Response::getLen() const
// {
// 	return (_response_content.length());
// }

// Constructs Status line based on status code. //
void	Response::_setStatusLine()
{
    _response_content.append("HTTP/1.1 " + toString(_status_code) + " ");
	_response_content.append(Error::getErrorDescription(_status_code));
    _response_content.append("\r\n");
}

string	Response::getStatusLineCgi()
{
	string	status_line = "";
    status_line.append("HTTP/1.1 " + toString(_status_code) + " ");
	status_line.append(Error::getErrorDescription(_status_code));
    status_line.append("\r\n");

	return (status_line);
}

int    Response::_buildBody()
{
	if (request.getBody().length() > _server.getClientMaxBodySize())
	{
		Log::Msg(ERROR, FUNC + "Max body exceeded.");
		_status_code = 413;
		return (1);
	}
	if ( _handleTarget() )
		return (1);
	if (_cgi_flag || _auto_index)
		return (0);
	if (_status_code)
		return (0);
	Log::Msg(DEBUG, FUNC + _target_file);

	if (request.getMethod() == GET || request.getMethod() == HEAD)
	{
		if (_readFile())
			return (1);
		_status_code = 200;
		return (0);
	}
	else if (request.getMethod() == POST ) //|| request.getMethod() == PUT)
	{
		std::string	filename = "";
		string	upload_file_path = "";
		std::ofstream file;
		
		if (_location_key.length() > 0)
			upload_file_path = _server.getLocationKey(_location_key)->getUploadPath();
		if (upload_file_path.empty())
			upload_file_path = _server.getUploadPath();
		upload_file_path += "/";
		
		Log::Msg(DEBUG, FUNC + "POST upload_file_path: " + upload_file_path);
		
		if (request.getMultiformFlag())
		{
			std::string body = request.getBody();
			body = removeBoundary(body, request.getBoundary(), filename);
			if (!filename.empty())
			{
				upload_file_path += filename;
				if (_fileExists(upload_file_path) && request.getMethod() == POST)
				{
					// Server has successfully processed the request,
					// and there is no content to send in the response body
					// because the file to be uploaded already exists.
					_status_code = 204;
					return (0);
				}
				file.open(upload_file_path.c_str(), std::ios::binary);
				if (file.fail())
				{
					_status_code = 500;
					Log::Msg(ERROR, FUNC + "Failed to open or create file for writing.");
					return (1);
				}
				file.write(body.c_str(), body.length());
			}
			else
			{
				// No filename provided in the request body
				_status_code = 400; // Bad Request
				Log::Msg(ERROR, FUNC + "No filename for upload provided.");
				return (1);
			}
		}
		else
		{
			filename = "default.file";
			upload_file_path += filename;
			file.open(upload_file_path.c_str(), std::ios::binary);
			if (file.fail())
			{
				_status_code = 500;
				Log::Msg(ERROR, FUNC + "Failed to open or create file for writing.");
				return (1);
			}
			file.write(request.getBody().c_str(), request.getBody().length());
			Log::Msg(DEBUG, FUNC + "Not Multiform: File successfully uploaded.");
		}
	}
	else if (request.getMethod() == DELETE)
	{
		Log::Msg(DEBUG, FUNC + "file at: " + _target_file);
		if (!_fileExists(_target_file))
		{
			Log::Msg(ERROR, FUNC + "file doesnt exist at: " + _target_file);
			_status_code = 404;
			return (1);
		}

		if (remove(_target_file.c_str()) != 0 )
		{
			Log::Msg(ERROR, FUNC + "remove: " + toString(strerror(errno)));
			_status_code = 500;
			return (1);
		}
	}
	_status_code = 204; // No Content.
	return (0);
}

int Response::_readFile()
{
	std::ifstream file(_target_file.c_str());
	if (!file.is_open())
	{
		if (errno == ENOENT)
		{
			Log::Msg(ERROR, FUNC + "Website file not found");
			_status_code = 404; // Not Found
		}
		else
		{
			Log::Msg(ERROR, FUNC + "Permission denied when trying to access website file");
			_status_code = 403; // Forbidden
		}
		return (1);
	}

	std::ostringstream ss;
	ss << file.rdbuf();
	_response_body_str = ss.str();
	return (0);
}

void 	Response::setServer(Server& server)
{
	_server = server;
}

void	Response::setRequest(HttpRequest& req)
{
	request = req;
}


/*
Process a body of data containing a multipart/form-data payload
and remove the boundary lines, leaving only the actual content.
It also extracts the filename associated with each part of the payload.
*/
std::string Response::removeBoundary(std::string& body, std::string& boundary, std::string& filename) //proc je public??
{
	std::string buffer;
	std::string new_body;
	bool is_boundary = false;
	bool is_content = false;

	if (body.find("--" + boundary) != std::string::npos && body.find("--" + boundary + "--") != std::string::npos)
	{
		for (size_t i = 0; i < body.size(); i++)
		{
			buffer.clear();
			while(body[i] != '\n')
			{
				buffer += body[i];
				i++;
					
			}
			if (!buffer.compare(("--" + boundary + "--\r"))) // end of boundary
			{
					is_content = true;
					is_boundary = false;
			}
			if (!buffer.compare(("--" + boundary + "\r"))) // beginning of boundary
				is_boundary = true;
			if (is_boundary) // we are inside a boundary, after beginning bound was found
			{
				if (!buffer.compare(0, 31, "Content-Disposition: form-data;"))
				{
					size_t start = buffer.find("filename=\"");
					if (start != std::string::npos)
					{
						size_t end = buffer.find("\"", start + 10);
						if (end != std::string::npos)
						{
							filename = buffer.substr(start + 10, end - start - 10);
							Log::Msg(DEBUG, FUNC + "Filename of file transcoded in multiform setted: " + filename);
						}
					}	
				}
				else if (!buffer.compare(0, 1, "\r") && !filename.empty())
				{
						//an empty line (r) is encountered, and a filename is already set.
						is_content = true;
						is_boundary = false;
				}
			}
			else if (is_content)
			{
					if (!buffer.compare(("--" + boundary + "\r")))
						is_boundary = true;
					else if (!buffer.compare(("--" + boundary + "--\r")))
					{
						// last boundary
						new_body.erase(new_body.end() - 1);
						break ;
					}
					else
						new_body += (buffer + "\n");
			}
		}
	}
	// Log::Msg(DEBUG, FUNC + "new body after boundary removal at the end: " + new_body);
	body.clear();
	return (new_body);
}

void   Response::clear()
{
    _target_file.clear();
    _body_bytes.clear();
    _body_length = 0;
    _response_content.clear();
    _response_body_str.clear();
    _location.clear();
    _status_code = 0;
    _cgi_flag = 0;
    _cgi_response_length = 0;
    _auto_index = 0;
}

void Response::setStatusCode(short code)
{
    _status_code = code;
}