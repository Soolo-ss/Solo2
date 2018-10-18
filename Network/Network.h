//
// Created by pc4 on 2018/10/8.
//

#ifndef SOLO_NETWORK_H
#define SOLO_NETWORK_H

#include "Connection.h"
#include "ConnectionManager.h"

#include <unordered_map>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Solo
{
    class Network
    {
    public:
        Network(tcp::endpoint point);

        void Start();

        void DoAccept();

    private:
        boost::asio::io_service service_;
        tcp::acceptor acceptor_;
        tcp::socket socket_;
        tcp::endpoint endpoint_;

        ConnectionManager connectionManager_;
    };
}

#endif //SOLO_NETWORK_H
