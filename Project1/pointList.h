#pragma once
#ifndef POINTLIST_H
#define POINTLIST_H
#include <iostream>
#include "PointNode.h"
using namespace std;

class PointList
{
private:
	PointNode* head;
public:
	PointList();
	~PointList();

	void insertPoint(string, int x, int y);
	bool isUniqueRouteName(string);
	void displayPoints();
	
};
#endif // !POINTLIST_H

