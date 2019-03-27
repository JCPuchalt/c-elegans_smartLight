#pragma once

#include "llibreries.hpp"


#define FIFO_SIZE 20

using namespace cv;
using namespace std;


struct image_t
{
    int tamany, _width, _heigh, _colors;
    char direccion[500];
    unsigned char* image;
};

/*
 * @ Classe Buffer Circular
 *
 * @ Crea una cola de imágenes que se irán guardando en disco
 * @
 * @
*/
class bufferCircular
{

    image_t imageBuffer[FIFO_SIZE];

    int idxLast;
    int idxFI;
    int enCola;

public:
    bufferCircular();

    void addImage(unsigned char* Im, char dir[], int tamany, int W, int H, int _colors);
    bool remImage(unsigned char* Im, char *direccion, int &tam, int &W, int &H, int &_colors );
    void reset(void);
    int checkAvail(void);

};
