## c-elegans_smartLight
This code is developed for assessing:
[1] "Active backlight for automating visual monitoring: an analysis of a lighting
control technique for Caenorhabditis elegans cultured on standard Petri plates" PLOS ONE
Copyright (c) 2018 UPV and Biopolis. A.Sánchez, J.Puchalt, P.Martorell and S.Genovés

### Research article
Puchalt JC, Sánchez-Salmerón AJ, Martorell Guerola P, Genovés Martínez S (2019) Active backlight for automating visual monitoring: An analysis of a lighting control technique for Caenorhabditis elegans cultured on standard Petri plates. PLOS ONE 14(4): e0215548. https://doi.org/10.1371/journal.pone.0215548

### Hardware description on [1]
Components:
* Raspberry Pi 3
* Pi NoIR Camera V1
* 7" Raspberry Pi Display
* Raspberry power supply (at least 2Amp)
* Micro SD card (at least 16GB)
* Assemble them as indicated in document https://github.com/JCPuchalt/c-elegans_smartLight/blob/master/S2_Appendix.pdf

The camera should be coplanar to the display focusing at the center of it at a distance of about 8cm.
The camera and the display must be firmly grasped, since any minimum displacement will decalibrate 
the camera-display correspondences.

### Easy software installation
* Install raspbian on Raspberry Pi 3. 
    1. Download raspbian installer (NOOBS) from https://www.raspberrypi.org/downloads/noobs/
    2. Unzip the downloaded files
    3. Format a Micro SD with FAT32
    4. Copy the unzipped files into the formatted Micro SD.
    5. Place Micro SD into Micro SD card slot of Raspberry Pi.
    6. Connect to Ethernet.
    7. Connect power supply and follow the raspbian installation instructions indicated on display.
    8. Write down the Raspberry Pi IP address. To find it, on a terminal type: 
        `ifconfig` 
* From Raspberry Pi open a Terminal. 
    1. Download set_up_whole_system.sh file into (i.e.) [/home/pi].       
    2. Launch file set_up_whole_system.sh and follow instructions. On terminal, set file folder (for example: /home/pi/Downloads) 
        ````
        cd /home/pi/Downloads
        sh ./set_up_whole_system.sh
        ````
        
        
## Software installation details of set_up_whole_system.sh. It will install and set:

* Download and install OpenCV 3.0 and opencv_contrib
* Download and install userland in [/home/pi/userland] 
(https://github.com/raspberrypi/userland)
* Set GPU memory at 256MB
* Temporarily increase the swap
* Free some memory
* Install libncurses5-dev libncursesw5-dev
* Install git
* Enable SSH
* Enable camera
* Disable camera led
* Download and install c-elegans_smartLight program. Copy code in a folder [/home/pi/]
    ````
    cd c-elegans_smartLight
    cmake .
    make
    ````

## Run code on Raspberry Pi
Connect from PC to Raspberry Pi through SSH (on Windows with PuTTY (https://www.ssh.com/ssh/putty/download) or Linux with terminal `ssh pi@Raspberry_IP_Adrees`). That terminal is connected to Raspberry, so type on that terminal:
````
cd c-elegans_smartLight
sudo ./c-elegans
````

## Notes

* Make sure that the calibration pattern is captured completely with the camera.
To move it you can modify it with the file "./calibracio/p1.xml".

* Some libraries such as WiringPi may be required for compilation, but they are not required for the application
