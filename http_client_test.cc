#include "gtest/gtest.hpp"
#include "http_client.hpp"
#include "request.hpp"
#include "response.hpp"

TEST(ClientTest, establish_connection)
{

	HTTPClient c;
	EXPECT_TRUE(c.establish_connection("www.google.com","http"));
	EXPECT_FALSE(c.establish_connection("www.google.com", "invalid"));
}

/*
TEST(ClientTest, send_request)
{
	HTTPClient c;
	auto request = Request::Parse("GET / HTTP/1.1\r\nHost:www.example.com\r\nConnection close\r\n\r\n);
	ASSERT_NE(request.get(), nullptr);
	


}
*/
