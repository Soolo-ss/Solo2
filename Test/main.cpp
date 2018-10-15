//
// Created by pc4 on 2018/10/11.
//

#include <iostream>
#include <vector>

#include "./TestClient.h"

int main()
{
    std::vector<int> a;

    for (int i = 0; i != 10000; ++i)
    {
        a.push_back(i);
    }

    a.clear();


    std::cout << a.capacity() << std::endl;

    a.push_back(3);

    std::cout << a.capacity() << std::endl;


    boost::asio::io_service service;

    tcp::endpoint localpoint = tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 20088);

    TestClient client(localpoint, service);

    client.StartTest();

    service.run();

    return 0;
}