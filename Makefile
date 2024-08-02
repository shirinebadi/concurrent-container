CXX = g++-11
CXXFLAGS = -std=c++20 -Wall -Ih -c -fopenmp
LXXFLAGS = -std=c++20 -Ih -fopenmp
SOURCES1 = main.cpp
OBJECTS1 = $(SOURCES1:.cpp=.o)
SOURCES2 = utils.cpp
OBJECTS2 = $(SOURCES2:.cpp=.o)
TARGET1 = main

all: $(TARGET1)

$(TARGET1): $(OBJECTS1) $(OBJECTS2)
	$(CXX) $(LXXFLAGS) -o $(TARGET1) $(OBJECTS1) $(OBJECTS2)

main.o: main.cpp utils.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

utils.o: utils.cpp variation.h
	$(CXX) $(CXXFLAGS) utils.cpp -o utils.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET1) $(OBJECTS1) $(OBJECTS2)
