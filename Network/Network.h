//
// Created by pc4 on 2018/10/8.
//

#ifndef SOLO_NETWORK_H
#define SOLO_NETWORK_H

#include <boost/asio.hpp>

#include <unordered_map>

#include "Connection.h"

using boost::asio::ip::tcp;

namespace Solo::Network
{
    class Network
    {
    public:
        Network()
            : acceptor_(service_),
              socket_(service_),
              newConnectionID_(0)
        { }

        void Start();

        void DoAccept();

    private:
        boost::asio::io_service service_;
        tcp::acceptor acceptor_;
        tcp::socket socket_;

        uint64_t newConnectionID_;
        std::unordered_map<uint64_t, Connection::ConnectionPtr> connections_;
    };
}

#endif //SOLO_NETWORK_H
