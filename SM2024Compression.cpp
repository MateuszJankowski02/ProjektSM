#include "SM2024Compression.h"
#include "SM2024Variables.h"

#include <iostream>
#include <fstream>
#include <vector>

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
