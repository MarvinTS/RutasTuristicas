#pragma once
#include "pointList.h"
#include <iostream>
#ifndef ROUTENODE_H
#define ROUTENODE_H

class RouteNode
{
private:
    string name;
    PointList pointList;
    RouteNode* next;
    RouteNode* prev;
public:
    RouteNode();
    RouteNode(string);
    ~RouteNode();

    void setName(string);
    void setPointList(PointList);
    void setNext(RouteNode*);
    void setPrev(RouteNode*);

    string getName();
    PointList& getPointList();
    RouteNode* getNext();
    RouteNode* getPrev();

};
#endif // !ROUTENODE_H
