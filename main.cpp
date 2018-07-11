#include <iostream>

#include "AOI/CrossAOIServer.h"

#include "Common/ObjectPool.h"
#include "Common/Singleton.h"

#include <memory>

int main()
{
	while(true)
	{
		auto pool = std::make_shared<ObjectPool<CrossAOINode>>();

		auto node = pool->acquire();
		auto node2 = pool->acquire();
		auto node3 = pool->acquire();

		node->x(2.3);
		node2->x(3.3);
		node3->x(3.5);

		CrossAOIServer aoi;

		aoi.insert(node.get());
		aoi.insert(node2.get());
		aoi.insert(node3.get());

		aoi.remove(node2.get());

		std::cout << pool->size() << std::endl;
	}

    return 0;
}