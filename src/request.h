
#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include <stdint.h>

class connection;

/// A request received from a client.
struct request
{
	int32_t size;
	std::string cmd;
	std::string uri;
	char * data;
	class connection * connection;
};

#endif // HTTP_REQUEST_HPP
