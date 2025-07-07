#ifndef PVE_H
#define PVE_H

#include "pokemon_common.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

struct TempJugador {
    double incrementoVida = 1.5;
    double incrementoAtaque = 1.8; 

    void subirStats(Pokemon& p) {
        p.Vida = static_cast<int>(p.Vida * incrementoVida);
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
        for (auto& atk : p.Ataques) {
            atk.danio = static_cast<int>(atk.danio * incrementoAtaque);
        }
    }
};

Pokemon seleccionarPokemonUsuario(vector<Pokemon>& pokemons) {
    cout << "\nSelecciona tu Pokemon:\n";
    for (size_t i = 0; i < pokemons.size(); ++i) {
        cout << i + 1 << ". " << pokemons[i].Nombre << " "
             << colorTipo(pokemons[i].Tipo)
             << " (Vida: " << pokemons[i].Vida << ")" << endl;
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
    pokemons.erase(pokemons.begin() + eleccion - 1);
    return elegido;
}

Pokemon seleccionarPokemonRandom(const vector<Pokemon>& pokemons) {
    if (pokemons.empty()) return Pokemon{};
    int idx = rand() % pokemons.size();
    return pokemons[idx];
}

void batallaPvE(vector<Pokemon>& pokemonsUsuario, vector<Pokemon>& pokemonsEnemigos) {
    srand(static_cast<unsigned>(time(0)));

    TempJugador tempJugador;
    TempBot tempBot;

    cout << "!Empieza el torneo PvE infinito!\n";

    int ronda = 1;

    for (;!pokemonsUsuario.empty(); ronda++) {
        cout << "\n=========== Ronda #" << ronda << "=============\n";

        cout << "\nSelecciona un Pokemon para la batalla:\n";
        Pokemon usuario = seleccionarPokemonUsuario(pokemonsUsuario);
        Pokemon enemigo = seleccionarPokemonRandom(pokemonsEnemigos);

        cout << "Tu Pokemon: " << usuario.Nombre << " vs Enemigo: " << enemigo.Nombre << " " << colorTipo(enemigo.Tipo) << "\n";

        usuario.vivo = true;
        enemigo.vivo = true;

        bool usuarioSobrevive = true;

        while (usuario.vivo && enemigo.vivo) {
            aplicarEfectos(usuario);
            aplicarEfectos(enemigo);

            if (usuario.Vida <= 0) {
                cout << usuario.Nombre << " fue debilitado por los efectos.\n";
                usuarioSobrevive = false;
                break;
            }
            if (enemigo.Vida <= 0) {
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
                if (multTipo > 1.0f) cout << "¡Es super efectivo!\n";
                else if (multTipo < 1.0f && multTipo > 0.0f) cout << "No es muy efectivo...\n";
                else if (multTipo == 0.0f) cout << "¡No afecta al rival!\n";

                enemigo.Vida -= danio;
                usuario.Ataques[ataqueUsuario - 1].pp--;
                cout << usuario.Nombre << " uso " << usuario.Ataques[ataqueUsuario - 1].nombre << " causando " << danio << " de dano!\n";

                if (enemigo.Vida <= 0) {
                    cout << enemigo.Nombre << " fue debilitado. !Ganaste esta batalla!\n";
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
            if (multTipo > 1.0f) cout << "¡Es super efectivo!\n";
            else if (multTipo < 1.0f && multTipo > 0.0f) cout << "No es muy efectivo...\n";
            else if (multTipo == 0.0f) cout << "¡No afecta al rival!\n";

            usuario.Vida -= danio;
            enemigo.Ataques[ataqueEnemigo].pp--;
            cout << enemigo.Nombre << " uso " << enemigo.Ataques[ataqueEnemigo].nombre << " causando " << danio << " de dano!\n";

            if (usuario.Vida <= 0) {
                cout << usuario.Nombre << " fue debilitado. ";
                usuarioSobrevive = false;
                cout << "!Perdiste este combate!\n";
                break;
            }
        }

        if (usuarioSobrevive) {
            tempJugador.subirStats(usuario);
            cout << usuario.Nombre << " ahora tiene Vida: " << usuario.Vida << "\n";
            pokemonsUsuario.push_back(usuario);
        } else {
            cout << "Te quedan " << pokemonsUsuario.size() << " Pokemones para seguir el torneo.\n";
        }

        for (auto& e : pokemonsEnemigos) {
            tempBot.subirStats(e);
        }

        if (pokemonsUsuario.empty()) {
            cout << "Todos tus Pokemon han sido derrotados! Fin del torneo.\n";
            break;
        }

        cout << "!Preparate para la proxima ronda!\n";
    }
}

#endif