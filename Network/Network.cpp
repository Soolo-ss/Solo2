//
// Created by pc4 on 2018/10/8.
//

#include "Network.h"

using namespace Solo::Network;

void Network::Start() {
    DoAccept();

    service_.run();
}

void Network::DoAccept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec)
        {
            auto newConnection = std::make_shared<Connection>(std::move(socket_));
            newConnectionID_++;

            connections_[newConnectionID_] = newConnection;
        }
        else
        {
            abort();
        }

        DoAccept();
    });
}
