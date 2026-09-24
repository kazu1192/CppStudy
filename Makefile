# CMake を使わずに engine を直接ビルドするための最小構成。
# ターゲットを増やすときは CMakeLists.txt と両方を更新すること。
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -fsanitize=address,undefined -g -MMD -MP -Isrc
LDFLAGS  := -fsanitize=address,undefined

SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS   := $(shell pkg-config --libs sdl3)

SRCS     := $(addprefix src/,main.cpp window.cpp entity.cpp scene.cpp input.cpp sdl_context.cpp)
OBJS     := $(SRCS:.cpp=.o)
DEPS     := $(OBJS:.o=.d)
TARGET   := app

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(SDL_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

.PHONY: clean
