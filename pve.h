#ifndef PVE_H
#define PVE_H

#include "pokemon_common.h"
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iostream>
#include <vector>


int calcularDanio(const Ataque& atk, const Pokemon& defensor) {
    return std::max(1, atk.danio - defensor.defensa);
}

Pokemon seleccionarPokemonUsuario(vector<Pokemon>& pokemons) {
    cout << "\nSelecciona tu Pokemon:\n";
    for (size_t i = 0; i < pokemons.size(); ++i) {
        cout << i + 1 << ". " << pokemons[i].nombre << " (Vida: " << pokemons[i].vida << ")\n";
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

    cout << "\u00a1Empieza el torneo PvE infinito!\n";

    while (!pokemonsUsuario.empty()) {
        cout << "\nSelecciona un Pok\u00e9mon para la batalla:\n";
        Pokemon usuario = seleccionarPokemonUsuario(pokemonsUsuario);
        Pokemon enemigo = seleccionarPokemonRandom(pokemonsEnemigos);

        cout << "Tu Pok\u00e9mon: " << usuario.nombre << " vs Enemigo: " << enemigo.nombre << "\n";

        usuario.vivo = true;
        enemigo.vivo = true;

        while (usuario.vivo && enemigo.vivo) {
            aplicarEfectos(usuario);
            aplicarEfectos(enemigo);

            if (usuario.vida <= 0) {
                cout << usuario.nombre << " fue debilitado por los efectos.\n";
                break;
            }
            if (enemigo.vida <= 0) {
                cout << enemigo.nombre << " fue debilitado por los efectos.\n";
                break;
            }

            bool turnoUsuario = usuario.velocidad >= enemigo.velocidad;

            if (turnoUsuario) {
                cout << "\nTu Pok\u00e9mon: " << usuario.nombre << " (Vida: " << usuario.vida << ")\n";
                cout << "Enemigo: " << enemigo.nombre << " (Vida: " << enemigo.vida << ")\n";

                cout << "Ataques:\n";
                for (int i = 0; i < 4; ++i) {
                    cout << i + 1 << ". " << usuario.ataques[i].nombre << " (Da\u00f1o: " << usuario.ataques[i].danio
                         << ", PP: " << usuario.ataques[i].pp << ")\n";
                }
                int ataqueUsuario;
                do {
                    cout << "Selecciona ataque (1-4): ";
                    cin >> ataqueUsuario;
                } while (ataqueUsuario < 1 || ataqueUsuario > 4 || usuario.ataques[ataqueUsuario - 1].pp <= 0);

                int danio = calcularDanio(usuario.ataques[ataqueUsuario - 1], enemigo);
                enemigo.vida -= danio;
                usuario.ataques[ataqueUsuario - 1].pp--;
                cout << usuario.nombre << " uso " << usuario.ataques[ataqueUsuario - 1].nombre << " causando " << danio << " de da\u00f1o!\n";

                if (enemigo.vida <= 0) {
                    cout << enemigo.nombre << " fue debilitado. \u00a1Ganaste esta batalla!\n";
                    break;
                }
            }

            int ataqueEnemigo = rand() % 4;
            while (enemigo.ataques[ataqueEnemigo].pp <= 0) {
                ataqueEnemigo = rand() % 4;
            }

            int danio = calcularDanio(enemigo.ataques[ataqueEnemigo], usuario);
            usuario.vida -= danio;
            enemigo.ataques[ataqueEnemigo].pp--;
            cout << enemigo.nombre << " uso " << enemigo.ataques[ataqueEnemigo].nombre << " causando " << danio << " de da\u00f1o!\n";

            if (usuario.vida <= 0) {
                cout << usuario.nombre << " fue debilitado. Perdiste el torneo.\n";
                return;
            }
        }

        cout << "\u00a1Prep\u00e1rate para la pr\u00f3xima batalla!\n";
    }
}

#endif
