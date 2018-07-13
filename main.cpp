#include <iostream>
#include <chrono>

#include "AOI/CrossAOIServer.h"
#include "Common/ObjectPool.h"

#include <vector>

int main()
{
    CrossAOIServer aoi;

    auto pool = std::make_shared<ObjectPool<CrossAOINode>>();

    std::vector<std::shared_ptr<CrossAOINode>> nodes;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i != 100000; ++i)
    {
        std::shared_ptr<CrossAOINode> node = pool->acquire();

        node->x((float)rand());

        aoi.insert(node.get());

        nodes.push_back(node);
    }

    std::cout << pool->size() << std::endl;
    std::cout << pool->all() << std::endl;

    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    return 0;
}