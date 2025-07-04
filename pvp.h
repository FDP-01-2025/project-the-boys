#ifndef PVP_H
#define PVP_H

#include "pokemon_common.h"
#include <limits>
#include <cstdlib> // Para rand
#include <ctime>   // Para time

int calcularDanio(int danioBase, int defensa) {
    int danioFinal = danioBase - (defensa / 4);
    return (danioFinal < 1) ? 1 : danioFinal;
}

vector<Pokemon> seleccionarPokemonsUsuario(vector<Pokemon>& pokemons, int cantidad) {
    vector<Pokemon> seleccionados;
    for (int i = 0; i < cantidad; ++i) {
        cout << "\nSelecciona el Pokémon #" << (i + 1) << ":\n";
        for (size_t j = 0; j < pokemons.size(); ++j) {
            cout << j + 1 << ". " << pokemons[j].nombre << " (Vida: " << pokemons[j].vida << ")\n";
        }
        int eleccion;
        do {
            cout << "Opción: ";
            cin >> eleccion;
            if (cin.fail() || eleccion < 1 || eleccion > (int)pokemons.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Opción inválida. Intenta de nuevo.\n";
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


void batallaPvP(vector<Pokemon> equipo1, vector<Pokemon> equipo2) {
    int idx1 = 0, idx2 = 0;
    srand(time(0));

    while (idx1 < (int)equipo1.size() && idx2 < (int)equipo2.size()) {
        Pokemon& jugador = equipo1[idx1];
        Pokemon& rival = equipo2[idx2];

        aplicarEfectos(jugador);
        aplicarEfectos(rival);

        if (jugador.vida <= 0) {
            cout << jugador.nombre << " se ha debilitado por efecto!\n";
            idx1++;
            continue;
        }
        if (rival.vida <= 0) {
            cout << rival.nombre << " se ha debilitado por efecto!\n";
            idx2++;
            continue;
        }

        cout << "\n[Jugador 1] Pokemon: " << jugador.nombre << " (Vida: " << jugador.vida << ")\n";
        cout << "[Jugador 2] Pokemon: " << rival.nombre << " (Vida: " << rival.vida << ")\n";

        bool jugadorVaPrimero = jugador.velocidad >= rival.velocidad;

        for (int turno = 0; turno < 2; ++turno) {
            Pokemon& atacante = (turno == 0) == jugadorVaPrimero ? jugador : rival;
            Pokemon& defensor = (turno == 0) == jugadorVaPrimero ? rival : jugador;

            cout << "\nTurno de " << (atacante.nombre) << " - Ataques disponibles:\n";
            for (int i = 0; i < 4; ++i) {
                cout << i + 1 << ". " << atacante.ataques[i].nombre << " (Dano: " << atacante.ataques[i].danio
                     << ", PP: " << atacante.ataques[i].pp << ")\n";
            }

            int ataque;
            do {
                cout << "Selecciona ataque (1-4): ";
                cin >> ataque;
            } while (ataque < 1 || ataque > 4 || atacante.ataques[ataque - 1].pp <= 0);

            int danioReal = calcularDanio(atacante.ataques[ataque - 1].danio, defensor.defensa);
            defensor.vida -= danioReal;
            atacante.ataques[ataque - 1].pp--;

            cout << atacante.nombre << " usó " << atacante.ataques[ataque - 1].nombre << " e hizo "
                 << danioReal << " de dano!\n";

            if (defensor.efecto == Ninguno && (rand() % 100 < 25)) {
                defensor.efecto = Veneno;
                defensor.rondasConEfecto = 3;
                cout << defensor.nombre << " fue envenenado!\n";
            }

            if (defensor.vida <= 0) {
                cout << defensor.nombre << " se ha debilitado!\n";
                if (&defensor == &jugador) idx1++;
                else idx2++;
                break;
            }
        }
    }

    if (idx1 >= (int)equipo1.size()) {
        cout << "\nJugador 2 ha ganado la batalla!\n";
    } else {
        cout << "\nJugador 1 ha ganado la batalla!\n";
    }
}

#endif
