//inspired by http://www.boost.org/doc/libs/1_49_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include "http_client.hpp"
#include "response.hpp"
#include "request.hpp"

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

bool HTTPClient::establish_connection (std::string &host, std::string &port)
{
	boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, port);
	boost::system::error_code e_c;
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, e_c), end;
	if (e_c)
		return false;

    // Try each endpoint until we successfully establish a connection.
	socket_ = new boost::asio::ip::tcp::socket(io_service);
    boost::asio::connect(socket_, endpoint_iterator);

	return true;

}

std::unique_ptr <Response> HTTPClient::send_request(const Request& request)
{
	//send the request and returns response
	boost::asio::streambuf req_buf;
	std::ostream request_stream(&req_buf);
	request_stream << request.ToString();
	
	boost::asio::write(socket_, req_buf);
	
	//read data 
	boost::asio::streambuf raw_resp;
	boost::system::error_code e_c;
	std::string string_read;
	std::size_t bytes_read;

	while (bytes_read = boost::asio::read (socket_, raw_resp, boost::asio::transfer_at_least(1), e_c))
	{

		std::string data = std::string(boost::asio::buffers_begin(raw_resp.data()), boost::asio::buffers_begin(raw_resp.data()) + bytes_read);
		string_read += data;
		raw_resp.consume(bytes_read);

	}
	

    auto response = Response::Parse(string_read);
	return response;

}


