DEBUG=0

CXX=g++
CXXFLAGS=-Wall -Wextra -I./include -I./vendor
LDFLAGS=`pkg-config --libs glfw3 glew glm`

SRC=$(wildcard *.cpp vendor/stb/*.cpp vendor/imgui/*.cpp)
OBJ=$(patsubst %.cpp,build/%.o,$(SRC))

ifeq ($(DEBUG), 0)
	CXXFLAGS+=-DDEBUG
else
	CXXFLAGS+=-O3
endif

all: $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o opengl $(LDFLAGS)
	@echo "opengl"

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -o $@ $< -c $(CXXFLAGS)
	@echo $@

clean:
	@rm -rf opengl build/
	@echo "clean"
