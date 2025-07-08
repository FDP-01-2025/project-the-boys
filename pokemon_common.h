#ifndef POKEMON_COMMON_H
#define POKEMON_COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

struct Ataque
{
    string nombre;
    int danio;
    int pp;
    int precision = 100;
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
};

map<pair<string, string>, float> TablaTipos = {
    {{"Fuego", "Planta"}, 2.0},
    {{"Fuego", "Hielo"}, 2.0},
    {{"Fuego", "Bicho"}, 2.0},
    {{"Fuego", "Fuego"}, 0.5},
    {{"Agua", "Fuego"}, 2.0},
    {{"Agua", "Roca"}, 2.0},
    {{"Agua", "Tierra"}, 2.0},
    {{"Agua", "Agua"}, 0.5},
    {{"Agua", "Planta"}, 0.5},
    {{"Agua", "Dragon"}, 0.5},
    {{"Fuego", "Agua"}, 0.5},
    {{"Fuego", "Roca"}, 0.5},
    {{"Fuego", "Dragon"}, 0.5},
    {{"Planta", "Agua"}, 2.0},
    {{"Planta", "Roca"}, 2.0},
    {{"Planta", "Tierra"}, 2.0},
    {{"Planta", "Fuego"}, 0.5},
    {{"Planta", "Planta"}, 0.5},
    {{"Planta", "Bicho"}, 0.5},
    {{"Planta", "Volador"}, 0.5},
    {{"Planta", "Veneno"}, 0.5},
    {{"Planta", "Dragon"}, 0.5},
    {{"Electrico", "Agua"}, 2.0},
    {{"Electrico", "Volador"}, 2.0},
    {{"Electrico", "Planta"}, 0.5},
    {{"Electrico", "Electrico"}, 0.5},
    {{"Electrico", "Dragon"}, 0.5},
    {{"Electrico", "Tierra"}, 0.0},
    {{"Hielo", "Planta"}, 2.0},
    {{"Hielo", "Tierra"}, 2.0},
    {{"Hielo", "Volador"}, 2.0},
    {{"Hielo", "Dragon"}, 2.0},
    {{"Hielo", "Fuego"}, 0.5},
    {{"Hielo", "Agua"}, 0.5},
    {{"Hielo", "Hielo"}, 0.5},
    {{"Lucha", "Normal"}, 2.0},
    {{"Lucha", "Roca"}, 2.0},
    {{"Lucha", "Hielo"}, 2.0},
    {{"Lucha", "Veneno"}, 0.5},
    {{"Lucha", "Volador"}, 0.5},
    {{"Lucha", "Psiquico"}, 0.5},
    {{"Lucha", "Bicho"}, 0.5},
    {{"Lucha", "Fantasma"}, 0.0},
    {{"Veneno", "Planta"}, 2.0},
    {{"Veneno", "Bicho"}, 2.0},
    {{"Veneno", "Veneno"}, 0.5},
    {{"Veneno", "Tierra"}, 0.5},
    {{"Veneno", "Roca"}, 0.5},
    {{"Veneno", "Fantasma"}, 0.5},
    {{"Tierra", "Fuego"}, 2.0},
    {{"Tierra", "Electrico"}, 2.0},
    {{"Tierra", "Roca"}, 2.0},
    {{"Tierra", "Veneno"}, 2.0},
    {{"Tierra", "Bicho"}, 0.5},
    {{"Tierra", "Planta"}, 0.5},
    {{"Tierra", "Volador"}, 0.0},
    {{"Volador", "Planta"}, 2.0},
    {{"Volador", "Lucha"}, 2.0},
    {{"Volador", "Bicho"}, 2.0},
    {{"Volador", "Electrico"}, 0.5},
    {{"Volador", "Roca"}, 0.5},
    {{"Psiquico", "Lucha"}, 2.0},
    {{"Psiquico", "Veneno"}, 2.0},
    {{"Psiquico", "Psiquico"}, 0.5},
    {{"Bicho", "Planta"}, 2.0},
    {{"Bicho", "Psiquico"}, 2.0},
    {{"Bicho", "Fuego"}, 0.5},
    {{"Bicho", "Lucha"}, 0.5},
    {{"Bicho", "Fantasma"}, 0.5},
    {{"Roca", "Fuego"}, 2.0},
    {{"Roca", "Hielo"}, 2.0},
    {{"Roca", "Volador"}, 2.0},
    {{"Roca", "Bicho"}, 2.0},
    {{"Roca", "Lucha"}, 0.5},
    {{"Roca", "Tierra"}, 0.5},
    {{"Fantasma", "Fantasma"}, 2.0},
    {{"Fantasma", "Normal"}, 0.0},
    {{"Fantasma", "Psiquico"}, 0.0},
    {{"Dragon", "Dragon"}, 2.0}};

inline float obtenerMultiplicador(string atacante, string defensor)
{
    auto clave = make_pair(atacante, defensor);
    if (TablaTipos.count(clave))
    {
        return TablaTipos[clave];
    }
    return 1.0;
}

inline std::string colorTipo(const std::string& tipo) {
    if (tipo == "Agua") return "\033[34m" + tipo + "\033[0m";
    if (tipo == "Fuego") return "\033[31m" + tipo + "\033[0m";
    if (tipo == "Planta") return "\033[32m" + tipo + "\033[0m";
    if (tipo == "Electrico") return "\033[33m" + tipo + "\033[0m";
    if (tipo == "Hielo") return "\033[36m" + tipo + "\033[0m";
    if (tipo == "Lucha") return "\033[91m" + tipo + "\033[0m";
    if (tipo == "Veneno") return "\033[32m" + tipo + "\033[0m";
    if (tipo == "Tierra") return "\033[33m" + tipo + "\033[0m";
    if (tipo == "Volador") return "\033[94m" + tipo + "\033[0m";
    if (tipo == "Psiquico") return "\033[95m" + tipo + "\033[0m";
    if (tipo == "Bicho") return "\033[92m" + tipo + "\033[0m";
    if (tipo == "Roca") return "\033[33m" + tipo + "\033[0m";
    if (tipo == "Fantasma") return "\033[95m" + tipo + "\033[0m";
    if (tipo == "Dragon") return "\033[34m" + tipo + "\033[0m";
    if (tipo == "Normal") return "\033[37m" + tipo + "\033[0m";
    return tipo;
}

inline int calcularDanioBase(int danioBase, int defensa) {
    int danioFinal = danioBase - (defensa / 4);
    return (danioFinal < 1) ? 1 : danioFinal;
}

inline bool ataqueAcierta(int precision) {
    return (rand() % 100) < precision;
}

inline vector<Pokemon> seleccionarPokemonsUsuario(vector<Pokemon>& pokemons, int cantidad) {
    vector<Pokemon> seleccionados;
    for (int i = 0; i < cantidad; ++i) {
        cout << "\nSelecciona el Pokemon #" << (i + 1) << ":\n";
        for (size_t j = 0; j < pokemons.size(); ++j) {
            cout << j + 1 << ". " << pokemons[j].Nombre << " "
                 << colorTipo(pokemons[j].Tipo)
                 << " (Vida: " << pokemons[j].Vida << ")" << endl;
        }
        int eleccion;
        do {
            cout << "Opcion: ";
            cin >> eleccion;
            if (cin.fail() || eleccion < 1 || eleccion > (int)pokemons.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Opcion invalida. Intenta de nuevo.\n";
            } else {
                break;
            }
        } while (true);

        Pokemon elegido = pokemons[eleccion - 1];
        seleccionados.push_back(elegido);
        pokemons.erase(pokemons.begin() + eleccion - 1);
    }
    return seleccionados;
}

inline void aplicarEfectos(Pokemon &p)
{
    if (p.efecto == Veneno)
    {
        int danio = p.VidaMaxima * 0.1;
        p.Vida -= danio;
        cout << p.Nombre << " sufre " << danio << " de dano por veneno!\n";
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
        cout << p.Nombre << " sufre " << danio << " de dano por quemadura!\n";
        p.RondasConEfecto--;
        if (p.RondasConEfecto <= 0)
        {
            p.efecto = Ninguno;
        }
    }
}

inline string trim(const string& s) {
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}

inline bool esNumero(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

inline int stringAInt(const string& s) {
    istringstream iss(s);
    int valor = 0;
    iss >> valor;
    if (iss.fail()) {
        return 0;
    }
    return valor;
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
        linea = trim(linea);
        if (linea.empty())
        {
            if (!p.Nombre.empty())
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
            p.Tipo = trim(linea.substr(5 + 1));
        }
        else if (linea.find("Vida:") == 0)
        {
            string valor = trim(linea.substr(5 + 1));
            if (esNumero(valor)) {
                p.Vida = stringAInt(valor);
                p.VidaMaxima = p.Vida;
            } else {
                cerr << "Error: Vida no es un numero valido: '" << valor << "'" << endl;
                p.Vida = p.VidaMaxima = 0;
            }
        }
        else if (linea.find("Puntos:") == 0)
        {
            string valor = trim(linea.substr(7 + 1));
            if (esNumero(valor)) {
                p.Puntos = stringAInt(valor);
            } else {
                cerr << "Error: Puntos no es un numero valido: '" << valor << "'" << endl;
                p.Puntos = 0;
            }
        }
        else if (linea.find("Ataque") == 0)
        {
            size_t pos1 = linea.find(":");
            if (pos1 == string::npos) continue;
            string resto = trim(linea.substr(pos1 + 1));
            size_t pos2 = resto.find(",");
            size_t pos3 = resto.find(",", pos2 + 1);

            if (pos2 != string::npos && pos3 != string::npos) {
                string nombre = trim(resto.substr(0, pos2));
                string danioStr = trim(resto.substr(pos2 + 1, pos3 - pos2 - 1));
                string ppStr = trim(resto.substr(pos3 + 1));

                p.Ataques[ataqueIndex].nombre = nombre;
                p.Ataques[ataqueIndex].danio = esNumero(danioStr) ? stringAInt(danioStr) : 0;
                p.Ataques[ataqueIndex].pp = esNumero(ppStr) ? stringAInt(ppStr) : 0;
                ataqueIndex++;
            } else {
                cerr << "Error en formato de ataque: '" << linea << "'" << endl;
            }
        }
    }

    if (!p.Nombre.empty())
    {
        lista.push_back(p);
    }

    return lista;
}

#endif