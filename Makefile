CXX = g++
CXXFLAGS = -O3 -std=c++17 -fopenmp

TARGETS = saxpy montecarlo_pi histogram_gather

all: $(TARGETS)

saxpy: saxpy_serial.cc
	$(CXX) $(CXXFLAGS) saxpy_serial.cc -o saxpy

montecarlo_pi: montecarlo_pi.cc
	$(CXX) $(CXXFLAGS) montecarlo_pi.cc -o montecarlo_pi

histogram_gather: histogram_gather.cc
	$(CXX) $(CXXFLAGS) histogram_gather.cc -o histogram_gather

clean:
	rm -f $(TARGETS)