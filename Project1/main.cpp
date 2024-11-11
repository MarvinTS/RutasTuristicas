#include <SFML/Graphics.hpp>
#include <vector>
#include "RouteList.h"
#include "pointList.h"
#include "pointNode.h"

void cargarMapa(const std::string& nombreArchivo) {
    // Crear una ventana con resolución de 800x600 píxeles
    sf::RenderWindow ventana(sf::VideoMode(1000, 600), "Mapa Turístico");

    // Cargar la imagen del mapa desde el nombre del archivo dado
    sf::Texture texturaMapa;
    if (!texturaMapa.loadFromFile(nombreArchivo)) {
        return;  // Manejo de error si la imagen no se carga correctamente
    }

    // Crear un sprite para mostrar el mapa
    sf::Sprite spriteMapa;
    spriteMapa.setTexture(texturaMapa);

    // Bucle principal para mantener la ventana abierta
    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        // Limpiar la ventana y dibujar el mapa
        ventana.clear();
        ventana.draw(spriteMapa);
        ventana.display();
    }
}




int main() { 

    cargarMapa("C:/Users/jafet/source/repos/Project1/x64/Debug/mapa_nuevo.png");

    RouteList list;
    list.insertRout("R1");
    list.insertRout("R2");
    list.insertRout("R3");
    list.insertRout("R4");

    list.insertPointToRoute("R2");
    list.insertPointToRoute("R2");

    list.removeRoute("R1");

    list.displayRoutes();
    cout << endl;
    RouteNode* node = list.searchRoute("R2");
    if (node != nullptr) {
        node->getPointList().displayPoints();
    }

    return 0;
}