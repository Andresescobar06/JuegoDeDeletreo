#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <random>
#include <thread> // Para utilizar this_thread::sleep_for
#include <set>   

using namespace std;

// Definiciones de las palabras
map<string, pair<string, vector<char>>> obtenerDefiniciones() {
    map<string, pair<string, vector<char>>> definiciones = {
        {"Balon", {"Pelota inflable utilizada en deportes.", {'b', 'a', 'l', 'o', 'n'}}},
        {"Porteria", {"Arco donde se marca gol.", {'p', 'o', 'r', 't', 'e', 'r', 'i', 'a'}}},
        {"Futbol", {"Deporte con balón y dos equipos.", {'f', 'u', 't', 'b', 'o', 'l'}}},
        {"Canasta", {"Aro donde se encesta en baloncesto.", {'c', 'a', 'n', 'a', 's', 't', 'a'}}},
        {"Puntos", {"Se suman al marcar en deportes.", {'p', 'u', 'n', 't', 'o', 's'}}},
        {"Raqueta", {"Instrumento para golpear en tenis.", {'r', 'a', 'q', 'u', 'e', 't', 'a'}}},
        {"Juego", {"Actividad de entretenimiento.", {'j', 'u', 'e', 'g', 'o'}}},
        {"Baloncesto", {"Deporte con canasta y balón.", {'b', 'a', 'l', 'o', 'n', 'c', 'e', 's', 't', 'o'}}},
        {"Equipo", {"Grupo que compite junto en deportes.", {'e', 'q', 'u', 'i', 'p', 'o'}}}
    };
    return definiciones;
}

// Función para barajar las definiciones
vector<pair<string, pair<string, vector<char>>>> barajarDefiniciones(const map<string, pair<string, vector<char>>>& definiciones) {
    vector<pair<string, pair<string, vector<char>>>> definiciones_vec(definiciones.begin(), definiciones.end());
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(definiciones_vec.begin(), definiciones_vec.end(), default_random_engine(seed));
    return definiciones_vec;
}

// Función para mostrar la palabra con letras adivinadas
void mostrarPalabra(const vector<char>& letras_adivinadas) {
    for (char letra : letras_adivinadas) {
        cout << letra << " ";
    }
    cout << endl;
    
}

// procedimiento pista
void pista(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador) {
    cout << "Pista: La palabra tiene " << definicion.second.second.size() << " letras." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    puntos_jugador -= 2; // Penalización de 2 puntos por usar la pista
}

// Función para verificar si la palabra se ha completado
bool verificarPalabraCompletada(const vector<char>& letras_adivinadas) {
    return find(letras_adivinadas.begin(), letras_adivinadas.end(), '_') == letras_adivinadas.end();
}

// Función para solicitar una letra al jugador
char pedirLetra(const string& jugador, set<char>& letras_ingresadas) {
    char letra;
    bool letra_repetida;
    do {
        cout << jugador << ", Ingresa una letra: ";
        cin >> letra;
        if (letras_ingresadas.count(letra) > 0) {
            cout << "Esa letra ya ha sido ingresada. Por favor, intenta con otra." << endl;
            letra_repetida = true;
        } else {
            letras_ingresadas.insert(letra);
            letra_repetida = false;
        }
    } while (letra_repetida);
    return letra;
}

// Función para manejar el turno de un jugador
bool manejarTurnoJugador(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador, int& perdidas_jugador, const string& jugador, set<char>& letras_ingresadas) {
    vector<char> letras_adivinadas(definicion.second.second.size(), '_');
    
    while (perdidas_jugador < 3 && puntos_jugador > 0 && !verificarPalabraCompletada(letras_adivinadas)) {
        cout << "Definición: " << definicion.second.first << endl;
        mostrarPalabra(letras_adivinadas);
        this_thread::sleep_for(chrono::milliseconds(500));
        char respuesta;
        cout << "¿Deseas una pista? (Sí = 's', No = 'n' o 'f' para salir): ";
        cin >> respuesta;
        this_thread::sleep_for(chrono::milliseconds(500));
        if (respuesta == 's' || respuesta == 'S') {
            pista(definicion, puntos_jugador);
        } 
        if (respuesta == 'f' || respuesta == 'F') {
            cout << "Juego finalizado por el jugador " << jugador << "." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            return true;
        }
        
        // Pedir la letra
        char letra_jugador = pedirLetra(jugador, letras_ingresadas);

        bool letra_correcta = false;
        for (size_t i = 0; i < definicion.second.second.size(); ++i) {
            if (definicion.second.second[i] == letra_jugador) {
                letras_adivinadas[i] = letra_jugador;
                letra_correcta = true;
            }
        }

        if (letra_correcta) {
            cout << "¡Letra correcta! " << jugador << " suma 5 puntos." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            puntos_jugador += 5;
            chrono::steady_clock::time_point start = chrono::steady_clock::now();
            // ... Código para medir el tiempo de respuesta ...

            chrono::steady_clock::time_point end = chrono::steady_clock::now();
            double tiempo = chrono::duration_cast<chrono::seconds>(end - start).count();

            if (tiempo <= 5) {
                puntos_jugador++;
                cout << "¡Bonus por rapidez! Sumas un punto adicional. :D" << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
            }

        } else {
            cout << "Letra incorrecta." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            puntos_jugador -= 2;
            perdidas_jugador++;
        }
    }

    if (perdidas_jugador >= 3 || puntos_jugador <= 0) {
        cout << "El jugador " << jugador << " ha perdido :(" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    } else {
        cout << "¡Palabra completada por " << jugador << "!" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return false;
}

// Nueva procedimiento para salir del juego y mostrar resultados
void salirDelJuego(const string& jugador1, const string& jugador2, int puntos_jugador1, int puntos_jugador2) {
    cout << "Resultados finales:" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << jugador1 << " Jugador 1: " << puntos_jugador1 << " puntos." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << jugador2 << " Jugador 2: " << puntos_jugador2 << " puntos." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));

    if (puntos_jugador1 > puntos_jugador2) {
        cout << jugador1 << " Jugador 1 ha ganado :)" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    } else if (puntos_jugador1 < puntos_jugador2) {
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << jugador2 << " Jugador 2 ha ganado :)" << endl;
    } else {
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "¡Empate!" << endl;
    }

    exit(0);  // Salir del programa
}

// procedimiento principal para jugar
void jugar(const vector<pair<string, pair<string, vector<char>>>>& definiciones_vec, const string& jugador1, const string& jugador2) {
    int puntos_jugador1 = 5, puntos_jugador2 = 5, perdidas_jugador1 = 0, perdidas_jugador2 = 0;
    set<char> letras_ingresadas_jugador1, letras_ingresadas_jugador2;

    bool jugador1_turno = true; // Variable para controlar el turno de los jugadores

    for (const auto& definicion : definiciones_vec) {
        if (jugador1_turno) {
            cout << "Turno del jugador " << jugador1 << ":" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            if (manejarTurnoJugador(definicion, puntos_jugador1, perdidas_jugador1, jugador1, letras_ingresadas_jugador1)) {
                salirDelJuego(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
            }
            if (verificarPalabraCompletada(definicion.second.second)) {
                cout << "Palabra completada por " << jugador1 << ", cambiando al siguiente jugador." << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
                jugador1_turno = false;
            }
        } else {
            cout << "Turno del jugador " << jugador2 << ":" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            if (manejarTurnoJugador(definicion, puntos_jugador2, perdidas_jugador2, jugador2, letras_ingresadas_jugador2)) {
                salirDelJuego(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
            }
            if (verificarPalabraCompletada(definicion.second.second)) {
                cout << "Palabra completada por " << jugador2 << ", cambiando al siguiente jugador." << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
                jugador1_turno = true;
            }
        }

        // Verificar condiciones de fin de juego aquí
        if (perdidas_jugador1 >= 3 || puntos_jugador1 <= 0 || perdidas_jugador2 >= 3 || puntos_jugador2 <= 0) {
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "Juego terminado debido a que al menos un jugador ha perdido." << endl;
            salirDelJuego(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
        }
    }

    salirDelJuego(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
}

int main() {
    cout << "**************************************" << endl;
    this_thread::sleep_for(chrono::milliseconds(500)); // Pausa de 500 milisegundos (0.5 segundos)
    cout << "*BIENVENIDO A DELETREA LA PALABRA ***" << endl;
    this_thread::sleep_for(chrono::milliseconds(500)); 
    cout << "**************************************" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000)); 
    // Inicialización
    string jugador1, jugador2;
    cout << "Ingrese el nombre del Jugador 1: ";
    cin >> jugador1;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Ingrese el nombre del Jugador 2: ";
    cin >> jugador2;
    this_thread::sleep_for(chrono::milliseconds(500));

    map<string, pair<string, vector<char>>> definiciones = obtenerDefiniciones();
    vector<pair<string, pair<string, vector<char>>>> definiciones_vec = barajarDefiniciones(definiciones);

    // Juego
    jugar(definiciones_vec, jugador1, jugador2);

    return 0;
}