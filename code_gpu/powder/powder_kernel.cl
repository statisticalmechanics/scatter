/**********************************************************
 * scattering result S(q)
 * 2d factor
 * loop over N particles
 * code for GPU kernel
 * 
 * Dingning Li and Kai Zhang, Duke Kunshan University, 2020
 **********************************************************/
constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void Powder(read_only image1d_t particle_x, read_only image1d_t particle_y, read_only image1d_t particle_z, read_only image1d_t imageA, read_only image1d_t imageB, read_only image1d_t imageC, write_only image1d_t imageOUT, float coefficient, int num, float f_q)
{
    const int x = get_global_id(0);
	int n;

	float resultOUT;
	float sum_cos;
	float sum_sin;
	float A = read_imagef(imageA, sampler, x).x * coefficient;
	float B = read_imagef(imageB, sampler, x).x * coefficient;
	float C = read_imagef(imageC, sampler, x).x * coefficient;
	
	for (sum_cos = 0, sum_sin = 0, n = 0; n < num; n++){
		sum_cos += f_q * cos(A * (read_imagef(particle_x, sampler, n).x) + B * (read_imagef(particle_y, sampler, n).x) + C * (read_imagef(particle_z, sampler, n).x));
		sum_sin += f_q * sin(A * (read_imagef(particle_x, sampler, n).x) + B * (read_imagef(particle_y, sampler, n).x) + C * (read_imagef(particle_z, sampler, n).x));
	}
	resultOUT = (sum_cos * sum_cos + sum_sin * sum_sin) / num;
	
    write_imagef(imageOUT, x, (float4)(resultOUT));
}
