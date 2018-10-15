#include <iostream>
#include "Network/Network.h"

#include <string>

int main()
{
    using namespace Solo::Network;


    tcp::endpoint ep(tcp::v4(), 20088);

    Network network(ep);

    network.Start();

    return 0;
}