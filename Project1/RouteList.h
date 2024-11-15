#pragma once
#ifndef ROUTELIST_H
#define ROUTELIST_H
#include <iostream>
#include "RouteNode.h"
#include <SFML/Graphics.hpp>
using namespace std;
class RouteList
{
private:
	RouteNode* head;
public:
	RouteList();
	~RouteList();

	void insertRout(string);
	RouteNode* searchRoute(string);
	bool isUniqueName(string);
	void insertPointToRoute(string);
	void removeRoute(string);
	void displayRoutes(sf::RenderWindow& ventana);

};
#endif // !ROUTELIST_H

