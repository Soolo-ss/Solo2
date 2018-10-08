//
// Created by pc4 on 2018/10/8.
//

#ifndef SOLO_CONNECTION_H
#define SOLO_CONNECTION_H

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    using ConnectionPtr = std::shared_ptr<Connection>;

    Connection(tcp::socket socket)
        : socket_(std::move(socket))
    {
        onAccept();
    }

    void onAccept()
    {

    }

private:
    tcp::socket socket_;
};


#endif //SOLO_CONNECTION_H
