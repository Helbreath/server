
#include <cstdio>
#include "server.hpp"

using namespace std::chrono_literals;

hbx::server * hbxserver;

std::shared_ptr<asio::io_context> io_context_ = nullptr;

#ifndef _WIN32
void sigfunc(int sig_no)
{
    if (sig_no == SIGINT)
    {
    }
    else if (sig_no == SIGHUP)
    {
    }
}
#endif

int main()
{
#ifndef _WIN32
    struct sigaction sa = { 0 };
    sa.sa_handler = &sigfunc;
    sigaction(SIGHUP, &sa, 0);
    sigaction(SIGUSR1, &sa, 0);
    sigaction(SIGUSR2, &sa, 0);
#endif

    io_context_ = std::make_shared<asio::io_context>();
    hbxserver = new hbx::server(io_context_);
    hbxserver->start_lserver();
    hbxserver->start_gservers();
    hbxserver->set_status(hbx::server_status::online);
    hbxserver->run();
//     std::mutex m;
//     std::unique_lock<std::mutex> l(m);
//     hbxserver->cv.wait(l);
    return 0;
}