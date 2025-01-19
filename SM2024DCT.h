#ifndef SM2024DCT_H_INCLUDED
#define SM2024DCT_H_INCLUDED

#include <SDL2/SDL.h>

constexpr int rozmiarBloku = 8;
extern int rozmiarZygzak;

constexpr int tabelaKwantyzacji[8][8] = {
    {16, 12, 14, 14, 18, 24, 49, 72},
    {11, 12, 13, 17, 22, 35, 64, 92},
    {10, 14, 16, 22, 37, 55, 78, 95},
    {16, 19, 24, 29, 56, 64, 87, 98},
    {24, 26, 40, 51, 68, 81, 103, 112},
    {40, 58, 57, 87, 109, 104, 121, 100},
    {51, 60, 69, 80, 103, 113, 120, 103},
    {61, 55, 56, 62, 77, 92, 101, 99}
};

struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
};

void wyswietlDane(macierz blok);
void wyswietlDCT(macierz blok);
macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);
void kompresjaStratnaDCT();
void zapisDCTDoPliku();
void odczytDCTZPliku();



#endif // SM2024DCT_H_INCLUDED
