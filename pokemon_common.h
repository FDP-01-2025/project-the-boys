#ifndef POKEMON_COMMON_H
#define POKEMON_COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

struct Ataque
{
    string nombre;
    int danio;
    int pp;
};

enum EfectoEstado
{
    Ninguno,
    Quemadura,
    Veneno
};

struct Pokemon
{
    string Nombre;
    string Tipo;
    int Vida;
    int VidaMaxima;
    int Puntos;
    int Velocidad;
    int Defensa;
    Ataque Ataques[4];
    bool vivo = true;
    EfectoEstado efecto = Ninguno;
    int RondasConEfecto = 0;
    int ContadorGolpes = 0;
};

map<pair<string, string>, float> TablaTipos = {
    {{"Fuego", "Hielo"}, 2.0},
    {{"Fuego", "Planta"}, 2.0},
    {{"Fuego", "Bicho"}, 2.0},
    {{"Fuego", "Fuego"}, 0.5},
    {{"Fuego", "Agua"}, 0.5},
    {{"Fuego", "Roca"}, 0.5},

    {{"Veneno", "Planta"}, 2.0},
    {{"Veneno", "Bicho"}, 2.0},
    {{"Veneno", "Veneno"}, 0.5},
    {{"Veneno", "Tierra"}, 0.5},

    {{"Agua", "Fuego"}, 2.0},
    {{"Agua", "Roca"}, 2.0},
    {{"Agua", "Agua"}, 0.5},

    {{"Hielo", "Planta"}, 2.0},
    {{"Hielo", "Tierra"}, 2.0},
    {{"Hielo", "Volador"}, 2.0},
    {{"Hielo", "Dragón"}, 2.0},
    {{"Hielo", "Fuego"}, 0.5},
    {{"Hielo", "Agua"}, 0.5},
    {{"Hielo", "Hielo"}, 0.5},
};

inline float obtenerMultiplicador(string atacante, string defensor)
{
    auto clave = make_pair(atacante, defensor);
    if (TablaTipos.count(clave))
    {
        return TablaTipos[clave];
    }
    return 1.0;
}

inline void aplicarEfectos(Pokemon &p)
{
    if (p.efecto == Veneno)
    {
        int danio = p.VidaMaxima * 0.1;
        p.Vida -= danio;
        cout << p.Nombre << " sufre " << danio << " de daño por veneno!\n";
        p.RondasConEfecto--;
        if (p.RondasConEfecto <= 0)
        {
            p.efecto = Ninguno;
        }
    }
    else if (p.efecto == Quemadura)
    {
        int danio = p.VidaMaxima * 0.05;
        p.Vida -= danio;
        cout << p.Nombre << " sufre " << danio << " de daño por quemadura!\n";
        p.RondasConEfecto--;
        if (p.RondasConEfecto <= 0)
        {
            p.efecto = Ninguno;
        }
    }
}

inline void aplicarDanio(Pokemon &atacante, Pokemon &defensor, Ataque ataque)
{
    float mult = obtenerMultiplicador(atacante.Tipo, defensor.Tipo);
    int danioFinal = static_cast<int>(ataque.danio * mult);
    defensor.Vida -= danioFinal;

    cout << atacante.Nombre << " usa " << ataque.nombre << " e inflige " << danioFinal << " de daño a " << defensor.Nombre << "!\n";

    atacante.ContadorGolpes++;

    if (atacante.Tipo == "Fuego" && atacante.ContadorGolpes == 4 && defensor.efecto == Ninguno)
    {
        defensor.efecto = Quemadura;
        defensor.RondasConEfecto = 3;
        cout << defensor.Nombre << " ha sido quemado!\n";
        atacante.ContadorGolpes = 0;
    }
    else if (atacante.Tipo == "Veneno" && atacante.ContadorGolpes == 6 && defensor.efecto == Ninguno)
    {
        defensor.efecto = Veneno;
        defensor.RondasConEfecto = 2;
        cout << defensor.Nombre << " ha sido envenenado!\n";
        atacante.ContadorGolpes = 0;
    }
}

vector<Pokemon> leerPokemons(const string &archivoNombre)
{
    ifstream archivo(archivoNombre);
    string linea;
    vector<Pokemon> lista;
    Pokemon p;
    int ataqueIndex = 0;

    while (getline(archivo, linea))
    {
        if (linea.empty())
        {
            lista.push_back(p);
            p = Pokemon();
            ataqueIndex = 0;
            continue;
        }

        if (p.Nombre.empty())
        {
            p.Nombre = linea;
        }
        else if (linea.find("Tipo:") == 0)
        {
            p.Tipo = linea.substr(6);
        }
        else if (linea.find("Vida:") == 0)
        {
            p.Vida = stoi(linea.substr(6));
            p.VidaMaxima = p.Vida;
        }
        else if (linea.find("Puntos:") == 0)
        {
            p.Puntos = stoi(linea.substr(8));
        }
        else if (linea.find("Ataque") == 0)
        {
            size_t pos1 = linea.find(":") + 2;
            size_t pos2 = linea.find(",", pos1);
            size_t pos3 = linea.find(",", pos2 + 1);

            p.Ataques[ataqueIndex].nombre = linea.substr(pos1, pos2 - pos1);
            p.Ataques[ataqueIndex].danio = stoi(linea.substr(pos2 + 1, pos3 - pos2 - 1));
            p.Ataques[ataqueIndex].pp = stoi(linea.substr(pos3 + 1));

            ataqueIndex++;
        }
    }

    if (!p.Nombre.empty())
    {
        lista.push_back(p);
    }

    return lista;
}

#endif