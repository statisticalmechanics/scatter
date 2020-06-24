/**********************************************************
 * scattering photography I(X, Y)
 * white color (fixed angle, varying wavelength)
 * backward reflection & forward reflection
 * the code reads xyz coordinates of particles from
 * one configuration file config.xyz
 * 
 * Dingning Li and Kai Zhang, Duke Kunshan University, 2020
 **********************************************************/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<cstdio>

#define MAX_PARTICLE_NUMBER 10000
#define MAX_X_RESOLUTION 500
#define MAX_Y_RESOLUTION 500


int main(void) {
	int forb;
	int i; // pixel index
	int n; // particle index number
	int Width = MAX_X_RESOLUTION; // number of qx along x direction
	int Height = MAX_Y_RESOLUTION; // number of qy along y direction
	int num_Particles = MAX_PARTICLE_NUMBER;
	
	double wavelength; // lambda
	double wave_number; // k = 2*pi / lambda

	int x_pixel; // 0 < x_pixel < Width
	int y_pixel; // 0 < y_pixel < Height

	double lambda_min;
	double lambda_max;
	double dlambda = 0.01;
	int num_lambda; // number of wavelengths scanned

	double X, Y; // X,Y coordinates on the screen
	double R, D, L; // R^2 = X^2 + Y^2   L^2 = D^2 + R^2

	double sum_sin;
	double sum_cos;

	printf("Front (0) of Back (1)_\n");
	scanf("%d", &forb);
	printf("Input LOWER limit for wavelength_\n");
	scanf("%lf", &lambda_min);
	printf("Input UPPER limit for wavelength_\n");
	scanf("%lf", &lambda_max);
	printf("Input number of particles_\n");
	scanf("%d", &num_Particles);
	printf("Input x_Resolution_\n");
	scanf("%d", &Width);
	printf("Input y_Resolution_\n");
	scanf("%d", &Height);
	printf("Input distance (D)_\n");
	scanf("%lf", &D);

	dlambda = 0.01;
	num_lambda = (int)((lambda_max - lambda_min) / dlambda) + 1;


	const int Resolution = Width * Height; // total number of pixels on the screen
	printf("%d particles\n", num_Particles);
	printf("%d wavelengths\n", num_lambda);
	printf("%d pixels \n", Resolution);

	double *rx = new double[num_Particles];
	double *ry = new double[num_Particles];
	double *rz = new double[num_Particles];
	double(*Ixy) = new double[Resolution];
	double(*qx_0) = new double[Resolution];
	double(*qy_0) = new double[Resolution];
	double(*qz_0) = new double[Resolution];
	double(*qx) = new double[Resolution];
	double(*qy) = new double[Resolution];
	double(*qz) = new double[Resolution];

	/***********read coordinates from configuration file *************/
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
	/***************************************************************/

	if (forb == 0) { // front
		for (i = 0; i < Resolution; i++) {
			x_pixel = i % Width;
			y_pixel = i / Width;
			X = x_pixel - Width / 2;
			Y = y_pixel - Height / 2;
			R = sqrt(X * X + Y * Y);

			L = sqrt(D * D + X * X + Y * Y);

			qx_0[i] = X / L;
			qy_0[i] = Y / L;
			qz_0[i] = -(L - D) / L;
		}
	}
	else { // back
		for (i = 0; i < Resolution; i++) {
			x_pixel = i % Width;
			y_pixel = i / Width;
			X = x_pixel - Width / 2;
			Y = y_pixel - Height / 2;
			R = sqrt(X * X + Y * Y);

			L = sqrt(D * D + X * X + Y * Y);

			qx_0[i] = X / L;
			qy_0[i] = Y / L;
			qz_0[i] = -(D + L) / L;
		}
	}

	for (i = 0; i < Resolution; i++) {
		Ixy[i] = 0.0;
		for (wavelength = lambda_min; wavelength <= lambda_max; wavelength += dlambda) {
			wave_number = 2.0 * M_PI / wavelength;
			qx[i] = wave_number * qx_0[i];
			qy[i] = wave_number * qy_0[i];
			qz[i] = wave_number * qz_0[i];

			sum_sin = 0.0; 
			sum_cos = 0.0; 
			for (n = 0; n < num_Particles; n++) {
				sum_cos += cos(qx[i] * rx[n] + qy[i] * ry[n] + qz[i] * rz[n]);
				sum_sin += sin(qx[i] * rx[n] + qy[i] * ry[n] + qz[i] * rz[n]);
			}

		Ixy[i] += sum_cos * sum_cos + sum_sin * sum_sin ; 
		}
		Ixy[i] /= num_Particles * num_lambda;
		if(i % 100 == 0) printf("%d %d %lf\n", i, Resolution, Ixy[i]);
	}

	/*********** Write scattering intensity to file *****************/
	FILE *fpout;
	fpout = fopen("Ixy.dat", "w");
	fclose(fpout);

	if ((fpout = fopen("Ixy.dat", "a")) == NULL)
	{
		printf("Open Ixy.dat Failed.\n");
		return 1;
	}
	for (i = 0; i < Resolution; i++) {
		x_pixel = i % Width;
		y_pixel = i / Width;
		fprintf(fpout, "%d\t%d\t%lf\n", x_pixel, y_pixel, Ixy[i]);
	}

	fclose(fpout);
	/***************************************************************/

	printf("------------------------------- End -------------------------------\n");
	return 0;

}
