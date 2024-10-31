#include "pointNode.h"
PointNode::PointNode() {

}
PointNode::PointNode(string name, int x, int y) {
    this->name = name;
    this->x = x;
    this->y = y;
    this->next = nullptr;
    this->prev = nullptr;
}
PointNode::~PointNode() {

}

void PointNode::setName(string name) {
    this->name = name;
}
void PointNode::setX(int x) {
    this->x = x;
}
void PointNode::setY(int y) {
    this->y = y;
}
void PointNode::setNext(PointNode* next) {
    this->next = next;
}
void PointNode::setPrev(PointNode* prev) {
    this->prev = prev;
}

string PointNode::getName() {
    return this->name;
}
int PointNode::getX() {
    return this->x;
}
int PointNode::getY() {
    return this->y;
}
PointNode* PointNode::getNext() {
    return this->next;
}
PointNode* PointNode::getPrev() {
    return this->prev;
}
