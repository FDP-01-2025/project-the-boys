#ifndef PVE_H
#define PVE_H

#include "pokemon_common.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <sys/stat.h>
#include <cstdlib>
#include <ctime>
#include <limits>

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

inline int contarObjetoEnMochila(const vector<string>& mochila, const string& nombreObjeto) {
    int contador = 0;
    for (const auto& obj : mochila)
        if (obj.find(nombreObjeto) != string::npos)
            contador++;
    return contador;
}

inline void agregarObjetoAMochila(vector<string>& mochila, const string& objeto) {
    mochila.push_back(objeto);
    guardarMochila("Mochila.txt", mochila);
}

inline void darObjetosPorRonda(vector<string>& mochila, int ronda, bool ganoRonda) {
    if (ronda % 5 == 0) {
        if (contarObjetoEnMochila(mochila, "Revive") == 0) {
            agregarObjetoAMochila(mochila, "Revive (revives a pokemon with half its hp)");
            cout << "Recibiste un Revive!\n";
        }
        if (contarObjetoEnMochila(mochila, "Large potion") == 0) {
            agregarObjetoAMochila(mochila, "Large potion (heals all hp)");
            cout << "Recibiste una Large potion!\n";
        }
    }
    if (ganoRonda) {
        agregarObjetoAMochila(mochila, "Small potion (heals 20 hp)");
        agregarObjetoAMochila(mochila, "Small potion (heals 20 hp)");
        agregarObjetoAMochila(mochila, "Medium potion (heals 100 hp)");
        agregarObjetoAMochila(mochila, "Medium potion (heals 100 hp)");
        cout << "Recibiste 2 Small potion y 2 Medium potion!\n";
    }
}

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

inline bool usarObjetoMochila(const string& objeto, vector<Pokemon>& equipo, bool &fueRevivirSinDebilitados) {
    fueRevivirSinDebilitados = false;
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
        bool hayDebilitado = false;
        for (const auto& poke : equipo) {
            if (!poke.vivo || poke.Vida == 0) {
                hayDebilitado = true;
                break;
            }
        }
        if (!hayDebilitado) {
            cout << "No hay ningun Pokemon debilitado para revivir.\n";
            fueRevivirSinDebilitados = true;
            return false;
        }
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

inline void abrirMochila(vector<Pokemon>& equipo) {
    vector<string> objetos = leerMochila("Mochila.txt");
    while (true) {
        int eleccion = mostrarMochila(objetos);
        if (eleccion < 0 || eleccion >= (int)objetos.size()) {
            cout << "No se uso ningun objeto.\n";
            return;
        }
        bool fueRevivirSinDebilitados = false;
        bool usado = usarObjetoMochila(objetos[eleccion], equipo, fueRevivirSinDebilitados);
        if (fueRevivirSinDebilitados) {
            char resp;
            while (true) {
                cout << "Quieres elegir otro objeto? (s/n): ";
                cin >> resp;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (resp == 's' || resp == 'S') {
                    break;
                } else if (resp == 'n' || resp == 'N') {
                    cout << "No se uso ningun objeto.\n";
                    return;
                } else {
                    cout << "Opcion invalida. Escribe 's' o 'n'.\n";
                }
            }
            continue;
        }
        if (usado) {
            objetos.erase(objetos.begin() + eleccion);
            guardarMochila("Mochila.txt", objetos);
        }
        cout << "Pulsa enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        break;
    }
}

inline void curarEquipo(vector<Pokemon>& equipo) {
    for (auto& poke : equipo) {
        if (poke.vivo && poke.Vida > 0)
            poke.Vida = poke.VidaMaxima;
    }
    cout << "\nTodo tu equipo ha sido curado!" << endl;
}

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

// --- La batalla principal PvE ---
inline void batallaPvE(vector<Pokemon>& pokemonsUsuario, vector<Pokemon>& pokemonsEnemigos) {
    crearMochilaSiNoExiste();
    srand(static_cast<unsigned>(time(0)));

    float mejoraVidaJugador = 0.0f;
    float mejoraDanioJugador = 0.0f;
    float mejoraEnemigo = 0.0f;

    vector<string> objetosIniciales = leerMochila("Mochila.txt");

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

        // Aplica mejoras acumuladas
        usuario.Vida += int(usuario.VidaMaxima * mejoraVidaJugador);
        for (auto& atk : usuario.Ataques)
            atk.danio += int(atk.danio * mejoraDanioJugador);

        enemigo.Vida += int(enemigo.VidaMaxima * mejoraEnemigo);
        for (auto& atk : enemigo.Ataques)
            atk.danio += int(atk.danio * mejoraEnemigo);

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

            for (int turno = 0; turno < 2; ++turno) {
                Pokemon& atacante = (turno == 0) == usuarioEsPrimero ? usuario : enemigo;
                Pokemon& defensor = (turno == 0) == usuarioEsPrimero ? enemigo : usuario;

                int idxAtaque = 0;
                if (&atacante == &usuario) {
                    cout << "\nTu turno (" << atacante.Nombre << "):\n";
                    for (int i = 0; i < 4; ++i)
                        cout << i+1 << ". " << atacante.Ataques[i].nombre << " (Dano: " << atacante.Ataques[i].danio
                             << ", PP: " << atacante.Ataques[i].pp
                             << ", Precision: " << atacante.Ataques[i].precision << "%)\n";
                    do {
                        cout << "Selecciona ataque (1-4): ";
                        cin >> idxAtaque;
                    } while (idxAtaque < 1 || idxAtaque > 4 || atacante.Ataques[idxAtaque-1].pp <= 0);
                    idxAtaque--;
                } else {
                    idxAtaque = rand() % 4;
                    while (atacante.Ataques[idxAtaque].pp <= 0) idxAtaque = rand() % 4;
                }

                Ataque& ataque = atacante.Ataques[idxAtaque];
                ataque.pp--;

                // Precision
                if (!ataqueAcierta(ataque.precision)) {
                    cout << atacante.Nombre << " fallo el ataque!\n";
                    continue;
                }

                // Critico
                bool critico = (rand() % 100) < 10;
                int danio = calcularDanioBase(ataque.danio, defensor.Defensa);
                float multTipo = obtenerMultiplicador(atacante.Tipo, defensor.Tipo);
                danio = static_cast<int>(danio * multTipo);

                if (critico) {
                    danio *= 2;
                    cout << "Golpe critico!\n";
                }
                if (danio < 1) danio = 1;

                if (multTipo > 1.0f) cout << "Es super efectivo!\n";
                else if (multTipo < 1.0f && multTipo > 0.0f) cout << "No es muy efectivo...\n";
                else if (multTipo == 0.0f) cout << "No afecta al rival!\n";

                defensor.Vida -= danio;
                cout << atacante.Nombre << " uso " << ataque.nombre << " causando " << danio << " de dano!\n";

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
                    defensor.vivo = false; defensor.Vida = 0;
                    if (&defensor == &usuario) usuarioSobrevive = false;
                    break;
                }
            }
        }

        // Actualiza el estado del usuario en el vector original
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

        // Mochila: reparto de objetos segun ronda y resultado
        vector<string> mochila = leerMochila("Mochila.txt");
        darObjetosPorRonda(mochila, ronda, usuarioSobrevive);

        // MEJORAS: Elegir tras ganar
        if (usuarioSobrevive) {
            char eleccion;
            do {
                cout << "Que quieres mejorar? (v: vida, d: dano): ";
                cin >> eleccion;
            } while (eleccion != 'v' && eleccion != 'd');
            if (eleccion == 'v') mejoraVidaJugador += 0.2f;
            else mejoraDanioJugador += 0.2f;
            mejoraEnemigo += 0.3f;
        }

        // Menu de opciones tras cada combate
        bool salir = false;
        do {
            cout << "\nElige una opcion:\n";
            cout << "1. Ver mochila\n";
            cout << "2. Siguiente batalla\n";
            cout << "3. Salir del torneo\n";
            int opcion;
            cin >> opcion;
            if (opcion == 1) abrirMochila(pokemonsUsuario);
            else if (opcion == 2) break;
            else if (opcion == 3) { salir = true; break; }
        } while (true);
        if (salir) break;

        // Restaurar objetos cada 10 rondas
        if (ronda % 10 == 0) {
            cout << "Tus objetos han sido restaurados!\n";
            guardarMochila("Mochila.txt", objetosIniciales);
        }

        // Curar equipo cada 4 rondas
        if (ronda % 4 == 0) curarEquipo(pokemonsUsuario);

        // Chequeo de fin de equipo
        bool hayVivos = false;
        for (auto& poke : pokemonsUsuario)
            if (poke.vivo && poke.Vida > 0) hayVivos = true;
        if (!hayVivos) {
            cout << "Todos tus Pokemon han sido debilitados! Fin del torneo.\n";
            break;
        }
    }

    // Registro de ronda maxima
    ofstream rec("records.txt", ios::app);
    rec << "Rondas alcanzadas: " << (ronda - 1) << endl;
    rec.close();
    cout << "Fin del torneo PvE! Alcanzaste la ronda #" << (ronda - 1) << endl;
}

#endif