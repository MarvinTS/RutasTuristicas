#pragma once
#include <iostream>
#ifndef POINTNODE_H
#define POINTNODE_H
using namespace std;

class PointNode
{
private:
    string name;
    int x; 
    int y;
    PointNode* next;
    PointNode* prev;
public:
    PointNode();
    PointNode(string, int, int);
    ~PointNode();

    void setName(string);
    void setX(int);
    void setY(int);
    void setNext(PointNode* next);
    void setPrev(PointNode* prev);

};
#endif // !POINTNODE_H
