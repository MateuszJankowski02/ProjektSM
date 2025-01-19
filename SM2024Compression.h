#ifndef SM2024COMPRESSION_H_INCLUDED
#define SM2024COMPRESSION_H_INCLUDED

#include <SDL2/SDL.h>

void fileCompressionRLE(int inputArray[], int arraySize);

void fileDecompressionRLE(int arraySize);


void LZWKompresja(const uint8_t wejscie[], int dlugosc, const char* nazwaPlikuWyjsciowego);

void LZWDekompresja(const char* nazwaPlikuWejsciowego, uint8_t wyjscie[], int& dlugoscWyjscia);


void LZWKompresja555(const uint16_t wejscie[], int dlugosc, const char* nazwaPlikuWyjsciowego);

void LZWDekompresja555(const char* nazwaPlikuWejsciowego, uint16_t wyjscie[], int& dlugoscWyjscia);


void ByteRunKompresja(const uint8_t wejscie[], int dlugosc, const char* nazwaPlikuWyjsciowego);

int rozmiarPliku(const char* nazwaPliku);

void ByteRunDekompresjaIRysowanie(const char* nazwaPlikuWejsciowego);

#endif // SM2024COMPRESSION_H_INCLUDED
