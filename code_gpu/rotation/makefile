CXX = g++
CXXFLAGS = -Wall -g
rotation_GPU: rotation_GPU.o utils.o
	$(CXX) $(CXXFLAGS) -o rotation_GPU rotation_GPU.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
rotation_GPU.o: rotation_GPU.cpp Template.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c rotation_GPU.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp