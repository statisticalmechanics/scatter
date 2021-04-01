# scatter

Unifying the Concepts of Scattering  and  Structure Factor in  Ordered and Disordered Samples

Dingning Li and Kai Zhang

Division of Natural and Applied Sciences, Duke Kunshan University, Kunshan, Jiangsu, 215300, China

kai.zhang@dukekunshan.edu.cn

J. Appl. Cryst. (2021). 54, 644-660
https://doi.org/10.1107/S1600576721001965
https://scripts.iucr.org/cgi-bin/paper?S1600576721001965


#	WHAT'S IN EACH FOLDER

	Generally speaking, files presented here are codes for
	simulation and results in paper with form of raw data.

	"code_cpu" contains CPU simulation codes. they are 
	straightforward ones that runs on CPUs.

	"code_gpu" contains GPU sim codes. they use gpu to do
	parallel calculation thus accelerated the speed. Notice
	that codes in "code_cpu" and "code_gpu" may not 
	correspond with each other.
	
	"data" contains raw data for figures presented in paper.
	they can be re-generated via python.
	
	"images" contains the three 3d pictures of the cylinder
	phase, leminar phase and sphere phase.
	
	"xyz" contains coordinate informations for models used 
	as simulation samples in paper (files end with .xyz). it
	also contains vmd scripts that generates observable 3d
	picture from xyz files.
