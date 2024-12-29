#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "SM2024HelperFunctions.h"
#include "SM2024KeyFunctions.h"
#include "SM2024Variables.h"
#include "SM2024CoreFunctions.h"

int normalizacja(int wartosc, int minimum, int maximum){

    int wynik = wartosc;
    if(wynik < minimum)wynik = minimum;
    if(wynik > maximum)wynik = maximum;

    return wynik;
}

bool porownajKolory(SDL_Color kolor1, SDL_Color kolor2){
    if(kolor1.r != kolor2.r)
        return false;
    if(kolor1.g != kolor2.g)
        return false;
    if(kolor1.b != kolor2.b)
        return false;

    return true;
}

Uint8 shiftByte(Uint8 b, Uint8 shiftAmount) {
    b = b << 8 - shiftAmount;
    return b;
}

float calculateSaturation(float minimum, float maximum, float luminance){
    if (minimum == maximum) return 0;

    if (luminance <= 0.5) return (maximum - minimum) / (maximum + minimum);

    if (luminance  > 0.5) return (maximum - minimum) / (2.0 - maximum - minimum);

    cout << "ERROR: Unexpected event when calculating saturation!\n";
    return 0;
}

float calculateHue(float r, float g, float b, float minimum, float maximum){

    if(r == maximum) return ((g - b) / (maximum - minimum)) * 60;

    if(g == maximum) return (2.0 + (b - r) / (maximum - minimum)) * 60;

    if(b == maximum) return (4.0 + (r - g) / (maximum - minimum)) * 60;

    cout << "ERROR: Unexpected event when calculating hue!\n";
    return 0;
}

int calculateHSLtoRGBMethod(float x){
    if ( (6.0 * x) < 1.0 ) return 1;
    if ( (2.0 * x) < 1.0 ) return 2;
    if ( (3.0 * x) < 2.0 ) return 3;
    return 4;
}

