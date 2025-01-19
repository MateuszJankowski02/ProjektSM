#include "SM2024Undersampling.h"
#include "SM2024Variables.h"
#include "SM2024CoreFunctions.h"
#include "SM2024ColorModels.h"


//4:2:0

void podprobkowanieYIQ(int dx, int dy){

    SDL_Color picked;
    YIQ transformed;
    YIQ cell[4];
    int half_width = szerokosc/2;

    for (int j = 0; j < wysokosc/2-1; j++){
        for (int i = 0; i < half_width-1; i++){

                cell[0] = getYIQ(i,j);
                cell[1] = getYIQ(i+1,j);
                cell[2] = getYIQ(i,j+1);
                cell[3] = getYIQ(i+1,j+1);
            float sharedI = 0;
            float sharedQ = 0;
            for (int c = 0; c < 4; c++){
                sharedI += cell[c].i;
                sharedQ += cell[c].q;
            }
            sharedI /= 4.0;
            sharedQ /= 4.0;

            for (int v = 0; v < 2; v++){
                for (int h = 0; h < 2; h++){
                    setYIQ(i+dx+h,j+dy+v, cell[h+v].y,sharedI,sharedQ);
                }
            }
            setYIQ(i+dx,j+dy, cell[0].y,sharedI,sharedQ);
            setYIQ(i+dx+1,j+dy, cell[1].y,sharedI,sharedQ);
            setYIQ(i+dx,j+dy+1, cell[2].y,sharedI,sharedQ);
            setYIQ(i+dx+1,j+dy+1, cell[3].y,sharedI,sharedQ);
        }
    }
}

void podprobkowanieYCbCr(int dx, int dy){
    SDL_Color picked;
    YCbCr transformed;
    YCbCr cell[4];
    int half_width = szerokosc/2;

    for (int i = 0; i < half_width-1; i++){
        for (int j = 0; j < wysokosc/2-1; j++){
                cell[0] = getYCbCr(i,j);
                cell[1] = getYCbCr(i+1,j);
                cell[2] = getYCbCr(i,j+1);
                cell[3] = getYCbCr(i+1,j+1);
            float sharedCb = 0;
            float sharedCr = 0;
            for (int c = 0; c < 4; c++){
                sharedCb += cell[c].cb;
                sharedCr += cell[c].cr;
            }
            sharedCb /= 4.0;
            sharedCr /= 4.0;

            setYCbCr(i+dx,j+dy, cell[0].y,sharedCb,sharedCr);
            setYCbCr(i+dx+1,j+dy, cell[1].y,sharedCb,sharedCr);
            setYCbCr(i+dx,j+dy+1, cell[2].y,sharedCb,sharedCr);
            setYCbCr(i+dx+1,j+dy+1, cell[3].y,sharedCb,sharedCr);
        }
    }
}

void podprobkowanieYUV(){
    YUV yuv11, yuv12, yuv21, yuv22;

    for(int x = 0; x < szerokosc/2; x += 2){
        for(int y = 0; y < wysokosc/2; y += 2){
            yuv11 = getYUV(x  , y  );
            yuv12 = getYUV(x+1, y  );
            yuv21 = getYUV(x  , y+1);
            yuv22 = getYUV(x+1, y+1);

            setYUV(x,     y,     yuv11.y, yuv11.u, yuv11.v); // 11
            setYUV(x + 1, y,     yuv12.y, yuv11.u, yuv11.v); // 12
            setYUV(x,     y + 1, yuv21.y, yuv11.u, yuv11.v); // 21
            setYUV(x + 1, y + 1, yuv22.y, yuv11.u, yuv11.v); // 22
        }
    }

    SDL_UpdateWindowSurface(window);
}

void podprobkowanieHSL(int dx, int dy){
    HSL cell[4];
    int half_width = szerokosc / 2;

    for (int i = 0; i < half_width - 1; i++) {
        for (int j = 0; j < wysokosc / 2 - 1; j++) {
            // Pobierz wartości HSL dla 4 pikseli w bloku 2x2
            cell[0] = getHSL(i, j);
            cell[1] = getHSL(i + 1, j);
            cell[2] = getHSL(i, j + 1);
            cell[3] = getHSL(i + 1, j + 1);

            // Oblicz średnie wartości dla H i S
            float sharedH = 0, sharedS = 0;
            for (int c = 0; c < 4; c++) {
                sharedH += cell[c].h;
                sharedS += cell[c].s;
            }
            sharedH /= 4.0;
            sharedS /= 4.0;

            // Zachowaj oryginalną wartość L dla każdego piksela
            setHSL(i + dx, j + dy, sharedH, sharedS, cell[0].l);
            setHSL(i + dx + 1, j + dy, sharedH, sharedS, cell[1].l);
            setHSL(i + dx, j + dy + 1, sharedH, sharedS, cell[2].l);
            setHSL(i + dx + 1, j + dy + 1, sharedH, sharedS, cell[3].l);
        }
    }
}
