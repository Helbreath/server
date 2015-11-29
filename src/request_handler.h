
#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <boost/noncopyable.hpp>

struct reply;
struct request;
class Client;

/// The common handler for all incoming requests.
class request_handler : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler();

  /// Handle a request and produce a reply.
  void handle_request(const request& req);
};

#endif // HTTP_REQUEST_HANDLER_HPP
