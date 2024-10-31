#include <SFML/Graphics.hpp>
#include <vector>
#include "RouteList.h"
#include "pointList.h"
#include "pointNode.h"
int main() { 

    RouteList list;
    list.insertRout("R1");
    list.insertRout("R2");
    list.insertRout("R3");
    list.insertRout("R4");

    list.insertPointToRoute("R2");
    list.insertPointToRoute("R2");

    list.removeRoute("R1");

    list.displayRoutes();
    cout << endl;
    RouteNode* node = list.searchRoute("R2");
    if (node != nullptr) {
        node->getPointList().displayPoints();
    }

    return 0;
}