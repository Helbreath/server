
#include "../utility.hpp"
#include "net_handler.hpp"
#include "socket.hpp"
#include "../server.hpp"

#include <asio/io_context.hpp>
#include <asio/placeholders.hpp>
#include <asio/read.hpp>
#include <vector>
#include <functional>

namespace hbx
{

socket::socket(asio::io_context & io_service, net_handler & handler, asio::ssl::context & context)
    : socket_(io_service, context),
    nh(handler)
{
    size = 0;
    handshake_complete = false;
}

void socket::start()
{
    asio::error_code ec;
    socket_.handshake(asio::ssl::stream_base::server, ec);

    handshake_complete = true;

    if (!ec)
    {
        asio::async_read(socket_, asio::buffer(buffer_, 2), std::bind(&socket::handle_read_header, shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
    }
    else
    {
        //error
        nh.server_->log->error("connection exception: {}", ec.message().c_str());
        stop();
    }
}

void socket::stop()
{
    try
    {
        socket_.lowest_layer().close();
    }
    catch (std::exception & e)
    {
        nh.server_->log->error("connection::stop() exception: {}", e.what());
    }
}

void socket::write(const char * data, const uint64_t size)
{
    try
    {
        char csize[2] = { 0, 0 };
        uint16_t tsize = uint16_t(size);
        *(uint16_t *)csize = tsize;
        std::array<asio::const_buffer, 2> buffers = {
            asio::buffer(csize, 2),
            asio::buffer(data, size)
        };
        asio::async_write(socket_, buffers, [this](asio::error_code ec, std::size_t) {
            if (ec)
            {
                //error
            }
        });
    }
    catch (std::exception & e)
    {
        nh.server_->log->error("asio::write_some() exception: {}", e.what());
    }
}

void socket::write(stream_write & sw)
{
    try
    {
        char csize[2] = { 0, 0 };
        uint16_t tsize = uint16_t(sw.position);
        *(uint16_t *)csize = tsize;
        std::array<asio::const_buffer, 2> buffers = { asio::buffer(csize, 2), asio::buffer(sw.data, sw.position) };
        if (socket_.lowest_layer().is_open())
            asio::write(socket_, buffers);
    }
    catch (std::exception & e)
    {
        nh.server_->log->error("asio::write_some() exception: {}", e.what());
    }
}

void socket::handle_read_header(const asio::error_code & e, std::size_t bytes_transferred)
{
    try
    {
        if (!e && bytes_transferred == 2)
        {
            size = *(int16_t *)((char *)buffer_.data());
            if (size > 8192 || size <= 0)//temporary set size .. shouldn't really be more than this anyway//2048
            {
                nh.server_->log->error("Invalid packet size : {}", size);
                nh.stop(shared_from_this());
                return;
            }
            asio::async_read(socket_, asio::buffer(buffer_, size), std::bind(&socket::handle_read, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
        }
        else if (e != asio::error::operation_aborted)
        {
            // socket was already closed
            nh.stop(shared_from_this());
            return;
        }
        else
        {
            nh.server_->log->error("handle_read_header() error");
            nh.stop(shared_from_this());
            return;
        }
    }
    catch (std::exception & ex)
    {
        nh.server_->log->error("handle_read() error - {}", ex.what());
    }
}

void socket::handle_read(const asio::error_code & e,
    std::size_t bytes_transferred)
{
    try
    {
        if (!e)
        {
            if (bytes_transferred != size)
            {
                nh.server_->log->error("Did not receive proper amount of bytes : rcv: {} needed: {}", (int32_t)bytes_transferred, size);
                nh.stop(shared_from_this());
                return;
            }
            //printf("uid("XI64")\n", uid);
            // read object size
            if ((size > 8192) || (size <= 0))//2048
            {
                //ERROR - object too large - close connection
                nh.server_->log->error("Invalid packet size : {}", size);
                nh.stop(shared_from_this());
                return;
            }

            // parse packet
            request_.size = size;
            request_.data = new char[size + 1];
            std::memcpy(request_.data, buffer_.data(), size);
            request_.socket_ = this;
            nh.handle_request(request_);

            asio::async_read(socket_, asio::buffer(buffer_, 2), std::bind(&socket::handle_read_header, shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
        }
        else if (e != asio::error::operation_aborted)
        {
            nh.stop(shared_from_this());
            return;
        }
        else
        {
            nh.server_->log->error("handle_read() error");
            nh.stop(shared_from_this());
            return;
        }
    }
    catch (std::exception & ex)
    {
        nh.server_->log->error("handle_read() error - {}", ex.what());
    }
}

void socket::handle_write(const asio::error_code & e)
{
    if (!e)
    {
        // 		// Initiate graceful connection closure.
        // 		asio::error_code ignored_ec;
        // 		socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }

    if (e != asio::error::operation_aborted)
    {
        nh.stop(shared_from_this());
    }
}

}
