CXXFLAGS = -Wextra -Wall -pedantic -std=c++17 -lpthread
FILES = *pp
SOURCE = src
TESTS_FOLDER = tests

main: $(SOURCE)/$(FILES) Makefile
	g++ -o main $(SOURCE)/*cpp $(CXXFLAGS)

debug: $(SOURCE)/$(FILES) Makefile
	g++ -g -o main $(SOURCE)/*cpp $(CXXFLAGS)

release: $(SOURCE)/$(FILES) Makefile
	g++ -O3 -o main $(SOURCE)/*cpp $(CXXFLAGS)

tests: $(TESTS_FOLDER)/$(FILES)
	g++ -g -fsanitize=address -o test $(TESTS_FOLDER)/*.cpp $(CXXFLAGS)