## c-elegans_smartLight
This code is developed for assessing:
[1] "Active backlight for automating visual monitoring: an analysis of a lighting
control technique for Caenorhabditis elegans cultured on standard Petri plates" PLOS ONE
Copyright (c) 2018 A.Sánchez, J.Puchalt, P.Martorell and S.Genovés

UPV and Biopolis license.



## Hardware description on [1]
Components:
* Raspberry Pi 3
* Pi NoIR Camera V1
* 7" Raspberry Pi Display
* Raspberry power supply (at least 2Amp)
* Micro SD card (at least 16GB)
* Assemble them as indicated in document ....

The camera should be coplanar to the display focusing at the center of it at a distance of about 8cm.
The camera and the display must be firmly grasped, since any minimum displacement will decalibrate 
the camera-display correspondences.

## Easy installation
* Install raspbian on Raspberry Pi 3. 
    1. Download raspbian installer (NOOBS) from https://www.raspberrypi.org/downloads/noobs/
    2. Unzip the downloaded files
    3. Format a Micro SD with FAT32
    4. Copy the unzipped files into the formatted Micro SD.
    5. Place Micro SD into Micro SD card slot of Raspberry Pi.
    6. Connect power supply and follow the raspbian installation instructions indicated on display.
    7. Write down the Raspberry Pi IP address. It will be indicated 
* From Raspberry Pi open a Terminal. 
    1. Download c-elegans_smartLight and unzip into [/home/pi]. For this type:
      wget https://github.com/JCPuchalt/c-elegans_smartLight
      
    Launch file set_up_whole_system.sh
        

## Requirements

* Run on Raspberry Pi (Raspbian Operating System) (https://www.raspberrypi.org/downloads/noobs/)

* OpenCV 3.0 and opencv_contrib

* Install userland-master in [/home/pi/userland-master] 
(https://github.com/raspberrypi/userland)

* GPU memory 256MB

* 

## Installation

* Copy code in a folder [/home/pi/]

* cd c-elegans_smartLight

* cmake .

* make


## Run code

* sudo ./c-elegans


## Notes

* Make sure that the calibration pattern is captured completely with the camera.
To move it you can modify it with the file "./calibracio/p1.xml".

* Some libraries such as WiringPi may be required for compilation, but they are not required for the application
