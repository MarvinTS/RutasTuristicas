#pragma once
#include "pointList.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef ROUTENODE_H
#define ROUTENODE_H

class RouteNode
{
private:
    string name;
    PointList pointList;
    RouteNode* next;
    RouteNode* prev;
    float posX, posY;
    sf::Color color;
public:
    RouteNode();
    RouteNode(string);
    ~RouteNode();

    void setName(string);
    void setPointList(PointList);
    void setNext(RouteNode*);
    void setPrev(RouteNode*);
    void setPosition(float, float);
    void setColor(sf::Color&);

    string getName();
    PointList& getPointList();
    RouteNode* getNext();
    RouteNode* getPrev();
    float getPosX();
    float getPosY();
    sf::Color getColor();

};
#endif // !ROUTENODE_H
