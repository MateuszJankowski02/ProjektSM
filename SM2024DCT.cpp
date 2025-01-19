#include "SM2024DCT.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <iomanip>
#include <math.h>
#include <vector>
#include <fstream>
#include "SM2024Variables.h"
#include "SM2024CoreFunctions.h"

using namespace std;

int rozmiarZygzak = 0;


void wyswietlDane(macierz blok){
    cout << "Dane pikselowe w macierzy:" << endl;
    for (int y=0;y<rozmiarBloku;y++){
        for (int x=0; x<rozmiarBloku; x++){
            cout << setw(4) << (int)blok.dane[x][y] << " ";
        }
        cout << endl;
    }
}

void wyswietlDCT(macierz blok){
    cout << "Wspolczynniki transformanty w macierzy:" << endl;
    for (int y=0;y<rozmiarBloku;y++){
        for (int x=0; x<rozmiarBloku; x++){
            cout << fixed << setw(6) << setprecision(2) << blok.dct[x][y] << " ";
        }
        cout << endl;
    }
}

static vector<Uint8> zbierajZygzakiem(const macierz &blok) {
    // Zbierz wartości DCT zygzakiem
    vector<Uint8> result(64);
    rozmiarZygzak = 64;
    for(int i = 0; i < 64; i++){
        int x = zigzagOrderArray[i][0];
        int y = zigzagOrderArray[i][1];
        result[i] = static_cast<Uint8>(blok.dct[x][y]);
    }
    // Usuń końcowe 0
    while(!result.empty() && result.back() == 0) {
        rozmiarZygzak = rozmiarZygzak - 1;
        result.pop_back();
    }
    return result;
}

static void przywrocZigzagDoBloku(std::vector<Uint8> daneZygzak, float dct[rozmiarBloku][rozmiarBloku], Uint8 rozmiar) {
    // Wyzeruj całą macierz
    for(int v=0; v<rozmiarBloku; v++){
        for(int u=0; u<rozmiarBloku; u++){
            dct[u][v] = 0;
        }
    }
    // Wypełnij odpowiadającymi współczynnikami
    for(int i=0; i < rozmiar; i++){
        int x = zigzagOrderArray[i][0];
        int y = zigzagOrderArray[i][1];
        dct[x][y] = daneZygzak[i];
    }
}



macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]){
    float wynik[rozmiarBloku][rozmiarBloku];

    for (int v=0; v < rozmiarBloku; ++v){
        for (int u = 0; u < rozmiarBloku; ++u){
            const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikiDCT = 0;

            for (int y = 0; y < rozmiarBloku; ++y){
                for (int x = 0; x < rozmiarBloku; ++x){
                    double uCosFactor = cos((double)(2*x+1) * M_PI * (double)u / (2*(double)rozmiarBloku));
                    double vCosFactor = cos((double)(2*y+1) * M_PI * (double)v / (2*(double)rozmiarBloku));
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikiDCT += pixel * uCosFactor * vCosFactor;
                }
            }
            wspolczynnikiDCT *= (2.0 / (double)rozmiarBloku) * cu * cv;

            // Zastosowanie tabeli kwantyzacji
            wspolczynnikiDCT /= tabelaKwantyzacji[u][v];
            wspolczynnikiDCT = round(wspolczynnikiDCT + 127.0f); // Normalizacja do [0,255]

            // Klampowanie wartości
            if(wspolczynnikiDCT < 0){
                wspolczynnikiDCT = 0;
            } else if(wspolczynnikiDCT > 255){
                wspolczynnikiDCT = 255;
            }

            wynik[u][v] = wspolczynnikiDCT;
        }
    }
    macierz rezulat;
    for (int j=0; j < rozmiarBloku; j++){
        for (int i=0; i < rozmiarBloku; i++){
            rezulat.dct[i][j] = wynik[i][j];
            rezulat.dane[i][j] = wartosci[i][j];
        }
    }
    return rezulat;
}

macierz idct(float DCT[rozmiarBloku][rozmiarBloku]){
    int wynik[rozmiarBloku][rozmiarBloku];

    for (int x = 0; x < rozmiarBloku; ++x){
        for (int y = 0; y < rozmiarBloku; ++y){

            double pixel = 0;

            for (int u = 0; u < rozmiarBloku; ++u){
                for (int v = 0; v < rozmiarBloku; ++v){
                    const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    // Dekwantyzacja
                    double wspolczynnikDCT = (DCT[u][v] - 127.0f) * tabelaKwantyzacji[u][v];
                    pixel += wspolczynnikDCT * cos((double)(2*x+1) * M_PI * u / (2.0 * rozmiarBloku)) * cu
                              * cos((double)(2*y+1) * M_PI * v / (2.0 * rozmiarBloku)) * cv;
                }
            }
            pixel *= (2.0 / (double)rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }
    macierz rezulat;
    for (int j = 0; j < rozmiarBloku; j++){
        for (int i = 0; i < rozmiarBloku; i++){
            if (wynik[i][j]>255) wynik[i][j]=255;
            if (wynik[i][j]<0) wynik[i][j]=0;
            rezulat.dane[i][j] = wynik[i][j];
            rezulat.dct[i][j] = DCT[i][j];
        }
    }
    return rezulat;
}

void kompresjaStratnaDCT(){

    const int blokSzer = szerokosc / rozmiarBloku;
    const int blokWys = wysokosc / rozmiarBloku;

    macierz blokDCT;
    macierz blokDane;
    macierz noweDane;
    int count = 0;

    // Przetwarzanie każdego bloku
    for (int by = 0; by < blokWys; ++by) {
        for (int bx = 0; bx < blokSzer; ++bx) {
            // Konwersja na skalę szarości
            for (int y = 0; y < rozmiarBloku; ++y) {
                for (int x = 0; x < rozmiarBloku; ++x) {
                    SDL_Color kolor = getPixel(bx * rozmiarBloku + x, by * rozmiarBloku + y);
                    blokDane.dane[x][y] = (kolor.r + kolor.g + kolor.b) / 3;

                    // Wyświetlanie oryginalnego obrazu w lewym górnym kwartale
                    setPixel(bx * rozmiarBloku + x, by * rozmiarBloku + y, blokDane.dane[x][y], blokDane.dane[x][y], blokDane.dane[x][y]);
                }
            }

            // Przeprowadzenie DCT z kwantyzacją
            blokDCT = dct(blokDane.dane);

            // Modyfikacja współczynników DCT
            for (int v = 0; v < rozmiarBloku; ++v) {
                for (int u = 0; u < rozmiarBloku; ++u) {
                    // Wyzerowanie współczynników w prawej dolnej części
                    if (u + v > 10) { // Prosta granica, można zmienić
                        blokDCT.dct[u][v] = 127.0f; // Shifted zero
                    } else if (blokDCT.dct[u][v] != 0) {
                        // Zaokrąglenie współczynników
                        blokDCT.dct[u][v] = round(blokDCT.dct[u][v]);
                    }
                }
            }

            // Zbieranie współczynników zygzakiem
            vector<Uint8> dctZygzak = zbierajZygzakiem(blokDCT);
            obrazekDCT.push_back(static_cast<Uint8>(rozmiarZygzak));
            obrazekDCTRozmiar += rozmiarZygzak + 1;
            for (int i = 0; i < rozmiarZygzak; i++) {
                obrazekDCT.push_back(dctZygzak[i]);
            }

            // Przeprowadzenie iDCT
            noweDane = idct(blokDCT.dct);

            // Wizualizacja nowego bloku w prawym górnym kwartale
            for (int y = 0; y < rozmiarBloku; ++y) {
                for (int x = 0; x < rozmiarBloku; ++x) {
                    Uint8 jasnosc = noweDane.dane[x][y];
                    setPixel(bx * rozmiarBloku + szerokosc / 2 + x, by * rozmiarBloku + y, jasnosc, jasnosc, jasnosc);
                }
            }
        }
    }
}

void zapisDCTDoPliku(){    // ZAPIS DO PLIKU dct
    cout<<"Zapisujemy plik 'obraz.dct' uzywajac metody write()"<<endl;
    ofstream wyjscie("obraz.dct" , ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*3);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));

    for(int i = 0; i < obrazekDCTRozmiar; i++){
        wyjscie.write((char*)&obrazekDCT[i], sizeof(Uint8));
    }
    wyjscie.close();
    cout << "Gotowe" << endl;
}

void odczytDCTZPliku() {
    std::ifstream wejscie("obraz.dct", std::ios::binary);
    if(!wejscie.is_open()){
        std::cerr << "Nie udalo sie otworzyc pliku: obraz.dct" << std::endl;
        return;
    }

    char identyfikatorRead[3];
    Uint16 szerokoscObrazkaRead = 0;
    Uint16 wysokoscObrazkaRead = 0;

    wejscie.read((char*)&identyfikatorRead, sizeof(char)*3);
    wejscie.read((char*)&szerokoscObrazkaRead, sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazkaRead, sizeof(Uint16));

    // Oblicz ile bloków mamy w poziomie i pionie
    int blokSzer = (szerokoscObrazkaRead/2) / rozmiarBloku;
    int blokWys = (wysokoscObrazkaRead/2) / rozmiarBloku;

    // Czytamy każdy blok (pierw rozmiarZygzak, potem rozmiarZygzak bajtów danych DCT)
    for(int by = 0; by < blokWys; ++by){
        for(int bx = 0; bx < blokSzer; ++bx){
            Uint8 rozmiarZygzak;
            wejscie.read((char*)&rozmiarZygzak, sizeof(Uint8));

            std::vector<Uint8> dctZygzak(rozmiarBloku*rozmiarBloku);
            for(int i = 0; i < rozmiarZygzak; i++){
                wejscie.read((char*)&dctZygzak[i],sizeof(Uint8));
            }


            // Odtworzenie macierzy DCT blok 8x8 z dekwantyzacją
            float blokDCT[rozmiarBloku][rozmiarBloku];
            przywrocZigzagDoBloku(dctZygzak, blokDCT, rozmiarZygzak);

            // Odtworzenie (IDCT)
            macierz noweDane = idct(blokDCT);

            // Wyświetlenie zrekonstruowanego bloku
            for(int y = 0; y < rozmiarBloku; ++y){
                for(int x = 0; x < rozmiarBloku; x++){
                    Uint8 jasnosc = noweDane.dane[x][y];
                    setPixel(bx * rozmiarBloku + x, by * rozmiarBloku + y, jasnosc, jasnosc, jasnosc);
                }
            }
        }
    }
    wejscie.close();

    std::cout << "Obraz z pliku obraz.dct zostal wyswietlony!" << std::endl;
    cout << "Odczyt z pliku\n";
    cout << "Id: " << identyfikatorRead[0] << identyfikatorRead[1] << identyfikatorRead[2] << endl;
    cout << "szerokosc: " << szerokoscObrazkaRead << endl;
    cout << "wysokosc: " << wysokoscObrazkaRead << endl;

    SDL_UpdateWindowSurface(window);
}
/*
void OdczytDCTZPliku(){    // ODCZYT PLIKU dct
    SDL_Color kolor;
    Uint8 kolorUint8;
    int counter = 0;
    char identyfikator[] = "   ";
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 tryb = 0;
    Uint8 dithering = 0;
    Uint8 rozmiarPalety = 0;
    Uint8 extractedBits;
    SDL_Color paleta[128];
    int offset = 0;

    cout<<"Odczytujemy plik 'obraz.z11' uzywajac metody read()"<<endl;
    ifstream wejscie("obraz.z11",ios::binary);
    wejscie.read((char*)&identyfikator,sizeof(char)*3);
    wejscie.read((char*)&szerokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&tryb,sizeof(Uint8));
    wejscie.read((char*)&dithering,sizeof(Uint8));

    Uint8 paleta7ZPlikuUint7[szerokoscObrazka*wysokoscObrazka];
    Uint8 paleta7ZPlikuUint8[szerokoscObrazka*wysokoscObrazka];
    memset(paleta7ZPlikuUint7,0,sizeof(paleta7ZPlikuUint7));
    memset(paleta,0,sizeof(paleta));

    if(tryb == 3 || tryb == 5){
        wejscie.read((char*)&rozmiarPalety, sizeof(Uint8));
        for(int i = 0; i < rozmiarPalety; i++){
            wejscie.read((char*)&paleta[i].r, sizeof(Uint8));
            wejscie.read((char*)&paleta[i].g, sizeof(Uint8));
            wejscie.read((char*)&paleta[i].b, sizeof(Uint8));
        }
        narysujPalete(paleta);
    }

    if(tryb != 0){
        while(!wejscie.eof()){
            wejscie.read((char*)&paleta7ZPlikuUint7[counter],sizeof(Uint8));
            counter++;
        }
        counter = 0;
        for(int x = 1; x < sizeof(paleta7ZPlikuUint7); x += 7){
                for(int i = 1; i <= 6; i++){
                    if(i == 1){
                        paleta7ZPlikuUint8[x+i-2+offset] = paleta7ZPlikuUint7[x+i-2] >> 1;
                        paleta7ZPlikuUint8[x+i-2+offset] = paleta7ZPlikuUint8[x+i-2+offset] << 1;
                    }
                    extractedBits = paleta7ZPlikuUint7[x+i-2] & ((1 << i) - 1); // ekstraktujemy 'i' ostanich bitów poprzedniej wartoœci
                    extractedBits = shiftByte(extractedBits, i);                          // odwracamy zekstrakowane bity

                    paleta7ZPlikuUint8[x+i-1+offset] = paleta7ZPlikuUint7[x+i-1] >> i+1;
                    paleta7ZPlikuUint8[x+i-1+offset] = paleta7ZPlikuUint8[x+i-1+offset] << 1;
                    paleta7ZPlikuUint8[x+i-1+offset] = paleta7ZPlikuUint8[x+i-1+offset] | extractedBits;

                    if(i == 6){
                        extractedBits = paleta7ZPlikuUint7[x+i-1] & ((1 << i+1) - 1);
                        extractedBits = shiftByte(extractedBits, i+1);
                        paleta7ZPlikuUint8[x+i+offset] = 0b00000000 | extractedBits;
                    }
                }
                offset++;
        }
        counter = 0;
        if(tryb == 1 || tryb == 5 || tryb == 4){
            for(int z = 0; z<szerokosc/2; z += 8){
                for(int y = 0; y < wysokosc/2; y++){
                    for(int x = z; x < z+8; x++){
                        kolorUint8 = paleta7ZPlikuUint8[counter];
                        counter++;
                        kolor = z7RGBna24RGB(kolorUint8);

                        setPixel(x+szerokosc/2,y+wysokosc/2,kolor.r,kolor.g,kolor.b);
                    }
                }
            }
        }else if(tryb == 2 || tryb == 3){
            for(int z = 0; z<szerokosc/2; z += 8){
                for(int y = 0; y < wysokosc/2; y++){
                    for(int x = z; x < z+8; x++){
                        kolorUint8 = paleta7ZPlikuUint8[counter];
                        counter++;
                        kolor = z7BWna24RGB(kolorUint8);

                        setPixel(x+szerokosc/2,y+wysokosc/2,kolor.r,kolor.g,kolor.b);
                        //setPixel(x+szerokosc/2,y+wysokosc/2,255,255,255);
                    }
                }
            }
        }else{
            cout << "Nieprawidlowy tryb" << endl;
        }
    }else{
        cout << "Nie wybrano żI sadnego trybu!" << endl;
    }
    cout << "Odczyt z pliku\n";
    cout << "Id: " << identyfikator << endl;
    cout << "szerokosc: " << szerokoscObrazka << endl;
    cout << "wysokosc: " << wysokoscObrazka << endl;
    cout << "tryb: " << (int)tryb << endl;
    cout << "dithering: " << (int)dithering << endl;
    cout << "rozmiar palety:" << (int)rozmiarPalety << endl << endl;

 SDL_UpdateWindowSurface(window);
}
*/
