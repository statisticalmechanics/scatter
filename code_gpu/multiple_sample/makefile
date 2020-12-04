CXX = g++
CXXFLAGS = -Wall -g
multiple_sample_GPU: multiple_sample_GPU.o utils.o
	$(CXX) $(CXXFLAGS) -o multiple_sample_GPU multiple_sample_GPU.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
multiple_sample_GPU.o: multiple_sample_GPU.cpp multisample_kernel.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c multiple_sample_GPU.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp
