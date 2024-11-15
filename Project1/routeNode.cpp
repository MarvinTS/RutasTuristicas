#include "routeNode.h"

RouteNode::RouteNode(string name) {
	this->name = name;
	this->posX = 0.0f;
	this->posY = 0.0f;
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
void RouteNode::setPosition(float x, float y) {
	posX = x;
	posY = y;
}
void RouteNode::setColor(sf::Color& newColor) {
	color = newColor;
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
float RouteNode::getPosX() {
	return posX;
}
float RouteNode::getPosY() {
	return posY;
}
sf::Color RouteNode::getColor() {
	return color;
}