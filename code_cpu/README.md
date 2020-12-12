****** Illustrations for How to use the codes ******

*************** general introduction ***************

The three pieces of codes here are program that uses 
CPU to calculate the diffraction pattern of an arbitrary 
sample. Variables and principles are based on the paper
https://arxiv.org/abs/2010.06126
I will refer to the paper in later section as THE PAPER.

******************* whitexray.cpp *******************

This piece of code is based on Laue method, details 
has been introduced in our paper, section 6.1 & 5.1.

- files needed:
	- config.xyz: sample file, being put in the same
	folder as the executable file. Go to 
	https://github.com/statisticalmechanics/scatter/tree/master/xyz
	and download "test_sample.xyz", rename it to be
	"config.xyz" and put it in the same folder as the
	executable.
- parameters:
	1. Front (0) of Back (1)_
	This asks whether the transmission method or 
	back-reflection method, as described at FIG. 6 
	in our paper. Input 0 means transmission method 
	and 1 means back-reflection method.
	2. Input LOWER limit for wavelength_
	3. Input UPPER limit for wavelength_
	As Laue method uses white x-rays which contains 
	waves in a frequency range, this parameter specifies
	the range for adopted frequency, in unit of “sigma”, 
	the length of atomic diameters.
	4. Input number of particles_
	Number of particles in the sample system. 
	5. Input x_Resolution_
	6. Input y_Resolution_
	The size of screen, the unit can be everything.
	7. Input distance (D)_
	The distance between the screen and the sample,
	as illustrated in FIG.6 in THE PAPER.
	The unit should consist with the previous two input
	values.

***************** powder_even.cpp *****************
This piece of code is based on powder method, details 
has been introduced in our paper, section 8.

- files needed:
	- config.xyz: sample file, being put in the same
	folder as the executable file. Go to 
	https://github.com/statisticalmechanics/scatter/tree/master/xyz
	and download "test_sample.xyz", rename it to be
	"config.xyz" and put it in the same folder as the
	executable.
- parameters:
	1. Input number of particles, any value bigger than
	10000 is illegal_
	Number of particles in the sample system. 
	2. Input number of points on a sphere_
	Specify the number of directions the tiny single 
	crystals will have in a polycrystal sample.
	3. Input wavelength_
	Wavelength chosen, in unit of “sigma”, the length 
	of atomic diameters.
