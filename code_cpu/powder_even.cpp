/**********************************************************
 * scattering result S(q)
 * powder method
 * even distribution of points on sphere
 * the code reads xyz coordinates of particles from
 * one configuration file config.xyz
 * 
 * Dingning Li and Kai Zhang, Duke Kunshan University, 2020
 **********************************************************/
#include<stdio.h>
#include<cstdlib>
#include<math.h>
#include<stdlib.h>
#include<cstdio>
#include <time.h>

#define MAX_PARTICLE_NUMBER 10000


int main(void) {
	int num_points; // number of points on q sphere
	int i; // index for points on sphere
	int n; // particle index number
	int num_Particles = MAX_PARTICLE_NUMBER;
	
	double phi = (sqrt((double)5) - 1) / 2;
	double wavelength; // lambda
	double coefficient; // multiply to q_0 to get correct magnitude
	double theta_i;
	double theta_range = 90.0;
	double increment;
	double I;
	
	double v_1;
	double v_2;
	double s;

	double sum_sin;
	double sum_cos;

	while (num_Particles > MAX_PARTICLE_NUMBER) {
		printf("Input number of particles, any value bigger than %d is illegal_\n", MAX_PARTICLE_NUMBER);
		scanf("%d", &num_Particles);
	}
	printf("Input number of points on a sphere_\n");
	scanf("%d", &num_points);
	printf("Input wavelength_\n");
	scanf("%lf", &wavelength);
	printf("Input increment for theta_\n");
	scanf("%lf", &increment);

	printf("%d particles\n", num_Particles);

	double *rx = new double[num_Particles];
	double *ry = new double[num_Particles];
	double *rz = new double[num_Particles];
	double(*qx_0) = new double[num_points];
	double(*qy_0) = new double[num_points];
	double(*qz_0) = new double[num_points];
	double(*qx) = new double[num_points];
	double(*qy) = new double[num_points];
	double(*qz) = new double[num_points];

	/************ read coordinates from configuration file **************/
	FILE *fpxyz;
	if ((fpxyz = fopen("config.xyz", "r")) == NULL) {
		fprintf(stderr, "ERROR: CANNOT FIND FILE config.xyz!\n");
		exit(1);
	}

	for (i = 0; i < 2; i++) {
		fscanf(fpxyz, "%*[^\n]%*c");
	}
	for (i = 0; i < num_Particles; i++) {
		fscanf(fpxyz, "%*s");
		fscanf(fpxyz, "%lf", &rx[i]);
		fscanf(fpxyz, "%lf", &ry[i]);
		fscanf(fpxyz, "%lf", &rz[i]);
	}

	if (fclose(fpxyz) != 0) {
		printf("ERROR: FAIL TO CLOSE FILE.\n");
	}
	/**********************************************************************/
	
	/*********** generate even q vectors on a unit q sphere *************/
	FILE *sphere_out;
	if ((sphere_out = fopen("sphere_even.xyz", "a")) == NULL)
	{
		printf("Open Failed.\n");
		return 1;
	}
	for (i = 0; i < num_points; i++) {
			qz_0[i] = (2 * (i + 1) - 1) / (double)num_points - 1;
			qx_0[i] = sqrt(1 - qz_0[i] * qz_0[i]) * cos(2 * M_PI * i * phi);
			qy_0[i] = sqrt(1 - qz_0[i] * qz_0[i]) * sin(2 * M_PI * i * phi);
			fprintf(sphere_out, "H\t%lf\t%lf\t%lf\n", qx_0[i], qy_0[i], qz_0[i]);
	}
	fclose(sphere_out);
	/**********************************************************************/
	
	/*** loop over q spheres with different radius (i.e. different q's)****/
	FILE *output;
	if ((output = fopen("Sq.dat", "a")) == NULL)
	{
		printf("Open Failed.\n");
		return 1;
	}
	
	for (theta_i = 1; theta_i < theta_range; theta_i += increment) {
		printf("%lf\n", theta_i);
		I = 0;
		coefficient = 4 * M_PI / wavelength * sin(theta_i / 180 * M_PI);
		for (i = 0; i < num_points; i++) {
			qx[i] = coefficient * qx_0[i];
			qy[i] = coefficient * qy_0[i];
			qz[i] = coefficient * qz_0[i];

			sum_sin = 0.0; 
			sum_cos = 0.0; 
			for (n = 0; n < num_Particles; n++) {
				sum_cos += cos(qx[i] * rx[n] + qy[i] * ry[n] + qz[i] * rz[n]);
				sum_sin += sin(qx[i] * rx[n] + qy[i] * ry[n] + qz[i] * rz[n]);
			}
			I += sum_cos * sum_cos + sum_sin * sum_sin; 
		}
		I /= num_Particles * num_points;
		fprintf(output, "%lf\t%lf\n", theta_i, I);
	}
	
	fclose(output);
	/**********************************************************************/

	printf("------------------------------- End -------------------------------\n");
	return 0;

}
