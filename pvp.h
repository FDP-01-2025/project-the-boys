#ifndef PVP_H
#define PVP_H

#include "pokemon_common.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std; 

// Logica principal de batalla PvP
inline void batallaPvP(vector<Pokemon> equipo1, vector<Pokemon> equipo2) {
    int idx1 = 0, idx2 = 0;
    srand(static_cast<unsigned>(time(0)));

    while (idx1 < (int)equipo1.size() && idx2 < (int)equipo2.size()) {
        Pokemon& jugador = equipo1[idx1];
        Pokemon& rival = equipo2[idx2];

        aplicarEfectos(jugador);
        aplicarEfectos(rival);

        if (jugador.Vida <= 0) {
            cout << jugador.Nombre << " se ha debilitado por efecto!\n";
            idx1++;
            continue;
        }
        if (rival.Vida <= 0) {
            cout << rival.Nombre << " se ha debilitado por efecto!\n";
            idx2++;
            continue;
        }

        cout << "\n[Jugador 1] Pokemon: " << jugador.Nombre << " (" << colorTipo(jugador.Tipo) << ") (Vida: " << jugador.Vida << ")\n";
        cout << "[Jugador 2] Pokemon: " << rival.Nombre << " (" << colorTipo(rival.Tipo) << ") (Vida: " << rival.Vida << ")\n";

        bool jugadorVaPrimero = jugador.Velocidad >= rival.Velocidad;

        for (int turno = 0; turno < 2; ++turno) {
            Pokemon& atacante = (turno == 0) == jugadorVaPrimero ? jugador : rival;
            Pokemon& defensor = (turno == 0) == jugadorVaPrimero ? rival : jugador;

            cout << "\nTurno de " << atacante.Nombre << " - Ataques disponibles:\n";
            for (int i = 0; i < 4; ++i) {
                cout << i + 1 << ". " << atacante.Ataques[i].nombre << " (Dano: " << atacante.Ataques[i].danio
                     << ", PP: " << atacante.Ataques[i].pp
                     << ", Precision: " << atacante.Ataques[i].precision << "%)\n";
            }

            int ataque;
            do {
                cout << "Selecciona ataque (1-4): ";
                cin >> ataque;
            } while (ataque < 1 || ataque > 4 || atacante.Ataques[ataque - 1].pp <= 0);

            atacante.Ataques[ataque - 1].pp--;

            // Precision
            if (!ataqueAcierta(atacante.Ataques[ataque - 1].precision)) {
                cout << atacante.Nombre << " fallo el ataque!\n";
                continue;
            }

            // Critico (10%)
            bool critico = (rand() % 100) < 10;

            int danioReal = calcularDanioBase(atacante.Ataques[ataque - 1].danio, defensor.Defensa);
            float multTipo = obtenerMultiplicador(atacante.Tipo, defensor.Tipo);
            danioReal = static_cast<int>(danioReal * multTipo);

            if (critico) {
                danioReal *= 2;
                cout << "Golpe critico!\n";
            }
            if (danioReal < 1) danioReal = 1;

            if (multTipo > 1.0f) cout << "Es super efectivo!\n";
            else if (multTipo < 1.0f && multTipo > 0.0f) cout << "No es muy efectivo...\n";
            else if (multTipo == 0.0f) cout << "No afecta al rival!\n";

            defensor.Vida -= danioReal;

            cout << atacante.Nombre << " uso " << atacante.Ataques[ataque - 1].nombre << " e hizo "
                 << danioReal << " de dano!\n";

            // Efectos de estado aleatorios (10% veneno, 15% quemadura)
            if (defensor.efecto == Ninguno) {
                int roll = rand() % 100;
                if (roll < 10) {
                    defensor.efecto = Veneno;
                    defensor.RondasConEfecto = 3;
                    cout << defensor.Nombre << " fue envenenado!\n";
                } else if (roll < 25) {
                    defensor.efecto = Quemadura;
                    defensor.RondasConEfecto = 3;
                    cout << defensor.Nombre << " fue quemado!\n";
                }
            }

            if (defensor.Vida <= 0) {
                cout << defensor.Nombre << " se ha debilitado!\n";
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