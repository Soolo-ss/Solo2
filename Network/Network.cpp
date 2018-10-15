//
// Created by pc4 on 2018/10/8.
//

#include "Network.h"

using namespace Solo::Network;

Network::Network(tcp::endpoint point)
    : service_(),
      acceptor_(service_, point),
      socket_(service_),
      endpoint_(point),
      connectionManager_()
{ }

void Network::Start() {
    DoAccept();

    service_.run();
}

void Network::DoAccept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            auto newConnection = std::make_shared<Connection>(std::move(socket_));

            connectionManager_.Start(newConnection);
        } else {
            abort();
        }

        DoAccept();
    });
}
