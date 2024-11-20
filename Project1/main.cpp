#include <SFML/Graphics.hpp>
#include <iostream>
#include <direct.h>
#include <string>
#include "RouteList.h"
#include "pointList.h"
#include "pointNode.h"

using namespace std;

// Estructura para representar un punto 2D
struct Punto {
    float x;
    float y;
    Punto* next;

    Punto(float x = 0, float y = 0) : x(x), y(y), next(nullptr) {}
};

// Lista enlazada para puntos 2D
class PuntoList {
public:
    Punto* head;

    PuntoList() : head(nullptr) {}

    ~PuntoList() {
        clear();
    }

    void addPunto(float x, float y) {
        Punto* newNode = new Punto(x, y);
        if (!head) {
            head = newNode;
        }
        else {
            Punto* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void clear() {
        Punto* current = head;
        while (current) {
            Punto* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        head = nullptr;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    size_t size() const {
        size_t count = 0;
        Punto* current = head;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }
};

// Estructura para representar un punto turístico
struct PuntoTuristico {
    sf::CircleShape punto;
    sf::Text nombre;
    PuntoTuristico* next;

    PuntoTuristico(sf::CircleShape punto, sf::Text nombre) : punto(punto), nombre(nombre), next(nullptr) {}
};

// Estructura para representar una ruta con su nombre y lista de puntos turísticos
struct Ruta {
    string nombre;
    PuntoList puntos;
    PuntoTuristico* headPuntoTuristico;
    sf::Color colorRuta;
    Ruta* next;

    Ruta(string nombre, sf::Color color) : nombre(nombre), headPuntoTuristico(nullptr), colorRuta(color), next(nullptr) {}

    // Función para verificar si un nombre de punto ya existe en la lista de puntos turísticos
    bool isPuntoNombreUnico(const string& nombre) const {
        PuntoTuristico* current = headPuntoTuristico;
        while (current) {
            if (current->nombre.getString() == nombre) {
                return false;  // El nombre ya existe
            }
            current = current->next;
        }
        return true;  // El nombre es único
    }

    bool selectPunto(float x, float y, PuntoTuristico*& puntoSeleccionado) {
        PuntoTuristico* current = headPuntoTuristico;
        while (current) {
            if (current->punto.getGlobalBounds().contains(x, y)) {
                puntoSeleccionado = current;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void removePunto(PuntoTuristico* punto) {
        if (!headPuntoTuristico || !punto) return;

        if (headPuntoTuristico == punto) {
            PuntoTuristico* toDelete = headPuntoTuristico;
            headPuntoTuristico = headPuntoTuristico->next;
            delete toDelete;
        }
        else {
            PuntoTuristico* current = headPuntoTuristico;
            while (current->next && current->next != punto) {
                current = current->next;
            }
            if (current->next) {
                PuntoTuristico* toDelete = current->next;
                current->next = toDelete->next;
                delete toDelete;
            }
        }

        // Limpiar la lista de puntos y regenerar la curva
        puntos.clear();
        PuntoTuristico* currentPunto = headPuntoTuristico;
        while (currentPunto) {
            puntos.addPunto(currentPunto->punto.getPosition().x, currentPunto->punto.getPosition().y);
            currentPunto = currentPunto->next;
        }
    }


};

// Lista enlazada para gestionar múltiples rutas
class RutaList {
public:
    Ruta* head;

    RutaList() : head(nullptr) {}

    ~RutaList() {
        clear();
    }

    void addRuta(string nombre, sf::Color color) {
        Ruta* newRuta = new Ruta(nombre, color);
        if (!head) {
            head = newRuta;
        }
        else {
            Ruta* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newRuta;
        }
    }

    Ruta* findRuta(string nombre) {
        Ruta* current = head;
        while (current) {
            if (current->nombre == nombre) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void clear() {
        Ruta* current = head;
        while (current) {
            Ruta* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        head = nullptr;
    }

    void removeRuta(Ruta* ruta) {
        if (!head || !ruta) return;

        if (head == ruta) {
            Ruta* toDelete = head;
            head = head->next;
            delete toDelete;
        }
        else {
            Ruta* current = head;
            while (current->next && current->next != ruta) {
                current = current->next;
            }
            if (current->next) {
                Ruta* toDelete = current->next;
                current->next = toDelete->next;
                delete toDelete;
            }
        }
    }


    void saveRoutesToFiles() {
        Ruta* current = head;
        int index = 1;
        _mkdir("rutas_guardadas");
        while (current) {
            std::ofstream file("rutas_guardadas/ruta_" + std::to_string(index) + ".txt");
            if (file.is_open()) {
                file << current->nombre << "\n";
                PuntoTuristico* punto = current->headPuntoTuristico;
                while (punto) {
                    file << punto->punto.getPosition().x << " " << punto->punto.getPosition().y << "\n";
                    punto = punto->next;
                }
                file.close();
            }
            current = current->next;
            ++index;
        }
    }

    void loadRoutesFromFiles() {
        listaRutas.clear();
        _mkdir("rutas_guardadas");
        for (int i = 1; ; ++i) {
            std::ifstream file("rutas_guardadas/ruta_" + std::to_string(i) + ".txt");
            if (!file.is_open()) break;

            std::string nombreRuta;
            getline(file, nombreRuta);
            listaRutas.addRuta(nombreRuta, sf::Color::Red); // Color predeterminado para cargar
            Ruta* newRuta = listaRutas.findRuta(nombreRuta);

            float x, y;
            while (file >> x >> y) {
                newRuta->puntos.addPunto(x, y);
                sf::CircleShape punto(5.0f);
                punto.setPosition(x, y);
                punto.setFillColor(sf::Color::Red);
                sf::Text texto("", fuente, 12);
                texto.setPosition(x + 10, y + 10);
                newRuta->headPuntoTuristico = new PuntoTuristico(punto, texto);  // Simplificación para cargar puntos
            }
            file.close();
        }
    }


};

// Función de interpolación Catmull-Rom
Punto catmullRom(Punto* p0, Punto* p1, Punto* p2, Punto* p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    float b0 = -t3 + 2.0f * t2 - t;
    float b1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
    float b2 = -3.0f * t3 + 4.0f * t2 + t;
    float b3 = t3 - t2;

    float x = 0.5f * (p0->x * b0 + p1->x * b1 + p2->x * b2 + p3->x * b3);
    float y = 0.5f * (p0->y * b0 + p1->y * b1 + p2->y * b2 + p3->y * b3);

    return Punto(x, y);
}

// Generar una curva Catmull-Rom usando listas enlazadas
PuntoList generateCatmullRomCurve(PuntoList& points) {
    PuntoList curvePoints;
    if (points.size() < 2) return curvePoints;

    Punto* first = points.head;
    Punto* last = first;
    while (last->next) {
        last = last->next;
    }

    Punto* extendedHead = new Punto(first->x, first->y);
    extendedHead->next = new Punto(first->x, first->y);
    Punto* current = points.head;
    Punto* extendedCurrent = extendedHead->next;
    while (current) {
        extendedCurrent->next = new Punto(current->x, current->y);
        extendedCurrent = extendedCurrent->next;
        current = current->next;
    }
    extendedCurrent->next = new Punto(last->x, last->y);
    extendedCurrent = extendedCurrent->next;
    extendedCurrent->next = new Punto(last->x, last->y);

    Punto* eCurrent = extendedHead;
    while (eCurrent->next && eCurrent->next->next && eCurrent->next->next->next) {
        for (float t = 0; t <= 1; t += 0.05f) {
            Punto interpolatedPoint = catmullRom(eCurrent, eCurrent->next, eCurrent->next->next, eCurrent->next->next->next, t);
            curvePoints.addPunto(interpolatedPoint.x, interpolatedPoint.y);
        }
        eCurrent = eCurrent->next;
    }

    while (extendedHead) {
        Punto* toDelete = extendedHead;
        extendedHead = extendedHead->next;
        delete toDelete;
    }

    return curvePoints;
}

// Dibujar líneas a partir de una lista enlazada de puntos
void drawCurve(sf::RenderWindow& window, PuntoList& curve) {
    Punto* current = curve.head;
    while (current && current->next) {
        sf::RectangleShape line(sf::Vector2f(
            sqrt(pow(current->next->x - current->x, 2) + pow(current->next->y - current->y, 2)),
            1.0f
        ));
        line.setFillColor(sf::Color::Black);  // Asegurar que las aristas sean negras
        line.setPosition(current->x, current->y);
        float angle = atan2(current->next->y - current->y, current->next->x - current->x) * 180 / 3.14159265;
        line.setRotation(angle);
        window.draw(line);
        current = current->next;
    }
}

int main() {
    const size_t size = 1024;
    char buffer[size];

    bool modoSeleccion = false;


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

    // rutas.loadRoutesFromFiles(); desdocumentar este metodo cuando se cambien los paths de los archivos

    sf::Font fuente;
    if (!fuente.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return -1;
    }

    sf::Color colorActualPunto = sf::Color::Red;
    RutaList listaRutas;
    Ruta* rutaSeleccionada = nullptr;
    sf::Color coloresPaleta[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta };
    float xPaleta = 50.0f, yPaleta = 500.0f;
    std::string nombrePuntoActual;

    while (ventana.isOpen()) {
        sf::Event evento;

        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::I) {
                std::cout << "Ingrese el nombre del punto: ";
                std::cin >> nombrePuntoActual;

                if (rutaSeleccionada && !rutaSeleccionada->isPuntoNombreUnico(nombrePuntoActual)) {
                    std::cout << "El nombre del punto ya existe. Intente otro nombre." << std::endl;
                    nombrePuntoActual.clear();
                }
                else {
                    std::cout << "Nombre del punto almacenado: " << nombrePuntoActual << std::endl;
                }
            }

            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::M) {
                modoSeleccion = !modoSeleccion;
                std::cout << (modoSeleccion ? "Modo de selección activado." : "Modo de selección desactivado.") << std::endl;
            }


            if (modoSeleccion && evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                float mouseX = static_cast<float>(evento.mouseButton.x);
                float mouseY = static_cast<float>(evento.mouseButton.y);

                // Verificar si se está seleccionando un punto
                Ruta* currentRuta = listaRutas.head;
                PuntoTuristico* puntoSeleccionado = nullptr;
                while (currentRuta) {
                    if (currentRuta->selectPunto(mouseX, mouseY, puntoSeleccionado)) {
                        rutaSeleccionada = currentRuta;  // Seleccionar la ruta correspondiente
                        currentRuta->colorRuta = sf::Color::Cyan;  // Resaltar la ruta seleccionada
                        std::cout << "Punto seleccionado en la ruta \"" << rutaSeleccionada->nombre << "\"." << std::endl;
                        break;
                    }
                    currentRuta = currentRuta->next;
                }
            }



            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Right && rutaSeleccionada) {
                float posX = static_cast<float>(evento.mouseButton.x);
                float posY = static_cast<float>(evento.mouseButton.y);

                if (!nombrePuntoActual.empty()) {
                    sf::CircleShape puntoTuristico(5.0f);
                    puntoTuristico.setFillColor(colorActualPunto);
                    puntoTuristico.setPosition(posX, posY);

                    sf::Text textoRuta(nombrePuntoActual, fuente, 12);
                    textoRuta.setFillColor(sf::Color::Black);
                    textoRuta.setPosition(posX + 10, posY + 10);

                    PuntoTuristico* newPunto = new PuntoTuristico(puntoTuristico, textoRuta);
                    if (!rutaSeleccionada->headPuntoTuristico) {
                        rutaSeleccionada->headPuntoTuristico = newPunto;
                    }
                    else {
                        PuntoTuristico* current = rutaSeleccionada->headPuntoTuristico;
                        while (current->next) {
                            current = current->next;
                        }
                        current->next = newPunto;
                    }

                    rutaSeleccionada->puntos.addPunto(posX + 5.0f, posY + 5.0f);
                    nombrePuntoActual.clear();  // Limpiar el nombre después de usarlo
                    std::cout << "Punto turístico insertado en la ruta \"" << rutaSeleccionada->nombre << "\" en (" << posX << ", " << posY << ") con el nombre \"" << textoRuta.getString().toAnsiString() << "\"." << std::endl;
                }
                else {
                    std::cout << "No hay un nombre de punto almacenado o el nombre ya se ha usado." << std::endl;
                }
            }

            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::A) {
                std::string nombreRuta;
                std::cout << "Ingrese el nombre de la nueva ruta: ";
                std::cin >> nombreRuta;

                if (!listaRutas.findRuta(nombreRuta)) {
                    listaRutas.addRuta(nombreRuta, colorActualPunto);
                    rutaSeleccionada = listaRutas.findRuta(nombreRuta);
                    std::cout << "Ruta \"" << nombreRuta << "\" creada y seleccionada." << std::endl;
                }
                else {
                    std::cout << "El nombre de la ruta ya existe. Intente otro nombre." << std::endl;
                }
            }

            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left && rutaSeleccionada) {
                float mouseX = static_cast<float>(evento.mouseButton.x);
                float mouseY = static_cast<float>(evento.mouseButton.y);

                for (int i = 0; i < 5; ++i) {
                    float xColor = xPaleta + i * 40.0f;
                    if (mouseX >= xColor && mouseX <= xColor + 30 && mouseY >= yPaleta && mouseY <= yPaleta + 30) {
                        colorActualPunto = coloresPaleta[i];
                        std::cout << "Color de punto turístico seleccionado." << std::endl;
                        break;
                    }
                }
            }
        }

        ventana.clear(sf::Color::White);
        ventana.draw(spriteMapa);

        Ruta* currentRuta = listaRutas.head;
        while (currentRuta) {
            if (currentRuta->puntos.size() >= 2) {
                PuntoList curva = generateCatmullRomCurve(currentRuta->puntos);
                drawCurve(ventana, curva);
            }

            PuntoTuristico* currentPunto = currentRuta->headPuntoTuristico;
            while (currentPunto) {
                ventana.draw(currentPunto->punto);
                ventana.draw(currentPunto->nombre);
                currentPunto = currentPunto->next;
            }

            currentRuta = currentRuta->next;
        }

        for (int i = 0; i < 5; ++i) {
            sf::RectangleShape cuadro(sf::Vector2f(30.0f, 30.0f));
            cuadro.setFillColor(coloresPaleta[i]);
            cuadro.setPosition(xPaleta + i * 40.0f, yPaleta);
            ventana.draw(cuadro);
        }

        rutas.displayRoutes(ventana);
        ventana.display();
    }

    // rutas.saveRoutesToFiles(); desdocumentar cuando se cambien los paths de los archivos

    return 0;
}