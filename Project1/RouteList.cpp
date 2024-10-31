#include "RouteList.h"
RouteList::RouteList() {
	this->head = nullptr;
}
RouteList::~RouteList() {

}

void RouteList::insertRout(string name) {
    RouteNode* newNode = new RouteNode(name);
    if (head == nullptr) {
        head = newNode;
    }
    else {
        RouteNode* current = head;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(newNode);
        newNode->setPrev(current);
    }
}

RouteNode* RouteList::searchRoute(string name) {
    RouteNode* current = head;
    while (current != nullptr) {
        if (current->getName() == name) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

bool RouteList::isUniqueName(string name) {
    RouteNode* current = head;
    while (current != nullptr) {
        if (!current->getPointList().isUniqueName(name)) {
            return false;
        }
        current = current->getNext();
    }
    return true;
}

void RouteList::insertPointToRoute(string name) {
    RouteNode* current = head;
    while (current != nullptr && current->getName() != name) {
        current = current->getNext();
    }
    if (current != nullptr) {
        string name;
        cout << "Nombre del punto: ";
        cin >> name;
        if (isUniqueName(name)) {
            current->getPointList().insertPoint(name, 50, 100);
        }
    }
}

void RouteList::removeRoute(string name) {
    if (head == nullptr) return;

    if (head->getName() == name) {
        RouteNode* temp = head;
        head = head->getNext();
        delete temp;
        return;
    }

    RouteNode* current = head->getNext();

    while (current != nullptr) {
        if (current->getName() == name) {
            current->getPrev()->setNext(current->getNext());
            current->getNext()->setPrev(current->getPrev());
            delete current;
            return;
        }
        current = current->getNext();
    }
}

void RouteList::displayRoutes() {
    RouteNode* current = head;
    while (current != nullptr) {
        std::cout << current->getName() << ", ";
        current = current->getNext();
    }
}
