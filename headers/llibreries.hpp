#pragma once

//#include <stdio.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/stitching.hpp>
#include <iomanip>

#include "visionSystem.hpp"
#include "camera.h"
#include "graphics.h"
#include "CE_Funcions.hpp"
#include "bufferCircular.hpp"
#include "Display.hpp"

#include <sys/time.h>
#include <curses.h>
#include <ncurses.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <resolv.h>
#include <bitset>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
//#include <thread>
#include <limits>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <errno.h>
#include <time.h>
#include <omp.h>
//#include <SFML/Audio.hpp>
//#include <libudev.h>
#include <locale.h>
#include <dirent.h>
#include <vector>
#include <wiringPiSPI.h>


using namespace cv;
using namespace std;


#define	pin_11	0
#define	pin_12	1
#define	pin_7	7


#define texelRatio 1// 0.1926/0.1790

extern int WIDTH_PANT;  // 800*texelRatio     //
extern int HEIGHT_PANT; // 480                  //480

#define WIDTH_HD  2592  // 3280
#define HEIGHT_HD 1944  // 2464

extern int WIDTH_FRAME;  // 3280
extern int HEIGHT_FRAME; // 2464

extern int  FPS, BRIGHTNESS, INTEGRATION_TIME;
extern char tmpbuff[WIDTH_HD*HEIGHT_HD*4];

extern CCamera* cam;



extern int Ur[60][2] ;
extern float puntos_pantalla[90][2];
extern ofstream ficher;

extern long int start_time;
