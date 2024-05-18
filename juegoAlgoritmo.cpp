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
#include <cctype> // Para usar tolower
#include <future>

using namespace std;

// Función para convertir una letra a minúsculas
char convertirMinuscula(char letra) {
    return tolower(letra);
}

// Función para convertir una palabra a minúsculas
vector<char> convertirMinusculas(const vector<char>& palabra) {
    vector<char> palabra_minusculas;
    for (char letra : palabra) {
        palabra_minusculas.push_back(convertirMinuscula(letra));
    }
    return palabra_minusculas;
}

// Definiciones de las palabras
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

// Procedimiento para dar una pista
void pista(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador) {
    cout << "Pista: La palabra tiene " << definicion.second.second.size() << " letras." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    puntos_jugador -= 2; // Penalización de 2 puntos por usar la pista
}

// Función para verificar si la palabra se ha completado
bool verificarPalabraCompletada(const vector<char>& letras_adivinadas) {
    return find(letras_adivinadas.begin(), letras_adivinadas.end(), '_') == letras_adivinadas.end();
}

// Función para solicitar una letra al jugador con límite de tiempo
char pedirLetra(const string& jugador, set<char>& letras_ingresadas) {
    char letra;
    bool letra_repetida;

    do {
        cout << jugador << ", ingresa una letra (tienes 10 segundos): ";
        
        // Configurar un futuro para obtener la letra con un límite de tiempo
        future<char> f = async(launch::async, []() {
            char letra;
            cin >> letra;
            return letra;
        });

        // Esperar hasta 10 segundos por la entrada
        if (f.wait_for(chrono::seconds(10)) == future_status::timeout) {
            cout << "¡Tiempo agotado!" << endl;
            return ' ';
        }

        letra = f.get();
        letra = convertirMinuscula(letra);

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

// Función para mostrar los puntajes actuales
void mostrarPuntajes(const string& jugador1, const string& jugador2, int puntos_jugador1, int puntos_jugador2) {
    cout << "Puntajes actuales:" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << jugador1 << " Jugador 1: " << puntos_jugador1 << " puntos." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << jugador2 << " Jugador 2: " << puntos_jugador2 << " puntos." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

// Función para manejar el turno de un jugador
bool manejarTurnoJugador(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador, int& perdidas_jugador, const string& jugador, set<char>& letras_ingresadas, vector<char>& letras_adivinadas, const string& jugador1, const string& jugador2, int puntos_jugador1, int puntos_jugador2) {
    bool letra_correcta = false;
    while (true) {
        cout << "Definición: " << definicion.second.first << endl;
        mostrarPalabra(letras_adivinadas);
        this_thread::sleep_for(chrono::milliseconds(500));

        // Pedir la opción
        cout << jugador << ", elige una opción:" << endl;
        cout << "1. Ingresar una letra" << endl;
        cout << "2. Pedir una pista (-2 puntos)" << endl;
        cout << "3. Salir del juego" << endl;
        int opcion;
        cin >> opcion;

        if (opcion == 1) {
            auto start = chrono::steady_clock::now();

            // Pedir la letra
            char letra_jugador = pedirLetra(jugador, letras_ingresadas);

            auto end = chrono::steady_clock::now();
            chrono::duration<double> tiempo_transcurrido = end - start;

            if (letra_jugador == ' ') {
                cout << "Perdiste tu turno por no ingresar una letra a tiempo." << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
                perdidas_jugador++;
                break;
            }

            letra_correcta = false;
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

                if (tiempo_transcurrido.count() <= 5) {
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
            break;
        } else if (opcion == 2) {
            pista(definicion, puntos_jugador);
        } else if (opcion == 3) {
            cout << jugador << " ha decidido salir del juego." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
            if (puntos_jugador1 > puntos_jugador2) {
                cout << jugador1 << " Jugador 1 va ganando." << endl;
            } else if (puntos_jugador1 < puntos_jugador2) {
                cout << jugador2 << " Jugador 2 va ganando." << endl;
            } else {
                cout << "Empate hasta ahora." << endl;
            }
            exit(0);
        } else {
            cout << "Opción no válida. Intenta de nuevo." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            perdidas_jugador++;
            break;
        }
    }

    return letra_correcta;
}

// Función principal del juego
void jugar() {
    string jugador1, jugador2;
    int puntos_jugador1 = 0, puntos_jugador2 = 0;
    int perdidas_jugador1 = 0, perdidas_jugador2 = 0;

    cout << "¡Bienvenidos al juego de palabras!" << endl;
    cout << "Jugador 1, ingresa tu nombre: ";
    cin >> jugador1;
    cout << "Jugador 2, ingresa tu nombre: ";
    cin >> jugador2;

    // Obtener y barajar las definiciones
    map<string, pair<string, vector<char>>> definiciones = obtenerDefiniciones();
    vector<pair<string, pair<string, vector<char>>>> definiciones_barajadas = barajarDefiniciones(definiciones);

    // Juego principal
    for (const auto& definicion : definiciones_barajadas) {
        vector<char> letras_adivinadas(definicion.second.second.size(), '_');
        set<char> letras_ingresadas;

        while (!verificarPalabraCompletada(letras_adivinadas)) {
            if (perdidas_jugador1 < 3) {
                cout << "Turno de " << jugador1 << ":" << endl;
                bool acierto_jugador1 = manejarTurnoJugador(definicion, puntos_jugador1, perdidas_jugador1, jugador1, letras_ingresadas, letras_adivinadas, jugador1, jugador2, puntos_jugador1, puntos_jugador2);
                if (acierto_jugador1 && verificarPalabraCompletada(letras_adivinadas)) {
                    cout << jugador1 << " ha completado la palabra y obtiene 10 puntos adicionales!" << endl;
                    puntos_jugador1 += 10;
                }
            } else {
                cout << jugador1 << " ha perdido el turno por 3 letras incorrectas." << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
            }

            if (verificarPalabraCompletada(letras_adivinadas)) break;

            if (perdidas_jugador2 < 3) {
                cout << "Turno de " << jugador2 << ":" << endl;
                bool acierto_jugador2 = manejarTurnoJugador(definicion, puntos_jugador2, perdidas_jugador2, jugador2, letras_ingresadas, letras_adivinadas, jugador1, jugador2, puntos_jugador1, puntos_jugador2);
                if (acierto_jugador2 && verificarPalabraCompletada(letras_adivinadas)) {
                    cout << jugador2 << " ha completado la palabra y obtiene 10 puntos adicionales!" << endl;
                    puntos_jugador2 += 10;
                }
            } else {
                cout << jugador2 << " ha perdido el turno por 3 letras incorrectas." << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
            }
        }

        cout << "La palabra era: " << definicion.first << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
    }

    // Mostrar el ganador final
    if (puntos_jugador1 > puntos_jugador2) {
        cout << "¡" << jugador1 << " es el ganador con " << puntos_jugador1 << " puntos!" << endl;
    } else if (puntos_jugador2 > puntos_jugador1) {
        cout << "¡" << jugador2 << " es el ganador con " << puntos_jugador2 << " puntos!" << endl;
    } else {
        cout << "¡Empate con " << puntos_jugador1 << " puntos cada uno!" << endl;
    }
}

// Función principal
int main() {
    jugar();
    return 0;
}