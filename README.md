# KGAFTEngine Graphics Engine
Made for vulkan and cmake

How to build:
Unrar the native libraries archive for your system into the current folder
Go to CmakeLists.txt and set the path to lunarg vulkan sdk, that installed in your system
run in command prompt:
git submodule init
git submodule update

cmake .
cmake --build .


Как собрать решение:
Разархивируйте архив с компилированными библиотеками для вашей операционной системы в текущую папку
Откройте CMakeLists.txt и укажите путь до sdk vulkan от lunarg

Выполните следующие комманды в коммандной строке:

git submodule init
git submodule update

cmake .
cmake --build .

