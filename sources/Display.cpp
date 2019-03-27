#include "../headers/llibreries.hpp"
#include "../headers/graphics.h"
#include "../headers/Display.hpp"




/**********************************************************************************************************************
Clase Display
**********************************************************************************************************************/
Display::Display(float in_ancho, float in_alto, bool esMaster)
{
	ancho  = in_ancho;
	alto   = in_alto ;
	Master = esMaster;
	num_conexions=0; token=0;
	totsCalibrats=false;
	minimFound=false;
	temps_k=0;
	refresca=false;
	pA=0.0;
	maximaProvabilitat=0.0;

	cout << "Display created: " << this << endl;

	ImagenDisplayVector   = (unsigned char*) malloc (sizeof(char) * (int)floor(ancho) *(int)floor(alto) *4);
	InterseccioOptimisada = (unsigned char*) malloc (sizeof(char) * 400 *200 *3);

	//memcpy(InterseccioOptimisada, 0, sizeof(char) * 400 *200 *3);

	ImagenDisplayTextura.CreateRGBA ( ancho, alto );
	ImagenDisplayTextura.GenerateFrameBuffer();
	//NUM_DISPLAYS++;
	//cout << "NUM_DISPLAYS -> " << NUM_DISPLAYS << endl;

}



void Display::colorDisplay (int R, int G, int B, int A)
{
	for(int cnt=0; cnt< (floor(ancho)*floor(alto)*4) ; cnt=cnt+4)  //
	{
		ImagenDisplayVector[cnt+0]= R;
		ImagenDisplayVector[cnt+1]= G;
		ImagenDisplayVector[cnt+2]= B;
		ImagenDisplayVector[cnt+3]= A;
	}
}


void Display::dibuixar(void)
{
	ImagenDisplayTextura.SetPixels(ImagenDisplayVector);

	BeginFrame();
	DrawTextureRect ( &ImagenDisplayTextura,        -1.0f, -1.0f, 1.0f, 1.0f, NULL);
	EndFrame();
}



void Display::posarCircul (int centro_x, int centro_y, int radi )
{

	int rad1 = (radi)*(radi);
	int anchoPant = (int)floor(ImagenDisplayTextura.GetWidth());

	for( int cnti=centro_x-radi; cnti<(centro_x+radi); cnti++ )
	{
		for( int cntj=centro_y-radi; cntj<(centro_y+radi); cntj++ )
		{
			int norma = pow((cnti-centro_x),2) + pow((cntj-centro_y),2);

			if(norma<=rad1)
			{
				long int posTEX  = cntj*anchoPant + cnti;

				ImagenDisplayVector[posTEX*4]  = 255;
				ImagenDisplayVector[posTEX*4+1]= 255;
				ImagenDisplayVector[posTEX*4+2]= 255;
				ImagenDisplayVector[posTEX*4+3]= 255;
			}
		}
	}

}


void Display::dibuixarPatroCalibracio (float OFFSET_X, float OFFSET_Y, float escala)
{
	float pantalla_x, pantalla_y;

	int fonfo=0;
	//ILLUMINACIO (display->pImagenDisplayTextura(), 0,0,0, NULL);
	for(int ite=0; ite<90; ite++) //42
	{
		if(ite%2) //UNO SÍ UNO NO
		{
			pantalla_x = puntos_pantalla[ite][0]*escala +  OFFSET_X;
			pantalla_y = puntos_pantalla[ite][1]*escala +  OFFSET_Y;

			int radi = 11;
			//cout << "X: " << pantalla_x << ", Y: " << pantalla_y << " R: " << radi << endl;
			posarCircul ( (int)round(pantalla_x), (int)round(pantalla_y), (int)radi );
		}
	}
	cout << "Drawn pattern on display" << endl;
}
