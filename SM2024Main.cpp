#include <iostream>

#include "SM2024Variables.h"
#include "SM2024KeyFunctions.h"
#include "SM2024CoreFunctions.h"
using namespace std;


int main(int argc, char* argv[]) {

    cout << "Witam studenta.\n";
    cout << "Klawisze od A do L wybieraja odpowiedni obrazek. Nacisnij 'O' aby wybrac wlasny plik.\n";
    cout << "1 - obraz z paleta narzuca\n";
    cout << "2 - obraz w skali szarosci narzuconej\n";
    cout << "3 - obraz w skali szarosci dedykowanej\n";
    cout << "4 - obraz z paleta wykryta\n";
    cout << "5 - obraz z paleta dedykowana\n";
    cout << "6 - brak ditheringu\n";
    cout << "7 - dithering (tablica Bayera 4x4)\n";
    cout << "8 - dithering (Floyd-Steinberg)\n";
    cout << "W - Zapis do pliku\n";
    cout << "E - Odczyt z pliku\n";
    cout << "R - odswiezenie ekranu (w przypadku braku wynikow)\n";
    cout << "T - zapis do pliku BMP\n";

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc*2, wysokosc*2, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    screen = SDL_GetWindowSurface(window);
    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
    return false;
    }
    SDL_UpdateWindowSurface(window);


    bool done = false;
    SDL_Event event;
    // g³ówna pêtla programu
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawi³o siê zdarzenie
        switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;

            // sprawdzamy czy zosta³ wciœniêty klawisz
            case SDL_KEYDOWN: {
                // wychodzimy, gdy wciœniêto ESC
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                if (event.key.keysym.sym == SDLK_1)
                    Funkcja1();
                if (event.key.keysym.sym == SDLK_2)
                    Funkcja2();
                if (event.key.keysym.sym == SDLK_3)
                    Funkcja3();
                if (event.key.keysym.sym == SDLK_4)
                    Funkcja4();
                if (event.key.keysym.sym == SDLK_5)
                    Funkcja5();
                if (event.key.keysym.sym == SDLK_6)
                    Funkcja6();
                if (event.key.keysym.sym == SDLK_7)
                    Funkcja7();
                if (event.key.keysym.sym == SDLK_8)
                    Funkcja8();
                if (event.key.keysym.sym == SDLK_w)
                    FunkcjaW();
                if (event.key.keysym.sym == SDLK_e)
                    FunkcjaE();
                if (event.key.keysym.sym == SDLK_r)
                    FunkcjaR();
                if (event.key.keysym.sym == SDLK_t)
                    FunkcjaT();
                if (event.key.keysym.sym == SDLK_y)
                    FunkcjaY();
                if (event.key.keysym.sym == SDLK_u)
                    FunkcjaU();
                if (event.key.keysym.sym == SDLK_i)
                    FunkcjaI();
                if (event.key.keysym.sym == SDLK_z)
                    FunkcjaZ();
                if (event.key.keysym.sym == SDLK_x)
                    FunkcjaX();
                if (event.key.keysym.sym == SDLK_c)
                    FunkcjaC();
                if (event.key.keysym.sym == SDLK_v)
                    FunkcjaV();
                if (event.key.keysym.sym == SDLK_a)
                    ladujBMP("obrazki/obrazek1.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_o){
                    string nazwa;
                    cout << "Wybrales odczyt wlasnego pliku BMP.\nProsze podac nazwe pliku:\t";
                    cin >> nazwa;
                    ladujBMP((nazwa + ".bmp").c_str(), szerokosc/2, wysokosc/2);
                    cout << endl;
                    FunkcjaR();
                }
                if (event.key.keysym.sym == SDLK_s)
                    ladujBMP("obrazki/obrazek2.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_d)
                    ladujBMP("obrazki/obrazek3.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_f)
                    ladujBMP("obrazki/obrazek4.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_g)
                    ladujBMP("obrazki/obrazek5.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_h)
                    ladujBMP("obrazki/obrazek6.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_j)
                    ladujBMP("obrazki/obrazek7.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_k)
                    ladujBMP("obrazki/obrazek8.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_l)
                    ladujBMP("obrazki/obrazek9.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_b)
                    czyscEkran(0, 0, 0);
                else
                    break;
               }
        }
        if (done) break;
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}
