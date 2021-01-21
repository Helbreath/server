

#pragma once

#include <memory>
#include <string>

namespace hbx
{

class gserver;
class client;

class chat_handler
{
public:
	chat_handler(gserver * gs_);
	~chat_handler() = default;

	void parse_chat(const std::shared_ptr<client> & client_, std::string message);

	gserver * gs;
};

}
