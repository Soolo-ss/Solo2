//
// Created by pc4 on 2018/10/8.
//

#include "Connection.h"

using namespace Solo::Network;

Connection::Connection(tcp::socket&& socket)
    : socket_(std::move(socket))
{ }

void Connection::Start() {
    DoRead();
}

void Connection::Stop() {

}

void DoRead() {

}
