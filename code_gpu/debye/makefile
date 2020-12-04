CXX = g++
CXXFLAGS = -Wall -g
debye_powder: debye_powder.o utils.o
	$(CXX) $(CXXFLAGS) -o debye_powder debye_powder.o utils.o /usr/local/cuda-10.0/lib64/libOpenCL.so
debye_powder.o: debye_powder.cpp debye_kernel.cl cl.h utils.h cl_version.h cl_platform.h cl_ext.h /usr/local/cuda-10.0/lib64/libOpenCL.so
	$(CXX) $(CXXFLAGS) -c debye_powder.cpp
utils.o: utils.cpp utils.h cl_ext.h cl.h cl_version.h cl_platform.h
	$(CXX) $(CXXFLAGS) -c utils.cpp
