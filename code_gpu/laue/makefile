CXX = g++
CXXFLAGS = -Wall -g
white_GPU: white_GPU.o utils.o
	$(CXX) $(CXXFLAGS) -o white_GPU white_GPU.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
white_GPU.o: white_GPU.cpp Template.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c white_GPU.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp