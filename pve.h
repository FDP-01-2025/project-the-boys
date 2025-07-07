#ifndef PVE_H
#define PVE_H

#include "pokemon_common.h"
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iostream>
#include <vector>

using namespace std;

struct TempJugador {
    double incrementoVida = 1.2;
    double incrementoAtaque = 1.5; 

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

int calcularDanio(const Ataque& atk, const Pokemon& defensor) {
    return std::max(1, atk.danio - defensor.Defensa);
}

Pokemon seleccionarPokemonUsuario(vector<Pokemon>& pokemons) {
    cout << "\nSelecciona tu Pokemon:\n";
    for (size_t i = 0; i < pokemons.size(); ++i) {
        cout << i + 1 << ". " << pokemons[i].Nombre << " (Vida: " << pokemons[i].Vida << ")\n";
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

    cout << "\u00a1Empieza el torneo PvE infinito!\n";

    int ronda = 1;

    for (;!pokemonsUsuario.empty(); ronda++) {
        cout << "\n=========== Ronda #" << ronda << "=============\n";

        cout << "\nSelecciona un Pok\u00e9mon para la batalla:\n";
        Pokemon usuario = seleccionarPokemonUsuario(pokemonsUsuario);
        Pokemon enemigo = seleccionarPokemonRandom(pokemonsEnemigos);

        cout << "Tu Pok\u00e9mon: " << usuario.Nombre << " vs Enemigo: " << enemigo.Nombre << "\n";

        usuario.vivo = true;
        enemigo.vivo = true;

        while (usuario.vivo && enemigo.vivo) {
            aplicarEfectos(usuario);
            aplicarEfectos(enemigo);

            if (usuario.Vida <= 0) {
                cout << usuario.Nombre << " fue debilitado por los efectos.\n";
                break;
            }
            if (enemigo.Vida <= 0) {
                cout << enemigo.Nombre << " fue debilitado por los efectos.\n";
                break;
            }

            bool turnoUsuario = usuario.Velocidad >= enemigo.Velocidad;

            if (turnoUsuario) {
                cout << "\nTu Pok\u00e9mon: " << usuario.Nombre << " (Vida: " << usuario.Vida << ")\n";
                cout << "Enemigo: " << enemigo.Nombre << " (Vida: " << enemigo.Vida << ")\n";

                cout << "Ataques:\n";
                for (int i = 0; i < 4; ++i) {
                    cout << i + 1 << ". " << usuario.Ataques[i].nombre << " (Da\u00f1o: " << usuario.Ataques[i].danio
                         << ", PP: " << usuario.Ataques[i].pp << ")\n";
                }
                int ataqueUsuario;
                do {
                    cout << "Selecciona ataque (1-4): ";
                    cin >> ataqueUsuario;
                } while (ataqueUsuario < 1 || ataqueUsuario > 4 || usuario.Ataques[ataqueUsuario - 1].pp <= 0);

                int danio = calcularDanio(usuario.Ataques[ataqueUsuario - 1], enemigo);
                enemigo.Vida -= danio;
                usuario.Ataques[ataqueUsuario - 1].pp--;
                cout << usuario.Nombre << " uso " << usuario.Ataques[ataqueUsuario - 1].nombre << " causando " << danio << " de da\u00f1o!\n";

                if (enemigo.Vida <= 0) {
                    cout << enemigo.Nombre << " fue debilitado. \u00a1Ganaste esta batalla!\n";
                    break;
                }
            }

            int ataqueEnemigo = rand() % 4;
            while (enemigo.Ataques[ataqueEnemigo].pp <= 0) {
                ataqueEnemigo = rand() % 4;
            }

            int danio = calcularDanio(enemigo.Ataques[ataqueEnemigo], usuario);
            usuario.Vida -= danio;
            enemigo.Ataques[ataqueEnemigo].pp--;
            cout << enemigo.Nombre << " uso " << enemigo.Ataques[ataqueEnemigo].nombre << " causando " << danio << " de da\u00f1o!\n";

            if (usuario.Vida <= 0) {
                cout << usuario.Nombre << " fue debilitado. Perdiste el torneo.\n";
                return;
            }
        }
        
        
        tempJugador.subirStats(usuario);
        cout << usuario.Nombre << " ahora tiene Vida: " << usuario.Vida << "\n";

        for (auto& e : pokemonsEnemigos) {
            tempBot.subirStats(e);
        }

        pokemonsUsuario.push_back(usuario);

        cout << "\u00a1Prep\u00e1rate para la pr\u00f3xima ronda!\n";
    
    }
}

#endif
