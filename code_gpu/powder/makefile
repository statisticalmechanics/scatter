CXX = g++
CXXFLAGS = -Wall -g
powder_GPU: powder_GPU.o utils.o
	$(CXX) $(CXXFLAGS) -o powder_GPU powder_GPU.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
powder_GPU.o: powder_GPU.cpp powder_kernel.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c powder_GPU.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp
