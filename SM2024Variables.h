#ifndef SM2024VARIABLES_H
#define SM2024VARIABLES_H

#include <SDL2/SDL.h>
#include <vector>

#define szerokosc 640
#define wysokosc 400

#define tytul "GK2023 - Projekt - Zespol 11"

#include <SDL2/SDL.h>

extern SDL_Window* window;
extern SDL_Surface* screen;

extern char identyfikator[];
extern SDL_Color paleta7DoPliku[128];
extern std::vector<Uint8> obrazekDCT;
extern int obrazekDCTRozmiar;
extern int counter;

extern Uint8 paleta7DoPlikuUint8[szerokosc/2*wysokosc/2];
extern Uint16 szerokoscObrazka;
extern Uint16 wysokoscObrazka;

extern Uint8 tryb;
extern Uint8 dithering;
extern Uint8 rozmiarPalety;

extern SDL_Color paleta7[320*200];
extern SDL_Color subPaleta[(szerokosc/2) * (wysokosc/2)];
extern int ileKolorow;

extern SDL_Color obrazek[(szerokosc/2)*(wysokosc/2)];
extern SDL_Color obrazekTMP[256];

extern int ileKubelkow;
extern SDL_Color paleta[128];

extern int tablicaBayera4[4][4];

extern int zigzagOrderArray[64][2];

extern float zaktualizawanaTablicaBayera4[4][4];

struct YUV {
    float y;
    float u;
    float v;
};

struct YIQ {
    float y;
    float i;
    float q;
};

struct YCbCr {
    float y;
    float cb;
    float cr;
};

struct HSL {
    float h;
    float s;
    float l;
};

#endif // !SM2024VARIABLES_H
