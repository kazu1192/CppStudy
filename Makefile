CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -fsanitize=address,undefined -g -MMD -MP
SRCS     := main.cpp texture.cpp entity.cpp scene.cpp
OBJS     := $(SRCS:.cpp=.o)
DEPS		 := $(OBJS:.o=.d)
TARGET   := app

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

.PHONY: clean
