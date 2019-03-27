#pragma once
#include <cstdlib>
#include "headers/llibreries.hpp"

using namespace cv;
using namespace std;


CCamera* cam;

class  bufferCircular;

int  WIDTH_FRAME, HEIGHT_FRAME, FPS, WIDTH_PANT, HEIGHT_PANT, BRIGHTNESS, INTEGRATION_TIME;
char root[500]="./Experiment/";

void caputure					(visionSystem* VisionSystem, int number);
void loadConfiguration(void);
void generadorDeRuta	( char direccionDeGrabacion[], int codicImage );
bool is_number				(const std::string& s);

int main(int argc, const char *argv[])
{
	// Load Camera and Display Configuration
	loadConfiguration();

	//initialise openGL graphics and the camera
	InitGraphics();
	cout << "\033[1;32mGraphics loaded\033[0m" << endl;


	// Create Display
	Display display( WIDTH_PANT, HEIGHT_PANT, true);
	display.colorDisplay(0,0,0,255);
	display.dibuixar();

	// Create Vision System
	visionSystem VisionSystem = visionSystem(1, 1, &display);

	//////// Calibration ////////////////
	VisionSystem.CE_Calibrar();

	// Draw red square on display
	VisionSystem.Init_Textura(255,0,0);
	display.dibuixar();

	long int i=0;

	// Programm main loop
	while(1)
	{

		cout << "Please set the plate centered on red square of display, then:" << endl;
		cout << "· Press 'c' to compensate." << endl;
		cout << "· Introduce capture iterations to capture." << endl;
		cout << "· Introduce folder: (" << root <<  ")" << endl;
		cout << "· Press 'e' to exit" << endl;
		cout << ">> ";
		fflush(stdout);

		string intro;
		cin >> intro;

		int number=5;
		char selection; //='c';
		if(is_number(intro)) {
			number=atoi(intro.c_str());
			selection='a';
		}else if (intro.length()==1 && !is_number(intro)) {
			selection=intro.at(0);
		}else if(!is_number(intro) && intro.length()>1){
			selection='r';
		}

		switch (selection) {
			case 'c':
				{
					//////////// Compensate ////////////////////
					VisionSystem.resetCompensation();
					VisionSystem.CE_Compensar_Rapido();
				}
				break;
			case 'a':
				{
					// Capture images and save them
					caputure(&VisionSystem, number);
				}
				break;
			case 'r':
				{
					strcpy(root,"./");
					strcat(root,intro.c_str());
				}
				break;
			case 'e':
				{
					//sleep(3000000);
					StopCamera();
					exit(0);
					break;
				}
		}
		i++;

	}

}


/************************************************
Check if a string is a integer
************************************************/
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


/************************************************
Capture and save plate
************************************************/
void caputure(visionSystem* VisionSystem, int number)
{
	int codicImage=0;
	while(codicImage<number) {
		// Initiating timer
		struct timespec tBucle_0 = Inicia_Temporisador();

		//////////// Take image ///////////////////
		VisionSystem->CE_Capturar ();

		//////////// Save image ////////////////////
		if(1) { //capturando
			// Genera ruta de imagen a grabar para modo autonomo
			char direccionDeGrabacion[150]="";
			codicImage++;
			generadorDeRuta( direccionDeGrabacion, codicImage);
			// Añadir al buffer para guardar en Memoria
			VisionSystem->saveImage(direccionDeGrabacion);
		}

		////////// Wait for a time //////////////////
		float tempsBucle = Medix_temps(tBucle_0);
		int espera= (int) floor((1-tempsBucle)*1000000000);
		//printf("\rLoop time: %.4f seconds", tempsBucle);
		//fflush(stdout);

		struct timespec tim, tim2;
		tim.tv_sec = 0;
		tim.tv_nsec = espera;

		if(espera>0) {
			if(nanosleep(&tim , &tim2) < 0 ) {
				printf("Nano sleep system call failed \n");
			}
		}
	}
	cout << endl;
}



/************************************************
Load Configuration
************************************************/
void loadConfiguration ( void )
{

	if(!existe("./configuracio.xml"))
	{
		cout << "Does not exist configuration file" << endl;
		FileStorage fs("./configuracio.xml", FileStorage::WRITE);
		fs << "WIDTH_IMAGE"				<< (int)1944;
		fs << "HEIGHT_IMAGE"			<< (int)1944;
		fs << "FPS"								<< (int)7;
		fs << "WIDTH_DISPLAY"			<< (int)800;
		fs << "HEIGHT_DISPLAY"		<< (int)480;
		fs << "BRIGHTNESS"				<< (int)25;
		fs << "INTEGRATION_TIME"	<< (int)100000;
		fs << "WIDTH_CAMERA"			<< (int)2592;
		fs << "HEIGHT_CAMERA"			<< (int)1944;

	}

	int WIDTH_CAMERA, HEIGHT_CAMERA;
	FileStorage fs("./configuracio.xml", FileStorage::READ);
	fs ["WIDTH_IMAGE"]			>> WIDTH_FRAME;
	fs ["HEIGHT_IMAGE"]			>> HEIGHT_FRAME;
	fs ["FPS"]							>> FPS;
	fs ["WIDTH_DISPLAY"]		>> WIDTH_PANT;
	fs ["HEIGHT_DISPLAY"]		>> HEIGHT_PANT;
	fs ["BRIGHTNESS"]				>> BRIGHTNESS;
	fs ["INTEGRATION_TIME"]	>> INTEGRATION_TIME;
	fs ["WIDTH_CAMERA"]			>> WIDTH_CAMERA;
	fs ["HEIGHT_CAMERA"]		>> HEIGHT_CAMERA;

	cout << "Configuration loaded" << endl;

}

/************************************************
Generate a folder, path and name of image
************************************************/
void generadorDeRuta( char direccionDeGrabacion[], int codicImage )
{

	char generandoDireccion[150]="";
	strcat(generandoDireccion, root);

	int it=0;
	while (generandoDireccion[it]!='\0') it++;
	if(generandoDireccion[it-1]!='/') strcat( generandoDireccion, "/");

	string sequencia = NOM_000X (codicImage);
	strcat( generandoDireccion, sequencia.c_str() );
	strcat( generandoDireccion, ".bmp");

	strcpy( direccionDeGrabacion, generandoDireccion);
	if(!existe(direccionDeGrabacion))
	{
		crear_carpeta (direccionDeGrabacion);
		//cout << "Folder generated" << endl;
	}
}
