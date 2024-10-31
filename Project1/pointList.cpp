#include "pointList.h"
PointList::PointList() {
	this->head = nullptr;
}
PointList::~PointList() {

}

void PointList::insertPoint(string name, int x, int y) {
    PointNode* newNode = new PointNode(name, x, y);
    if (head == nullptr) {
        head = newNode;
    }
    else {
        PointNode* current = head;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(newNode);
        newNode->setPrev(current);
    }
}
bool PointList::isUniqueName(string name) {
    PointNode* current = head;
    while (current != nullptr) {
        if (current->getName() == name) {
            return false;
        }
        current = current->getNext();
    }
    return true;
}

void PointList::displayPoint() {
    PointNode* current = head;
    while (current != nullptr) {
        cout << current->getName() << ", ";
        current = current->getNext();
    }
}