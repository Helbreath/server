
#include "../utility.hpp"
#include "net_handler.hpp"
#include "../server.hpp"
#include "../client.hpp"

#include <asio/ip/basic_resolver.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/connect.hpp>
#include <asio/streambuf.hpp>
#include <asio/read.hpp>
#include <asio/read_until.hpp>
#include <asio/ip/address.hpp>

namespace hbx
{

net_handler::net_handler(std::shared_ptr<asio::io_context> io_, server * _server)
    : io_context_(io_)
    , ctx(asio::ssl::context::tlsv13_server)
    , acceptor_(*io_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 2848))
    , server_(_server)
{

    ctx.set_options(asio::ssl::context::default_workarounds
        | asio::ssl::context::no_sslv2
        | asio::ssl::context::single_dh_use);

    ctx.use_certificate_chain_file("config/server.crt");
    ctx.use_private_key_file("config/server.key", asio::ssl::context::pem);

#pragma region dh
    char dh[] = "-----BEGIN DH PARAMETERS-----\n"
        "MIICCAKCAgEArOGgxAq72jKvDwwU4PqVTpEUAJqFboyaXUu/E9p7dE1BLfgpG59o\n"
        "xYPI3iS18aXnZL+v7J8kDsLj0tbAh0H8VC0GesgUSOprv8AlwErrk6H1PGNQEIhR\n"
        "v7RLq2TXp4hJMyMjuQ+m7oKNCZ910Igxa185qTN7cNs6WCiFVEgXVki5Lb4F+Jn4\n"
        "9Q3BvofDuDCUX3xOEGgBQemfnlaEtaJyd2zz7JCGMZ4AY0lPXMktxrY6MGEcFema\n"
        "PU3SxRdlIdBfqLn2+tE0smPRuNf3zAJrbp9SsPevwuxRbrPd+z86SpAvbBB93tAB\n"
        "1J8L1+dy2DvdR7+MAJX+IxSToF6j0Kk1I8vugCV2Sn7p6BNrjAw3OUP24pWBi1I/\n"
        "6otETlb2OhLvcy2D/KUxYP56XLu4SqSEI1EnPSYfNUto870Ze0c94gLaIKrpVi5B\n"
        "TLKN+Lkt5bO2GONSKdBhCKpisxHAZr0dYizEDE1kLhTC9xrDQUEoOC32ZXY8HH2e\n"
        "7kX+/uPDm21OwmY0Qput4AHhe5aeMgrX//A3mHHMhlwzzDI+4x74lELxt4hsOSme\n"
        "bw7N6vl1s4ECHrL/hPnlubW/g2T0WrMFJVHiToPMN9x0Cl6h79PTyYZIVLSzxkLq\n"
        "oOs10FJt0IBvSDSCpaLTKIUCwLHPzqfSoeaenKl0wrRvuwyqdPu2qdsCAQI=\n"
        "-----END DH PARAMETERS-----\n";
#pragma endregion

    std::string dhstr = dh;
    asio::const_buffer dh_buff(dhstr.c_str(), dhstr.length());
    ctx.use_tmp_dh(dh_buff);

    new_socket_ = std::make_shared<socket>(*io_context_, *this, ctx);

    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.listen();
    acceptor_.async_accept(new_socket_->get_socket(), std::bind(&net_handler::handle_accept, this, std::placeholders::_1));
}

net_handler::~net_handler()
{

}

void net_handler::handle_accept(const asio::error_code & e)
{
    if (!e)
    {
        std::shared_ptr<socket> clientsocket = new_socket_;

        asio::ip::tcp::endpoint endp = clientsocket->get_socket().remote_endpoint();
        asio::ip::address address = endp.address();
        clientsocket->address = address.to_string();
        server_->log->info("Client connected: {}:{}", clientsocket->address, endp.port());

        std::shared_ptr<client> client_ = std::make_shared<client>();
        //client->lastpackettime = unixtime();
        client_->socket_ = clientsocket;
        clientsocket->client_ = client_.get();
        {
            std::unique_lock<std::mutex> l(server_->cl_m);
            server_->clients.insert(client_);
        }

        start(clientsocket);

        new_socket_ = std::make_shared<socket>(*io_context_, *this, ctx);
        acceptor_.async_accept(new_socket_->get_socket(), std::bind(&net_handler::handle_accept, this, std::placeholders::_1));
    }
}

void net_handler::start(std::shared_ptr<socket> c)
{
    connections.insert(c);
    c->start();
}

void net_handler::stop(std::shared_ptr<socket> c)
{
    c->stop();
    connections.erase(c);
    server_->close_client(c->client_->shared_from_this());
}

void net_handler::stop()
{
    acceptor_.close();
    for (const auto& c : connections)
        c->stop();
    connections.clear();
}

void net_handler::handle_request(const request & req)
{
    server & s = *server_;

    try
    {
        message_entry msg{ req.data, req.size };
        s.handle_message(msg, req.socket_->client_->shared_from_this());
    }
    catch (std::exception & e)
    {
        s.log->error("handle_request() exception: %s", e.what());
        stop(req.socket_->shared_from_this());
    }
}


}
