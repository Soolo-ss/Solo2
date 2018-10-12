//
// Created by pc4 on 2018/10/11.
//

#ifndef SOLO_TESTCLIENT_H
#define SOLO_TESTCLIENT_H

#include <boost/asio.hpp>

#include <iostream>

using boost::asio::ip::tcp;

class TestClient
{
public:
    TestClient(tcp::endpoint point, boost::asio::io_service& service)
        : socket_(service),
          point_(point)
    { }

    void StartTest()
    {
        DoConnect(point_);
    }

    void DoConnect(tcp::endpoint point)
    {
        socket_.async_connect(point_, [this](const boost::system::error_code& ec){
            if (ec)
            {
                std::cout << ec.message() << std::endl;
            }
            else
            {
                std::cout << "Connect to point success " << std::endl;
            }

            DropSocket(std::move(socket_));
        });
    }

    void DropSocket(tcp::socket sock)
    {

    }

private:
    tcp::socket socket_;
    tcp::endpoint point_;
};


#endif //SOLO_TESTCLIENT_H
