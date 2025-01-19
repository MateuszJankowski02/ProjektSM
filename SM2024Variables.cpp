#include "SM2024Variables.h"
#include <SDL2/SDL.h>

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

char identyfikator[] = "MKJ";
SDL_Color paleta7DoPliku[128];
std::vector<Uint8> obrazekDCT;
int obrazekDCTRozmiar;
int counter = 0;

Uint8 paleta7DoPlikuUint8[szerokosc/2*wysokosc/2];
Uint16 szerokoscObrazka = szerokosc;
Uint16 wysokoscObrazka = wysokosc;

Uint8 tryb = 0;
Uint8 dithering = 3;
Uint8 rozmiarPalety = 0;

SDL_Color paleta7[320*200];
SDL_Color subPaleta[(szerokosc/2) * (wysokosc/2)];
int ileKolorow = 0;

SDL_Color obrazek[(szerokosc/2)*(wysokosc/2)];
SDL_Color obrazekTMP[256];

int ileKubelkow = 0;
SDL_Color paleta[128];

int tablicaBayera4[4][4] = { {6, 14, 8, 16},
    {10, 2, 12, 4},
    {7, 15, 5, 13},
    {11, 3, 9, 1}
};

int zigzagOrderArray[64][2] = {
    {0,0},{0,1},{1,0},{2,0},{1,1},{0,2},{0,3},{1,2},
    {2,1},{3,0},{4,0},{3,1},{2,2},{1,3},{0,4},{0,5},
    {1,4},{2,3},{3,2},{4,1},{5,0},{6,0},{5,1},{4,2},
    {3,3},{2,4},{1,5},{0,6},{0,7},{1,6},{2,5},{3,4},
    {4,3},{5,2},{6,1},{7,0},{7,1},{6,2},{5,3},{4,4},
    {3,5},{2,6},{1,7},{2,7},{3,6},{4,5},{5,4},{6,3},
    {7,2},{7,3},{6,4},{5,5},{4,6},{3,7},{4,7},{5,6},
    {6,5},{7,4},{7,5},{6,6},{5,7},{6,7},{7,6},{7,7}
};

float zaktualizawanaTablicaBayera4[4][4];
