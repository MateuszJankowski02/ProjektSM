#include "SM2024Compression.h"
#include "SM2024Variables.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "SM2024HelperFunctions.h"
#include "SM2024KeyFunctions.h"
#include "SM2024Variables.h"
#include "SM2024CoreFunctions.h"

using namespace std;


void fileCompressionRLE(int inputArray[], int arraySize){

    int i = 0;
    ofstream wyjscie("compression.rle" , ios::binary);

    while(i < arraySize){

        if( (i < arraySize-1) && (inputArray[i] == inputArray[i+1]) ){

            int j = 0;
            while((i+j < arraySize-1) && (inputArray[i+j] == inputArray[i+j+1]) && (j<254)) j++;

            int temp = j+1;

            wyjscie.write((char*)&temp ,sizeof(Uint8));
            wyjscie.write((char*)&inputArray[i+j], sizeof(Uint8));

            i += j+1;
        }else{

            int j = 0;
            while((i+j < arraySize-1) && (inputArray[i+j] != inputArray[i+j+1]) && (j<254)) j++;

            if((i+j == arraySize-1) && (j<253)) j++;

            wyjscie.write((char*)0 ,sizeof(Uint8));
            wyjscie.write((char*)&j, sizeof(Uint8));
            for(int k = 0; k < j; k++) wyjscie.write((char*)&inputArray[i+k] ,sizeof(Uint8));

            if(j % 2 != 0) wyjscie.write((char*)0 ,sizeof(Uint8));

            i+=j;
        }
    }
    wyjscie.close();
}

void fileDecompressionRLE(int arraySize){


    int i = 0;
    int inputArray[arraySize];
    ifstream wejscie("compression.rle", ios::binary);

    for(int a = 0; a < arraySize; a++){
        wejscie.read((char*)&inputArray[a], sizeof(Uint8));
    }

    for(int l = 0; l < 2000; l++){
        cout << l << ": " << inputArray[l] << endl;
    }

    while(i < arraySize){

        if(inputArray[i] > 0){
            int j = inputArray[i];
            i++;

            for(int k = 0; k < j; k++){
                //cout << inputArray[i] << endl;
            }
            i++;
        }else if (inputArray[i] == 0){
            i++;
            int j = inputArray[i];
            i++;

            for(int k = 0; k < j; k++){
                //cout << inputArray[i] << endl;
                i++;
            }

            if (j%2 != 0) i++;
        }
    }
    wejscie.close();
}


//kompresje bytTurt
void ByteRunKompresja(const uint8_t wejscie[], int dlugosc, const char* nazwaPlikuWyjsciowego) {
    ofstream wyjscie(nazwaPlikuWyjsciowego, ios_base::binary);
    if (!wyjscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku wyjsciowego!" << endl;
        return;
    }

    int i = 0;

    while (i < dlugosc) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])) {
            // Sekwencja powtarzajych si bajtów
            int j = 0;
            while ((i + j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + j + 1]) && (j < 127)) {
                j++;
            }
            wyjscie.put(-j);             // Zapisz dugo powtarzajcych si bajtow
            wyjscie.put(wejscie[i]);     // Zapisz warto powtarzajcego si bajtu
            i += (j + 1);
        } else {
            // Sekwencja rónych bajtów
            int j = 0;
            while ((i + j < dlugosc - 1) && (wejscie[i + j] != wejscie[i + j + 1]) && (j < 128)) {
                j++;
            }
            if ((i + j == dlugosc - 1) && (j < 128)) j++;

            wyjscie.put(j - 1);  // Zapisz dugo unikalnych bajtów
            for (int k = 0; k < j; k++) {
                wyjscie.put(wejscie[i + k]); // Zapisz unikalne bajty
            }
            i += j;
        }
    }

    wyjscie.close();
    cout << "Kompresja zakonczona. Wynik zapisano w pliku: " << nazwaPlikuWyjsciowego << endl;
}

int rozmiarPliku(const char* nazwaPliku) {
    ifstream wejscie;
    wejscie.open(nazwaPliku, ios_base::binary);
    if (!wejscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku!" << endl;
        return -1;
    }
    wejscie.seekg(0, ios_base::end);
    int rozmiar = wejscie.tellg();
    wejscie.close();
    return rozmiar;

}

void ByteRunDekompresjaIRysowanie(const char* nazwaPlikuWejsciowego) {
    // Dekompresja danych
    ifstream wejscie(nazwaPlikuWejsciowego, ios_base::binary);
    if (!wejscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku wejsciowego!" << endl;
        return;
    }

    vector<uint8_t> zdekompresowaneDane;

    while (!wejscie.eof()) {
        int8_t kod = wejscie.get();
        if (wejscie.eof()) break; // Koniec pliku

        if (kod < 0) {
            // Powtarzajce sie bajty
            uint8_t wartosc = wejscie.get();
            if (wejscie.eof()) break; // Koniec pliku

            int dlugosc = -kod + 1;
            zdekompresowaneDane.insert(zdekompresowaneDane.end(), dlugosc, wartosc);
        } else {
            // Sekwencja rónych bajtów
            int dlugosc = kod + 1;
            for (int i = 0; i < dlugosc; i++) {
                uint8_t wartosc = wejscie.get();
                if (wejscie.eof()) break; // Koniec pliku

                zdekompresowaneDane.push_back(wartosc);
            }
        }
    }
    wejscie.close();

    cout << "Dekompresja zakonczona. Odczytano " << zdekompresowaneDane.size() << " bajtow." << endl;

    // Rysowanie obrazu
    int indeks = 0;
    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = szerokosc / 2; x < szerokosc; x++) {
            if (indeks < zdekompresowaneDane.size()) {
                uint8_t szary = zdekompresowaneDane[indeks++];
                setPixel(x, y, szary, szary, szary); // Ustawiamy szary kolor na podstawie wartoci pikseli
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}
