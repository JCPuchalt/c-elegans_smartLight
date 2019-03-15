# c-elegans_smartLight
This code is developed for assessing:
[1] "Active backlight for automating visual monitoring: an analysis of a lighting
control technique for Caenorhabditis elegans cultured on standard Petri plates" PLOS ONE
Copyright (c) 2018 A.Sánchez, J.Puchalt, P.Martorell and S.Genovés

UPV and Biopolis license.



# Hardware description on [1]
The camera should be coplanar to the display focusing at the center of it at a distance of about 8cm.
The camera and the display must be firmly grasped, since any minimum displacement will decalibrate 
the camera-display correspondences.

* Raspberry Pi 3
* Pi NoIR Camera V1
* 7" Raspberry Pi Display

## Requirements

* Run on Raspberry Pi (Raspbian OS)

* OpenCV 3.0 and opencv_contrib

* Install userland-master in [/home/pi/userland-master] 
(https://github.com/raspberrypi/userland)

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
