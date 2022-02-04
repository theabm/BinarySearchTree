EXE = main.x
CXX = g++
CXXFLAGS = -I include -g -std=c++17 -DNDEBUG -Wall -Wextra

SRC= main.cpp
OBJ=$(SRC:.cpp=.o)
INC = include/node.h  include/iterator.h  include/BST.h

# eliminate default suffixes
.SUFFIXES:
SUFFIXES =

# just consider our own suffixes
.SUFFIXES: .cpp .o

all: $(EXE)

.PHONY: all

clean:
	rm -rf $(OBJ) $(EXE) include/*~ *~ html latex

.PHONY: clean

$(EXE): $(OBJ)
	$(CXX) $^ -o $(EXE)

documentation: Doxygen/doxy.in
	doxygen $^

.PHONY: documentation

main.o: include/node.h include/iterator.h include/BST.h

format: $(SRC) $(INC)
	@clang-format -i $^ -verbose || echo "Please install clang-format to run this commands"

.PHONY: format
