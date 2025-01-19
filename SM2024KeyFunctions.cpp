#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "SM2024HelperFunctions.h"
#include "SM2024KeyFunctions.h"
#include "SM2024Variables.h"
#include "SM2024CoreFunctions.h"
#include "SM2024KeyFunctions.h"
#include "SM2024Variables.h"
#include "SM2024CoreFunctions.h"
#include "SM2024BitConversion.h"
#include "SM2024ColorPalette.h"
#include "SM2024MedianCut.h"
#include "SM2024HelperFunctions.h"
#include "SM2024Dithering.h"
#include "SM2024ColorModels.h"
#include "SM2024Undersampling.h"
#include "SM2024ColorFiltering.h"
#include "SM2024Compression.h"
#include "SM2024DCT.h"

using namespace std;


void Funkcja1() {   // DEDYKOWANA RGB

    tryb = 1;
    dithering = 3;
    cout << "Wybrano tryb obrazu numer 1! (Obraz z paleta narzucona)" << endl;

    Uint8 kolor7bit;
    SDL_Color kolor, nowyKolor;

    memset(paleta7DoPlikuUint8, 0, sizeof(paleta7DoPlikuUint8));

    for(int z = 0; z<szerokosc/2; z += 8){
        for(int y = 0; y < wysokosc/2; y++){
            for(int x = z; x < z+8; x++){
                kolor = getPixel(x,y);
                kolor7bit = z24RGBna7RGB(kolor);
                nowyKolor = z7RGBna24RGB(kolor7bit);

                setPixel(x+szerokosc/2,y,nowyKolor.r,nowyKolor.g,nowyKolor.b);
            }
        }
    }

    SDL_UpdateWindowSurface(window);

}

void Funkcja2() {   // DEDYKOWANA BW

    tryb = 2;
    dithering = 3;
    cout << "Wybrano tryb obrazu numer 2! (Obraz w skali szarosci narzuconej)" << endl;

    //int counter = 0;

    Uint8 szary7bit;
    SDL_Color kolor, nowyKolor;

    for(int z = 0; z<szerokosc/2; z += 8){
        for(int y = 0; y < wysokosc/2; y++){
            for(int x = z; x < z+8; x++){
                kolor = getPixel(x,y);

                szary7bit = z24RGBna7BW(kolor);
                //paleta7DoPlikuUint8[counter] = szary7bit;
                //counter++;
                nowyKolor = z7BWna24RGB(szary7bit);

                setPixel(x+szerokosc/2,y,nowyKolor.r,nowyKolor.g,nowyKolor.b);
            }
        }
    }

    SDL_UpdateWindowSurface(window);

}

void Funkcja3() {   // MEDIAN CUT BW
    tryb = 3;
    dithering = 3;
    cout << "Wybrano tryb obrazu numer 3! (Obraz w skali szarosci dedykowanej)" << endl;

    ileKubelkow=0;
    int numer=0;
    int szary=0;
    SDL_Color kolor;
    int indeks;
    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
            szary=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
            obrazek[numer]= {szary, szary, szary};
            numer++;
        }
    }
    cout << "Generowanie palety (moze to chwile zajac)" << endl;
    medianCutBW(0,numer-1,7);

    numer=0;

    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
                 kolor=getPixel(x,y);
                 szary=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
                 indeks=znajdzSasiadaBW(szary);
                 setPixel(x+szerokosc/2,y,paleta[indeks].r,paleta[indeks].g,paleta[indeks].b);
        }
    }

    //cout << "Paleta do pliku:" << endl;
    for(int x = 0; x < 128; x++){
        paleta7DoPliku[x] = paleta[x];
        //cout << "R:" << (int)paleta7DoPliku[x].r << "G:" << (int)paleta7DoPliku[x].g << "B:" << (int)paleta7DoPliku[x].b << endl;
    }

    narysujPalete(paleta);

    cout << "Gotowe. Dedykowana paleta zostala utworzona" << endl;

    SDL_UpdateWindowSurface(window);
}

void Funkcja4() {   // WYKRYTA
    tryb = 4;
    dithering = 3;

    czyscPalete();
    SDL_Color kolor;
    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
            sprawdzKolor(kolor);
        }
    }
    if(ileKolorow < 128){
        //cout<<"Ile kolorow: "<<ileKolorow<<endl;

        //cout << "Paleta do pliku:" << endl;
        for(int y=0;y<wysokosc/2;y++){
            for(int x=0;x<szerokosc/2;x++){
                kolor=getPixel(x,y);
                setPixel(x + szerokosc/2, y, kolor.r, kolor.g, kolor.b);
            }
        }
        for(int x = 0; x < 128; x++){
            paleta7DoPliku[x] = paleta7[x];
            //cout << "R:" << (int)paleta7DoPliku[x].r << "G:" << (int)paleta7DoPliku[x].g << "B:" << (int)paleta7DoPliku[x].b << endl;
        }

        narysujPalete(paleta7);
    }else{
        cout << "Ilosc kolorow palety przekracza nalozony limit (128). Nastapi ograniczenie kolorow algorytmem median cut." << endl;
        Funkcja5();
    }
    SDL_UpdateWindowSurface(window);

}

void Funkcja5() {   // MEDIAN CUT RGB

    if(tryb != 4){
        tryb = 5;
        dithering = 3;
        cout << "Wybrano tryb obrazu numer 5! (Obraz z paleta dedykowana)" << endl;
    }


    ileKubelkow = 0;
    int numer = 0;
    SDL_Color kolor;
    int indeks;
    for(int y = 0 ; y < wysokosc/2 ; y++){
        for(int x = 0 ; x< szerokosc/2 ; x++){
            kolor = getPixel(x,y);
            obrazek[numer] = {kolor.r, kolor.g, kolor.b};
            //setPixel(x + szerokosc/2, y, kolor.r, kolor.g, kolor.b);
            numer++;
        }
    }

    cout << "Generowanie palety (moze to chwile zajac)" << endl;

    medianCut(0, numer-1, 7);

    numer=0;

    for(int y = 0 ; y < wysokosc/2 ; y++){
        for(int x=0 ; x < szerokosc/2 ; x++){
                 kolor = getPixel(x,y);
                 indeks = znajdzSasiada(kolor, paleta);
                 setPixel(x + szerokosc/2 ,y ,paleta[indeks].r ,paleta[indeks].g,paleta[indeks].b);
        }
    }

    narysujPalete(paleta);


    //cout << "Paleta do pliku:" << endl;
    for(int x = 0; x < 128; x++){
        paleta7DoPliku[x] = paleta[x];
        //cout << "R:" << (int)paleta7DoPliku[x].r << "G:" << (int)paleta7DoPliku[x].g << "B:" << (int)paleta7DoPliku[x].b << endl;
    }

    cout << "Gotowe. Dedykowana paleta zostala utworzona" << endl;

    SDL_UpdateWindowSurface(window);

}

void Funkcja6() {   // BRAK DITHERINGU
    dithering = 0;
    SDL_Color kolor;
    int counter = 0;

    cout << "Wybrano dithering numer 0! (Brak ditheringu)" << endl;

    memset(paleta7DoPlikuUint8,0,sizeof(paleta7DoPlikuUint8));

    if(tryb == 1 || tryb == 4 || tryb == 5){  //KOLOR
        int kolor7bit;
        for(int z = szerokosc/2; z<szerokosc; z += 8){
            for(int y = 0; y < wysokosc/2; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    kolor7bit = z24RGBna7RGB(kolor);
                    paleta7DoPlikuUint8[counter] = kolor7bit;
                    counter++;
                }
            }
        }

    }else if(tryb == 2 || tryb == 3){ //BW
        int szary7bit;
        for(int z = szerokosc/2; z<szerokosc; z += 8){
            for(int y = 0; y < wysokosc/2; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    szary7bit = z24RGBna7BW(kolor);
                    paleta7DoPlikuUint8[counter] = szary7bit;
                    counter++;
                }
            }
        }
    }else{
        cout << "Tryb obrazu nie zostal wybrany!" << endl;
    }

    for(int y = wysokosc/2; y < wysokosc; y++ ){
        for(int x = 0; x < szerokosc/2; x++){
            setPixel(x,y,0,0,0);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {   // TABLICA BAYERA 4X4

    dithering = 1;
    int counter = 0;
    SDL_Color kolor;

    memset(paleta7DoPlikuUint8,0,sizeof(paleta7DoPlikuUint8));

    cout << "Wybrano dithering numer 1! (Tablica Bayera 4x4)" << endl;

    if(tryb == 1 || tryb == 4 || tryb == 5){  //KOLOR
        int kolor7bit;
        bayer4x4RGB();
        for(int z = 0; z<szerokosc/2; z += 8){
            for(int y = wysokosc/2; y < wysokosc; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    kolor7bit = z24RGBna7RGB(kolor);
                    paleta7DoPlikuUint8[counter] = kolor7bit;
                    counter++;
                }
            }
        }

    }else if(tryb == 2 || tryb == 3){ //BW
        int szary7bit;
        bayer4x4BW();
        for(int z = 0; z<szerokosc/2; z += 8){
            for(int y = wysokosc/2; y < wysokosc; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    szary7bit = z24RGBna7BW(kolor);
                    paleta7DoPlikuUint8[counter] = szary7bit;
                    counter++;
                }
            }
        }
    }else{
        cout << "Tryb obrazu nie zostal wybrany!" << endl;
    }

}

void Funkcja8() {   // FLOYD-STEINBERG
    dithering = 2;
    int counter = 0;
    SDL_Color kolor;

    memset(paleta7DoPlikuUint8,0,sizeof(paleta7DoPlikuUint8));

    cout << "Wybrano dithering numer 2! (Floyd-Steinberg)" << endl;

    if(tryb == 1 || tryb == 4 || tryb == 5){ // RGB
        int kolor7bit;
        floydRGB();
        for(int z = 0; z<szerokosc/2; z += 8){
            for(int y = wysokosc/2; y < wysokosc; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    kolor7bit = z24RGBna7RGB(kolor);
                    paleta7DoPlikuUint8[counter] = kolor7bit;
                    counter++;
                }
            }
        }
    }else if(tryb == 2 || tryb == 3){ // BW
        int szary7bit;
        floydBW();
        for(int z = 0; z<szerokosc/2; z += 8){
            for(int y = wysokosc/2; y < wysokosc; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    szary7bit = z24RGBna7BW(kolor);
                    paleta7DoPlikuUint8[counter] = szary7bit;
                    counter++;
                }
            }
        }
    }else{
        cout << "Tryb obrazu nie zostal wybrany!" << endl;
    }

}

void FunkcjaW(){    // ZAPIS DO PLIKU Z11
    Uint8 extractedBits;

    if(dithering == 3){
        cout << "PROSZE WYBRAC OPCJE DITHERINGU!" << endl;
        return;
    }

    cout<<"Zapisujemy plik 'obraz.z11' uzywajac metody write()"<<endl;
    ofstream wyjscie("obraz.z11" , ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*3);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&tryb, sizeof(Uint8));
    wyjscie.write((char*)&dithering, sizeof(Uint8));
    if(tryb == 3 || tryb == 5){
        rozmiarPalety = ileKubelkow;
        wyjscie.write((char*)&rozmiarPalety, sizeof(Uint8));
        for(int i = 0; i < rozmiarPalety; i++){
            wyjscie.write((char*)&paleta[i].r, sizeof(Uint8));
            wyjscie.write((char*)&paleta[i].g, sizeof(Uint8));
            wyjscie.write((char*)&paleta[i].b, sizeof(Uint8));
        }
    }
    if(tryb != 0){
        for(int x = 1; x < sizeof(paleta7DoPlikuUint8); x += 8){
            for(int i = 1; i <= 7; i++){
                extractedBits = (paleta7DoPlikuUint8[x+i-1] >> (8 - i)) & ((1 << i) - 1);   // ekstrakturemy 'i' ostatnich bitów z aktualnego elementu
                paleta7DoPlikuUint8[x+i-1] = paleta7DoPlikuUint8[x+i-1] << i;               // przesuwamy aktualn¹ wartoœæ tabeli o 'i' bitów
                paleta7DoPlikuUint8[x+i-2] = paleta7DoPlikuUint8[x+i-2] | extractedBits;    // bramk¹ OR ³¹czymy wyekstrakowane bity z poprzedni¹ wartoœci¹ w tabeli

                wyjscie.write((char*)&paleta7DoPlikuUint8[x+i-2], sizeof(Uint8));
            }
        }
        cout << "Gotowe!" << endl;
    }else{
        cout << "Nie wybrano trybu" << endl;
    }

    wyjscie.close();

}

void FunkcjaE(){    // ODCZYT PLIKU Z11
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

void FunkcjaR(){    // REFRESH
    cout << "Odswiezono ekran." << endl;
    SDL_UpdateWindowSurface(window);
}

void FunkcjaT(){    // ZAPIS DO BMP
    if(dithering == 3){
        cout << "Prosze wybrac opcje ditheringu." << endl;
        return;
    }
    string nazwa;
    cout << "Wybrales zapis do pliku BMP.\nProsze podac jak bedzie nazywac sie plik:\t";
    cin >> nazwa;

    if(dithering == 1 || dithering == 2){
        zapiszBMP((nazwa + ".bmp").c_str(), 0, wysokosc/2);
    }else{
        zapiszBMP((nazwa + ".bmp").c_str(), szerokosc/2, 0);
    }
    cout << "\nPlik o nazwie " << nazwa << ".bmp zostal utworzony.\n\n";
}

void FunkcjaY(){    // YUV I YIQ
    YUV yuv;
    YIQ yiq;

    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            yuv = getYUV(x,y);
            setYUV(x + szerokosc/2, y, yuv.y, yuv.u, yuv.v);
        }
    }

    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            yiq = getYIQ(x,y);
            setYIQ(x, y + wysokosc/2, yiq.y, yiq.i, yiq.q);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void FunkcjaU(){    // DITHERING BAYER
    applyBayerDithering(szerokosc,wysokosc);
    /*
    SDL_Color pixel;
    for(int y=0; y<wysokosc/2; y++){
        for(int x=0; x<szerokosc/2; x++){

            pixel = getPixel(x,y);
            float r = pixel.r;
            float g = pixel.g;
            float b = pixel.b;

            setRGB555(x+szerokosc/2,y, r, g, b);

        }
    }*/

    SDL_UpdateWindowSurface(window);
}

void FunkcjaI(){    // RGB565
    SDL_Color pixel;

    for(int y=0; y<wysokosc/2; y++){
        for(int x=0; x<szerokosc/2; x++){

            pixel = getPixel(x,y);
            float r = pixel.r;
            float g = pixel.g;
            float b = pixel.b;

            setRGB565(x+szerokosc/2,y, r, g, b);

        }
    }

    SDL_UpdateWindowSurface(window);
}

void FunkcjaZ(){    // HSL

    HSL hsl;
    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            hsl = getHSL(x, y);
            setHSL(x + szerokosc/2, y, hsl.h, hsl.s, hsl.l);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void FunkcjaX(){    // undersampling -> zapis obrazka uint8 po kompresji
/*
    //tablica obrazek na uint8
    vector<uint8_t>tablica;

    for(int j=0;j<wysokosc/2;j++){
        for(int i=0;i<szerokosc/2;i++){
            SDL_Color kolor = getPixel(i, j);
            //cout << "getPixel: (" << i << ", " << j << ") - R: " << (int)kolor.r << ", G: " << (int)kolor.g << ", B: " << (int)kolor.b << endl;

            uint8_t szary = (kolor.r + kolor.g + kolor.b) / 3;
            tablica.push_back(szary);
        }
    }

    const char* nazwaPlikuWyjsciowego = "obrazek.byterun";
    ByteRunKompresja(tablica.data(), tablica.size(), nazwaPlikuWyjsciowego);

    ByteRunDekompresjaIRysowanie(nazwaPlikuWyjsciowego);
*/

//LZW na szarym
/*
uint8_t tablica[wysokosc / 2 * szerokosc / 2];
    int indeks = 0;

    for (int j = 0; j < wysokosc / 2; j++) {
        for (int i = 0; i < szerokosc / 2; i++) {
            SDL_Color kolor = getPixel(i, j);
            uint8_t szary = (kolor.r + kolor.g + kolor.b) / 3;
            tablica[indeks++] = szary;
        }
    }

    const char* nazwaPlikuWyjsciowego = "obrazek.lzw";
    LZWKompresja(tablica, indeks, nazwaPlikuWyjsciowego);

    uint8_t zdekompresowaneDane[wysokosc * szerokosc];
    int dlugoscZdekompresowanych;
    LZWDekompresja(nazwaPlikuWyjsciowego, zdekompresowaneDane, dlugoscZdekompresowanych);

    indeks = 0;
    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = szerokosc / 2; x < szerokosc; x++) {
            if (indeks < dlugoscZdekompresowanych) {
                uint8_t szary = zdekompresowaneDane[indeks++];
                setPixel(x, y, szary, szary, szary);
            }
        }
    }

    SDL_UpdateWindowSurface(window);

    */
        uint16_t tablica[wysokosc / 2 * szerokosc / 2];
    int indeks = 0;

    for (int j = 0; j < wysokosc / 2; j++) {
        for (int i = 0; i < szerokosc / 2; i++) {
            Uint16 kolor555 = getRGB555_(i, j);
            tablica[indeks++] = kolor555;
        }
    }

    const char* nazwaPlikuWyjsciowego = "obrazek.lzw555";
    LZWKompresja555(tablica, indeks, nazwaPlikuWyjsciowego);

    uint16_t zdekompresowaneDane[wysokosc * szerokosc];
    int dlugoscZdekompresowanych;
    LZWDekompresja555(nazwaPlikuWyjsciowego, zdekompresowaneDane, dlugoscZdekompresowanych);

    indeks = 0;
    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = szerokosc / 2; x < szerokosc; x++) {
            if (indeks < dlugoscZdekompresowanych) {
                Uint16 kolor555 = zdekompresowaneDane[indeks++];
                Uint8 r = (kolor555 >> 10) << 3;
                Uint8 g = ((kolor555 >> 5) & 0x1F) << 3;
                Uint8 b = (kolor555 & 0x1F) << 3;
                setRGB555(x, y, r, g, b);
            }
        }
    }

    SDL_UpdateWindowSurface(window);



}

void FunkcjaC(){    // rle


    int arraySize = 64000;
    int myArray[arraySize];

    fill_n(myArray, 64000, 5);

    fileCompressionRLE(myArray, arraySize);
    cout << endl;
    fileDecompressionRLE(arraySize);

}

void FunkcjaV(){

    podprobkowanieYUV();

    SDL_UpdateWindowSurface(window);

    kompresjaStratnaDCT();

    SDL_UpdateWindowSurface(window);

    zapisDCTDoPliku();
    odczytDCTZPliku();

    SDL_UpdateWindowSurface(window);
}
