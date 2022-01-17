OBJS	= main.cpp.o\
	  Score.cpp.o\
	  letterConvert.cpp.o\
	  ArguementHandler/ArgHandle.cpp.o\
	  ArguementHandler/Arguement.cpp.o

OUT	= main
CXX	= g++
CC      = gcc
BUILD_CXX_FLAGS	 = -Wall -std=c++17 -g
BULID_CC_FLAGS   =
LINK_OPTS	 =

all: $(OBJS)
	$(CXX) -g $(OBJS) -o $(OUT) $(LINK_OPTS)


%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

%.c.o: %.c
	$(CXX) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT)
	rm -f em_build/VLUtil.js em_build/VLUtil.wasm

em:
	em++ letterConvert.cpp Score.cpp emscripten.cpp -o em_build/VLUtil.js -std=c++17 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap
