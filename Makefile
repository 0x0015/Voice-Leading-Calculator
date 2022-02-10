OBJS	= Score.cpp.o\
	  letterConvert.cpp.o\
	  emscripten.cpp.o

OUT	= em_build/VLUtil.js
CXX	= em++
BUILD_CXX_FLAGS	 = -std=c++17
LINK_OPTS	 = -s WASM=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap -s ASSERTIONS=1

all: $(OBJS)
	$(CXX) -g $(OBJS) -o $(OUT) $(LINK_OPTS)


%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT) em_build/VLUtil.wasm

em:
	em++ letterConvert.cpp Score.cpp emscripten.cpp -o em_build/VLUtil.js -std=c++17 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap -s ASSERTIONS=1
