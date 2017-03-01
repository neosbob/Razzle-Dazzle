#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <ostream>
#include <string>
#include "response.hpp"
#include "request.hpp"

class HTTPClient
{
public:
	HTTPClient();
	bool establish_connection (std::string &host, std::string &port);
	std::unique_ptr <Response> send_request(const Request& request);
	~HTTPClient();
private:
	boost:asio::ip::tcp::socket socket_;
};


#endif
