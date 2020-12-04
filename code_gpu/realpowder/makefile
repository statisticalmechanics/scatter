CXX = g++
CXXFLAGS = -Wall -g
real_powder_GPU: real_powder_GPU.o utils.o
	$(CXX) $(CXXFLAGS) -o real_powder_GPU real_powder_GPU.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
real_powder_GPU.o: real_powder_GPU.cpp realpowder_kernel.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c real_powder_GPU.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp
