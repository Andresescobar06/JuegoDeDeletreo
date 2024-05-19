#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <random>
#include <thread>
#include <set>
#include <cctype>
#include <future>

using namespace std;

void cambiarColor(const string& color) {
    if (color == "reset") {
        cout << "\033[0m";
    } else if (color == "rojo") {
        cout << "\033[31m";
    } else if (color == "verde") {
        cout << "\033[32m";
    } else if (color == "amarillo") {
        cout << "\033[33m";
    }
}

char convertirMinuscula(char letra) {
    return tolower(letra);
}

vector<char> convertirMinusculas(const vector<char>& palabra) {
    vector<char> palabra_minusculas;
    for (char letra : palabra) {
        palabra_minusculas.push_back(convertirMinuscula(letra));
    }
    return palabra_minusculas;
}

map<string, pair<string, vector<char>>> obtenerDefiniciones() {
    map<string, pair<string, vector<char>>> definiciones = {
        {"Balon", {"Pelota inflable utilizada en deportes.", convertirMinusculas({'B', 'a', 'l', 'o', 'n'})}},
        {"Porteria", {"Arco donde se marca gol.", convertirMinusculas({'P', 'o', 'r', 't', 'e', 'r', 'i', 'a'})}},
        {"Futbol", {"Deporte con balón y dos equipos.", convertirMinusculas({'F', 'u', 't', 'b', 'o', 'l'})}},
        {"Canasta", {"Aro donde se encesta en baloncesto.", convertirMinusculas({'C', 'a', 'n', 'a', 's', 't', 'a'})}},
        {"Puntos", {"Se suman al marcar en deportes.", convertirMinusculas({'P', 'u', 'n', 't', 'o', 's'})}},
        {"Raqueta", {"Instrumento para golpear en tenis.", convertirMinusculas({'R', 'a', 'q', 'u', 'e', 't', 'a'})}},
        {"Juego", {"Actividad de entretenimiento.", convertirMinusculas({'J', 'u', 'e', 'g', 'o'})}},
        {"Baloncesto", {"Deporte con canasta y balón.", convertirMinusculas({'B', 'a', 'l', 'o', 'n', 'c', 'e', 's', 't', 'o'})}},
        {"Equipo", {"Grupo que compite junto en deportes.", convertirMinusculas({'E', 'q', 'u', 'i', 'p', 'o'})}}
    };
    return definiciones;
}

vector<pair<string, pair<string, vector<char>>>> barajarDefiniciones(const map<string, pair<string, vector<char>>>& definiciones) {
    vector<pair<string, pair<string, vector<char>>>> definiciones_vec(definiciones.begin(), definiciones.end());
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(definiciones_vec.begin(), definiciones_vec.end(), default_random_engine(seed));
    return definiciones_vec;
}

void mostrarPalabra(const vector<char>& letras_adivinadas) {
    for (char letra : letras_adivinadas) {
        cout << letra << " ";
    }
    cout << endl;
}

void pista(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador) {
    cout << "Pista: La palabra tiene " << definicion.second.second.size() << " letras." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    puntos_jugador -= 2;
}

bool verificarPalabraCompletada(const vector<char>& letras_adivinadas) {
    return find(letras_adivinadas.begin(), letras_adivinadas.end(), '_') == letras_adivinadas.end();
}

char pedirLetra(const string& jugador, set<char>& letras_ingresadas) {
    char letra;
    bool letra_repetida;

    do {
        cout << jugador << ", ingresa una letra (tienes 10 segundos): ";
        this_thread::sleep_for(chrono::milliseconds(500));
        
        future<char> f = async(launch::async, []() {
            char letra;
            cin >> letra;
            return letra;
        });

        if (f.wait_for(chrono::seconds(10)) == future_status::timeout) {
            cambiarColor("rojo");
            cout << "¡Tiempo agotado!" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            return ' ';
        }

        letra = f.get();
        letra = convertirMinuscula(letra);

        if (letras_ingresadas.count(letra) > 0) {
            cout << "Esa letra ya ha sido ingresada. Por favor, intenta con otra." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            letra_repetida = true;
        } else {
            letras_ingresadas.insert(letra);
            letra_repetida = false;
        }
    } while (letra_repetida);

    return letra;
}

void mostrarPuntajes(const string& jugador1, const string& jugador2, int puntos_jugador1, int puntos_jugador2) {
    cout << "Puntajes actuales:" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << jugador1 << " Jugador 1: " << puntos_jugador1 << " puntos." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << jugador2 << " Jugador 2: " << puntos_jugador2 << " puntos." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

bool verificarFinJuego(int puntos_jugador1, int puntos_jugador2, const string& jugador1, const string& jugador2) {
    if (puntos_jugador1 <= 0) {
        cambiarColor("rojo");
        cout << jugador1 << " ha llegado a 0 puntos y ha perdido el juego." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cambiarColor("reset");
        mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
        if (puntos_jugador2 > puntos_jugador1) {
            cambiarColor("amarillo");
            cout << jugador2 << " es el ganador!" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        } else {
            cout << "Empate!" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        return true;
    }
    if (puntos_jugador2 <= 0) {
        cambiarColor("rojo");
        cout << jugador2 << " ha llegado a 0 puntos y ha perdido el juego." << endl;
        cambiarColor("reset");
        this_thread::sleep_for(chrono::milliseconds(500));
        mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
        if (puntos_jugador1 > puntos_jugador2) {
            cambiarColor("amarillo");
            cout << jugador1 << " es el ganador!" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        } else {
            cout << "Empate!" << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        return true;
    }
    return false;
}

bool manejarTurnoJugador(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador, int& perdidas_jugador, const string& jugador, set<char>& letras_ingresadas, vector<char>& letras_adivinadas, const string& jugador1, const string& jugador2, int& puntos_jugador1, int& puntos_jugador2) {
    bool letra_correcta = false;
    while (true) {
        cout << "Definición: " << definicion.second.first << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
        mostrarPalabra(letras_adivinadas);
        cambiarColor("amarillo");
        cout << "-------------------------------" << endl;
        cambiarColor("reset");
        cout << jugador << ", elige una opción: " << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "1. Ingresar una letra" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "2. Pedir una pista (-2 puntos)" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cambiarColor("rojo");
        cout << "3. Salir del juego" << endl;
        cambiarColor("reset");
        cambiarColor("amarillo");
        cout << "-------------------------------" << endl;
        cambiarColor("reset");
        int opcion;
        cin >> opcion;

        if (opcion == 1) {
            auto start = chrono::steady_clock::now();
            char letra_jugador = pedirLetra(jugador, letras_ingresadas);
            auto end = chrono::steady_clock::now();
            chrono::duration<double> tiempo_transcurrido = end - start;

            if (letra_jugador == ' ') {
                cout << "No ingresaste ninguna letra. Perdiste 1 punto." << endl;
                puntos_jugador--;
                perdidas_jugador++;
                break;  // Finaliza el turno del jugador
            }

            letra_correcta = false;
            for (int i = 0; i < definicion.second.second.size(); ++i) {
                if (letra_jugador == definicion.second.second[i]) {
                    letras_adivinadas[i] = letra_jugador;
                    letra_correcta = true;
                }
            }

            if (letra_correcta) {
                cout << "¡Letra correcta! Ganas 5 puntos." << endl;
                puntos_jugador=+5;
                break;  // Finaliza el turno del jugador
            } else {
                cout << "Letra incorrecta. Pierdes 1 punto." << endl;
                puntos_jugador--;
                perdidas_jugador++;
                break;  // Finaliza el turno del jugador
            }
        } else if (opcion == 2) {
            pista(definicion, puntos_jugador);
        } else if (opcion == 3) {
            cout << jugador << " ha decidido salir del juego." << endl;
            mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
            if (puntos_jugador1 > puntos_jugador2) {
                cout << jugador1 << " es el ganador con " << puntos_jugador1 << " puntos!" << endl;
            } else if (puntos_jugador2 > puntos_jugador1) {
                cout << jugador2 << " es el ganador con " << puntos_jugador2 << " puntos!" << endl;
            } else {
                cout << "¡Es un empate!" << endl;
            }
            return true;
        } else {
            cout << "Opción no válida. Intenta de nuevo." << endl;
        }
        
        if (verificarFinJuego(puntos_jugador1, puntos_jugador2, jugador1, jugador2)) {
            return true;
        }
    }

    return false;
}

int main() {
    cambiarColor("amarillo");
    cout << "***********************************************" << endl;
    cambiarColor("amarillo"); // Cambia el color al amarillo
    cout << "* Bienvenido al juego de adivinada la palabra!*" << endl;
    cambiarColor("amarillo");
    cout << "***********************************************" << endl;
    cambiarColor("reset"); // Restaura el color predeterminado
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "-Cada jugador debe adivinar la palabra en función de la definición dada.                -" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "-Cada letra correcta suma 5 puntos, una pista cuesta 2 puntos y un fallo resta 2 puntos.-" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "-El juego termina cuando se adivinan todas las palabras o si alguien decide salir.      -" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "-                  ---¡QUE COMIENCE EL JUEGO!---                                        -" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));

    cambiarColor("reset");

    cout << "Jugador 1, ingresa tu nombre: ";
    string jugador1;
    cin >> jugador1;

    cout << "Jugador 2, ingresa tu nombre: ";
    string jugador2;
    cin >> jugador2;

    int puntos_jugador1 = 5;
    int puntos_jugador2 = 5;
    int perdidas_jugador1 = 0;
    int perdidas_jugador2 = 0;

    map<string, pair<string, vector<char>>> definiciones = obtenerDefiniciones();
    vector<pair<string, pair<string, vector<char>>>> definiciones_barajadas = barajarDefiniciones(definiciones);
    int definicion_actual = 0;

    while (definicion_actual < definiciones_barajadas.size()) {
        pair<string, pair<string, vector<char>>> definicion = definiciones_barajadas[definicion_actual];
        vector<char> letras_adivinadas(definicion.second.second.size(), '_');
        set<char> letras_ingresadas;

        while (!verificarPalabraCompletada(letras_adivinadas)) {
            if (manejarTurnoJugador(definicion, puntos_jugador1, perdidas_jugador1, jugador1, letras_ingresadas, letras_adivinadas, jugador1, jugador2, puntos_jugador1, puntos_jugador2)) {
                return 0;
            }
            if (verificarFinJuego(puntos_jugador1, puntos_jugador2, jugador1, jugador2)) {
                return 0;
            }

            if (verificarPalabraCompletada(letras_adivinadas)) {
                break;
            }

            if (manejarTurnoJugador(definicion, puntos_jugador2, perdidas_jugador2, jugador2, letras_ingresadas, letras_adivinadas, jugador1, jugador2, puntos_jugador1, puntos_jugador2)) {
                return 0;
            }
            if (verificarFinJuego(puntos_jugador1, puntos_jugador2, jugador1, jugador2)) {
                return 0;
            }
        }

        cout << "¡Felicidades! La palabra era: ";
        mostrarPalabra(definicion.second.second);

        definicion_actual++;
        mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
    }

    cout << "El juego ha terminado." << endl;
    if (puntos_jugador1 > puntos_jugador2) {
        cout << jugador1 << " es el ganador con " << puntos_jugador1 << " puntos!" << endl;
    } else if (puntos_jugador2 > puntos_jugador1) {
        cout << jugador2 << " es el ganador con " << puntos_jugador2 << " puntos!" << endl;
    } else {
        cout << "¡Es un empate!" << endl;
    }

    return 0;
}
