/*****************************************************************************
 * Copyright (c) 2013-2016 Intel Corporation
 * All rights reserved.
 *
 * WARRANTY DISCLAIMER
 *
 * THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
 * MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Intel Corporation is the author of the Materials, and requests that all
 * problem reports or change requests be submitted to it directly
 *****************************************************************************/
constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void Rotation(read_only image1d_t particle_x, read_only image1d_t particle_y, read_only image1d_t particle_z, read_only image2d_t imageA, read_only image2d_t imageB, read_only image2d_t imageC,  write_only image2d_t imageOUT, float rotation_angle, int num)
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
	float p_x;
	float p_z;
	float p_y;

	for (sum_cos = 0, sum_sin = 0, n = 0; n < num; n++){
		p_x = (read_imagef(particle_z, sampler, n).x) * sin (rotation_angle * M_PI / 180) + (read_imagef(particle_x, sampler, n).x) * cos (rotation_angle * M_PI / 180);
		p_z = (read_imagef(particle_z, sampler, n).x) * cos (rotation_angle * M_PI / 180) - (read_imagef(particle_x, sampler, n).x) * sin (rotation_angle * M_PI / 180);
		p_y = (read_imagef(particle_y, sampler, n).x);
		/*p_z = (read_imagef(particle_z, sampler, n).x) * cos (rotation_angle * M_PI / 180) + (read_imagef(particle_y, sampler, n).x) * sin(rotation_angle * M_PI / 180);
		p_y = (read_imagef(particle_y, sampler, n).x) * cos (rotation_angle * M_PI / 180) - (read_imagef(particle_z, sampler, n).x) * sin(rotation_angle * M_PI / 180);*/
		sum_cos += cos(B * p_y + A * p_x + C * p_z);
		sum_sin += sin(B * p_y + A * p_x + C * p_z);
	}
	resultOUT = (sum_cos * sum_cos + sum_sin * sum_sin) / num;
	write_imagef(imageOUT, (int2)(x, y), (float4)(resultOUT));
}