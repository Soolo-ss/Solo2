//
// Created by pc4 on 2018/10/15.
//

#ifndef SOLO_CONNECTIONMANAGER_H
#define SOLO_CONNECTIONMANAGER_H

#include "Connection.h"

#include <set>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Solo {
    class ConnectionManager {
    public:
        ConnectionManager();

        //new connection
        void Start(Connection::ConnectionPtr connection);

        void Stop(Connection::ConnectionPtr connection);

    private:
        std::set<Connection::ConnectionPtr> connections_;
    };
}

#endif //SOLO_CONNECTIONMANAGER_H
