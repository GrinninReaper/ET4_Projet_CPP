
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

main: funct.o main.o
	$(CXX) $^ -o $@

.PHONY: clean

clean:
	rm -f *.o *~ core 


