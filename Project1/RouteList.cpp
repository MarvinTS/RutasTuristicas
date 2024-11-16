#include "RouteList.h"


RouteList::RouteList() {
	this->head = nullptr;
}
RouteList::~RouteList() {

}

void RouteList::insertRoute(string name) {
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

bool RouteList::isUniqueRouteName(string name) {
    RouteNode* current = head;
    while (current != nullptr) {
        if (current->getName() == name) {
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
        if (isUniqueRouteName(name)) {
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

void RouteList::displayRoutes(sf::RenderWindow& ventana) {
    sf::Font fuente;
    if (!fuente.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return;
    }

    RouteNode* current = head;
    while (current != nullptr) {
        sf::Text textoRuta(current->getName(), fuente, 20);
        textoRuta.setPosition(current->getPosX(), current->getPosY());
        textoRuta.setFillColor(current->getColor());  // Usar el color asignado a la ruta
        ventana.draw(textoRuta);

        current = current->getNext();
    }
}
