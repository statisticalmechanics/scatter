CXX = g++
CXXFLAGS = -Wall -g
2dfactor: 2dfactor.o utils.o
	$(CXX) $(CXXFLAGS) -o 2dfactor 2dfactor.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
2dfactor.o: 2dfactor.cpp Template.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c 2dfactor.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp