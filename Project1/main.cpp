#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <direct.h>
#include <string>
#include "RouteList.h"
#include "pointList.h"
#include "pointNode.h"

using namespace std;

// Función de interpolación Catmull-Rom para suavizar las curvas
sf::Vector2f catmullRom(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    float b0 = -t3 + 2.0f * t2 - t;
    float b1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
    float b2 = -3.0f * t3 + 4.0f * t2 + t;
    float b3 = t3 - t2;

    return 0.5f * (p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3);
}

std::vector<sf::Vector2f> generateCatmullRomCurve(const std::vector<sf::Vector2f>& points) {
    std::vector<sf::Vector2f> curvePoints;
    if (points.size() < 2) return curvePoints;

    // Extender los extremos para incluir las curvas en los primeros y últimos segmentos
    sf::Vector2f firstPoint = points.front();
    sf::Vector2f lastPoint = points.back();
    std::vector<sf::Vector2f> extendedPoints = { firstPoint, firstPoint };
    extendedPoints.insert(extendedPoints.end(), points.begin(), points.end());
    extendedPoints.push_back(lastPoint);
    extendedPoints.push_back(lastPoint);

    for (size_t i = 1; i < extendedPoints.size() - 2; ++i) {
        for (float t = 0; t <= 1; t += 0.05f) {
            curvePoints.push_back(catmullRom(extendedPoints[i - 1], extendedPoints[i], extendedPoints[i + 1], extendedPoints[i + 2], t));
        }
    }
    return curvePoints;
}

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
    std::vector<sf::Vector2f> puntosRuta;

    std::vector<sf::Color> coloresPaleta = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta };
    float xPaleta = 50.0f, yPaleta = 500.0f;

    std::string nombreRuta;
    while (ventana.isOpen()) {
        sf::Event evento;

        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::I) {
              
                std::cout << "Ingrese el nombre de la ruta: ";
                std::cin >> nombreRuta;

                // Verificar si el nombre de la ruta es único
                if (rutas.isUniqueRouteName(nombreRuta)) {
                    rutas.insertRoute(nombreRuta);
                    rutaActual = rutas.searchRoute(nombreRuta);
                }
                else {
                    nombreRuta = "";
                    std::cout << "El nombre de la ruta ya existe. Intente otro nombre." << std::endl;
                    continue;
                }
            }

            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left && rutaActual != nullptr && nombreRuta!= "") {
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

            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Right && rutaActual != nullptr && nombreRuta != "") {
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
                puntosRuta.push_back(sf::Vector2f(posX + 5.0f, posY + 5.0f));
                nombreRuta = "";
                std::cout << "Punto turístico insertado en (" << posX << ", " << posY << ") con color seleccionado." << std::endl;
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

            // Dibujar las curvas de la ruta
            if (puntosRuta.size() >= 2) {
                std::vector<sf::Vector2f> curva = generateCatmullRomCurve(puntosRuta);
                sf::VertexArray curvaRuta(sf::LineStrip, curva.size());
                for (size_t i = 0; i < curva.size(); ++i) {
                    curvaRuta[i].position = curva[i];
                    curvaRuta[i].color = colorActualPunto;
                }
                ventana.draw(curvaRuta);
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