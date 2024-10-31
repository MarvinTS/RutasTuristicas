#include "routeNode.h"

RouteNode::RouteNode(string name) {
	this->name = name;
}

RouteNode::~RouteNode() {

}

void RouteNode::setName(string name) {
	this->name = name;
}
void RouteNode::setNext(RouteNode* next) {
	this->next = next;
}
void RouteNode::setPrev(RouteNode* prev) {
	this->prev = prev;
}
void RouteNode::setPointList(PointList poinList) {
	this->pointList = poinList;
}

string RouteNode::getName() {
	return this->name;
}
RouteNode* RouteNode::getNext() {
	return this->next;
}
RouteNode* RouteNode::getPrev() {
	return this->prev;
}
PointList& RouteNode::getPointList() {
	return this->pointList;
}
