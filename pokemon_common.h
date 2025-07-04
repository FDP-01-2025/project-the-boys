#ifndef POKEMON_COMMON_H
#define POKEMON_COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Ataque {
    string nombre;
    int danio;
    int pp;
};

enum EfectoEstado { Ninguno, Quemadura, Veneno };

struct Pokemon {
    string nombre;
    int vida;
    int vidaMaxima;
    int puntos;
    int velocidad;  // Nuevo
    int defensa;    // Nuevo
    Ataque ataques[4];
    bool vivo = true;
    EfectoEstado efecto = Ninguno;
    int rondasConEfecto = 0;
};

inline void aplicarEfectos(Pokemon& p) {
    if (p.efecto == Veneno) {
        int danioVeneno = p.vidaMaxima * 0.1;
        p.vida -= danioVeneno;
        cout << p.nombre << " sufre " << danioVeneno << " de dano por veneno!\n";
    }
}



vector<Pokemon> leerPokemons(const string& archivoNombre) {
    ifstream archivo(archivoNombre);
    string linea;
    vector<Pokemon> lista;
    Pokemon p;
    int ataqueIndex = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) {
            lista.push_back(p);
            p = Pokemon();
            ataqueIndex = 0;
            continue;
        }

        if (p.nombre.empty()) {
            p.nombre = linea;
        } else if (linea.find("Vida:") == 0) {
            p.vida = stoi(linea.substr(6));
        } else if (linea.find("Puntos:") == 0) {
            p.puntos = stoi(linea.substr(8));
        } else if (linea.find("Ataque") == 0) {
            size_t pos1 = linea.find(":") + 2;
            size_t pos2 = linea.find(",", pos1);
            size_t pos3 = linea.find(",", pos2 + 1);

            p.ataques[ataqueIndex].nombre = linea.substr(pos1, pos2 - pos1);
            p.ataques[ataqueIndex].danio = stoi(linea.substr(pos2 + 1, pos3 - pos2 - 1));
            p.ataques[ataqueIndex].pp = stoi(linea.substr(pos3 + 1));

            ataqueIndex++;
        }
    }

    if (!p.nombre.empty()) {
        lista.push_back(p);
    }

    return lista;
}

#endif