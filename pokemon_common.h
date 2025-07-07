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
    {{"Fuego", "Planta"}, 2.0},
    {{"Fuego", "Hielo"}, 2.0},
    {{"Fuego", "Bicho"}, 2.0},
    {{"Fuego", "Fuego"}, 0.5},
    {{"Agua", "Fuego"}, 2.0},
    {{"Agua", "Roca"}, 2.0},
    {{"Agua", "Tierra"}, 2.0},
    {{"Agua", "Agua"}, 0.5},
    {{"Agua", "Planta"}, 0.5},
    {{"Agua", "Dragón"}, 0.5},
    {{"Fuego", "Agua"}, 0.5},
    {{"Fuego", "Roca"}, 0.5},
    {{"Fuego", "Dragón"}, 0.5},
    {{"Planta", "Agua"}, 2.0},
    {{"Planta", "Roca"}, 2.0},
    {{"Planta", "Tierra"}, 2.0},
    {{"Planta", "Fuego"}, 0.5},
    {{"Planta", "Planta"}, 0.5},
    {{"Planta", "Bicho"}, 0.5},
    {{"Planta", "Volador"}, 0.5},
    {{"Planta", "Veneno"}, 0.5},
    {{"Planta", "Dragón"}, 0.5},
    {{"Eléctrico", "Agua"}, 2.0},
    {{"Eléctrico", "Volador"}, 2.0},
    {{"Eléctrico", "Planta"}, 0.5},
    {{"Eléctrico", "Eléctrico"}, 0.5},
    {{"Eléctrico", "Dragón"}, 0.5},
    {{"Eléctrico", "Tierra"}, 0.0},
    {{"Hielo", "Planta"}, 2.0},
    {{"Hielo", "Tierra"}, 2.0},
    {{"Hielo", "Volador"}, 2.0},
    {{"Hielo", "Dragón"}, 2.0},
    {{"Hielo", "Fuego"}, 0.5},
    {{"Hielo", "Agua"}, 0.5},
    {{"Hielo", "Hielo"}, 0.5},
    {{"Lucha", "Normal"}, 2.0},
    {{"Lucha", "Roca"}, 2.0},
    {{"Lucha", "Hielo"}, 2.0},
    {{"Lucha", "Veneno"}, 0.5},
    {{"Lucha", "Volador"}, 0.5},
    {{"Lucha", "Psíquico"}, 0.5},
    {{"Lucha", "Bicho"}, 0.5},
    {{"Lucha", "Fantasma"}, 0.0},
    {{"Veneno", "Planta"}, 2.0},
    {{"Veneno", "Bicho"}, 2.0},
    {{"Veneno", "Veneno"}, 0.5},
    {{"Veneno", "Tierra"}, 0.5},
    {{"Veneno", "Roca"}, 0.5},
    {{"Veneno", "Fantasma"}, 0.5},
    {{"Tierra", "Fuego"}, 2.0},
    {{"Tierra", "Eléctrico"}, 2.0},
    {{"Tierra", "Roca"}, 2.0},
    {{"Tierra", "Veneno"}, 2.0},
    {{"Tierra", "Bicho"}, 0.5},
    {{"Tierra", "Planta"}, 0.5},
    {{"Tierra", "Volador"}, 0.0},
    {{"Volador", "Planta"}, 2.0},
    {{"Volador", "Lucha"}, 2.0},
    {{"Volador", "Bicho"}, 2.0},
    {{"Volador", "Eléctrico"}, 0.5},
    {{"Volador", "Roca"}, 0.5},
    {{"Psíquico", "Lucha"}, 2.0},
    {{"Psíquico", "Veneno"}, 2.0},
    {{"Psíquico", "Psíquico"}, 0.5},
    {{"Bicho", "Planta"}, 2.0},
    {{"Bicho", "Psíquico"}, 2.0},
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
    {{"Fantasma", "Psíquico"}, 0.0},
    {{"Dragón", "Dragón"}, 2.0}};
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
                cerr << "Error: Vida no es un número válido: '" << valor << "'" << endl;
                p.Vida = p.VidaMaxima = 0;
            }
        }
        else if (linea.find("Puntos:") == 0)
        {
            string valor = trim(linea.substr(7 + 1));
            if (esNumero(valor)) {
                p.Puntos = stringAInt(valor);
            } else {
                cerr << "Error: Puntos no es un número válido: '" << valor << "'" << endl;
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