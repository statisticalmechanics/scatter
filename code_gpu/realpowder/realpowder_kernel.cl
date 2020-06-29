/**********************************************************
 * scattering photography Ixy
 * real powder
 * loop over N particles
 * code for GPU kernel
 * 
 * Dingning Li and Kai Zhang, Duke Kunshan University, 2020
 **********************************************************/
constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void Realpowder(read_only image1d_t particle_x, read_only image1d_t particle_y, read_only image1d_t particle_z, read_only image2d_t imageA, read_only image2d_t imageB, read_only image2d_t imageC,  write_only image2d_t imageOUT, int num)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);
	int n;

	float resultOUT;
	float sum_cos;
	float sum_sin;
	float A = read_imagef(imageA, sampler, (int2)(x, y)).x;
	float B = read_imagef(imageB, sampler, (int2)(x, y)).x;
	float C = read_imagef(imageC, sampler, (int2)(x, y)).x;
	
	for (sum_cos = 0, sum_sin = 0, n = 0; n < num; n++){
		sum_cos += cos(A * (read_imagef(particle_x, sampler, n).x) + B * (read_imagef(particle_y, sampler, n).x) + C * (read_imagef(particle_z, sampler, n).x));
		sum_sin += sin(A * (read_imagef(particle_x, sampler, n).x) + B * (read_imagef(particle_y, sampler, n).x) + C * (read_imagef(particle_z, sampler, n).x));
	}
	resultOUT = (sum_cos * sum_cos + sum_sin * sum_sin) / num;
	write_imagef(imageOUT, (int2)(x, y), (float4)(resultOUT));
}
