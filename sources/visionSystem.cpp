#pragma once

#include "../headers/llibreries.hpp"
#include "../headers/visionSystem.hpp"


#ifdef _DEBUG
#pragma comment(lib, "opencv_core246d.lib")
#pragma comment(lib, "opencv_imgproc246d.lib")   //MAT processing
#pragma comment(lib, "opencv_highgui246d.lib")
#pragma comment(lib, "opencv_stitching246d.lib");

#else
#pragma comment(lib, "opgrencv_core246.lib")
#pragma comment(lib, "opencv_imgproc246.lib")
#pragma comment(lib, "opencv_highgui246.lib")
#pragma comment(lib, "opencv_stitching246.lib");
#endif

using namespace cv;
using namespace std;


/** Histograma de Referencia **/
double H_Referencia[256]={0};

float puntos_pantalla[90][2]=
{
			     {0, 0}, {42*1*texelRatio, 7*42},	{0, 0}, {42*3*texelRatio, 7*42}, 	{0, 0}, {42*5*texelRatio,  7*42},   {0, 0},  {42*7*texelRatio,  7*42},   {0, 0},  {42*9*texelRatio,   7*42},   {0, 0},
	{0, 42*6}, 	     {0, 0}, {42*2*texelRatio, 6*42}, 	{0, 0}, {42*4*texelRatio, 6*42}, 	{0, 0}, {42*6*texelRatio,  6*42},   {0, 0},  {42*8*texelRatio,  6*42},   {0, 0},  {42*10*texelRatio,  6*42},

			     {0, 0}, {42*1*texelRatio, 5*42},	{0, 0}, {42*3*texelRatio, 5*42},	{0, 0}, {42*5*texelRatio,  5*42},   {0, 0},  {42*7*texelRatio,  5*42},   {0, 0},  {42*9*texelRatio,   5*42},   {0, 0},
	{0, 42*4}, 	     {0, 0}, {42*2*texelRatio, 4*42}, 	{0, 0}, {42*4*texelRatio, 4*42},	{0, 0}, {42*6*texelRatio,  4*42},   {0, 0},  {42*8*texelRatio,  4*42},   {0, 0},  {42*10*texelRatio,  4*42},

			     {0, 0}, {42*1*texelRatio, 3*42}, 	{0, 0}, {42*3*texelRatio, 3*42},	{0, 0}, {42*5*texelRatio,  3*42},   {0, 0},  {42*7*texelRatio,  3*42},   {0, 0},  {42*9*texelRatio,   3*42},   {0, 0},
	{0, 42*2},  	     {0, 0}, {42*2*texelRatio, 2*42},  	{0, 0}, {42*4*texelRatio, 2*42},	{0, 0}, {42*6*texelRatio,  2*42},   {0, 0},  {42*8*texelRatio,  2*42},   {0, 0},  {42*10*texelRatio,  2*42},

			     {0, 0}, {42*1*texelRatio,   42}, 	{0, 0}, {42*3*texelRatio, 1*42},	{0, 0}, {42*5*texelRatio,  1*42},   {0, 0},  {42*7*texelRatio,  1*42}, 	 {0, 0},  {42*9*texelRatio,   1*42},   {0, 0},
	{0,    0}, 	     {0, 0}, {42*2*texelRatio,    0}, 	{0, 0}, {42*4*texelRatio,    0},	{0, 0}, {42*6*texelRatio,     0},   {0, 0},  {42*8*texelRatio,     0},   {0, 0},  {42*10*texelRatio,     0}

};


/***************************************************************************************
Constructuor de Clase: "camara". Inicio de clase. Crea, inicializa... variables
****************************************************************************************/
visionSystem::visionSystem( int iD, int numero_camaras, Display* _display )
{
	// Setting display
	display = _display;
	ImagenDisplayVector  = display->pImagenDisplayVector();
	ImagenDisplayTextura = display->pImagenDisplayTextura();

	// Setting Camera
	cam = StartCamera( WIDTH_HD, HEIGHT_HD, FPS,1,false ); // -> 7 -> 8
	MMAL_COMPONENT_T *camComponent = cam->get_CameraComponent();
	raspicamcontrol_set_shutter_speed(camComponent, INTEGRATION_TIME);
	raspicamcontrol_set_brightness(camComponent, BRIGHTNESS);
	cout << "Camera created: " << WIDTH_HD << "x"<< HEIGHT_HD << " @" << FPS << "fps - INTEGRATION_TIME: " << INTEGRATION_TIME << ", BRIGHTNESS: " << BRIGHTNESS << endl;

	// Reference histogram
	H_Referencia[43]=0.1;
	H_Referencia[44]=0.3;
	H_Referencia[45]=1.2;
	H_Referencia[46]=5.5;
	H_Referencia[47]=16.5;
	H_Referencia[48]=28.0;
	H_Referencia[49]=25.5;
	H_Referencia[50]=13.8;
	H_Referencia[51]=5.6;
	H_Referencia[52]=1.8;
	H_Referencia[53]=0.5;
	H_Referencia[54]=0.1;

	corregir_x=0; corregir_y=0; CNTA=0;
	flash=0;
  Iteracions_Compensacio = 1;
	IteradorPrograma=0;
	minim_error_compensacio = 100.0;
	mijaInicial=0;
  MET_Ant=0; mija_error_compensacio=0;
  It_Erro=0; min_found=false;
  alpha=0; tamInterseccio=0; tamFusionadaParcial=0;
	errorQuadraticMig_Anterior=0;
	tamInterseccionParcial=0;
	enviaInfoAlMaster=false;
	enviaInfoAlPrincipal=false;
	tablaParcial_Enviada=false;
	num_camaras = 1;

	bloquejat=false; capturando=false; inici_local=false;
	inici_remot=false; iniciat=false; fin_captura=false;

	compensada=true;

	FYmin=0; FYmax=0;
	minimIdx = 10000000;
	maximIdx = 0;

	reset_comp=false;
	calibrada=false;

	idCam = iD;
	escala=1;


	mostrar_tex.CreateRGBA ( WIDTH_PANT, HEIGHT_PANT );
	mostrar_tex.GenerateFrameBuffer();
	cout << "Texture created -> " << idCam << endl;

	strcpy(ruta_homography		, "./configuracio/homografia1.xml");
	strcpy(ruta_patron		, "./configuracio/p1.xml");
	loadPatron();

	tabla_calibracion						= (long int*) 	   malloc (sizeof(long int)  	* WIDTH_PANT*HEIGHT_PANT*2);
	quadrat											= (unsigned char*) malloc (sizeof(char) 			* WIDTH_FRAME * HEIGHT_FRAME );
	vector_mostrar_ant 					= (unsigned char*) malloc (sizeof(char)      	* WIDTH_PANT*HEIGHT_PANT*4);
	vector_mostrar_minim_error 	= (unsigned char*) malloc (sizeof(char)      	* WIDTH_PANT*HEIGHT_PANT*4);
	cout << "Image space reserved -> " << idCam << endl;

	memset(quadrat,  0, sizeof(char)      *  WIDTH_FRAME * HEIGHT_FRAME );
	memset(tabla_calibracion,        0, sizeof(int)*WIDTH_PANT*HEIGHT_PANT*2);

	// Create a Circular Buffer to saving images on disk
	buffer = new bufferCircular();

	// Create an image thread
	cout << "Saving thread launch ->" << idCam << endl;
	pthread_t save_id;
	pthread_create(&save_id, NULL, CE_Guardar, this );


	Init_It_Imagens();

	Init_Erro_Minim();
	Init_It_Error();
	Init_Erro_Mija();
	cout << "\033[1;32mVision System ->" << idCam << ". Ready! \033[0m" << endl;

}



/************************************************
Guarda la Imagen
************************************************/
void* CE_Guardar ( void* SistVis )
{
	visionSystem *SV = (visionSystem*) SistVis;
	bufferCircular *Buffer = SV->getBuffer();

	unsigned char* imageAgravar = (unsigned char*) malloc ( sizeof(char)* WIDTH_FRAME * HEIGHT_FRAME* 2);

	while(1)
	{
		do
		{
		    usleep(1000);
		}while(Buffer->checkAvail()<=0);

		//cout << "En Cola: " << Buffer->checkAvail() << endl;

		struct timespec t1 = Inicia_Temporisador();
		char rutaGrabacion[150]="";
		int tamTotal,_w,_h, _colors;
		Buffer->remImage( imageAgravar, rutaGrabacion, tamTotal,_w,_h,_colors);
		printf("\rSaved image:  %s size %i Bytes", rutaGrabacion, tamTotal);
		fflush(stdout);
/*
		FILE *f = fopen(rutaGrabacion,"w");
		if (f == 0)
			printf("No he pogut obrir %s\n", rutaGrabacion);
		else
		{
			fwrite( imageAgravar,1,tamTotal,f);
			fclose(f);
		}*/
		if(_colors==1) {
			Mat dig(Size(_w, _h), CV_8UC1, imageAgravar);
			imwrite( rutaGrabacion, dig);
		}
		if(_colors==4) {
			Mat dig(Size(_w, _h), CV_8UC4, imageAgravar);
			Mat out;   //WIDTH_FRAME, HEIGHT_FRAME
			cvtColor(dig, out, cv::COLOR_BGR2RGB);
			imwrite( rutaGrabacion, out);
		}

		//cout << "Temps guardar: "<< Medix_temps(t1) << endl;
	}
	free(imageAgravar);

}

void visionSystem::saveImage(char direccionDeGrabacion[])
{
	buffer->addImage( quadrat, direccionDeGrabacion, WIDTH_FRAME*HEIGHT_FRAME,WIDTH_FRAME,HEIGHT_FRAME, 1 );
}

/************************************************
Fixar el Display
************************************************/
void visionSystem::setDisplay (Display* disp)
{
	display=disp;

	ImagenDisplayVector  = display->pImagenDisplayVector();
	ImagenDisplayTextura = display->pImagenDisplayTextura();

}




/************************************************
Compensated texture area set colour
************************************************/
void visionSystem::texturaColor(int Rc, int Gc, int Bc )
{

    for(int k=0; k<WIDTH_PANT*HEIGHT_PANT*4; k=k+4 )
    {

         ImagenDisplayVector[k]   = Rc;
         ImagenDisplayVector[k+1] = Gc;
         ImagenDisplayVector[k+2] = Bc;
         ImagenDisplayVector[k+3] = 255;
    }

}


/************************************************
Inicialisar Fondo
************************************************/
void visionSystem::Init_Textura(int Rc, int Gc, int Bc)
{
	for(int cnt=0; cnt<CNTA; cnt=cnt+2 )
	{
		long int posIM  = tabla_calibracion[cnt];
		long int posTEX = tabla_calibracion[cnt+1];

		ImagenDisplayVector[posTEX]   =  Rc;
		ImagenDisplayVector[posTEX+1] =  Gc;
		ImagenDisplayVector[posTEX+2] =  Bc;
		ImagenDisplayVector[posTEX+3] = 255;
	}

}



/************************************************
 Procedimiento para Calibrar Display/Cámara
************************************************/
bool visionSystem::CE_Calibrar (void)
{
	CE_Capturar();

	if(!calibrada){

		cout << "Calibrating camera id: " << idCam << " -> " << ruta_homography << endl;
		display->dibuixarPatroCalibracio(OFFSET_X, OFFSET_Y, escala);
		display->dibuixar();
		usleep(1000000);

		CE_Capturar();
		guardarTest("./Pattern", quadrat);

		// CALCULA HOMOGRAFIA (MATRIU DE CALIBRACIO)
		if( calcular_matriz_calibracion_inversa() ){
			calibrada=true;
			//break;
		}
	}

	display->colorDisplay(0,0,0,255);
	display->dibuixar();

	// CALCULA LA TABLA ASSOCIADA A LA MATRIU DE CALIBRACIO (MENYS TEMPS COMPUTACIONAL)
	if( calibrada ){
		Calculo_Tabla_Calibracion();
		Init_Textura(255,140,0);
		cout << "\033[1;32mVision System ->" << idCam << ". Calibrated! \033[0m" << endl;
	}

	return calibrada;
}




/*******************************************************************
 Calcular la Matriz de Calibracion Inversa entre Display y Cámara
********************************************************************/
bool visionSystem::calcular_matriz_calibracion_inversa( void )
{
	bool found = false;
	unsigned char* image_wb = (unsigned char*) malloc (sizeof(char)*WIDTH_FRAME*HEIGHT_FRAME);
	memset( image_wb, 255, WIDTH_FRAME*HEIGHT_FRAME);

	Size imageSize;

	vector<Point2f> imagePoints;
	vector<Point2f> displayPoints;

	Size boardSize;
	boardSize.height = 11; //7;
	boardSize.width  = 4;  //4; //3;

	float squareSize;            // The size of a square in your defined unit (point, millimeter,etc).
	squareSize = 42*escala;

	for( int cnt=0; cnt<WIDTH_FRAME*HEIGHT_FRAME; cnt++ )
		if( quadrat[cnt]>20 )
			image_wb[cnt]=0;

	guardarTest("./Pattern_BW", image_wb);

	Mat view = Mat (Size(WIDTH_FRAME,HEIGHT_FRAME), CV_8UC1, image_wb );

	//loadHomographyInverseParams();


	if (!view.empty())
	{
		imageSize = view.size();  // Format input image.

		found = findCirclesGrid(view, boardSize, imagePoints, CALIB_CB_ASYMMETRIC_GRID);
		for (int kk=0; kk<imagePoints.size();kk++) {
			imagePoints.at(kk) = imagePoints.at(kk)*3.0;
			//cout << imagePoints.at(kk) << endl;
		}

		if (found)
		{
			cout << "Circle pattern found!!! Calibrating... " << imagePoints.size() << " points" << endl;
			// Crea Patrón
			for (int i=0; i<boardSize.height ; i++)
				for ( int j=0; j<boardSize.width; j++)
					displayPoints.push_back(Point2f( OFFSET_X + (i*squareSize*texelRatio), OFFSET_Y + ( ( 2*j + i%2)*squareSize) )); // 105.191061452

			// Crea la homografía
			Mat dHiActual = findHomography(displayPoints, imagePoints);
			double displayPointsErr = this->computeReprojectionErrorsH( displayPoints, imagePoints, dHiActual, &corregir_x, &corregir_y);

			dHi=dHiActual;
			this->saveInverseHomographyParams( imagePoints, displayPoints, dHiActual, displayPointsErr);
		}
		else{
			cout << "\033[1;31mNOT calibrated!!! Calibration pattern have not found... \nPlease try to make the camera grabs the entire circle pattern. \nIn order to help you, you can check the image generated with name \"Pattern.bmp\" \nand modify \"./configuracio/p1.xml\"  \033[0m" << endl;
			loadHomographyInverseParams();
			exit(0);
		}
	}

	dHiInv=dHi.inv();
	calculoCentroDisplay();

	free(image_wb);
	return found;
}


// Calculo de Centro de Display
void visionSystem::calculoCentroDisplay(void)
{
	Mat punt = Mat(3, 1, CV_64FC1);
	punt.at<double>(0,0)= (double)972;
	punt.at<double>(1,0)= (double)972;
	punt.at<double>(2,0)= 1.0;

	Mat punt_T = dHiInv*punt;
	centroDisplay.x = (int)floor(punt_T.at<double>(0,0)/punt_T.at<double>(0,2));
	centroDisplay.y = (int)floor(punt_T.at<double>(0,1)/punt_T.at<double>(0,2));

	cout << "DISPLAY centre for camera = [" << centroDisplay.x << "," << centroDisplay.y << "]" << endl;
}


/**************************************************************************************
 * @ Convierte la Matriz de calibracion en una Tabla de Asignacion (Tabla de Calibracion)
 * @ Lo hace para aquellos texels
***************************************************************************************/
int visionSystem::Calculo_Tabla_Calibracion()
{

		int Ancho_Pant = (int)floor(ImagenDisplayTextura->GetWidth());
		int Alto_Pant  = (int)floor(ImagenDisplayTextura->GetHeight());

		Mat punt = Mat(3, 1, CV_64FC1);

		//loadHomographyInverseParams();
		cout << "Creating LookUp table..." << endl;
		int cnt = 0;

		// En función del Texel (del Display)
		for(int cnt_i=0; cnt_i<Ancho_Pant; cnt_i++ )
		{
			for(int cnt_j=0; cnt_j<Alto_Pant; cnt_j++ )
			{
				punt.at<double>(0,0)= (double)cnt_i;
				punt.at<double>(1,0)= (double)cnt_j;
				punt.at<double>(2,0)= 1.0;

				Mat punt_T = dHi*punt;

				int y = (int)floor(punt_T.at<double>(0,0)/punt_T.at<double>(0,2));
				int x = (int)floor(punt_T.at<double>(0,1)/punt_T.at<double>(0,2));

				if(x>=0 && x<HEIGHT_FRAME && y>=0 && y<WIDTH_FRAME )
				{
					long int posIM  = x*WIDTH_FRAME + y;
					long int posTEX = (cnt_j*floor(Ancho_Pant) + cnt_i)*4;

					tabla_calibracion[cnt]   = posIM;
					tabla_calibracion[cnt+1] = posTEX;

					cnt=cnt+2;
				}
			}
		}

		CNTA = cnt;
		cout << "LookUp table build, size: " << CNTA << " Bytes" << endl;

		return cnt;

}



/************************************************
 Captura imagen
************************************************/
bool visionSystem::CE_Capturar()
{
	const void* frame_data; int frame_sz;

	while( !cam->BeginReadFrame(0,frame_data,frame_sz) ) {};
	cam->EndReadFrame(0);


	if(WIDTH_HD==WIDTH_FRAME) {
		memcpy(quadrat,(unsigned char*)frame_data, frame_sz );
	}
	else {
		QUADRAT (quadrat, (unsigned char*)frame_data);
	}

}



/************************************************
Compensa la iluminación del Display
************************************************/
bool visionSystem::CE_Compensar ( void )
{

	// UNA VOLTA CALCULADA LA TABLA COMPENSA D'UNA FORMA MES EFICIENT
	if(CNTA) {
//        	TransformarImagen_Y_CompensarDisplay();
		TransformarImagen_Y_CompensarDisplay ( true, _canalRoig, _canalVert, _canalBlau  );

	}

	//guardarTest("./Text", ImagenDisplayVector, CV_8UC4, (int)floor(ImagenDisplayTextura->GetWidth()), ImagenDisplayTextura->GetHeight() );
	Sum_It_Comp();
}


/************************************************
Compensa Rápido la iluminación del Display
************************************************/
bool visionSystem::CE_Compensar_Rapido ( void )
{

	CE_Capturar();
	compensada=false;

	// UNA VOLTA CALCULADA LA TABLA COMPENSA D'UNA FORMA MES EFICIENT
	if(CNTA)
	{
		usleep(3000);

		struct timespec t1 = Inicia_Temporisador();
    while(!min_found) // --- YES
    {
			CE_Capturar();
			CE_Capturar();
 
      			TransformarImagen_Y_CompensarDisplay ( true, _canalRoig, _canalVert, _canalBlau );  // --- YES

			Sum_It_Comp();
			display->dibuixar();  // --- YES
			printf("\rFixing de control action... %i iterations, %.2f seconds", Get_It_Comp(), Medix_temps(t1));
			fflush(stdout);
		}
		//cout << endl;
		//cout << Get_It_Comp()<<"iters, Temps Compensar: "<< Medix_temps(t1) << endl;
		cout << endl << "Minimum error found. Image has been compensated!" << endl;
	}

	compensada=true;

	return min_found;
}

/************************************************
Generate a folder, path and name of image
************************************************/
void visionSystem::generadorDeRuta( char direccionDeGrabacion[], int codicImage )
{

	string sequencia = NOM_000X (codicImage);
	strcat( direccionDeGrabacion, sequencia.c_str() );
	strcat( direccionDeGrabacion, ".bmp");

	if(!existe(direccionDeGrabacion))
	{
		crear_carpeta (direccionDeGrabacion);
		//cout << "Folder generated" << endl;
	}
}


// Transformar y Compensar Imagen
int visionSystem::TransformarImagen_Y_CompensarDisplay ( bool automaticStop )
{
	long int error_total=0;
	int N = CNTA/2;
	double mija=0;

	for(int cnt=0; cnt<CNTA; cnt=cnt+2 )
	{
		long int posIM  = tabla_calibracion[cnt];
		long int posTEX = tabla_calibracion[cnt+1];

		int roig = ImagenDisplayVector[posTEX];
		int vert = ImagenDisplayVector[posTEX+1];
		int blau = ImagenDisplayVector[posTEX+2];
		//int alfa = ImagenDisplayVector[posTEX+3];

		int error = (int)(floor(0.9*( 48-quadrat[posIM])));

		int col_global= roig + vert + blau + error;

		error_total = error_total + abs(48 - quadrat[posIM]);
		mija += (double)quadrat[posIM];

		ImagenDisplayVector[posTEX]   = col_global;
		ImagenDisplayVector[posTEX+1] = col_global-255;
		ImagenDisplayVector[posTEX+2] = col_global-510;
		ImagenDisplayVector[posTEX+3] = 255;

		if (col_global-510<0)   ImagenDisplayVector[posTEX+2] =0;
		if (col_global-255<0)   ImagenDisplayVector[posTEX+1] =0;
		if (col_global<0)       ImagenDisplayVector[posTEX]   =0;
		if (col_global-510>255) ImagenDisplayVector[posTEX+2] =255;
		if (col_global-255>255) ImagenDisplayVector[posTEX+1] =255;
		if (col_global>255)     ImagenDisplayVector[posTEX]   =255;

	}
//	cout 	<< idCam << " Pixels de TABLA: " << CNTA/2
//		<< ". Bytes de textura: " << idxEnviarTextura << endl;

	float Mija_error 	= (float)( (double)error_total/(double)N);
	float Mija_error_ant	= mija_error_compensacio;

	Inc_It_Error();

	// MET = Mija Error Total
	float MET_Act = (MET_Ant*( Iteracions_Compensacio-1) + Mija_error) / Iteracions_Compensacio;
	float dMET = MET_Ant - MET_Act;
	float dM = Mija_error_ant - Mija_error;


	//if(Iteracions_Compensacio==1) Set_alpha( alpha );


	// Calcul del minim
	if( (Mija_error < minim_error_compensacio) && !min_found )
	{
		minim_error_compensacio = Mija_error;
        	//if(IteradorPrograma<3) {
			memcpy( vector_mostrar_minim_error, vector_mostrar_ant, WIDTH_PANT*HEIGHT_PANT*4); //<--------------------------------
		//}
//mijaInicial=(mija/(double)N);

        	Init_It_Error();
//        	cout << "Encontrado." << endl;

	}
//	cout << "Media: " << Mija_error <<  ", minimo: " << minim_error_compensacio << endl;

	// Bloqueig
	if( !min_found && It_Erro>15) //abs(dMET)<0.1 && abs(dM)<0.1 &&
	{
		min_found=true;
		//codicImage=0;
		Init_It_Imagens();
		//pSVx->Init_It_Imagens();
	}

	// Reset
	if( reset_comp ) //if( abs(dM)>6 && min_found )
	{
		reset_comp=false; //Set_Reset_Comp( false );
		Init_Textura(255,127,0);
		//Init_Textura_Groc();
		Init_Erro_Minim();
		Init_Erro_Mija();
		Init_It_Error();
		Init_MET_Ant(); //Mija Error Total An
		Init_It_Comp();
		min_found = false;
	}

	//cout << "Mija Ant: " << Mija_error_ant << ", Mija Actual: " << Mija_error  << ", dM: " << dM << endl;
	//cout << "MET_Ant: "  << MET_Ant << ", MET_Act: "  << MET_Act << ", dMET: "  << dMET << endl;
	//cout << "Minim      : "  << minim_error_compensacio << ", Alpha: " << alpha << endl;

	MET_Ant = MET_Act;
	mija_error_compensacio = Mija_error;


	//mostrar_tex->SetPixels( vector_mostrar_ant );  ImagenDisplayTextura
	if ( !min_found ) mostrar_tex.SetPixels( vector_mostrar_ant );
	if (  min_found )
	{
		mostrar_tex.SetPixels( vector_mostrar_minim_error );
		cout << "Bloqueado" << endl;
	}

	return min_found;
}

void visionSystem::reconfigurarCamara()
{
	MMAL_COMPONENT_T *cam1 = cam->get_CameraComponent();
	StopCamera();
	raspicamcontrol_set_shutter_speed(cam1, 140000);
	raspicamcontrol_set_brightness(cam1, 25);
	cam = StartCamera( WIDTH_HD, HEIGHT_HD, 7,1,false );

}



/**********************************************************************
Compensación con 4 cámaras
***********************************************************************/
int visionSystem::TransformarImagen_Y_CompensarDisplay ( void )
{
	long int error_total=0;


	for(int cnt=0; cnt<CNTA; cnt=cnt+2 )
	{
		long int posIM  = tabla_calibracion[cnt];
		long int posTEX = tabla_calibracion[cnt+1];

		int roig = ImagenDisplayVector[posTEX  ];
		int vert = ImagenDisplayVector[posTEX+1];
		int blau = ImagenDisplayVector[posTEX+2];
		//int alfa = ImagenDisplayVector[posTEX+3];

		int error = (int)(floor(0.9*( 48-quadrat[posIM])));

		int col_global= roig + vert + blau + error;

		error_total = error_total + abs( 48-quadrat[posIM]);

		ImagenDisplayVector[posTEX]   = col_global;
		ImagenDisplayVector[posTEX+1] = col_global-255;
		ImagenDisplayVector[posTEX+2] = col_global-510;
		ImagenDisplayVector[posTEX+3] = 255;

		if (col_global-510<0)   ImagenDisplayVector[posTEX+2] =0;
		if (col_global-255<0)   ImagenDisplayVector[posTEX+1] =0;
		if (col_global<0)       ImagenDisplayVector[posTEX]   =0;
		if (col_global-510>255) ImagenDisplayVector[posTEX+2] =255;
		if (col_global-255>255) ImagenDisplayVector[posTEX+1] =255;
		if (col_global>255)     ImagenDisplayVector[posTEX]   =255;

	}

    return false;
}


// Transformar y Compensar Imagen
int visionSystem::TransformarImagen_Y_CompensarDisplay ( bool automaticStop, bool Roig, bool Vert, bool Blau  )
{
	long int error_total=0;
	int N = CNTA/2;

	double MODA[256]={0};
	double F_RELATIVAS[256]={0};
	double mija_Eixida=0;

	int ref = 48;

	for(int cnt=0; cnt<CNTA; cnt=cnt+2 )
	{
		long int posIM  = tabla_calibracion[cnt];
		long int posTEX = tabla_calibracion[cnt+1];

		int roig = ImagenDisplayVector[posTEX  ];
		int vert = ImagenDisplayVector[posTEX+1];
		int blau = ImagenDisplayVector[posTEX+2];
		//int alfa = ImagenDisplayVector[posTEX+3];

		int error = (int)(floor(0.9*( ref-quadrat[posIM])));

		int col_global= roig + vert + blau + error;
		MODA[quadrat[posIM]]++;
		mija_Eixida+=quadrat[posIM];
		error_total = error_total + abs(ref - quadrat[posIM]);

		ImagenDisplayVector[posTEX]   = col_global;
		ImagenDisplayVector[posTEX+1] = col_global-255;
		ImagenDisplayVector[posTEX+2] = col_global-510;
		ImagenDisplayVector[posTEX+3] = 255;

		if (col_global-510<0)   ImagenDisplayVector[posTEX+2] =0;
		if (col_global-255<0)   ImagenDisplayVector[posTEX+1] =0;
		if (col_global<0)       ImagenDisplayVector[posTEX]   =0;
		if (col_global-510>255) ImagenDisplayVector[posTEX+2] =255;
		if (col_global-255>255) ImagenDisplayVector[posTEX+1] =255;
		if (col_global>255)     ImagenDisplayVector[posTEX]   =255;

	}
//	cout 	<< idCam << " Pixels de TABLA: " << CNTA/2
//		<< ". Bytes de textura: " << idxEnviarTextura << endl;
/*
	float Mija_error 	= (float)( (double)error_total/(double)N);
	float Mija_error_ant	= mija_error_compensacio;

	Inc_It_Error();

	// MET = Mija Error Total
	float MET_Act = (MET_Ant*( Iteracions_Compensacio-1) + Mija_error) / Iteracions_Compensacio;
	float dMET = MET_Ant - MET_Act;
	float dM = Mija_error_ant - Mija_error;


	//if(Iteracions_Compensacio==1) Set_alpha( alpha );


	// Calcul del minim
	if( (Mija_error < minim_error_compensacio) && !min_found )
	{
		minim_error_compensacio = Mija_error;
        	memcpy( vector_mostrar_minim_error, vector_mostrar_ant, 480*431*4);
        	Init_It_Error();
//        	cout << "Encontrado." << endl;

	}
//	cout << "Media: " << Mija_error <<  ", minimo: " << minim_error_compensacio << endl;

	// Bloqueig
	if( !min_found && It_Erro>15) //abs(dMET)<0.1 && abs(dM)<0.1 &&
	{
		min_found=true;
		//codicImage=0;
		Init_It_Imagens();
		//pSVx->Init_It_Imagens();
	}

	// Reset
	if( reset_comp ) //if( abs(dM)>6 && min_found )
	{
		reset_comp=false; //Set_Reset_Comp( false );
		Init_Textura(255,127,0);
		//Init_Textura_Groc();
		Init_Erro_Minim();
		Init_Erro_Mija();
		Init_It_Error();
		Init_MET_Ant(); //Mija Error Total An
		Init_It_Comp();
		min_found = false;
	}

	//cout << "Mija Ant: " << Mija_error_ant << ", Mija Actual: " << Mija_error  << ", dM: " << dM << endl;
	//cout << "MET_Ant: "  << MET_Ant << ", MET_Act: "  << MET_Act << ", dMET: "  << dMET << endl;
	//cout << "Minim      : "  << minim_error_compensacio << ", Alpha: " << alpha << endl;

	MET_Ant = MET_Act;
	mija_error_compensacio = Mija_error;


	//mostrar_tex->SetPixels( vector_mostrar_ant );  ImagenDisplayTextura
	if ( !min_found ) mostrar_tex.SetPixels( vector_mostrar_ant );
	if (  min_found )
	{
		mostrar_tex.SetPixels( vector_mostrar_minim_error );
		cout << "Bloqueado" << endl;
	}

	return min_found;
*/
	float Mija_error 	= (float)( (double)error_total/(double)N);
	float Mija_error_ant	= mija_error_compensacio;

	It_Erro++;

	// Calcular les freqüencies Relatives
	for(int i=0; i<256; i++ ) F_RELATIVAS[i] = MODA[i]/N*100.0;

	// Calcular l'error
	double errorHistograma[256]={0.0};
	for(int i=0; i<256; i++ ) errorHistograma[i] = H_Referencia[i] - F_RELATIVAS[i];

	// Error quadratic
	double errorQuadratic[256]={0.0};
	for(int i=0; i<256; i++ ) errorQuadratic[i] = errorHistograma[i]*errorHistograma[i];

	// Error quadratic mig
	double errorQuadraticMig=0;
	for(int i=0; i<256; i++ ) errorQuadraticMig = errorQuadraticMig + errorQuadratic[i];
	errorQuadraticMig = errorQuadraticMig/100.0;

	// Calcul de la derivada de la funcio de l'error quadratic mig
	int derv_error = (int)(errorQuadraticMig_Anterior - errorQuadraticMig);
	errorQuadraticMig_Anterior = errorQuadraticMig;

mija_Eixida = mija_Eixida/(double)N;
//cout << It_Erro << "Y: " << mija_Eixida << " · error: " << Mija_error << " · derv_error: " << derv_error << endl;
//

//ficher_analisis << Iteracions_Compensacio << ", " << modaFondo;
//for(int i=0; i<256; i++) ficher_analisis << " " << F_RELATIVAS[i];
//ficher_analisis << "]" << endl;
//ficher_analisis << endl;

//ficher_analisis << (float)mija_Eixida << ", " << Mija_error << ", " << derv_error << endl;
///-----------
	// Calcul del minim
	if( ( derv_error == 0) && !min_found )
	{
		minim_error_compensacio = Mija_error;
        	//memcpy( vector_mostrar_minim_error, vector_mostrar_ant, 480*431*4);
if(IteradorPrograma<3)
		memcpy( vector_mostrar_minim_error, ImagenDisplayVector, WIDTH_PANT*HEIGHT_PANT*4);

mijaInicial=mija_Eixida;
        	//Init_It_Error();
	}
	else {
		It_Erro=0;
	}

	// Bloqueig
	if( !min_found && It_Erro>8)
	{
		min_found=true;
		Init_It_Imagens();
	}

	// Reset
	if( reset_comp )
	{
		resetCompensation();
	}


	//if ( !min_found ) mostrar_tex.SetPixels( vector_mostrar_ant );
	if (  min_found )
	{
		//mostrar_tex.SetPixels( vector_mostrar_minim_error );
		//cout << "Minimum error found. Image has been compensated!" << endl;
	}

	return min_found;
}

// Reset the compesation
void visionSystem::resetCompensation()
{
	reset_comp=false;
	Init_Textura(255,140,0);
	//Init_Textura_Groc();
	Init_Erro_Minim();
	Init_Erro_Mija();
	Init_It_Error();
	Init_It_Comp();
	min_found = false;
	display->dibuixar();
}



/**************************************************************************
Guarda una image
***************************************************************************/
void visionSystem::guardarTest(const char* nombre, unsigned char* im)
{

	char rutaIm[100];
	strcpy(rutaIm, nombre);
	string identificador1 = int2string(idSV);
	string identificador2 = int2string(idCam);

	strcat( rutaIm, identificador1.c_str() );
	strcat( rutaIm, identificador2.c_str() );

	strcat(rutaIm, ".bmp");
	Mat dig(Size(WIDTH_FRAME, HEIGHT_FRAME), CV_8UC1, im);
	imwrite( rutaIm, dig);

}



/*************************************************************************************************************
	Calcular los errores de proyección
**************************************************************************************************************/
double visionSystem::computeReprojectionErrorsH(const vector<Point2f>& imagePoints,
	const vector<Point2f>& displayPoints, const Mat& dHiActual, float* corregir_x, float* corregir_y)
{
	double totalErr = 0, err;
	double dx[44], dy[44];
	double dxmax=0, dymax=0, dxmin=10000, dymin=10000;

	int tamany = (int)imagePoints.size();
	vector<Point2f> displayPointsEst(tamany);

	//perspectiveTransform(to be transformed, output, dHiActual);
	perspectiveTransform(imagePoints, displayPointsEst, dHiActual);

	char ruta[100]="./analisis/errores_data";
	string ID = int2string(idCam);
	strcat( ruta, ID.c_str() );
	strcat( ruta,".xml");


	FileStorage fs(ruta, FileStorage::WRITE);
	for (int kk=0; kk < tamany; kk++) {
		dx[kk] = displayPointsEst[kk].x - displayPoints[kk].x;
		dy[kk] = displayPointsEst[kk].y - displayPoints[kk].y;
		//err = sqrt((dx*dx)+(dy*dy));
		fs << "errorx" << dx[kk];
		fs << "errory" << dy[kk];
		*corregir_x += (float)dx[kk];
		*corregir_y += (float)dy[kk];
	}

	for(int cnt=0; cnt<21; cnt++) if( dx[cnt]>dxmax ) dxmax = dx[cnt];
	for(int cnt=0; cnt<21; cnt++) if( dy[cnt]>dymax ) dymax = dy[cnt];

	for(int cnt=0; cnt<21; cnt++) if( dx[cnt]<dxmin ) dxmin = dx[cnt];
	for(int cnt=0; cnt<21; cnt++) if( dy[cnt]<dymin ) dymin = dy[cnt];

	*corregir_x = *corregir_x/28;
	fs << "corregit_x" << *corregir_x;

	*corregir_y = *corregir_y/28;
	fs << "corregit_y" << *corregir_y;

	fs << "dxmax" << dxmax;
	fs << "dymax" << dymax;
	fs << "dxmin" << dxmin;
	fs << "dymin" << dymin;

	fs.release();

	err = norm(displayPoints, displayPointsEst, NORM_L2);
	totalErr = err*err;
	return std::sqrt(totalErr / tamany);
}


/********************************************************************************
Guarda la Homografia Inversa en un archivo
*********************************************************************************/
void visionSystem::saveInverseHomographyParams(vector<Point2f> displayPoints, vector<Point2f> imagePoints, Mat dHi, double displayPointsErr)
{
	char rutaGravar[150]="";
	strcpy ( rutaGravar, ruta_homography );

	cout << "Saving calibration data in... "<< rutaGravar << endl;
	FileStorage fs(rutaGravar, FileStorage::WRITE);

       if (!displayPoints.empty())
       {
             int tamany = (int)displayPoints.size();
             Mat displayPtMat(2, tamany, CV_32FC1);
             for (size_t i = 0; i < tamany; i++)
             {
                    displayPtMat.at<float>(0, i) = displayPoints[i].x;
                    displayPtMat.at<float>(1, i) = displayPoints[i].y;
             }
             fs << "displayPoints" << displayPtMat;
       }

       if (!imagePoints.empty())
       {
             int tamany = (int)imagePoints.size();
             Mat imagePtMat(2, tamany, CV_32FC1);
             for (size_t i = 0; i < tamany; i++)
             {
                    imagePtMat.at<float>(0, i) = imagePoints[i].x;
                    imagePtMat.at<float>(1, i) = imagePoints[i].y;
             }
             fs << "image_points" << imagePtMat;
       }

       fs << "dHi" << dHi;
       fs << "displayPoints_reprojection_error" << displayPointsErr;

}




/********************************************************************************
Guarda la Homografia Inversa en un archivo
*********************************************************************************/
void visionSystem::savePatron (Mat Mpatron)
{

	//cout << "Guardando datos de patrón en... "<< ruta_patron << endl;

	FileStorage fs(ruta_patron, FileStorage::WRITE);
	fs << "Mpatron" << Mpatron;


}


/********************************************************************************
Carga la Homografia Inversa
*********************************************************************************/
void visionSystem::loadHomographyInverseParams(void)
{

	if(!existe(ruta_homography))
		cout << "No existe archivo: " << ruta_homography << endl;
	else{
		//cout << "Cargando datos de calibracion... " << ruta_homography << endl;
		FileStorage fs(ruta_homography, FileStorage::READ);
		fs["dHi"] >> dHi;
		fs["displayPoints_reprojection_error"] >> erroCalibracio;
	}

}




/********************************************************************************
Guarda el patrón
*********************************************************************************/
void visionSystem::savePatron (void)
{
	//cout << "Guardando datos de patrón en... "<< ruta_patron << endl;

	FileStorage fs(ruta_patron, FileStorage::WRITE);
	fs << "OFFSET_X" << OFFSET_X;
	fs << "OFFSET_Y" << OFFSET_Y;
	fs << "escala"   << escala;
}


/********************************************************************************
Carga el patrón
*********************************************************************************/
void visionSystem::loadPatron(void)
{
	if(!existe(ruta_patron))
		cout << "Does not exist: " << ruta_patron << endl;

	else {
		FileStorage fs(ruta_patron, FileStorage::READ);
		fs["OFFSET_X"] >> OFFSET_X;
		fs["OFFSET_Y"] >> OFFSET_Y;
		fs["escala"]   >> escala;
		cout << "Loading pattern... [" << OFFSET_X << "," << OFFSET_Y << "] in " << ruta_patron << endl;
	}

}
