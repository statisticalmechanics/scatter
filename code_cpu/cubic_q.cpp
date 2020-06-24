/**********************************************************
 * scattering photography I(X, Y)
 * choosing q on a cubic (fixed angle, varying wavelength)
 * backward reflection
 * the code reads xyz coordinates of particles from
 * one configuration file config.xyz which may contain more
 * than one configuration
 * 
 * It will generate three output files, one presenting xyz 
 * coordinates for q vectors directly (out.xyz); one lists 
 * the scale of q vectors and corresponding Sq values (out.q); 
 * one groups q vectors the similiar magnitude together and 
 * averages their Sq value as one single Sq output for that 
 * q group (out.q.draw)
 * 
 * Notice that for selecting coordinates of q, both positive 
 * and negative are put into consideration.
 * 
 * Dingning Li and Kai Zhang, Duke Kunshan University, 2020
 **********************************************************/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<algorithm>

#define MAX_PARTICLE_NUMBER 50000
#define MAX_q_Per_Dimension 1000

using namespace std;

struct VEC {
	double x;
	double y;
	double z;
	double scale;
};

bool cmp(VEC a, VEC b) {
	return a.scale < b.scale;
}


int main(void) {
	//printf("test.");
	int pause_variable;
	int i;
	int n;
	int number_x = MAX_q_Per_Dimension + 1;
	int number_y = MAX_q_Per_Dimension + 1;
	int number_z = MAX_q_Per_Dimension + 1;
	int q_number;
	int a, b, c;
	int configuration;
	int q_count = 0;
	double length_X;
	double length_Y;
	double length_Z;
	double delta_q_x;
	double delta_q_y;
	double delta_q_z;
	double q_scale;
	double sum_sin = 0;
	double sum_cos = 0;
	double q_resolution;
	double s_q_draw = 0.0;
	double q_draw = 0.0;
	int num_Particles = MAX_PARTICLE_NUMBER + 1;

	printf("Input configurations_");
	scanf("%d", &configuration);
	printf("Input length for x direction_\n");
	scanf("%lf", &length_X);
	printf("Input length for y direction_\n");
	scanf("%lf", &length_Y);
	printf("Input length for z direction_\n");
	scanf("%lf", &length_Z);
	while (num_Particles > MAX_PARTICLE_NUMBER) {
		printf("Input number of particles, any value bigger than %d is illegal_\n", MAX_PARTICLE_NUMBER);
		scanf("%d", &num_Particles);
	}
	while (number_x > MAX_q_Per_Dimension) {
		printf("Input upper bound for x side length of q cubic, any value bigger than %d is illegal_\n", MAX_q_Per_Dimension);
		scanf("%d", &number_x);
	}
	while (number_y > MAX_q_Per_Dimension) {
		printf("Input upper bound for y side length of q cubic, any value bigger than %d is illegal_\n", MAX_q_Per_Dimension);
		scanf("%d", &number_y);
	}
	while (number_z > MAX_q_Per_Dimension) {
		printf("Input upper bound for z side length of q cubic, any value bigger than %d is illegal_\n", MAX_q_Per_Dimension);
		scanf("%d", &number_z);
	}
	printf("Input resolution for drawing S(q) vs q_\n");
	scanf("%lf", &q_resolution);
	q_number = (2 * number_x + 1) * (2 * number_y + 1) * (2 * number_z + 1);

	VEC *q_vec = new VEC[q_number];
	double *s_q = new double[q_number];
	for (n = 0; n < q_number; n++) {
		s_q[n] = 0;
	}
	double *particle_x = new double[num_Particles];
	double *particle_y = new double[num_Particles];
	double *particle_z = new double[num_Particles];

	FILE *coordinate_file;
	if ((coordinate_file = fopen("config.xyz", "r")) == NULL) {
		fprintf(stderr, "ERROR: CANNOT FIND FILE!\n");
		exit(1);
	}

	delta_q_x = 2 * M_PI / length_X;
	delta_q_y = 2 * M_PI / length_Y;
	delta_q_z = 2 * M_PI / length_Z;
	for (a = -number_x; a < (number_x + 1); a++) {
		for (b = -number_y; b < (number_y + 1); b++) {
			for (c = -number_z; c < (number_z + 1); c++) {
				q_vec[q_count].x = delta_q_x * a;
				q_vec[q_count].y = delta_q_y * b;
				q_vec[q_count].z = delta_q_z * c;
				q_vec[q_count].scale = sqrt(q_vec[q_count].x * q_vec[q_count].x + q_vec[q_count].y * q_vec[q_count].y + q_vec[q_count].z * q_vec[q_count].z);
				q_count++;
			}
		}
	}
	sort(q_vec, q_vec + q_count, cmp);

	for (c = 0; c < configuration; c++) {
		printf("configuration: %d\n", c);
		for (n = 0; n < 2; n++) {
			fscanf(coordinate_file, "%*[^\n]%*c");
		}
		for (n = 0; n < num_Particles; n++) {
			fscanf(coordinate_file, "%*s");
			fscanf(coordinate_file, "%lf", &particle_x[n]);
			fscanf(coordinate_file, "%lf", &particle_y[n]);
			fscanf(coordinate_file, "%lf\n", &particle_z[n]);
			//printf("rxyz:%lf\t%lf\t%lf\n",particle_x[n],particle_y[n],particle_x[n]);
		}

		for (i = 0; i < q_number; i++) {
			for (sum_sin = 0, sum_cos = 0, n = 0; n < num_Particles; n++) {
				sum_cos += cos(q_vec[i].x * particle_x[n] + q_vec[i].y * particle_y[n] + q_vec[i].z * particle_z[n]);
				sum_sin += sin(q_vec[i].x * particle_x[n] + q_vec[i].y * particle_y[n] + q_vec[i].z * particle_z[n]);
			}
			s_q[i] += (sum_cos * sum_cos + sum_sin * sum_sin) / num_Particles;
		}
	}
	if (fclose(coordinate_file) != 0) {
		printf("ERROR: FAIL TO CLOSE FILE.\n");
	}


	FILE *out_xyz_sq;
	if ((out_xyz_sq = fopen("out.xyz", "a")) == NULL)
	{
		printf("Open Failed.\n");
		return 1;
	}
	for (i = 0; i < q_number; i++) {
		fprintf(out_xyz_sq, "%lf\t%lf\t%lf\t%lf\n", q_vec[i].x, q_vec[i].y, q_vec[i].z, s_q[i] / configuration);
	}
	fclose(out_xyz_sq);

	FILE *out_q_sq;
	if ((out_q_sq = fopen("out.q", "a")) == NULL)
	{
		printf("Open Failed.\n");
		return 1;
	}
	for (i = 0; i < q_number; i++) {
		fprintf(out_q_sq, "%lf\t%lf\n", q_vec[i].scale, s_q[i] / configuration);
	}
	fclose(out_q_sq);

	FILE *out_q_draw_sq;
	if ((out_q_draw_sq = fopen("out.q.draw", "a")) == NULL)
	{
		printf("Open Failed.\n");
		return 1;
	}

	i = 0;
	while (i < q_count) {
		for (n = 0, s_q_draw = 0, q_draw = 0; (i + n) < q_count && (q_vec[i + n].scale - q_vec[i].scale) < q_resolution; n++) {
			s_q_draw += s_q[i + n] / configuration;
			q_draw += q_vec[i].scale;
		}
		s_q_draw /= n;
		q_draw /= n;
		fprintf(out_q_draw_sq, "%lf\t%lf\n", q_draw, s_q_draw);
		i += n;
	}
	fclose(out_q_draw_sq);

	delete(particle_x);
	delete(particle_y);
	delete(particle_z);
	delete(s_q);
	delete(q_vec);
	return 0;
}
