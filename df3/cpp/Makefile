CXXFLAGS = -Wextra -Wall -pedantic -std=c++17 -lpthread
FILES = *pp
SOURCE = src

main: $(SOURCE)/$(FILES) Makefile
	g++ -o main $(SOURCE)/*cpp $(CXXFLAGS)

debug: $(SOURCE)/$(FILES) Makefile
	g++ -g -fsanitize=address -o main $(SOURCE)/*cpp $(CXXFLAGS)

release: $(SOURCE)/$(FILES) Makefile
	g++ -O3 -o main $(SOURCE)/*cpp $(CXXFLAGS)