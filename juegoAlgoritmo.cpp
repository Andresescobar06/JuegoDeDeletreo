#include<iostream>     // Para operaciones de entrada y salida.
#include<string>       // Para manipulación de cadenas de caracteres.
#include<map>          // Para utilizar contenedores de tipo map.
#include<vector>       // Para utilizar contenedores de tipo vector.
#include<algorithm>    // Para utilizar funciones de algoritmos estándar (e.g., shuffle).
#include<ctime>        // Para manejar fechas y horas.
#include<chrono>       // Para medir intervalos de tiempo.
#include<random>       // Para generar números aleatorios.
#include<thread>       // Para operaciones de multihilo.
#include<set>          // Para utilizar contenedores de tipo set.
#include<cctype>       // Para operaciones con caracteres.
#include<future>       // Para operaciones con futuros y tareas asíncronas.
#include<fstream>      // Para operaciones de entrada y salida de archivos.

using namespace std;
//Propósito: Leer datos de un archivo de registro y devolverlos en un vector de pares
vector<pair<string, int>> obtenerDatReg(const string& nombreArch){
	cout<<"Leyendo datos del archivo de registro: "<<nombreArch<<endl;
	ifstream archivo(nombreArch);
	if(!archivo.is_open()){
		cerr<< "Error: no se pudo abrir el archivo de registro."<<endl;
		return {};
	}
	vector<pair<string, int>> datos;
	string nombre;
	int puntos;
	while (archivo >> nombre >> puntos){
		cout<< "Nombre: " << nombre << ", Puntos: " << puntos<<endl;
		datos.push_back({nombre, puntos});
	}
	archivo.close();
	return datos;
}

//Propósito: Ayudar en el proceso de ordenamiento por el algoritmo Radix Sort
void contar(vector<pair<string, int>>& datos, int exp){
	int n = datos.size();
	vector<pair<string, int>> resultado(n);
	vector<int> conteo(10,0);
	for(int i =0;i< n;i=i+1){
		conteo[(datos[i].second/exp) % 10]++;
	}
	for(int i =1;i < 10;i=i + 1){
		conteo[i] += conteo[i-1];
	}
	for (int i=n-1;i>=0;i=i- 1){
		resultado[conteo[(datos[i].second/exp)%10] - 1] = datos[i];
		conteo[(datos[i].second/exp) % 10]--;
	}
	for (int i =0;i <n;i=i+ 1){
		datos[i]=resultado[i];
	}
}

//Propósito: Ordenar un vector de datos usando el algoritmo Radix Sort
void radixSort(vector< pair<string,int >>& datos){
	cout << "Aplicando algoritmo de ordenamiento Radix Sort" << endl;
	int maximo = datos[0].second;
	for (size_t i = 1; i < datos.size(); i++){
		if (datos[i].second> maximo){
			maximo = datos[i].second;
		}
	}
	for (int exp = 1; maximo/exp > 0; exp *= 10){
		contar(datos, exp);
	}
}

//Propósito: Escribir un resumen de los datos ordenados en un archivo
void escribirResumen(const vector<pair<string, int>>& datos_ordenados, const string& nombreArch){
	cout<<"Escribiendo resumen en el archivo: "<<nombreArch<<endl;
	ofstream archivo(nombreArch, ios::out);
	if (!archivo.is_open()){
		cerr << "Error: no se pudo abrir o crear el archivo de resumen." <<endl;
		return;
	}
	for (const auto& dato : datos_ordenados){
		cout << "Subiendo dato: " <<dato.first<< ": " <<dato.second<< " puntos" <<endl;
		archivo << dato.first << ": " << dato.second << " puntos" << endl;
	}
	archivo.close();
	if (archivo.is_open()){
		cout << "Archivo cerrado correctamente." << endl;
	} else {
		cout << "Error: archivo no se cerró correctamente." << endl;
	}
	cout << "Resumen escrito exitosamente." << endl;
	}

//Propósito: Cambiar el color del texto en la consola
void cambiarColor(const string& color){
	if (color == "reset"){
		cout << "\033[0m";
	} else if (color == "rojo"){
		cout << "\033[31m";
	} else if (color == "verde"){
		cout << "\033[32m";
	} else if (color == "amarillo"){
		cout << "\033[33m";
	}
}

//Propósito: Convertir un carácter o una palabra a minúsculas.
char convertirMin(char letra){
	return tolower(letra);
}

//Propósito: Convertir letra por letra una palabra a minúsculas
vector<char> convertirMin(const vector<char>& palabra){
	vector<char> palabra_minusculas;
	for (char letra : palabra) {
		palabra_minusculas.push_back(convertirMin(letra));
	}
	return palabra_minusculas;
}

//Propósito: Leer definiciones desde un archivo y almacenarlas en un map
map<string, pair<string, vector<char>>>obtenerDefDesdeArch(const string& nombreArch){
	map<string, pair<string, vector<char>>>definiciones;
	ifstream archivo(nombreArch);
	if (archivo.is_open()){
		string palabra, definicion;
		while (getline(archivo, palabra, '-') && getline(archivo, definicion)){
			definiciones[palabra] = make_pair(definicion, convertirMin(vector<char>(palabra.begin(), palabra.end())));
		}
		archivo.close();
	} else {
		cerr<< "No se pudo abrir el archivo: " <<nombreArch<< endl;
	}
	return definiciones;
}

//Propósito: Barajar las definiciones para aleatorizarlas
vector< pair<string, pair <string, vector<char>>>> mesclardef(const map<string, pair<string, vector<char>>>& definiciones){
	vector <pair<string, pair< string, vector<char >>>>definiciones_vec(definiciones.begin(), definiciones.end());
	unsigned seed =chrono::system_clock::now().time_since_epoch().count();
	shuffle(definiciones_vec.begin(), definiciones_vec.end(), default_random_engine(seed));
	return definiciones_vec;
}

//Propósito: Mostrar las letras adivinadas hasta el momento
void mostrarPal(const vector<char>& letras_adivinadas){
	for (char letra : letras_adivinadas){
		cout << letra << " ";
	}
	cout << endl;
}

//Propósito: Dar una pista al jugador, indicando la cantidad de letras de la palabra y restar puntos
void pista(const pair<string, pair<string, vector<char>>>& definicion, int& puntos_jugador){
	cout << "Pista: La palabra tiene " <<definicion.second.second.size()<< " letras." <<endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	puntos_jugador -= 2;
}

//Propósito: Verificar si la palabra ha sido completada
bool verificarPalComp(const vector<char>& letras_adivinadas){
	return find(letras_adivinadas.begin(), letras_adivinadas.end(),'_') ==letras_adivinadas.end();
}

//Propósito: Pedir una letra al jugador y verificar si ya ha sido ingresada
char pedirLetra(const string& jugador, set<char>& letras_ingresadas){
	char letra;
	bool letra_repetida;
	do {
		cout <<jugador<< ", ingresa una letra (tienes 10 segundos): ";
		this_thread::sleep_for(chrono::milliseconds(500));
		future<char> f = async(launch::async, [](){
			char letra;
			cin >> letra;
			return letra;
		});
		if (f.wait_for(chrono::seconds(10)) == future_status::timeout){
			cambiarColor("rojo");
			cout <<"¡Tiempo agotado!"<<endl;
			this_thread::sleep_for(chrono::milliseconds(500));
			return ' ';
		}
		letra = f.get();
		letra = convertirMin(letra);
		if (letras_ingresadas.count(letra) > 0){
			cout << "Esa letra ya ha sido ingresada. Por favor, intenta con otra." <<endl;
			this_thread::sleep_for(chrono::milliseconds(500));
			letra_repetida = true;
		} else {
			letras_ingresadas.insert(letra);
			letra_repetida = false;
		}
	} while (letra_repetida);
	return letra;
}

//Propósito: Mostrar los puntajes actuales de los jugadores
void mostrarPuntajes(const string& jugador1,  const string& jugador2, int puntos_jugador1, int puntos_jugador2){
	cout << "Puntajes actuales:"<<endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << jugador1<<" Jugador 1: "<< puntos_jugador1 <<" puntos."<<endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout<<jugador2 <<" Jugador 2: " << puntos_jugador2<< " puntos."<< endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
}

//Propósito: Verificar si el juego ha terminado y mostrar los puntajes finales
bool verificarFinJuego(int puntos_jugador1, int puntos_jugador2, const string& jugador1, const string& jugador2){
	if(puntos_jugador1 <= 0){
		cambiarColor("rojo");
		cout << jugador1<<" ha llegado a 0 puntos y ha perdido el juego." <<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cambiarColor("reset");
		mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
		
		if (puntos_jugador2 >puntos_jugador1) {
			cambiarColor("amarillo");
			cout<<jugador2 <<" es el ganador!"<< endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		} else {
			cout <<"Empate!"<<endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}
		
		vector<pair<string, int>> datos = obtenerDatReg("registro.txt");
		radixSort(datos);
		escribirResumen (datos,"resumen_clasificacion.txt");
		cout<< "Se ha generado el archivo de resumen 'resumen_clasificacion.txt'." <<endl;
		return true;
	}
	if (puntos_jugador2 <= 0) {
		cambiarColor("rojo");
		cout <<jugador2<< " ha llegado a 0 puntos y ha perdido el juego."<<endl;
		cambiarColor("reset");
		this_thread::sleep_for(chrono::milliseconds(500));
		mostrarPuntajes (jugador1, jugador2, puntos_jugador1, puntos_jugador2);
		if(puntos_jugador1> puntos_jugador2){
			cambiarColor("amarillo");
			cout << jugador1<<" es el ganador!" <<endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		} else {
			cout << "Empate!" << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}
		
		vector<pair<string, int>> datos = obtenerDatReg("registro.txt");
		radixSort(datos);
		escribirResumen(datos, "resumen_clasificacion.txt" );
		cout << "Se ha generado el archivo de resumen 'resumen_clasificacion.txt'."<< endl;
		return true;
	}
	return false;
}

//Propósito: Manejar el turno de un jugador, permitiéndole ingresar una letra, pedir una pista o salir del juego
bool manejarTurnoJugador(const pair< string, pair <string, vector<char>>>& definicion,  int& puntos_jugador, int& perdidas_jugador, const string& jugador, set<char>& letras_ingresadas, vector<char>& letras_adivinadas, const string& jugador1, const string& jugador2, int& puntos_jugador1, int& puntos_jugador2, ofstream& registro) {
	bool letra_correcta= false;
	while (true) {
		cout <<"Definición: "<<definicion.second.first<<endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		mostrarPal(letras_adivinadas);
		cambiarColor("amarillo");
		cout<<"-------------------------------"<<endl;
		cambiarColor("reset");
		cout<<jugador << ", elige una opción: "<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"1. Ingresar una letra"<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"2. Pedir una pista (-2 puntos)"<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cambiarColor("rojo");
		cout<< "3. Salir del juego"<<endl;
		cambiarColor("reset");
		cambiarColor("amarillo");
		cout<<"-------------------------------"<<endl;
		cambiarColor("reset");
		int opcion;
		cin >> opcion;
		
		if (opcion==1) {
			auto start=chrono::steady_clock::now();
			char letra_jugador = pedirLetra(jugador, letras_ingresadas);
			auto end =chrono::steady_clock::now();
			chrono::duration<double>tiempo_transcurrido=end - start;
			if (letra_jugador == ' ') {
				cout <<"No ingresaste ninguna letra. Perdiste 1 punto."<<endl;
				puntos_jugador--;
				perdidas_jugador++;
				break; // Es para finalizar el turno del jugador
			}			
			letra_correcta = false;
			for (int i=0; i<definicion.second.second.size();++i){
				if (letra_jugador==definicion.second.second[i]) {
					letras_adivinadas[i]=letra_jugador;
					letra_correcta=true;
				}			
			}
			if (letra_correcta) {
				cout<<"¡Letra correcta! Ganas 5 puntos."<<endl;
				puntos_jugador += 5;
				break; // Es para finalizar el turno del jugador
			} else {
				cout<<"Letra incorrecta. Pierdes 1 punto."<<endl;
				puntos_jugador--;
				perdidas_jugador++;
				break; // Es para finalizar el turno del jugador
			}
		} else if (opcion == 2) {
			pista(definicion, puntos_jugador);
		} else if (opcion == 3) {
			ofstream registro ( "registro.txt", ios::app);
			cout<< "Se ha generado el archivo de resumen 'resumen_clasificacion.txt'."<< endl;
			cout<<jugador <<" ha decidido salir del juego." <<endl;
			mostrarPuntajes (jugador1, jugador2, puntos_jugador1, puntos_jugador2);
			
			if (puntos_jugador1> puntos_jugador2) {
				cout <<jugador1 << " es el ganador con " <<puntos_jugador1 <<" puntos!"<< endl;
			} else if (puntos_jugador2 >puntos_jugador1) {
				cout <<jugador2 << " es el ganador con "<<puntos_jugador2 << " puntos!"<< endl;
			} else {
				cout <<"¡Es un empate!" << endl;
			}	
			
			registro << jugador1 << " " << puntos_jugador1 << " " << endl;
			registro << jugador2 << " " << puntos_jugador2 << " " << endl;
			vector<pair<string, int>> datos = obtenerDatReg("registro.txt");
			radixSort(datos);
			escribirResumen(datos,"resumen_clasificacion.txt" );
			registro.close();
			return true;
		} else {
			cout << "Opción no válida. Intenta de nuevo." << endl;
		}
		if(verificarFinJuego(puntos_jugador1, puntos_jugador2, jugador1, jugador2)){
			registro <<jugador1<<": "<<puntos_jugador1<<" puntos"<<endl;
			registro <<jugador2<<": "<<puntos_jugador2<<" puntos"<<endl;
			registro.close();
			return true;
		}
	}
	return false;
}


int main() {
	ofstream registro("registro.txt", ios::app);
	cambiarColor("amarillo");
	cout<<"*" << endl;
	cambiarColor("amarillo"); // Cambia el color al amarillo
	cout<< "* Bienvenido al juego de adivinada la palabra!*" <<endl;
	cambiarColor("amarillo");
	cout << "*"<< endl;
	cambiarColor("reset"); // Restaura el color predeterminado
	this_thread::sleep_for(chrono::milliseconds(500));
	cout << "-----------------------------------------------------------------------------------------"<< endl;
	cout <<"-Cada jugador debe adivinar la palabra en función de la definición dada. -" <<endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	cout <<"-Cada letra correcta suma 5 puntos, una pista cuesta 2 puntos y un fallo resta 2 puntos.-" <<endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	cout << "-El juego termina cuando se adivinan todas las palabras o si alguien decide salir. -"<< endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	cout<< "- ---¡QUE COMIENCE EL JUEGO!--- -"<< endl;
	cout << "-----------------------------------------------------------------------------------------"<<endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	cambiarColor("reset");
	cout<< "Jugador 1, ingresa tu nombre: " ;
	string jugador1;
	cin >>jugador1;
	cout <<"Jugador 2, ingresa tu nombre: ";
	string jugador2;
	cin >>jugador2;
	int puntos_jugador1 =5;
	int puntos_jugador2 =5;
	int perdidas_jugador1 =0;
	int perdidas_jugador2 =0;
	map<string, pair<string, vector<char>>>definiciones=obtenerDefDesdeArch("preguntas.txt");
	vector<pair<string, pair<string, vector<char>>>>definiciones_barajadas=mesclardef(definiciones);
	int definicion_actual=0;
	while (definicion_actual < definiciones_barajadas.size()){
		pair<string, pair<string, vector<char>>>definicion=definiciones_barajadas[definicion_actual];
		vector<char> letras_adivinadas(definicion.second.second.size(), '_');
		set<char> letras_ingresadas;
		while (!verificarPalComp(letras_adivinadas)){
			if (manejarTurnoJugador(definicion, puntos_jugador1, perdidas_jugador1, jugador1, letras_ingresadas, letras_adivinadas, jugador1, jugador2, puntos_jugador1, puntos_jugador2, registro)){
				return 0;
			}
			if(verificarFinJuego(puntos_jugador1, puntos_jugador2, jugador1, jugador2) ){
				return 0;
			}
			if (verificarPalComp(letras_adivinadas)){
				break;
			}
			if (manejarTurnoJugador(definicion, puntos_jugador2, perdidas_jugador2, jugador2, letras_ingresadas, letras_adivinadas, jugador1, jugador2, puntos_jugador1, puntos_jugador2, registro)){
				return 0;
			}
			if(verificarFinJuego(puntos_jugador1, puntos_jugador2, jugador1, jugador2) ){
				return 0;
			}
		}
		cout << "¡Felicidades! La palabra era: ";
		mostrarPal(definicion.second.second);
		mostrarPuntajes(jugador1, jugador2, puntos_jugador1, puntos_jugador2);
	}
	definicion_actual++;
	cout <<"El juego ha terminado." << endl;
	if (puntos_jugador1 >puntos_jugador2) {
		cout<< jugador1<<" es el ganador con " << puntos_jugador1<< " puntos!"<<endl;
	} else if (puntos_jugador2> puntos_jugador1) {
		cout<< jugador2<<" es el ganador con " <<puntos_jugador2<< " puntos!"<<endl;
	} else{
		cout<<"¡Es un empate!"<<endl;
	}
	return 0;
}