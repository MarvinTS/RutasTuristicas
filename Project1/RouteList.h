#pragma once
#ifndef ROUTELIST_H
#define ROUTELIST_H
#include <iostream>
#include "RouteNode.h"
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
	void displayRoutes();
};
#endif // !ROUTELIST_H

