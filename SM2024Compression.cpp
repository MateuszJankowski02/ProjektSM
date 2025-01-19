#include "SM2024Compression.h"
#include "SM2024Variables.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstdint>

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

//lzw
void LZWKompresja(const uint8_t wejscie[], int dlugosc, const char* nazwaPlikuWyjsciowego) {
    ofstream wyjscie(nazwaPlikuWyjsciowego, ios::binary);
    if (!wyjscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku wyjsciowego!" << endl;
        return;
    }

    map<string, int> slownik;
    for (int i = 0; i < 256; i++) {
        slownik[string(1, i)] = i;
    }

    string w = "";
    int kod = 256;
    for (int i = 0; i < dlugosc; i++) {
        char c = wejscie[i];
        string wc = w + c;
        if (slownik.find(wc) != slownik.end()) {
            w = wc;
        } else {
            int kodDoZapisania = slownik[w];
            wyjscie.write(reinterpret_cast<const char*>(&kodDoZapisania), sizeof(int));
            if (kod < 4096) {
                slownik[wc] = kod++;
            }
            w = string(1, c);
        }
    }

    if (!w.empty()) {
        int kodDoZapisania = slownik[w];
        wyjscie.write(reinterpret_cast<const char*>(&kodDoZapisania), sizeof(int));
    }

    wyjscie.close();
    cout << "Kompresja LZW zakonczona. Wynik zapisano w pliku: " << nazwaPlikuWyjsciowego << endl;
}

void LZWDekompresja(const char* nazwaPlikuWejsciowego, uint8_t wyjscie[], int& dlugoscWyjscia) {
    ifstream wejscie(nazwaPlikuWejsciowego, ios::binary);
    if (!wejscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku wejsciowego!" << endl;
        dlugoscWyjscia = 0;
        return;
    }

    map<int, string> slownik;
    for (int i = 0; i < 256; i++) {
        slownik[i] = string(1, i);
    }

    int kod = 256;
    int poprzedni, obecny;
    wejscie.read(reinterpret_cast<char*>(&poprzedni), sizeof(int));
    string w = slownik[poprzedni];
    dlugoscWyjscia = 0;

    for (char c : w) {
        wyjscie[dlugoscWyjscia++] = c;
    }

    while (wejscie.read(reinterpret_cast<char*>(&obecny), sizeof(int))) {
        string wejscieTekst;
        if (slownik.find(obecny) != slownik.end()) {
            wejscieTekst = slownik[obecny];
        } else {
            wejscieTekst = w + w[0];
        }

        for (char c : wejscieTekst) {
            wyjscie[dlugoscWyjscia++] = c;
        }

        if (kod < 4096) {
            slownik[kod++] = w + wejscieTekst[0];
        }

        w = wejscieTekst;
    }

    wejscie.close();
    cout << "Dekompresja LZW zakonczona. Odczytano " << dlugoscWyjscia << " bajtow." << endl;
}

//lzw555

void LZWKompresja555(const uint16_t wejscie[], int dlugosc, const char* nazwaPlikuWyjsciowego) {
    ofstream wyjscie(nazwaPlikuWyjsciowego, ios::binary);
    if (!wyjscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku wyjsciowego!" << endl;
        return;
    }

    map<string, int> slownik;
    for (int i = 0; i < 65536; i++) {
        slownik[string(reinterpret_cast<const char*>(&i), sizeof(uint16_t))] = i;
    }

    string w = "";
    int kod = 65536;
    for (int i = 0; i < dlugosc; i++) {
        string wc = w + string(reinterpret_cast<const char*>(&wejscie[i]), sizeof(uint16_t));
        if (slownik.find(wc) != slownik.end()) {
            w = wc;
        } else {
            int kodDoZapisania = slownik[w];
            wyjscie.write(reinterpret_cast<const char*>(&kodDoZapisania), sizeof(int));
            if (kod < 1048576) { // Przyklad: maksymalnie 2^20 kodow
                slownik[wc] = kod++;
            }
            w = string(reinterpret_cast<const char*>(&wejscie[i]), sizeof(uint16_t));
        }
    }

    if (!w.empty()) {
        int kodDoZapisania = slownik[w];
        wyjscie.write(reinterpret_cast<const char*>(&kodDoZapisania), sizeof(int));
    }

    wyjscie.close();
    cout << "Kompresja LZW555 zakonczona. Wynik zapisano w pliku: " << nazwaPlikuWyjsciowego << endl;
}

void LZWDekompresja555(const char* nazwaPlikuWejsciowego, uint16_t wyjscie[], int& dlugoscWyjscia) {
    ifstream wejscie(nazwaPlikuWejsciowego, ios::binary);
    if (!wejscie.is_open()) {
        cerr << "Nie mozna otworzyc pliku wejsciowego!" << endl;
        dlugoscWyjscia = 0;
        return;
    }

    map<int, string> slownik;
    for (int i = 0; i < 65536; i++) {
        slownik[i] = string(reinterpret_cast<const char*>(&i), sizeof(uint16_t));
    }

    int kod = 65536;
    int poprzedni, obecny;
    wejscie.read(reinterpret_cast<char*>(&poprzedni), sizeof(int));
    string w = slownik[poprzedni];
    dlugoscWyjscia = 0;

    for (size_t i = 0; i < w.size(); i += sizeof(uint16_t)) {
        uint16_t kolor;
        memcpy(&kolor, w.data() + i, sizeof(uint16_t));
        wyjscie[dlugoscWyjscia++] = kolor;
    }

    while (wejscie.read(reinterpret_cast<char*>(&obecny), sizeof(int))) {
        string wejscieTekst;
        if (slownik.find(obecny) != slownik.end()) {
            wejscieTekst = slownik[obecny];
        } else {
            wejscieTekst = w + w.substr(0, sizeof(uint16_t));
        }

        for (size_t i = 0; i < wejscieTekst.size(); i += sizeof(uint16_t)) {
            uint16_t kolor;
            memcpy(&kolor, wejscieTekst.data() + i, sizeof(uint16_t));
            wyjscie[dlugoscWyjscia++] = kolor;
        }

        if (kod < 1048576) {
            slownik[kod++] = w + wejscieTekst.substr(0, sizeof(uint16_t));
        }

        w = wejscieTekst;
    }

    wejscie.close();
    cout << "Dekompresja LZW555 zakonczona. Odczytano " << dlugoscWyjscia << " elementow." << endl;
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
