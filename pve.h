#ifndef PVE_H
#define PVE_H

#include "pokemon_common.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <sys/stat.h>

using namespace std;

// --- Crea Mochila.txt si no existe ---
inline void crearMochilaSiNoExiste() {
    struct stat buffer;
    if (stat("Mochila.txt", &buffer) != 0) {
        ofstream file("Mochila.txt");
        file << "Small potion (heals 20 hp)\n";
        file << "Medium potion (heals 100 hp)\n";
        file << "Large potion (heals all hp)\n";
        file << "Revive (revives a pokemon with half its hp)\n";
        file.close();
        cout << "Archivo Mochila.txt creado con objetos iniciales." << endl;
    }
}

// Lee objetos de Mochila.txt
inline vector<string> leerMochila(const string& archivo = "Mochila.txt") {
    vector<string> objetos;
    ifstream file(archivo);
    string linea;
    while (getline(file, linea)) {
        if (!linea.empty())
            objetos.push_back(linea);
    }
    return objetos;
}

// Guarda la mochila actualizada (cuando se usa un objeto)
inline void guardarMochila(const string& archivo, const vector<string>& objetos) {
    ofstream file(archivo, ios::trunc);
    for (const auto& obj : objetos) {
        file << obj << endl;
    }
}

// Muestra la mochila y devuelve el indice elegido por el usuario (o -1 si cancela)
inline int mostrarMochila(const vector<string>& objetos) {
    cout << "\n--- Mochila ---" << endl;
    if (objetos.empty()) {
        cout << "La mochila esta vacia." << endl;
        return -1;
    }
    for (size_t i = 0; i < objetos.size(); ++i) {
        cout << (i + 1) << ". " << objetos[i] << endl;
    }
    cout << "0. Cancelar" << endl;
    int opcion;
    do {
        cout << "Elige un objeto (0-" << objetos.size() << "): ";
        cin >> opcion;
        if (cin.fail() || opcion < 0 || opcion > (int)objetos.size()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion invalida. Intenta de nuevo.\n";
        } else {
            break;
        }
    } while (true);
    return opcion - 1;
}

// Elige un Pokemon objetivo (puede estar vivo o muerto si revive)
inline int elegirPokemonObjetivo(const vector<Pokemon>& equipo, bool paraRevive) {
    cout << "\nElige el Pokemon ";
    if (paraRevive)
        cout << "a revivir:" << endl;
    else
        cout << "a curar:" << endl;

    vector<int> indicesValidos;
    for (size_t i = 0; i < equipo.size(); ++i) {
        if (paraRevive) {
            if (!equipo[i].vivo || equipo[i].Vida == 0)
                cout << (indicesValidos.size() + 1) << ". " << equipo[i].Nombre
                     << " (Vida: " << equipo[i].Vida << "/" << equipo[i].VidaMaxima << ") [DEBILITADO]" << endl,
                indicesValidos.push_back(i);
        } else {
            if (equipo[i].vivo && equipo[i].Vida > 0)
                cout << (indicesValidos.size() + 1) << ". " << equipo[i].Nombre
                     << " (Vida: " << equipo[i].Vida << "/" << equipo[i].VidaMaxima << ")" << endl,
                indicesValidos.push_back(i);
        }
    }
    if (indicesValidos.empty()) {
        cout << "No hay Pokemon validos para esta accion.\n";
        return -1;
    }
    cout << "0. Cancelar" << endl;
    int eleccion;
    do {
        cout << "Opcion: ";
        cin >> eleccion;
        if (cin.fail() || eleccion < 0 || eleccion > (int)indicesValidos.size()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion invalida. Intenta de nuevo.\n";
        } else {
            break;
        }
    } while (true);
    if (eleccion == 0)
        return -1;
    return indicesValidos[eleccion - 1];
}

// Extrae puntos de cura de la descripcion del objeto
inline int puntosDeCura(const string& objeto) {
    smatch m;
    regex exp("\\b(\\d+)\\s*hp\\b", regex::icase);
    if (regex_search(objeto, m, exp) && m.size() > 1) {
        return stoi(m[1]);
    }
    if (objeto.find("all hp") != string::npos)
        return -1;
    return 0;
}

// Aplica el efecto del objeto y devuelve true si se uso, false si no
inline bool usarObjetoMochila(const string& objeto, vector<Pokemon>& equipo) {
    if (objeto.find("potion") != string::npos) {
        int puntos = puntosDeCura(objeto);
        int idx = elegirPokemonObjetivo(equipo, false);
        if (idx == -1) return false;
        if (!equipo[idx].vivo || equipo[idx].Vida == 0) {
            cout << equipo[idx].Nombre << " no puede ser curado porque esta debilitado.\n";
            return false;
        }
        int antes = equipo[idx].Vida;
        if (puntos == -1) {
            equipo[idx].Vida = equipo[idx].VidaMaxima;
            cout << "Curaste completamente a " << equipo[idx].Nombre << ".\n";
        } else {
            equipo[idx].Vida += puntos;
            if (equipo[idx].Vida > equipo[idx].VidaMaxima)
                equipo[idx].Vida = equipo[idx].VidaMaxima;
            cout << "Curaste a " << equipo[idx].Nombre << " por " << (equipo[idx].Vida - antes) << " puntos.\n";
        }
        return true;
    } else if (objeto.find("revive") != string::npos) {
        int idx = elegirPokemonObjetivo(equipo, true);
        if (idx == -1) return false;
        if (equipo[idx].vivo && equipo[idx].Vida > 0) {
            cout << equipo[idx].Nombre << " ya esta vivo.\n";
            return false;
        }
        equipo[idx].vivo = true;
        equipo[idx].Vida = equipo[idx].VidaMaxima / 2;
        if (equipo[idx].Vida == 0) equipo[idx].Vida = 1;
        cout << "Reviviste a " << equipo[idx].Nombre << " con " << equipo[idx].Vida << " puntos de vida.\n";
        return true;
    }
    cout << "Objeto invalido o no implementado.\n";
    return false;
}

// --- Opcion de mochila ---
inline void abrirMochila(vector<Pokemon>& equipo) {
    vector<string> objetos = leerMochila("Mochila.txt");
    int eleccion = mostrarMochila(objetos);
    if (eleccion < 0 || eleccion >= (int)objetos.size()) {
        cout << "No se uso ningun objeto.\n";
        return;
    }
    bool usado = usarObjetoMochila(objetos[eleccion], equipo);
    if (usado) {
        objetos.erase(objetos.begin() + eleccion);
        guardarMochila("Mochila.txt", objetos);
    }
    cout << "Pulsa enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

inline void curarEquipo(vector<Pokemon>& equipo) {
    for (auto& poke : equipo) {
        if (poke.vivo && poke.Vida > 0)
            poke.Vida = poke.VidaMaxima;
    }
    cout << "\nTodo tu equipo ha sido curado!" << endl;
}

// --- Estructuras ---
struct TempJugador {
    double incrementoVida = 1.5;
    double incrementoAtaque = 1.8;

    void subirStats(Pokemon& p) {
        p.Vida = static_cast<int>(p.Vida * incrementoVida);
        if (p.Vida > p.VidaMaxima) p.Vida = p.VidaMaxima;
        for (auto& atk : p.Ataques) {
            atk.danio = static_cast<int>(atk.danio * incrementoAtaque);
        }
    }
};
struct TempBot {
    double incrementoVida = 1.1;
    double incrementoAtaque = 1.3;

    void subirStats(Pokemon& p) {
        p.Vida = static_cast<int>(p.Vida * incrementoVida);
        if (p.Vida > p.VidaMaxima) p.Vida = p.VidaMaxima;
        for (auto& atk : p.Ataques) {
            atk.danio = static_cast<int>(atk.danio * incrementoAtaque);
        }
    }
};

// Solo permite seleccionar Pokemon vivos con vida mayor a 0
inline Pokemon seleccionarPokemonUsuario(vector<Pokemon>& pokemons) {
    cout << "\nSelecciona tu Pokemon:\n";
    vector<int> indicesValidos;
    for (size_t i = 0; i < pokemons.size(); ++i) {
        cout << i + 1 << ". " << pokemons[i].Nombre << " "
             << colorTipo(pokemons[i].Tipo)
             << " (Vida: " << pokemons[i].Vida << ")" << (pokemons[i].vivo && pokemons[i].Vida > 0 ? "" : " [DEBILITADO]") << endl;
        if (pokemons[i].vivo && pokemons[i].Vida > 0)
            indicesValidos.push_back(i);
    }
    if (indicesValidos.empty()) {
        cout << "No tienes Pokemon vivos para combatir!\n";
        return Pokemon();
    }
    int eleccion;
    do {
        cout << "Opcion: ";
        cin >> eleccion;
        if (cin.fail() || eleccion < 1 || eleccion > (int)pokemons.size() || !(pokemons[eleccion-1].vivo && pokemons[eleccion-1].Vida > 0)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion invalida o el Pokemon esta debilitado. Intenta de nuevo.\n";
        } else {
            break;
        }
    } while (true);

    return pokemons[eleccion - 1];
}

inline Pokemon seleccionarPokemonRandom(const vector<Pokemon>& pokemons) {
    if (pokemons.empty()) return Pokemon{};
    int idx = rand() % pokemons.size();
    return pokemons[idx];
}

inline void batallaPvE(vector<Pokemon>& pokemonsUsuario, vector<Pokemon>& pokemonsEnemigos) {
    crearMochilaSiNoExiste();
    srand(static_cast<unsigned>(time(0)));

    TempJugador tempJugador;
    TempBot tempBot;

    cout << "Empieza el torneo PvE infinito!\n";

    int ronda = 1;

    for (;!pokemonsUsuario.empty(); ronda++) {
        cout << "\n=========== Ronda #" << ronda << "=============\n";

        Pokemon usuario = seleccionarPokemonUsuario(pokemonsUsuario);
        if (usuario.Nombre.empty()) {
            cout << "No tienes Pokemon vivos para combatir!\n";
            break;
        }
        Pokemon enemigo = seleccionarPokemonRandom(pokemonsEnemigos);

        cout << "Tu Pokemon: " << usuario.Nombre << " vs Enemigo: " << enemigo.Nombre << " " << colorTipo(enemigo.Tipo) << "\n";

        usuario.vivo = true;
        enemigo.vivo = true;

        bool usuarioSobrevive = true;

        while (usuario.vivo && enemigo.vivo) {
            aplicarEfectos(usuario);
            aplicarEfectos(enemigo);

            if (usuario.Vida <= 0) {
                usuario.vivo = false;
                usuario.Vida = 0;
                cout << usuario.Nombre << " fue debilitado por los efectos.\n";
                usuarioSobrevive = false;
                break;
            }
            if (enemigo.Vida <= 0) {
                enemigo.vivo = false;
                enemigo.Vida = 0;
                cout << enemigo.Nombre << " fue debilitado por los efectos.\n";
                break;
            }

            bool usuarioEsPrimero = usuario.Velocidad >= enemigo.Velocidad;

            if (usuarioEsPrimero) {
                cout << "\nTu Pokemon: " << usuario.Nombre << " (Vida: " << usuario.Vida << ")\n";
                cout << "Enemigo: " << enemigo.Nombre << " (Vida: " << enemigo.Vida << ")\n";

                cout << "Ataques:\n";
                for (int i = 0; i < 4; ++i) {
                    cout << i + 1 << ". " << usuario.Ataques[i].nombre << " (Dano: " << usuario.Ataques[i].danio
                         << ", PP: " << usuario.Ataques[i].pp << ")\n";
                }
                int ataqueUsuario;
                do {
                    cout << "Selecciona ataque (1-4): ";
                    cin >> ataqueUsuario;
                } while (ataqueUsuario < 1 || ataqueUsuario > 4 || usuario.Ataques[ataqueUsuario - 1].pp <= 0);

                int danio = calcularDanioBase(usuario.Ataques[ataqueUsuario - 1].danio, enemigo.Defensa);
                danio = static_cast<int>(danio * obtenerMultiplicador(usuario.Tipo, enemigo.Tipo));
                if (danio < 1) danio = 1;

                float multTipo = obtenerMultiplicador(usuario.Tipo, enemigo.Tipo);
                if (multTipo > 1.0f) cout << "Es super efectivo!\n";
                else if (multTipo < 1.0f && multTipo > 0.0f) cout << "No es muy efectivo...\n";
                else if (multTipo == 0.0f) cout << "No afecta al rival!\n";

                enemigo.Vida -= danio;
                usuario.Ataques[ataqueUsuario - 1].pp--;
                cout << usuario.Nombre << " uso " << usuario.Ataques[ataqueUsuario - 1].nombre << " causando " << danio << " de dano!\n";

                if (enemigo.Vida <= 0) {
                    enemigo.vivo = false;
                    enemigo.Vida = 0;
                    cout << enemigo.Nombre << " fue debilitado. Ganaste esta batalla!\n";
                    break;
                }
            }

            int ataqueEnemigo = rand() % 4;
            while (enemigo.Ataques[ataqueEnemigo].pp <= 0) {
                ataqueEnemigo = rand() % 4;
            }

            int danio = calcularDanioBase(enemigo.Ataques[ataqueEnemigo].danio, usuario.Defensa);
            danio = static_cast<int>(danio * obtenerMultiplicador(enemigo.Tipo, usuario.Tipo));
            if (danio < 1) danio = 1;

            float multTipo = obtenerMultiplicador(enemigo.Tipo, usuario.Tipo);
            if (multTipo > 1.0f) cout << "Es super efectivo!\n";
            else if (multTipo < 1.0f && multTipo > 0.0f) cout << "No es muy efectivo...\n";
            else if (multTipo == 0.0f) cout << "No afecta al rival!\n";

            usuario.Vida -= danio;
            enemigo.Ataques[ataqueEnemigo].pp--;
            cout << enemigo.Nombre << " uso " << enemigo.Ataques[ataqueEnemigo].nombre << " causando " << danio << " de dano!\n";

            if (usuario.Vida <= 0) {
                usuario.vivo = false;
                usuario.Vida = 0;
                cout << usuario.Nombre << " fue debilitado. ";
                usuarioSobrevive = false;
                cout << "Perdiste este combate!\n";
                break;
            }
        }

        // Actualiza el estado del usuario en el vector original
        for (auto& poke : pokemonsUsuario) {
            if (poke.Nombre == usuario.Nombre) {
                poke.Vida = usuario.Vida;
                poke.vivo = usuario.vivo;
            }
        }

        if (usuarioSobrevive) {
            tempJugador.subirStats(usuario);
            cout << usuario.Nombre << " ahora tiene Vida: " << usuario.Vida << "\n";
            for (auto& poke : pokemonsUsuario) {
                if (poke.Nombre == usuario.Nombre) {
                    poke.Vida = usuario.Vida;
                    poke.vivo = usuario.vivo;
                    poke.Ataques[0] = usuario.Ataques[0];
                    poke.Ataques[1] = usuario.Ataques[1];
                    poke.Ataques[2] = usuario.Ataques[2];
                    poke.Ataques[3] = usuario.Ataques[3];
                }
            }
        } else {
            int vivos = 0;
            for (auto& poke : pokemonsUsuario)
                if (poke.vivo && poke.Vida > 0) vivos++;
            cout << "Te quedan " << vivos << " Pokemon para seguir el torneo.\n";
        }

        for (auto& e : pokemonsEnemigos) {
            tempBot.subirStats(e);
        }

        bool hayVivos = false;
        for (auto& poke : pokemonsUsuario)
            if (poke.vivo && poke.Vida > 0) hayVivos = true;

        if (!hayVivos) {
            cout << "Todos tus Pokemon han sido debilitados! Fin del torneo.\n";
            break;
        }

        if (ronda % 4 == 0) {
            curarEquipo(pokemonsUsuario);
        }

        cout << "\nQuieres abrir la mochila? (s/n): ";
        char opcionMochila;
        cin >> opcionMochila;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (opcionMochila == 's' || opcionMochila == 'S') {
            abrirMochila(pokemonsUsuario);
        }

        cout << "Preparate para la proxima ronda!\n";
    }
}

#endif