CXX=g++
CXXFLAGS=-Wall -Wextra -I./include -I./vendor
LIBS=`pkg-config --libs glfw3 glew glm`
SRC=*.cpp vendor/stb_image/stb_image.cpp

all:
	$(CXX) -o app $(CXXFLAGS) $(LIBS) $(SRC)

debug:
	$(CXX) -o app-debug $(CXXFLAGS) $(LIBS) $(SRC) -DDEBUG
