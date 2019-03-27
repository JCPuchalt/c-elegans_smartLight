#pragma once

#include "../headers/llibreries.hpp"



// SEGMENTA LA IMAGEN EN BW, Y ENCUENTRA EL CENTROIDE DE TODOS LOS PUNTOS EN BLANCO
void find_white_pixels(unsigned char* image, int ample, int alt, ofstream& centroide, ofstream& punt_image, float pantalla_x, float pantalla_y)
{

	float error_x = 0.0, error_y = 0.0;
	float yt=0.0, xt=0.0, N=0.0, Centro_x, Centro_y;
	int tamany = ample*alt;

	for(int cnt=0; cnt<tamany; cnt++)
	{	//UMBRAL 20
		if(image[cnt]> 20)
		{
			image[cnt]=255;
			float y = floor(cnt/ample);
			float x = cnt - (y*ample);
			yt+=y;
			xt+=x;
			N++;
		}
		else image[cnt]=0;
	}
	if(N!=0)
	{	//CALCULA CENTROIDE
		Centro_x = floor(xt/N);
		error_x  = Centro_x - (float)pantalla_x;
		Centro_y = floor(yt/N);
		error_y  = Centro_y - (float)pantalla_y;
	}


	if(Centro_x>0 && Centro_x<WIDTH_FRAME && Centro_y>0 && Centro_y<HEIGHT_FRAME)
	{
		if (centroide.is_open())
			centroide << "[" << Centro_x   << " " << Centro_y   << " 1]' " << endl;
				//  << error_x    << ", " << error_y    << endl;
		else cout << "Unable to open file";


		if (punt_image.is_open())
			punt_image << "[" << pantalla_x << " " << pantalla_y << " 1]' " << endl;
		else cout << "Unable to open file";
	}

}



/*********************************************************
 Reduce la imagen cuadrada a un círculo
*********************************************************/
int CIRCUL (unsigned char* circul, unsigned char* image)
{
	//cout << "Creando círculo" << endl;
	int ANCHO = 1944; // 2592 // HEIGHT_FRAME; //2464 // 1944
	int ALTO =  1944; // 2592 // HEIGHT_FRAME; //2464 // 1944
	int x0 = ANCHO/2; int y0=ALTO/2; int r=ALTO/2; //r=972;
	int xs,cx,k,N,N1,XN,Xc;
	int ni=0; int r2=pow(r,2);

	// Generar Circul de Visio
	N = y0-r;
	if(N<0) N=0;
	k = N*ANCHO;
	for( int j=0; j<ALTO; j++ )
	{
		xs=(int)floor(x0-sqrt(r2-pow(r-j,2)));
		cx=(int)floor(abs(sqrt(r2 - pow(r-j,2))));
		N=j*ANCHO;

		N1=N+ANCHO;
		XN=N+xs;
		Xc=XN+2*cx;
		memcpy(&circul[ni], &image[XN], Xc-XN);
		ni = ni +Xc-XN;
	}
	//cout << "Número de bytes: " << ni << endl;
	return ni;

}



/********************************************************************************
Genera una image quadrada a partir d'una rectangular (format vector de char)
*********************************************************************************/
int QUADRAT (unsigned char* quadrat, unsigned char* image)
{
	int D=(WIDTH_HD-WIDTH_FRAME)/2, ni=0;

	for( int j=0; j<HEIGHT_HD; j++ )
	{
		memcpy(&quadrat[ni], &image[j*WIDTH_HD+D], HEIGHT_HD);
		ni = ni+HEIGHT_HD;
	}

	return ni;
}


/*
/ @ provabilitat en un punt
/ @ x; la variable
/ @ mhu; La Mija
/ @ sigma=0.2; Desviacio (sigma2=0.2|90%) (sigma2=0.5|55%) (sigma2=1.0|40%) (sigma2=5.0|20%)
*/
float gaussian_pdf (float x, float mhu, float sigma)
{
	float C = 1/(sqrt(2*M_PI*sigma));
	float a = (x-mhu)/sigma;
	float F = C*exp(-0.5*a*a);

	return F;
}

/*
/ @ provabilitat acumulada
/ @ Esta s'ha de sumar
/
*/
float gaussian_acumulada (float x, float mhu, float sigma)
{
	float Z = (x-mhu)/sigma;
	float C = 1/(sqrt(2*M_PI));
	float F = C*exp(-0.5*Z*Z);

	return F;
}


/***************************************************
De Entero a String
****************************************************/
string int2string (int sancer)
{
	stringstream converter;
	converter.str("");
	converter << sancer;
	string convertit = converter.str();

	return convertit;
}

/***************************************************
De Float a String
****************************************************/
string float2string (float sancer)
{
	stringstream converter;
	converter.str("");
	converter << sancer;
	string convertit = converter.str();

	return convertit;
}

/*******************************************************
Convierte un entero a string y le añade 0s ->  000001
********************************************************/
string NOM_000X ( int sequencia)
{
	string NOM1   = int2string(sequencia);

	if(sequencia>=    0 && sequencia<     10) NOM1 = "00000" + NOM1;
	if(sequencia>     9 && sequencia<    100) NOM1 = "0000" + NOM1;
	if(sequencia>    99 && sequencia<   1000) NOM1 = "000" + NOM1;
	if(sequencia>   999 && sequencia<  10000) NOM1 = "00" + NOM1;
	if(sequencia>  9999 && sequencia< 100000) NOM1 = "0" + NOM1;

	return NOM1;
}


const char* itoa( int entero )
{
	char cadena[300];

	string cadenaString = int2string(entero);
	for(int k = 0; k < cadenaString.size() ; k++) cadena[k]=cadenaString[k];

	return cadena;
}


/***************************************************
De Matriz a array
****************************************************/
bool mat2array ( char* matriuEnviar, Mat matriu, int m, int n)
{
	string array="";
	for (int iCNT=0; iCNT<m; iCNT++)
	{
		for (int jCNT=0; jCNT<n; jCNT++)
		{
			string insertar;
			double valor = (double) matriu.at<double>(iCNT,jCNT);
			double2string(&insertar, valor);
			//cout << insertar << endl;
			array.append(insertar);
		}
	}

	int tam = array.size();
	string2array( matriuEnviar, array);

	return true;
}

/***************************************************
De double a string
****************************************************/
bool double2string (string *array, double valor)
{
	stringstream converter;
	converter.str("");
	converter << scientific << setprecision(16) << valor;

	string convertit;
	if(valor>=0) *array = " " + converter.str();
	else *array = converter.str();

	return true;
}


/***************************************************
De string a array
****************************************************/
bool string2array ( char *array , string strng)
{
	for(int cnt=0; cnt < strng.size(); cnt++) {
		array[cnt] = strng[cnt];
	}
	array[strng.size()]='\0';

	return true;
}


/***************************************************
De array a matriz
****************************************************/
bool array2matriu( Mat *matriuOut, char matriuRebuda[], int m, int n )
{
	Mat matriu = Mat(Size(m, n), CV_64FC1);

	int precisio =16;
	int tamDouble = precisio + 7;
	int tam = tamDouble*m*n;
	char valor[tamDouble+1]="";
	int it=0;
	double valorDouble=0.0;

	int iCNT=0, jCNT=0;

	for (int cnt=0; cnt < tam; cnt++)
	{
		valor[it] = matriuRebuda[cnt];
		it++;
		if(it >= tamDouble) {
			//valor[it]='\0';
			array2double ( &valorDouble, valor, precisio);
			matriu.at<double>(iCNT,jCNT) = (double)valorDouble;
			it=0;
			jCNT++;
			if(jCNT>=m)
			{
				jCNT=0;
				iCNT++;
			}
		}
	}

	*matriuOut = matriu;
	//cout << matriu << endl;

	return true;
}


/***************************************************
De array a double
****************************************************/
bool array2double (double* valor, char array[], int precisio)
{
	double vDoble;

	string doble(array);

	stringstream converter;
	converter.str("");
	converter << doble;
	converter.precision(precisio);

	converter >> scientific >> setprecision(precisio) >> vDoble;

	*valor = vDoble;
	//cout.precision(16);
	//cout << vDoble << scientific << endl;

	return true;
}

/***************************************************
Enviar image al Servidor
****************************************************/
int ENVIAR_IMAGE (char* image, int sock, int TAM )  //const uint8_t* image
{
	int enviat;
	char FI[5]="0";
	int ni=TAM;

	// Enviar l'image al servidor
	if( (enviat = send(sock, (unsigned char*)image, ni, 0)) ==-1 ) goto FINISH;

	// Rebre una ultima instruccio del Servidor
	if(recv(sock,FI,sizeof(FI),0)==-1) goto FINISH;

	return 1;

FINISH:
        return -1;

}

/*********************************************************
I2C. Recibe la temperatura del sensor por el puerto: I2C
**********************************************************/
float temp(int fd)
{
	int menut, gran, data, j=0;
	float temperatura;

	if(fd==-1)
	{
        	return -1;
	}
	else
	{
	        data=wiringPiI2CReadReg16(fd,0x05);

	        if(data==-1) mvprintw(12,0,"No se reciben datos"); //printf("No se reciben datos\n");
	        else
	        {
	        	gran=data & 0x00FF;
			gran=(gran&0x1F);
			menut=(data & 0xFF00) >> 8;
			menut=(menut & 0xFC);
			if ((gran&0x10)==0x10) printf("NEGATIVO");
			else
			{
				temperatura=gran*16+menut/(float)16;
				j++;
			}
		}

	}
	//close(fd);
	return temperatura;
}

/*********************************************************
Hora Actual
**********************************************************/
//Hora Actual
void Hora_Actual(char buf[])
{
	//time_t     now = time(0);
	time_t rawtime;
	struct tm *timeinfo;
	//struct tm  tstruct;
	//char       buf[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//tstruct = *localtime(&now);
	//strftime(buf, sizeof(buf), "%d/%m/%Y - %X", &tstruct);
	strftime(buf, sizeof(buf), "%R", timeinfo);
}

/*********************************************************
Temps. Dos funcions: Iniciar y medir
**********************************************************/
//Iniciar
struct timespec Inicia_Temporisador(void)
{
	struct timespec t0;
	clock_gettime(CLOCK_REALTIME, &t0);

	return t0;
}

//Medir
double Medix_temps (struct timespec t0 )
{
	struct timespec tf;
	clock_gettime(CLOCK_REALTIME, &tf);

	double temps = 	(double)(tf.tv_sec  - t0.tv_sec) + (double)(tf.tv_nsec - t0.tv_nsec)/1.0e9;

	return temps;
}


// -----FONDO GUARDAT (YA DEPRES) -------------------
void FONDO_GUARDAT(char* ruta3, float* param_gauss)
{
	FILE * pFile;
	long lSize;

	size_t result;

	if((pFile = fopen ( ruta3 , "rb" ))!=NULL)
	{
		if (pFile==NULL) {fputs ("File error",stderr); } //exit (1);

		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);

		if (param_gauss == NULL) {fputs ("Memory error",stderr); } //exit (2);

		result = fread (param_gauss ,1,lSize,pFile);
		if (result != lSize) {fputs ("Reading error",stderr); } //exit (3);

	}
	fclose(pFile);
}



// ---------------------------------------------------------------------------------------------------------
void GET_GAUSS(float * P, char* rut )
{
	ifstream file ( rut );
	string value;
	float p;
	int i=0;
	while ( file.good() )
	{
		getline ( file,value, '\n' );
		P[i] = strtof(value.c_str(),0);
		cout << P[i] << endl;
		i++;
	}
	file.close();

}


// ------------------------------------------------------------------------------------------------------------------

void CALCUL_MIJA_DESVIACIO (unsigned char* buffer_k, int N_Experiencies, int* N, float* Mig, float* param_gauss, int lSize, unsigned char* Min_Max, int k, int Min, int Max )
{

	int pixel =0, n;

	//CALCUL MAXIMS i MINIMS
	if(k<N_Experiencies/5)
	{
		int valor_pixel;
		pixel = 0;
		while( pixel<lSize )
		{
			valor_pixel = buffer_k[pixel];
			if(valor_pixel<Min_Max[Min+pixel]) Min_Max[Min+pixel]=valor_pixel;
			if( valor_pixel > Min_Max[Max+pixel] )	Min_Max[Max+pixel]=valor_pixel;
			pixel++;
		}
	}

	//CALCUL MIJA_UMBRAL
	if(k==N_Experiencies/5)
	{
		pixel = 0;
		while( pixel<lSize )
		{
			Mig[pixel] = (Min_Max[Min+pixel]+Min_Max[Max+pixel])*0.6; //0.5 o 0.6 o 0.7...
			pixel++;
		}

	}

	//EXPERIENCIA
	if(k>N_Experiencies/5)
	{
		int valor_pixel;
		pixel = 0;
		while( pixel<lSize )  //*(pa+n))
		{
			valor_pixel = buffer_k[pixel];
			//if(valor_pixel>Mig[pixel]) ---------------------------------------
			{
				N[pixel]++;
				n = N[pixel];
				param_gauss[pixel+lSize] = (float)(param_gauss[pixel+lSize]*(n-1) + valor_pixel)/n;//CALCUL DE LA MIJA
				param_gauss[pixel] = param_gauss[pixel] + pow(valor_pixel,2); 	//CALCUL DESVIACIO TIPICA
			}
			pixel++;
		}
	}


	/*
	//MOSTRAR PER PANTALLA
	Mat original(Size(ample,alt), CV_8UC1,buffer_k);
	char* window_name = "Proces";
	namedWindow(window_name,0 );
	moveWindow(window_name, 0, 0);
	imshow( window_name, original );  //digital
	cvWaitKey(15); */

}



// -----------------------------------------------------------------------------------------------

int COMPRESSIO (unsigned char* comprimit, unsigned char* digital1, int canvis, int tam_comp )
{
	unsigned int j1=0;
	int dr=0;
	int cnt=0;
	while(j1<tam_comp)
	{
		if (digital1[j1] == 255) j1++;
		else
		{
			comprimit[dr] = (int)(j1 & 16711680)>>16;    //dr+2 8323072
			comprimit[dr+1] = (int)(j1 & 65280)>>8;      //dr+1
			comprimit[dr+2] = (int)(j1 & 255);           //dr+0
			comprimit[dr+4] = digital1[j1];         //dr+4

			cnt=1;
			j1++;
			while(digital1[j1] != 255 && cnt<255)
			{
				comprimit[dr+4+cnt] = digital1[j1];
				j1++;
				cnt++;
			}
			//j1++;
			comprimit[dr+3] = cnt;
			//cout << " T: " << (int)comprimit[dr+3] << endl;
			dr = dr + (3+1+cnt);  //tam_comp2 = tam_comp2 + (3+1+cnt);
			//cout << (bitset<8>)comprimit[dr-3-cnt] << "-" << (bitset<8>)comprimit[dr-2-cnt] << "-" << (bitset<8>)comprimit[dr-1-cnt] << "-" << (int)comprimit[dr-cnt] << endl;
			//sleep(1);
		}
		//cout << "Num Segment: " << segment << "Tam Segment: " << cnt << "Tam Total: " << tam_comp2 << endl;
	}

	//cout << "Pixels variados: " << canvis << ", Tam Total Comprimido: " << dr << endl;

	/*
	if(tam_comp2 > ni)
	{
		tam = ni;           // El tamany de la capçalera de l'image
		memcpy(send_data, actual, tam );
	}
	else
	{
		tam = tam_comp2;     // El tamany de la capçalera de l'image
		memcpy(send_data, comprimit, tam );
	} */
	return dr;
}

// ----------------------------------------------------------------------------------------

void DESCOMPRESSIO ( unsigned char* digital1, unsigned char* comprimit, int tam_comp, int tam_comp2 )
{
	// Descompressio
	memset ( digital1, 255,sizeof(char)*tam_comp );
	unsigned int cabecera=0;
	//j1=0;
	//segment=0;
	int dr=0;
	int cnt=0;
	//tam_comp2=0;
	while(dr<tam_comp2)
	{
		cabecera =  (unsigned int)((comprimit[dr] & 255)<<16) + (unsigned int)((comprimit[dr+1] & 255)<<8) + (unsigned int)(comprimit[dr+2] & 255);
		//cout << "Cabecera: " << cabecera << ", Num Bts: " << (int)comprimit[dr+3] << endl;
		//sleep(1);
		cnt=0;
		while(cnt<=(int)comprimit[dr+3])
		{
			digital1[cabecera+cnt] = comprimit[dr+4+cnt];
			cnt++;
		}
		dr=cnt+3+dr;
		//cout << dr << endl;
	}
	//cout << "Tam Total Descomprimido: " << tam_comp2 << endl;

}

// -----------------------------------------------------------------------------------

void RECONSTRUIR_CIRCUL (unsigned char* digital2, unsigned char* digital1, int ample, int alt)
{
	int r=972, y0=972;
	int xs,cx,k2=0,NUM, Xc, XN, N1,j,i1, x0=1296, r2=pow(r,2);
	NUM = y0-r;
	if(NUM<0) NUM=0;
	i1 = NUM*ample;
	for( j=0; j<alt; j++ )
	{
		xs=(int)floor(x0-abs(sqrt(r2-pow(r-j,2))));
		cx=(int)floor(abs(sqrt(r2 - pow(r-j,2))));
		NUM=j*ample;
		N1=NUM+ample;
		XN=NUM+xs;
		Xc=XN+2*cx;

		//for(i1=i1; i1<XN; i1++) data[i1] = 255;
		memset ( &digital2[i1], 255, XN-i1 );
		i1 = XN -1;
		memcpy(&digital2[i1],&digital1[k2],Xc-XN);
		i1=i1+Xc-XN;
		k2=k2+Xc-XN;
		//for(i=Xc; i<N1; i++)	data[i] = 255;
		memset ( &digital2[i1], 255, N1-i1 );
		i1 = N1 - 1;
	}
	//printf("\nTamany de mensage reconstruit:  %i\n",i1);
}

// ---- ---------------------------------------------------------------------

void MOSTRAR_IMAGE (unsigned char* digital2, int ample, int alt, int colors)
{
	if(colors==1)
	{
		Mat dig(Size(ample,alt), CV_8UC1, digital2);
		char* window_name = "Proces";
		namedWindow(window_name,0 );
		moveWindow(window_name, 0, 0);
		imshow( window_name, dig );
		//cvWaitKey(15);
	}
	if(colors==3)
	{
		Mat dig(Size(ample,alt), CV_8UC3, digital2);
		char* window_name = "Proces";
		namedWindow(window_name,0 );
		moveWindow(window_name, 0, 0);
		imshow( window_name, dig );
		//cvWaitKey(15);
	}
	if(colors==4)
	{
		Mat dig(Size(ample,alt), CV_8UC4, digital2);
		char* window_name = "Proces";
		namedWindow(window_name,0 );
		moveWindow(window_name, 0, 0);
		imshow( window_name, dig );
		//cvWaitKey(15);
	}
}

/*********************************************************
Genera un nom d'archiu a partir d'un conte sequencial
**********************************************************/
void GEN_NOM ( int numero, char* nombre )
{

	stringstream conversora4;
	conversora4.str("");
	conversora4 << numero;
	string NOM1 = conversora4.str();

	if(numero>=    0 && numero<        10) NOM1 = "00000" + NOM1 + ".bmp";
	if(numero>      9 && numero<      100) NOM1 = "0000" + NOM1   + ".bmp";
	if(numero>    99 && numero<    1000) NOM1 = "000" + NOM1     + ".bmp";
	if(numero>  999 && numero<  10000) NOM1 = "00" + NOM1       + ".bmp";
	if(numero>9999 && numero<100000) NOM1 = "0" + NOM1         + ".bmp";

	int kt = NOM1.size();
	for(int k = 0; k < kt ; k++) nombre[k]=NOM1[k];

}




/***********************************************************************
Funciones para crear carpetas
************************************************************************/
bool existe(char nombre[])
{
    struct stat datos_proyecte;
    if( lstat(nombre, &datos_proyecte)!=-1 ) return 1;
    else return 0;

}

string C2S(char i)
{
    ostringstream ss;
    ss << i;
    return ss.str();
}

string AC2S(char i[])
{
    ostringstream ss;
    ss << i;
    return ss.str();
}



void generar_carpeta (char *generada1, char *generada2, char nombre[], int dia, char condicion, int probeta )
{

    string generar1 = AC2S(nombre) + "/dia_"+ I2S(dia) + "/cond_" + C2S(condicion) + "/placa_" + I2S(probeta)  + "/" + '\0';
    string generar2 = AC2S(nombre) + "/dia_"+ I2S(dia) + "/cond_" + C2S(condicion) + "/placa_" + I2S(probeta+1)+ "/" + '\0';

    for(int k=0; k<generar1.size(); k++) generada1[k]=generar1[k];
    for(int k=0; k<generar2.size(); k++) generada2[k]=generar2[k];

}

void generar_ruta_completa (char *ruta_completa, char raiz[], char generada[])
{
    string generar = AC2S(raiz) + AC2S(generada);
    for(int k=0; k<generar.size(); k++) ruta_completa[k]=generar[k];
}

bool crear_carpeta (char generada[])
{
    char crea[150]="";
    int ki= 0;
    for (int k=0; generada[k]!='\0'; k++)
    {
        crea[k]= generada[k];

        if(generada[k]=='/')
        {
            crea[k]= '\0';
            mkdir(crea, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            crea[k]= generada[k];
        }
    }
    return true;
}

string I2S(int i)
{
    ostringstream ss;
    ss << i;
    return ss.str();
}

string F2S(float i)
{
    ostringstream ss;
    ss << i;
    return ss.str();
}
