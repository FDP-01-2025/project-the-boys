#include <iostream>
#include <fstream>
#include "pokemon_common.h"
#include "pvp.h"
#include "pve.h"

using namespace std;

void PvPmatch()
{
    ifstream archivo("pvp.txt");

    if (archivo.is_open())
    {
        cout << endl;
        string linea;
        while (getline(archivo, linea))
        {
            cout << linea << endl;
        }
        archivo.close();
        cout << "..." << endl;
    }
    else
    {
        cout << "Error al abrir el archivo" << endl;
    }
}

void PvEmatch()
{
    ifstream archivo("pve2.txt");

    if (archivo.is_open())
    {
        cout << endl;
        string linea;
        while (getline(archivo, linea))
        {
            cout << linea << endl;
        }
        archivo.close();
        cout << "..." << endl;
    }
    else
    {
        cout << "Error al abrir el archivo" << endl;
    }
}

void Matchrecords()
{
    cout << "\n[Historial de partidas proximamente disponible.]\n";
}

int main()
{
    int menu;

    // Cargar pokémons para ambos modos desde el archivo común
    vector<Pokemon> pokemonsParaPvp = leerPokemons("Pokemon_formato_para_juego.txt");
    vector<Pokemon> pokemonsParaPve = leerPokemons("Pokemon_formato_para_juego.txt");

    do
    {
        cout << "\nWelcome to pokemon world's" << endl;
        cout << "What do you want to do?" << endl;
        cout << "1. PvP match" << endl;
        cout << "2. PvE match" << endl;
        cout << "3. Match records" << endl;
        cout << "4. Exit game" << endl;
        cin >> menu;

        switch (menu)
        {
        case 1:
        {
            PvPmatch();
            cout << "\nPvP - Jugador 1 selecciona su equipo:\n";

            if (pokemonsParaPvp.size() < 6)
            {
                cout << "No hay suficientes Pokemon para PvP.\n";
                break;
            }

            vector<Pokemon> copiaPokemons = pokemonsParaPvp;
            vector<Pokemon> equipo1 = seleccionarPokemonsUsuario(copiaPokemons, 3);
            vector<Pokemon> equipo2 = seleccionarPokemonsUsuario(copiaPokemons, 3);

            batallaPvP(equipo1, equipo2);
            break;
        }

        case 2:
        {
            PvEmatch();
            vector<Pokemon> equipoUsuario = seleccionarPokemonsUsuario(pokemonsParaPve, 3);
            vector<Pokemon> enemigos = pokemonsParaPve;
            batallaPvE(equipoUsuario, enemigos);
            break;
        }

        case 3:
            Matchrecords();
            break;

        case 4:
            cout << "Closing game" << endl;
            break;

        default:
            cout << "Please select a valid option" << endl;
            break;
        }

    } while (menu != 4);

    return 0;
}