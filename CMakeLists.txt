cmake_minimum_required(VERSION 2.8)
project( c-elegans )
SET(COMPILE_DEFINITIONS -Werror)

find_package( OpenCV REQUIRED  )


include_directories (/usr/include)
include_directories (/usr/local/include)
include_directories (/opt/vc/include) 


include_directories(/home/pi/userland)
include_directories(/home/pi/userland/interface/vcos)
include_directories(/home/pi/userland/interface/vcos/pthreads)
include_directories(/home/pi/userland/interface/vmcs_host/linux)
include_directories(/home/pi/userland/host_applications/linux/libs/bcm_host/include)

link_directories(/opt/vc/lib)


add_executable(c-elegans c-elegans.cpp sources/visionSystem.cpp sources/CE_Funcions.cpp 
			sources/camera.cpp sources/cameracontrol.cpp sources/graphics.cpp
			sources/lodepng.cpp sources/bufferCircular.cpp sources/Display.cpp )



target_link_libraries(c-elegans -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui
				-lopencv_objdetect -lopencv_flann -lopencv_photo -lopencv_stitching -lopencv_videostab -fopenmp
				-lwiringPi -lm -lwiringPiDev -lpthread 
				libmmal_util.so libmmal_core.so libmmal_vc_client.so libvcos.so libbcm_host.so -IEGL GLESv2 EGL ncurses ${OpenCV_LIBS} -lrt )


