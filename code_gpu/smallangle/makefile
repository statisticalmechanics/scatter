CXX = g++
CXXFLAGS = -Wall -g
smallangle: smallangle.o utils.o
	$(CXX) $(CXXFLAGS) -o smallangle smallangle.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
smallangle.o: smallangle.cpp smallangle_kernel.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c smallangle.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp
