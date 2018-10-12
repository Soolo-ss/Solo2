//
// Created by pc4 on 2018/10/11.
//

#include <iostream>

#include "./TestClient.h"

int main()
{
    boost::asio::io_service service;

    tcp::endpoint localpoint = tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 20088);

    TestClient client(localpoint, service);

    client.StartTest();

    service.run();

    return 0;
}