CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude -MMD -MP
SRC      = $(wildcard src/*.cpp)
OBJ      = $(SRC:src/%.cpp=build/%.o)
.RECIPEPREFIX = >

dbms: $(OBJ)
> $(CXX) $(CXXFLAGS) -o $@ $(OBJ)

build/%.o: src/%.cpp
> @mkdir -p build
> $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
> rm -rf build dbms

-include $(OBJ:.o=.d)
