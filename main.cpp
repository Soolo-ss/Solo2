#include <iostream>

#include "AOI/CrossAOIServer.h"

#include "Common/ObjectPool.h"
#include "Common/Singleton.h"

#include <memory>

int main()
{

    auto pool = std::make_shared<ObjectPool<CrossAOINode>>();

    auto node = pool->acquire();

    node->x(2.3);

    auto node2 = pool->acquire();

    node->x(3.3);

    CrossAOIServer aoi;

    aoi.insert(node.get());
    aoi.insert(node.get());

    return 0;
}