#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <direct.h>
#include <string>
#include "RouteList.h"
#include "pointList.h"
#include "pointNode.h"

using namespace std;

int main() {
    const size_t size = 1024;
    char buffer[size];

    _getcwd(buffer, size);
    std::string rutaActual_cwd(buffer);
    std::string subruta = "\\resources\\mapa_nuevo.png";
    std::string rutaCompleta = rutaActual_cwd + subruta;
    std::cout << "Ruta completa: " << rutaCompleta << std::endl;

    sf::RenderWindow ventana(sf::VideoMode(1000, 600), "Mapa Turístico");
    sf::Texture texturaMapa;
    if (!texturaMapa.loadFromFile(rutaCompleta)) {
        std::cerr << "Error al cargar la imagen del mapa." << std::endl;
        return -1;
    }

    sf::Sprite spriteMapa;
    spriteMapa.setTexture(texturaMapa);
    RouteList rutas;
    bool modoInsercion = false;
    RouteNode* rutaActual = nullptr;

    sf::Font fuente;
    if (!fuente.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return -1;
    }

    struct PuntoTuristico {
        sf::CircleShape punto;
        sf::Text nombre;
    };

    std::vector<PuntoTuristico> puntosTuristicos;
    sf::Color colorActualPunto = sf::Color::Red;

    // VertexArray para dibujar la línea entre los puntos turísticos
    sf::VertexArray lineaRuta(sf::LinesStrip);

    std::vector<sf::Color> coloresPaleta = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta };
    float xPaleta = 50.0f, yPaleta = 500.0f;

    while (ventana.isOpen()) {
        sf::Event evento;

        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::I) {
                std::string nombreRuta;
                std::cout << "Ingrese el nombre de la ruta: ";
                std::cin >> nombreRuta;

                if (rutas.isUniqueName(nombreRuta)) {
                    rutas.insertRout(nombreRuta);
                    rutaActual = rutas.searchRoute(nombreRuta);
                }
                else {
                    std::cout << "El nombre de la ruta ya existe. Intente otro nombre." << std::endl;
                }
            }

            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left && rutaActual != nullptr) {
                float mouseX = static_cast<float>(evento.mouseButton.x);
                float mouseY = static_cast<float>(evento.mouseButton.y);

                // Verificar si se seleccionó un color de la paleta
                for (size_t i = 0; i < coloresPaleta.size(); ++i) {
                    float xColor = xPaleta + i * 40.0f;
                    if (mouseX >= xColor && mouseX <= xColor + 30 && mouseY >= yPaleta && mouseY <= yPaleta + 30) {
                        colorActualPunto = coloresPaleta[i];
                        std::cout << "Color de punto turístico seleccionado." << std::endl;
                        break;
                    }
                }

                // Cambiar color de un punto existente al hacer clic en él
                for (auto& punto : puntosTuristicos) {
                    float puntoX = punto.punto.getPosition().x;
                    float puntoY = punto.punto.getPosition().y;
                    if (mouseX >= puntoX && mouseX <= puntoX + 10 && mouseY >= puntoY && mouseY <= puntoY + 10) {
                        punto.punto.setFillColor(colorActualPunto);
                        std::cout << "Color del punto turístico cambiado." << std::endl;
                    }
                }
            }

            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Right && rutaActual != nullptr) {
                float posX = static_cast<float>(evento.mouseButton.x);
                float posY = static_cast<float>(evento.mouseButton.y);

                // Crear un círculo para el nuevo punto turístico
                sf::CircleShape puntoTuristico(5.0f);
                puntoTuristico.setFillColor(colorActualPunto);
                puntoTuristico.setPosition(posX, posY);

                // Crear un texto para el nombre de la ruta en el mismo punto
                sf::Text textoRuta(rutaActual->getName(), fuente, 12);
                textoRuta.setFillColor(sf::Color::Black);
                textoRuta.setPosition(posX + 10, posY + 10);

                puntosTuristicos.push_back({ puntoTuristico, textoRuta });

                // Agregar el nuevo punto al VertexArray para la línea de la ruta
                lineaRuta.append(sf::Vertex(sf::Vector2f(posX + 5.0f, posY + 5.0f), colorActualPunto));

                std::cout << "Punto turístico añadido en (" << posX << ", " << posY << ") con color seleccionado." << std::endl;
            }
        }

        ventana.clear(sf::Color::White);
        ventana.draw(spriteMapa);

        if (rutaActual != nullptr) {
            sf::Text textoRutaPrincipal(rutaActual->getName(), fuente, 20);
            textoRutaPrincipal.setPosition(rutaActual->getPosX(), rutaActual->getPosY());
            textoRutaPrincipal.setFillColor(rutaActual->getColor());
            ventana.draw(textoRutaPrincipal);

            // Dibujar los puntos turísticos y sus nombres
            for (const auto& punto : puntosTuristicos) {
                ventana.draw(punto.punto);
                ventana.draw(punto.nombre);
            }

            // Dibujar la línea de la ruta
            if (lineaRuta.getVertexCount() > 1) {
                ventana.draw(lineaRuta);
            }

            // Dibujar la paleta de colores para los puntos turísticos
            for (size_t i = 0; i < coloresPaleta.size(); ++i) {
                sf::RectangleShape cuadro(sf::Vector2f(30.0f, 30.0f));
                cuadro.setFillColor(coloresPaleta[i]);
                cuadro.setPosition(xPaleta + i * 40.0f, yPaleta);
                ventana.draw(cuadro);
            }
        }

        rutas.displayRoutes(ventana);
        ventana.display();
    }

    return 0;
}