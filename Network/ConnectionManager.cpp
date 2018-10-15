//
// Created by pc4 on 2018/10/15.
//

#include "ConnectionManager.h"

#include <iostream>

using namespace Solo::Network;

ConnectionManager::ConnectionManager()
    : connections_()
{ }

void ConnectionManager::Start(Connection::ConnectionPtr connection) {
    connections_.insert(connection);
    connection->Start();
}

void ConnectionManager::Stop(Connection::ConnectionPtr connection) {
    connection->Stop();
    connections_.erase(connection);
}
