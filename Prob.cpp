#include <iostream>
#include <cstdlib>  // rand, srand
#include <ctime>    // time

using namespace std;

int calcularPrecision(int daño) {
    const int MAX_DAÑO = 120;   
    const int MIN_PRECISION = 50; 
    const int MAX_PRECISION = 100;

    int precision = MAX_PRECISION - (daño * 50 / MAX_DAÑO);

    if (precision < MIN_PRECISION) precision = MIN_PRECISION;

    return precision;
}

bool ataqueAcierta(int precision) {
    int numero = rand() % 100 + 1; 
    return numero <= precision;
}

int main() {
    srand(time(0));  

    int precisionAtaque = calcularPrecision(daño);

    cout << "El Pokémon usa su ataque...\n";

    if (ataqueAcierta(precisionAtaque)) {
        cout << "¡El ataque acertó!\n";
    } else {
        cout << "¡El ataque falló!\n";
    }

    return 0;
}