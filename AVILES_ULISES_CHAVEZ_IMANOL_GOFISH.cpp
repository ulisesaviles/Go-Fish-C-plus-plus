#include <iostream>
#include <locale.h>
#include <string>
#include <ctime> 
using namespace std;
inline int random(int inicio, int limite) { return inicio + rand() % (1 + limite - inicio); }
void print_cards(string manos[6][4], short n_jugadores, short turno, string pool, bool tomo, bool lastRound);
string evaluador(int turno, string manos[6][4]);
int lector_escalera(string caracter);
void desicion(bool gameOver, string jugadas[4], short n_jugadores);
bool valido(string opcion, string manos[6][4], int turno, string v);
int main() {
	srand(time(NULL)); //semilla
	setlocale(LC_ALL, "spanish"); //acentos
	//Hacer baraja
	string baraja_0[52];
	for (short posicion = 0, valor = 1; posicion < 52; valor++, posicion++)
	{
		if (valor == 1)
			baraja_0[posicion] = "A";
		else if (valor == 10)
			baraja_0[posicion] = "X";
		else if (valor > 1 && valor < 11)
			baraja_0[posicion] = to_string(valor);
		else if (valor == 11)
			baraja_0[posicion] = "J";
		else if (valor == 12)
			baraja_0[posicion] = "Q";
		else if (valor == 13)
		{
			baraja_0[posicion] = "R";
			valor = 0;
		}
	}
	//Barajear
	string baraja[52];
	for (short posicion, i = 0; i < 52; i++)
	{
		do {
			posicion = random(0, 51);
			if (baraja_0[posicion] != "")
			{
				baraja[i] = baraja_0[posicion];
				baraja_0[posicion] = "";
				break;
			}
		} while (baraja_0[posicion] == "");
	}
	//cin N_juadores
	string n_jugadores = "0";
	cout << "Bienvenido a Go-Fish!\nIntroduce el número de jugadores: ";
	while (n_jugadores != "2" && n_jugadores != "3" && n_jugadores != "4")
	{
		cin >> n_jugadores;
		if (n_jugadores != "2" && n_jugadores != "3" && n_jugadores != "4")
			cout << "Introduce un valor válido: ";
	}
	//Repartir
	string manos[6][4];
	for (short i = 0, jugadores = 0, b = 51; jugadores <= stoi(n_jugadores)-1; i++, b --)
	{
		manos[i][jugadores] = baraja[b];
		baraja[b] = "";
		if (i == 4)
		{
			jugadores++;
			i = -1;
		}
	}
	//Play
	bool gameOver = false, lastRound = false;
	short turno = random(1, stoi(n_jugadores)), n=0;
	string pool = " ", utilizadas[42],opcion,v,jugadas[4];
	for (int i = 0, u=0; gameOver==false;turno++)
	{
		opcion = "0";
		system("cls"); print_cards(manos, stoi(n_jugadores), turno, pool,false,lastRound);
		cout << "\n\nDe donde deseas tomar tu carta?\n1) Baraja\n2) Pool\n"; 
		while (opcion != "1" && opcion != "2")
		{
			cin >> opcion;
			if (opcion != "1" && opcion != "2" || opcion == "2" && pool == " ")
			{
				cout << "Introduce un valor válido: ";
				opcion = "0";
			}
		}
		if (opcion == "1")
		{
			manos[5][turno-1] = baraja[i];
			i++;
		}
		else if (opcion == "2")
		{
			u--;
			manos[5][turno-1] = pool;
			pool = " ";
		}
		system("cls"); print_cards(manos, stoi(n_jugadores), turno, pool, true, lastRound);
		//Evaluador
		v = evaluador(turno - 1, manos);
		if (v != "")
		{
			//Jugadas maestras
			if (v.substr(0, 1) == "e" && v.substr(2, 1) == "e" && v.substr(4, 1) == "e" || v.substr(0, 1) == "d" && v.substr(2, 1) == "d" || v.substr(0, 1) == "c" && v.substr(2, 1) == "e")
			{
				gameOver = true;
				cout << "\n\nGame Over, ha ganado el jugador " << turno;
				break;
			}
			//last round
			else
				lastRound = true;
		}
		if (lastRound == true)
		{
			jugadas[turno-1] = v;
			n++;
		}
		if (n == stoi(n_jugadores))
			break;
		//Tirar al pool
		else
		{
			opcion = "0";
			cout << "\n\nIntroduce la posición de la carta que deseas tirar al pool: ";
			while (opcion != "1" && opcion != "2" && opcion != "3" && opcion != "4" && opcion != "5" && opcion != "6")
			{
				cin >> opcion;
				if (opcion != "1" && opcion != "2" && opcion != "3" && opcion != "4" && opcion != "5" && opcion != "6"||valido(to_string(stoi(opcion) - 1), manos, turno - 1, v) == false)
				{
					cout << "Introduce un valor válido: ";
					opcion = "0";
				}
			}
			pool = manos[stoi(opcion) - 1][turno-1];
			utilizadas[u] = manos[stoi(opcion) - 1][turno - 1];
			u++;
			if (opcion != "6")
				manos[stoi(opcion) - 1][turno-1] = manos[5][turno-1];
			manos[5][turno-1] = "";
		}
		if (turno == stoi(n_jugadores))
			turno = 0;
		if (i + stoi(n_jugadores) * 5 == 51)
		{
			for (int i = 0; i < 42; i++)
			{
				baraja[i] = utilizadas[i];
				if (utilizadas[i] == "")
					break;
			}
			i = 0;
			u = 0;
		}
	}
	//Desicion
	desicion(gameOver, jugadas, stoi(n_jugadores));
	cin.ignore();
	cin.get();
}
void print_cards(string manos[6][4], short n_jugadores, short turno, string pool, bool tomo, bool lastRound)
{
	if (n_jugadores==4&&turno==1&&tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |   |     |\n|  " << manos[0][0] << "  |   |  " << manos[1][0] << "  |   |  " << manos[2][0] << "  |   |  " << manos[3][0] << "  |   |  " << manos[4][0] << "  |   |  " << manos[5][0] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 1 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  " << manos[0][0] << "  |   |  " << manos[1][0] << "  |   |  " << manos[2][0] << "  |   |  " << manos[3][0] << "  |   |  " << manos[4][0] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 2 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |   |     |\n|  " << manos[0][1] << "  |   |  " << manos[1][1] << "  |   |  " << manos[2][1] << "  |   |  " << manos[3][1] << "  |   |  " << manos[4][1] << "  |   |  " << manos[5][1] << "  |    Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|     _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 2 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  " << manos[0][1] << "  |   |  " << manos[1][1] << "  |   |  " << manos[2][1] << "  |   |  " << manos[3][1] << "  |   |  " << manos[4][1] << "  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 3 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____     _____     |_____|\n|     |   |     |   |     |   |     |   |     |   |     |\n|  " << manos[0][2] << "  |   |  " << manos[1][2] << "  |   |  " << manos[2][2] << "  |   |  " << manos[3][2] << "  |   |  " << manos[4][2] << "  |   |  " << manos[5][2] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|     Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 3 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |   \n|  " << manos[0][2] << "  |   |  " << manos[1][2] << "  |   |  " << manos[2][2] << "  |   |  " << manos[3][2] << "  |   |  " << manos[4][2] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 4 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____     _____     |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |   |     |    |_____|\n|  " << manos[0][3] << "  |   |  " << manos[1][3] << "  |   |  " << manos[2][3] << "  |   |  " << manos[3][3] << "  |   |  " << manos[4][3] << "  |   |  " << manos[5][3] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 4 && turno == 4 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|\n\nJugador 2: \n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|               Pool:\n                                                              _____\nJugador 4:                                                   |     |\n _____     _____     _____     _____     _____               |  " << pool << "  |\n|     |   |     |   |     |   |     |   |     |              |_____|\n|  " << manos[0][3] << "  |   |  " << manos[1][3] << "  |   |  " << manos[2][3] << "  |   |  " << manos[3][3] << "  |   |  " << manos[4][3] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 3 && turno == 1 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |   |     |\n|  " << manos[0][0] << "  |   |  " << manos[1][0] << "  |   |  " << manos[2][0] << "  |   |  " << manos[3][0] << "  |   |  " << manos[4][0] << "  |   |  " << manos[5][0] << "  |    Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|     _____\n                                                             |     |\nJugador 2:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |               Pool:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  " << pool << "  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 3 && turno == 1 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  " << manos[0][0] << "  |   |  " << manos[1][0] << "  |   |  " << manos[2][0] << "  |   |  " << manos[3][0] << "  |   |  " << manos[4][0] << "  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 2:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |               Pool:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  " << pool << "  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 3 && turno == 2 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 2:                                                   |  ?  |\n _____     _____     _____     _____     _____     _____     |_____|\n|     |   |     |   |     |   |     |   |     |   |     |\n|  " << manos[0][1] << "  |   |  " << manos[1][1] << "  |   |  " << manos[2][1] << "  |   |  " << manos[3][1] << "  |   |  " << manos[4][1] << "  |   |  " << manos[5][1] << "  |     Pool:\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|     _____\n                                                             |     |\nJugador 3:                                                   |  " << pool << "  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 3 && turno == 2 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 2:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  " << manos[0][1] << "  |   |  " << manos[1][1] << "  |   |  " << manos[2][1] << "  |   |  " << manos[3][1] << "  |   |  " << manos[4][1] << "  |               Pool:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  " << pool << "  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 3 && turno == 3 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 2:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |               Pool:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  " << pool << "  |\n _____     _____     _____     _____     _____     _____     |_____|\n|     |   |     |   |     |   |     |   |     |   |     |\n|  " << manos[0][2] << "  |   |  " << manos[1][2] << "  |   |  " << manos[2][2] << "  |   |  " << manos[3][2] << "  |   |  " << manos[4][2] << "  |   |  " << manos[5][2] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 3 && turno == 3 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:\n _____     _____     _____     _____     _____\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |              Baraja:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 2:                                                   |  ?  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |               Pool:\n|_____|   |_____|   |_____|   |_____|   |_____|               _____\n                                                             |     |\nJugador 3:                                                   |  " << pool << "  |\n _____     _____     _____     _____     _____               |_____|\n|     |   |     |   |     |   |     |   |     |\n|  " << manos[0][2] << "  |   |  " << manos[1][2] << "  |   |  " << manos[2][2] << "  |   |  " << manos[3][2] << "  |   |  " << manos[4][2] << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|";
	else if (n_jugadores == 2 && turno == 1 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:                                                      Baraja:\n _____     _____     _____     _____     _____     _____         _____\n|     |   |     |   |     |   |     |   |     |   |     |       |     |\n|  " << manos[0][0] << "  |   |  " << manos[1][0] << "  |   |  " << manos[2][0] << "  |   |  " << manos[3][0] << "  |   |  " << manos[4][0] << "  |   |  " << manos[5][0] << "  |       |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|       |_____|\n\nJugador 2:                                                       Pool:\n _____     _____     _____     _____     _____                   _____\n|     |   |     |   |     |   |     |   |     |                 |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |                 |  " << pool << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|                 |_____|";
	else if (n_jugadores == 2 && turno == 1 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:                                                      Baraja:\n _____     _____     _____     _____     _____                   _____\n|     |   |     |   |     |   |     |   |     |                 |     |\n|  " << manos[0][0] << "  |   |  " << manos[1][0] << "  |   |  " << manos[2][0] << "  |   |  " << manos[3][0] << "  |   |  " << manos[4][0] << "  |                 |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|                 |_____|\n\nJugador 2:                                                       Pool:\n _____     _____     _____     _____     _____                   _____\n|     |   |     |   |     |   |     |   |     |                 |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |                 |  " << pool << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|                 |_____|";
	else if (n_jugadores == 2 && turno == 2 && tomo == true)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:                                                      Baraja:\n _____     _____     _____     _____     _____                   _____\n|     |   |     |   |     |   |     |   |     |                 |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |                 |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|                 |_____|\n\nJugador 2:                                                       Pool:\n _____     _____     _____     _____     _____     _____         _____\n|     |   |     |   |     |   |     |   |     |   |     |       |     |\n|  " << manos[0][1] << "  |   |  " << manos[1][1] << "  |   |  " << manos[2][1] << "  |   |  " << manos[3][1] << "  |   |  " << manos[4][1] << "  |   |  " << manos[5][1] << "  |       |  " << pool << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|   |_____|       |_____|";
	else if (n_jugadores == 2 && turno == 2 && tomo == false)
		cout << "Jugador en turno: " << turno << "\n\nJugador 1:                                                      Baraja:\n _____     _____     _____     _____     _____                   _____\n|     |   |     |   |     |   |     |   |     |                 |     |\n|  ?  |   |  ?  |   |  ?  |   |  ?  |   |  ?  |                 |  ?  |\n|_____|   |_____|   |_____|   |_____|   |_____|                 |_____|\n\nJugador 2:                                                       Pool:\n _____     _____     _____     _____     _____                   _____\n|     |   |     |   |     |   |     |   |     |                 |     |\n|  " << manos[0][1] << "  |   |  " << manos[1][1] << "  |   |  " << manos[2][1] << "  |   |  " << manos[3][1] << "  |   |  " << manos[4][1] << "  |                 |  " << pool << "  |\n|_____|   |_____|   |_____|   |_____|   |_____|                 |_____|";
}
string evaluador(int turno, string manos[6][4])
{
	string r="", opcion, tercias="", pares="", cuartas="", acum="";
	//Buscador de escaleras
	for (short a = 0, b = 1, i=0, contador=0; a < 6;i++)
	{	
		if (lector_escalera(manos[a][turno]) == lector_escalera(manos[i][turno]) - b)
		{
			if (contador == 0)
				acum += manos[a][turno] + " ";
			contador++;
			acum += manos[i][turno]+" ";
			i = -1;
			b++;
		}
		else if (contador == 3 && manos[a][turno] == "X" && manos[i][turno] == "A")
			contador++;
		if (i == 5 && contador < 4)
		{
			i = -1;
			a++;
			b = 1;
			contador = 0;
			acum = "";
		}
		if (contador == 4)
		{
			if (manos[a][turno] == "X")
			{
				cout << "\n\nDeseas jugar con flor imperial? [s][n] "; cin >> opcion;
			}
			else
			{
				cout << "\n\nDeseas jugar con la escalera: "<<acum<<"? [s][n] "; cin >> opcion;
			}
			if (opcion == "s")
			{
				if (manos[a][turno] == "X")
					r += "a" + manos[a][turno];
				else
					r += "b" + manos[a][turno];
			}
			break;
		}
	}
	//Buscador de cuarta
	for (short reps = 0, a = 0, b = 1; a <= 2; b++)
	{
		if (manos[a][turno] == manos[a + b][turno])
			reps++;
		if (reps == 3)
		{
			/*cout << "\n\nDeseas jugar con cuarta de " << manos[a][turno] << "? [s][n] "; cin >> opcion;
			if (opcion == "s")
				r += "c" + manos[a][turno];*/
			reps = 0;
			cuartas+= manos[a][turno];
			break;
		}
		if (b + a == 6)
		{
			a++;
			b = 0;
			reps = 0;
		}
	}
	//Buscador de tercias
	for (short reps = 0, a = 0, b = 1; r.size()==0&&a<=4&&cuartas.size()==0; b++)
	{
		if (manos[a][turno] == manos[a + b][turno])
			reps++;
		if (reps == 2)
		{
			tercias += manos[a][turno];
			reps = 0;
		}
		if (b + a == 6)
		{
			a++;
			b = 0;
			reps = 0;
		}
	}
	//Buscador de pares
	for (short reps = 0, a = 0, b = 1; r.size() == 0 && a <= 5 && tercias.size()<2; b++)
	{
		if (manos[a][turno] == manos[a + b][turno])
			reps++;
		if (reps == 1)
		{
			if(manos[a][turno]!=tercias && manos[a][turno] != cuartas)
				pares += manos[a][turno];
			reps = 0;
		}
		if (b + a == 6)
		{
			a++;
			b = 0;
			reps = 0;
		}
	}
	if (pares.size() == 3)
		r += "e" + pares.substr(0, 1) + "e" + pares.substr(1, 1) + "e" + pares.substr(2, 1);
	else if (tercias.size()==2)
		r += "d" + tercias.substr(0, 1) + "d" + tercias.substr(1, 1);
	else if (cuartas.size() == 1)
	{
		if(pares.size()==1)
			r += "c" + cuartas + "e" + pares;
		else
		{
			cout << "\n\nDeseas jugar con cuarta de " << cuartas << "? [s][n] "; cin >> opcion;
			if (opcion == "s")
				r += "c" + cuartas;
		}
	}
	else if (pares.size() == 1 && tercias.size() == 1)
	{
		cout << "\n\nDeseas jugar con par de " << pares.substr(0, 1) << " y tercia de " << tercias.substr(0, 1) << "? [s] [n]"; cin >> opcion;
		if (opcion == "s")
			r += "e" + pares.substr(0, 1) + "d" + tercias.substr(0, 1);
	}
	return r;
}
int lector_escalera(string caracter)
{
	if (caracter == "A")
		return 1;
	else if (caracter == "X")
		return 10;
	else if (caracter == "J")
		return 11;
	else if (caracter == "Q")
		return 12;
	else if (caracter == "R")
		return 13;
	else
		return stoi(caracter);
}
void desicion(bool gameOver, string jugadas[4], short n_jugadores)
{
	string puntuaciones[4] = { "0","0","0","0" };
	//for (int i = 0; i < 4; i++)
	//	/*cout << jugadas[i] << "\n";*/
		if (gameOver == false)
		{
			for (int i = 0; i < n_jugadores; i++)
			{
				/*cout << "Tamaño: "<<jugadas[i].size() << "\n";*/
				if (jugadas[i].size() != 0)
				{
					for (int x = 0; x < jugadas[i].size(); x++)
					{
						/*cout << x << " lecturas " << jugadas[i].substr(x, 1) << "\n";*/
						if (jugadas[i].substr(x, 1) == "d")
							puntuaciones[i] = to_string(50 + lector_escalera(jugadas[i].substr(x + 1, 1)));
						else if (jugadas[i].substr(x, 1) == "e")
							puntuaciones[i] = to_string(40 + lector_escalera(jugadas[i].substr(x + 1, 1)));
						else if (jugadas[i].substr(x, 1) == "c")
							puntuaciones[i] = to_string(300 + lector_escalera(jugadas[i].substr(x + 1, 1)));
						else if (jugadas[i].substr(x, 1) == "a")
							puntuaciones[i] = to_string(500);
						else if (jugadas[i].substr(x, 1) == "b")
							puntuaciones[i] = to_string(400 + lector_escalera(jugadas[i].substr(x + 1, 1)));
					}
				}
				else
					puntuaciones[i] = "0";
			}

			/*for (int i = 0; i < 4; i++)
				cout << "Puntuacion del J" << i + 1 << " = " << puntuaciones[i] << "\n";*/
			if (stoi(puntuaciones[0]) >= stoi(puntuaciones[1]) && stoi(puntuaciones[0]) >= stoi(puntuaciones[2]) && stoi(puntuaciones[0]) >= stoi(puntuaciones[3]))
				cout << "\n\nEl ganador es el jugador 1, felicidades!";
			else if (stoi(puntuaciones[1]) >= stoi(puntuaciones[0]) && stoi(puntuaciones[1]) >= stoi(puntuaciones[2]) && stoi(puntuaciones[1]) >= stoi(puntuaciones[3]))
				cout << "\n\nEl ganador es el jugador 2, felicidades!";
			else if (stoi(puntuaciones[2]) >= stoi(puntuaciones[1]) && stoi(puntuaciones[2]) >= stoi(puntuaciones[0]) && stoi(puntuaciones[2]) >= stoi(puntuaciones[3]))
				cout << "\n\nEl ganador es el jugador 3, felicidades!";
			else if (stoi(puntuaciones[3]) >= stoi(puntuaciones[1]) && stoi(puntuaciones[3]) >= stoi(puntuaciones[2]) && stoi(puntuaciones[3]) >= stoi(puntuaciones[0]))
				cout << "\n\nEl ganador es el jugador 4, felicidades!";
		}
}
bool valido(string opcion, string manos[6][4], int turno, string v)
{
	if (opcion != "1" && opcion != "2" && opcion != "3" && opcion != "4" && opcion != "5" && opcion != "0")
	{
		return false;
	}
	else
	{
		if (v.size() == 0)
			return true;
		else
		{
			//Pares, tercias y caurtas
			for (int i = 0; i < v.size() - 1; i += 2)
			{
				if (v.substr(i, 1) == "c" && manos[stoi(opcion)][turno] == v.substr(i + 1, 1) || v.substr(i, 1) == "d" && manos[stoi(opcion)][turno] == v.substr(i + 1, 1) || v.substr(i, 1) == "e" && manos[stoi(opcion)][turno] == v.substr(i + 1, 1))
				{
					cout << "Esa carta forma parte de la jugada actual\n";
					return false;
				}
			}
			//Escaleras
			if (v.substr(0, 1) == "a" || v.substr(0, 1) == "b")
			{
				for (int i = 0; i < 5; i++)
				{
					if (lector_escalera(manos[stoi(opcion)][turno]) == lector_escalera(v.substr(1, 1))+i)
					{
						cout << "Esa carta forma parte de la jugada actual\n";
						return false;
					}
				}
				if (v.substr(0, 1) == "a"&&manos[stoi(opcion)][turno]=="A")
				{
					cout << "Esa carta forma parte de la jugada actual\n";
					return false;
				}
			}
		}
	}
}