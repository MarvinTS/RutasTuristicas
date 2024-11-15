#include <SFML/Graphics.hpp>
#include <vector>
#include "RouteList.h"
#include "pointList.h"
#include "pointNode.h"

void cargarMapa(sf::RenderWindow& ventana, const std::string& nombreArchivo, sf::Texture& texturaMapa, sf::Sprite& spriteMapa) {
    if (!texturaMapa.loadFromFile(nombreArchivo)) {
        std::cerr << "Error al cargar la imagen del mapa." << std::endl;
        return;
    }
    spriteMapa.setTexture(texturaMapa);
}
void seleccionarPuntosTuristicos(RouteNode* rutaActual, sf::RenderWindow& ventana, sf::Sprite& spriteMapa) {
    if (!rutaActual) return;

    sf::Font fuente;
    if (!fuente.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return;
    }

    struct PuntoTuristico {
        sf::CircleShape punto;
        sf::Text nombre;
    };

    std::vector<PuntoTuristico> puntosTuristicos;  // Almacena los puntos tur�sticos con sus nombres
    sf::Color colorActualPunto = sf::Color::Red;   // Color inicial para los puntos

    // Colores disponibles en la paleta
    std::vector<sf::Color> coloresPaleta = { sf::Color::Red, sf::Color::White, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Black };
    float xPaleta = 50.0f, yPaleta = 500.0f;

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
                return;
            }

            // Selecci�n de color de puntos tur�sticos en la paleta
            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                float mouseX = static_cast<float>(evento.mouseButton.x);
                float mouseY = static_cast<float>(evento.mouseButton.y);

                // Verificar si se seleccion� un color de la paleta
                for (size_t i = 0; i < coloresPaleta.size(); ++i) {
                    float xColor = xPaleta + i * 40.0f;
                    if (mouseX >= xColor && mouseX <= xColor + 30 && mouseY >= yPaleta && mouseY <= yPaleta + 30) {
                        colorActualPunto = coloresPaleta[i];
                        std::cout << "Color de punto tur�stico seleccionado." << std::endl;
                        break;
                    }
                }

                // Cambiar color de un punto existente al hacer clic en �l
                for (auto& punto : puntosTuristicos) {
                    float puntoX = punto.punto.getPosition().x;
                    float puntoY = punto.punto.getPosition().y;
                    if (mouseX >= puntoX && mouseX <= puntoX + 10 && mouseY >= puntoY && mouseY <= puntoY + 10) {
                        punto.punto.setFillColor(colorActualPunto);
                        std::cout << "Color del punto tur�stico cambiado." << std::endl;
                    }
                }
            }

            // Selecci�n de puntos tur�sticos en el mapa (clic derecho para a�adir)
            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Right) {
                float posX = static_cast<float>(evento.mouseButton.x);
                float posY = static_cast<float>(evento.mouseButton.y);

                // Crear un c�rculo para el nuevo punto tur�stico
                sf::CircleShape puntoTuristico(5.0f); // Tama�o del c�rculo para el punto tur�stico
                puntoTuristico.setFillColor(colorActualPunto); // Aplica el color actual
                puntoTuristico.setPosition(posX, posY);

                // Crear un texto para el nombre de la ruta en el mismo punto
                sf::Text textoRuta(rutaActual->getName(), fuente, 12); // Tama�o de fuente ajustado
                textoRuta.setFillColor(sf::Color::Black); // Color del texto
                textoRuta.setPosition(posX + 10, posY + 10); // Ajusta la posici�n para que no se superponga al punto

                // A�adir el punto y el nombre a la lista de puntos tur�sticos
                puntosTuristicos.push_back({ puntoTuristico, textoRuta });

                std::cout << "Punto tur�stico a�adido en (" << posX << ", " << posY << ") con color seleccionado." << std::endl;
            }
        }

        // Dibujar el mapa, el nombre de la ruta, la paleta de colores y los puntos tur�sticos
        ventana.clear(sf::Color::White);

        // Dibujar el mapa
        ventana.draw(spriteMapa);

        // Dibujar el nombre de la ruta en su posici�n principal
        sf::Text textoRutaPrincipal(rutaActual->getName(), fuente, 20);
        textoRutaPrincipal.setPosition(rutaActual->getPosX(), rutaActual->getPosY());
        textoRutaPrincipal.setFillColor(rutaActual->getColor());
        ventana.draw(textoRutaPrincipal);

        // Dibujar los puntos tur�sticos y sus nombres
        for (const auto& punto : puntosTuristicos) {
            ventana.draw(punto.punto);   // Dibuja el punto
            ventana.draw(punto.nombre);  // Dibuja el nombre de la ruta al lado del punto
        }

        // Dibujar la paleta de colores para los puntos tur�sticos
        for (size_t i = 0; i < coloresPaleta.size(); ++i) {
            sf::RectangleShape cuadro(sf::Vector2f(30.0f, 30.0f));
            cuadro.setFillColor(coloresPaleta[i]);
            cuadro.setPosition(xPaleta + i * 40.0f, yPaleta);
            ventana.draw(cuadro);
        }

        ventana.display();
    }
}

int main() {
    sf::RenderWindow ventana(sf::VideoMode(1000, 600), "Mapa Tur�stico");
    sf::Texture texturaMapa;
    if (!texturaMapa.loadFromFile("C:/Users/jafet/source/repos/Project1/x64/Debug/mapa_nuevo.png")) {
        std::cerr << "Error al cargar la imagen del mapa." << std::endl;
        return -1;
    }

    // Crear el sprite del mapa
    sf::Sprite spriteMapa;
    spriteMapa.setTexture(texturaMapa);
    RouteList rutas;
    bool modoInsercion = false;
    RouteNode* rutaActual = nullptr;

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            // Activa el modo de inserci�n cuando el usuario presione una tecla espec�fica (por ejemplo, la tecla "I")
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::I) {
                std::string nombreRuta;
                std::cout << "Ingrese el nombre de la ruta: ";
                std::cin >> nombreRuta;

                if (rutas.isUniqueName(nombreRuta)) {
                    rutas.insertRout(nombreRuta);
                    rutaActual = rutas.searchRoute(nombreRuta);  // Buscar la ruta reci�n creada
                    seleccionarPuntosTuristicos(rutaActual, ventana, spriteMapa);  // Llamar a la funci�n para posici�n y color
                }
                else {
                    std::cout << "El nombre de la ruta ya existe. Intente otro nombre." << std::endl;
                }
            }
        }

        // Dibujar el mapa y rutas
        ventana.clear(sf::Color::White);
        ventana.draw(spriteMapa);  // Aseg�rate de tener definido `spriteMapa`
        rutas.displayRoutes(ventana);
        ventana.display();
    }

    return 0;
}

