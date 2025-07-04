#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Ataque {
    string nombre;
    int dano; // Si no hace daño, será -1
    int pp;
};

struct Pokemon {
    string nombre;
    int vida;
    vector<Ataque> ataques;
};

// Mostrar todos los Pokémon
void mostrarPokedex(const vector<Pokemon>& pokedex) {
    cout << "----- POKEDEX -----" << endl;
    for (const auto& p : pokedex) {
        cout << "Nombre: " << p.nombre << " | Vida: " << p.vida << endl;
    }
}

// Mostrar ataques de un Pokémon
void mostrarAtaques(const Pokemon& p) {
    cout << "Ataques de " << p.nombre << ":" << endl;
    for (const auto& atk : p.ataques) {
        cout << "- " << atk.nombre << " | Daño: ";
        if (atk.dano == -1) cout << "(Efecto especial)";
        else cout << atk.dano;
        cout << " | PP: " << atk.pp << endl;
    }
}

// Buscar Pokémon por nombre
Pokemon* buscarPokemon(vector<Pokemon>& pokedex, const string& nombre) {
    for (auto& p : pokedex) {
        if (p.nombre == nombre) {
            return &p;
        }
    }
    return nullptr;
}

int main() {
    vector<Pokemon> pokedex = {
        { "Machamp", 383, { {"Cross Chop", 100, 5}, {"Karate Chop", 50, 25}, {"Brick Break", 75, 15}, {"Vital Throw", 70, 10} } },
        { "Clefable", 393, { {"Moonblast", 95, 15}, {"Meteor Mash", 90, 10}, {"Thunder Wave", -1, 20}, {"Soft-Boiled", -1, 10} } },
        { "Ninetales", 349, { {"Flamethrower", 90, 15}, {"Fire Blast", 110, 5}, {"Will-O-Wisp", -1, 15}, {"Energy Ball", 90, 10} } },
        { "Wigglytuff", 483, { {"Hyper Voice", 90, 10}, {"Play Rough", 90, 10}, {"Thunderbolt", 90, 15}, {"Sing", 40, 15} } },
        { "Golbat", 353, { {"Poison Fang", 50, 15}, {"Wing Attack", 60, 35}, {"Leech Life", 80, 10}, {"Confuse Ray", -1, 10} } },
        { "Vileplume", 353, { {"Petal Dance", 120, 10}, {"Sludge Bomb", 90, 10}, {"Solar Beam", 120, 10}, {"Sleep Powder", -1, 15} } },
        { "Golem", 363, { {"Earthquake", 100, 10}, {"Rock Slide", 75, 10}, {"Tackle", 35, 35}, {"Double-Edge", 120, 15} } },
        { "Rapidash", 333, { {"Flamethrower", 90, 15}, {"Stomp", 65, 20}, {"Fire Spin", 35, 15}, {"Horn Attack", 65, 25} } },
        { "Slowbro", 393, { {"Psychic", 90, 10}, {"Surf", 90, 15}, {"Ice Beam", 90, 10}, {"Body Slam", 85, 15} } },
        { "Cloyster", 303, { {"Ice Shard", 40, 30}, {"Icicle Spear", 50, 10}, {"Water Pulse", 60, 20}, {"Rock Blast", 50, 10} } },
        { "Gengar", 323, { {"Shadow Ball", 80, 15}, {"Lick", 20, 30}, {"Dark Pulse", 80, 15}, {"Psychic", 90, 10} } },
        { "Kingler", 313, { {"Crabhammer", 90, 10}, {"Vice Grip", 55, 30}, {"Slash", 70, 20}, {"Stomp", 65, 20} } },
        { "Dugtrio", 273, { {"Earthquake", 100, 10}, {"Rock Slide", 75, 10}, {"Sucker Punch", 70, 5}, {"Sand Attack", -1, 15} } },
        { "Venomoth", 343, { {"Bug Buzz", 90, 10}, {"Psychic", 90, 10}, {"Sleep Powder", -1, 15}, {"Quiver Dance", -1, 25} } },
        { "Persian", 333, { {"Slash", 70, 20}, {"Power Gem", 80, 20}, {"Fake Out", 40, 10}, {"Taunt", -1, 20} } },
        { "Golduck", 363, { {"Hydro Pump", 110, 5}, {"Psychic", 90, 10}, {"Ice Beam", 90, 10}, {"Confuse Ray", -1, 10} } },
        { "Primeape", 333, { {"Close Combat", 120, 5}, {"Earthquake", 100, 10}, {"Rock Slide", 75, 10}, {"U-turn", 70, 20} } },
        { "Electrode", 323, { {"Thunderbolt", 90, 15}, {"Explosion", 250, 5}, {"Swift", 60, 20}, {"Tackle", 40, 35} } },
        { "Exeggutor", 393, { {"Psychic", 90, 10}, {"Seed Bomb", 80, 15}, {"Body Slam", 85, 15}, {"Stomp", 65, 20} } },
        { "Hitmonlee", 303, { {"High Jump Kick", 130, 10}, {"Brick Break", 75, 15}, {"Rolling Kick", 60, 15}, {"Double Kick", 60, 30} } },
        { "Hitmonchan", 303, { {"Fire Punch", 75, 15}, {"Ice Punch", 75, 15}, {"Thunder Punch", 75, 15}, {"Mach Punch", 40, 30} } },
        { "Rhydon", 413, { {"Earthquake", 100, 10}, {"Rock Slide", 75, 10}, {"Horn Attack", 65, 25}, {"Double-Edge", 120, 15} } },
        { "Poliwrath", 383, { {"Dynamic Punch", 100, 5}, {"Waterfall", 80, 15}, {"Ice Punch", 75, 15}, {"Bulk Up", -1, 20} } },
        { "Kangaskhan", 413, { {"Dizzy Punch", 70, 10}, {"Crunch", 80, 15}, {"Stomp", 65, 20}, {"Brick Break", 75, 15} } },
        { "Starmie", 323, { {"Psychic", 90, 10}, {"Surf", 90, 15}, {"Thunderbolt", 90, 15}, {"Rapid Spin", 50, 40} } },
        { "Scyther", 343, { {"Wing Attack", 60, 35}, {"Slash", 70, 20}, {"X-Scissor", 80, 15}, {"Fury Cutter", 40, 20} } },
        { "Victreebel", 363, { {"Leaf Blade", 90, 15}, {"Sludge Bomb", 90, 10}, {"Solar Beam", 120, 10}, {"Sleep Powder", -1, 15} } },
        { "Tentacruel", 363, { {"Surf", 90, 15}, {"Sludge Wave", 95, 10}, {"Barrier", -1, 20}, {"Poison Jab", 80, 20} } },
        { "Electabuzz", 333, { {"Thunderbolt", 90, 15}, {"Thunder Punch", 75, 15}, {"Swift", 60, 20}, {"Low Kick", 60, 25} } },
        { "Magmar", 333, { {"Flamethrower", 90, 15}, {"Fire Punch", 75, 15}, {"Ember", 40, 25}, {"Brick Break", 75, 15} } },
        { "Pinsir", 333, { {"X-Scissor", 80, 15}, {"Vice Grip", 55, 30}, {"Seismic Toss", 60, 20}, {"Rock Slide", 75, 10} } },
        { "Tauros", 353, { {"Body Slam", 85, 15}, {"Hyper Beam", 150, 5}, {"Earthquake", 100, 10}, {"Horn Attack", 65, 25} } },
        { "Gyarados", 393, { {"Waterfall", 80, 15}, {"Bite", 60, 25}, {"Crunch", 80, 15}, {"Earthquake", 100, 10} } },
        { "Magneton", 303, { {"Thunderbolt", 90, 15}, {"Flash Cannon", 80, 10}, {"Tri Attack", 80, 10}, {"Thunder Wave", -1, 20} } },
        { "Dodrio", 323, { {"Drill Peck", 80, 20}, {"Jump Kick", 100, 10}, {"Body Slam", 85, 15}, {"Roost", -1, 10} } },
        { "Lapras", 463, { {"Ice Beam", 90, 10}, {"Surf", 90, 15}, {"Body Slam", 85, 15}, {"Water Gun", 40, 25} } },
        { "Farfetch'd", 307, { {"Brave Bird", 120, 15}, {"Leaf Blade", 90, 15}, {"Knock Off", 65, 20}, {"Swords Dance", -1, 20} } },
        { "Ditto", 299, { {"Pound", 40, 35}, {"Tackle", 40, 35}, {"Scratch", 40, 35}, {"Pound", 40, 35} } },
        { "Dewgong", 383, { {"Ice Beam", 90, 10}, {"Surf", 90, 15}, {"Rest", -1, 10}, {"Sheer Cold", -1, 5} } },
        { "Vaporeon", 463, { {"Surf", 90, 15}, {"Ice Beam", 90, 10}, {"Aurora Beam", 65, 20}, {"Bite", 60, 25} } },
        { "Jolteon", 333, { {"Thunderbolt", 90, 15}, {"Double Kick", 30, 30}, {"Swift", 60, 20}, {"Shock Wave", 60, 20} } },
        { "Flareon", 333, { {"Flamethrower", 90, 15}, {"Bite", 60, 25}, {"Ember", 40, 25}, {"Strength", 80, 15} } },
        { "Omastar", 343, { {"Ancient Power", 60, 5}, {"Surf", 90, 15}, {"Ice Beam", 90, 10}, {"Earth Power", 90, 10} } },
        { "Hypno", 373, { {"Psychic", 90, 10}, {"Shadow Ball", 80, 15}, {"Hypnosis", -1, 20}, {"Dream Eater", 100, 15} } },
        { "Onix", 273, { {"Earthquake", 120, 30}, {"Rock Slide", 75, 15}, {"Iron Tail", 100, 25}, {"Sandstorm", -1, 5} } },
        { "Kabutops", 323, { {"Rock Slide", 75, 10}, {"Slash", 70, 20}, {"Waterfall", 80, 15}, {"Aqua Jet", 40, 20} } },
        { "Aerodactyl", 363, { {"Rock Slide", 75, 10}, {"Wing Attack", 60, 35}, {"Hyper Beam", 150, 5}, {"Bite", 60, 25} } },
        { "Snorlax", 523, { {"Body Slam", 85, 15}, {"Hyper Beam", 150, 5}, {"Earthquake", 100, 10}, {"Headbutt", 70, 15} } },
        { "Articuno", 383, { {"Ice Beam", 90, 10}, {"Hurricane", 110, 10}, {"Drill Peck", 80, 20}, {"Quick Attack", 40, 30} } },
        { "Zapdos", 383, { {"Thunderbolt", 90, 15}, {"Drill Peck", 80, 20}, {"Quick Attack", 40, 30}, {"Heat Wave", 95, 10} } },
        { "Moltres", 383, { {"Flamethrower", 90, 15}, {"Sky Attack", 140, 5}, {"Heat Wave", 95, 10}, {"Wing Attack", 60, 35} } },
        { "Marowak", 323, { {"Bonemerang", 50, 10}, {"Fire Punch", 75, 15}, {"Thunder Punch", 75, 15}, {"Swords Dance", -1, 20} } },
        { "Dragonite", 385, { {"Dragon Claw", 80, 15}, {"Wing Attack", 60, 35}, {"Thunder Punch", 75, 15}, {"Ice Punch", 75, 15} } },
        { "Weezing", 333, { {"Sludge Bomb", 120, 30}, {"Flamethrower", 65, 10}, {"Thunderbolt", 50, 5}, {"Toxic", -1, 20} } },
        { "Seadra", 313, { {"Hydro Pump", 110, 30}, {"Dragon Pulse", 85, 15}, {"Ice Beam", 90, 20}, {"Agility", -1, 5} } },
        { "Seaking", 363, { {"Waterfall", 80, 15}, {"Megahorn", 120, 10}, {"Poison Jab", 80, 20}, {"Drill Run", 80, 10} } },
        { "Mr. Mime", 283, { {"Psychic", 90, 20}, {"Dazzling Gleam", 80, 10}, {"Thunder Wave", -1, 20}, {"Light Screen", -1, 30} } },
        { "Jynx", 333, { {"Ice Beam", 90, 20}, {"Psychic", 70, 10}, {"Lovely Kiss", -1, 10}, {"Focus Blast", 120, 40} } },
        { "Porygon", 333, { {"Tri Attack", 80, 10}, {"Thunderbolt", 90, 15}, {"Ice Beam", 90, 10}, {"Slam", 90, 10} } },
        { "Mewtwo", 415, { {"Psychic", 90, 10}, {"Shadow Ball", 80, 15}, {"Aura Sphere", 80, 20}, {"Thunderbolt", 90, 15} } },
        { "Mew", 403, { {"Psychic", 90, 10}, {"Shadow Ball", 80, 15}, {"Flamethrower", 90, 15}, {"Hyper Beam", 150, 5} } }
    };

    int opcion;
    do {
        cout << "\n1. Mostrar Pokedex\n";
        cout << "2. Mostrar ataques de un Pokemon\n";
        cout << "3. Salir\n";
        cout << "Elige una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            mostrarPokedex(pokedex);
        }
        else if (opcion == 2) {
            string nombre;
            cout << "Nombre del Pokemon: ";
            cin.ignore(); // Limpiar buffer
            getline(cin, nombre);

            Pokemon* p = buscarPokemon(pokedex, nombre);
            if (p != nullptr) {
                mostrarAtaques(*p);
            } else {
                cout << "No se encontro el Pokemon." << endl;
            }
        }

    } while (opcion != 3);

    cout << "Programa terminado." << endl;

    return 0;
}
