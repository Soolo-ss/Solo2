//
// Created by pc4 on 2018/10/8.
//

#ifndef SOLO_CONNECTION_H
#define SOLO_CONNECTION_H

#include <boost/asio.hpp>

#include <iostream>

using boost::asio::ip::tcp;

namespace Solo::Network
{
    class Connection : public std::enable_shared_from_this<Connection>
    {
    public:
        using ConnectionPtr = std::shared_ptr<Connection>;

        Connection(tcp::socket&& socket);

        void Start();

        void Stop();

    private:
        void DoRead();

    private:
        tcp::socket socket_;

        std::vector<char, 8192> buffer_;
    };
}



#endif //SOLO_CONNECTION_H
