# Unreal DNN Object Detection

## Installation
1. [Install CUDA](https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html#installing-cuda-development-tools)
2. [Install CUDNN](https://developer.nvidia.com/cudnn)
3. [Install CMake](https://cmake.org/download/)
4. [Clone OpenCV](https://github.com/opencv/opencv)
5. [Clone OpenCV contrib](https://github.com/opencv/opencv_contrib)

## Setup and run
1. Configure OpenCV in CMAKE with flags
```c++
BUILD_WITH_STATIC_CRT = OFF
BUILD_SHARED_LIBS = OFF
WITH_CUDA = ON
OPENCV_DNN_CUDA = ON
CUDA_FAST_MATH = ON 
CUDA_TOOLKIT_ROOT_DIR = (cuda toolkit path)
OPENCV_EXTRA_MODULES_PATH = (opencv ontribs path) 
```

2. Compile OpenCV
```c++
cmake.exe --build . --config Release --target INSTALL
```

3. Add folders to the root project dir
```c++
ThirdParty\OpenCV\include
ThirdParty\OpenCV\libs
Binaries\Win64
```

4. Move files from OpenCV directory
```c++
build\install\include\ -> ThirdParty\OpenCV\include
build\install\x64\vc16\staticlib\ -> ThirdParty\OpenCV\libs
```

5. Donwload [yolov3.cfg](https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg), [yolov3.weights](https://pjreddie.com/media/files/yolov3.weights), [coco.names](https://github.com/pjreddie/darknet/blob/master/data/coco.names)

6. Move these files to the 
```c++
Binaries\Win64
```

7. Generate Visual Studio project files for UE

8. Launch
