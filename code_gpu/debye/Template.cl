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

__kernel void Add(read_only image1d_t rx, read_only image1d_t ry, read_only image1d_t rz, float qvalue, int num, write_only image1d_t imageOUT)
{
    const int x = get_global_id(0);
	int n;
	float resultOUT = 0;

	float rix = (read_imagef(rx, sampler, x).x);
	float riy = (read_imagef(ry, sampler, x).x);
	float riz = (read_imagef(rz, sampler, x).x);
	float rij;

	for (n = 0; n < num; n++){
		if (n == x) {
			resultOUT += 1;
		}
		else {
			rij = sqrt(((read_imagef(rx, sampler, n).x) - rix) * ((read_imagef(rx, sampler, n).x) - rix) + ((read_imagef(ry, sampler, n).x) - riy) * ((read_imagef(ry, sampler, n).x) - riy) + ((read_imagef(rz, sampler, n).x) - riz) * ((read_imagef(rz, sampler, n).x) - riz));
			resultOUT += sin(qvalue * rij) / qvalue / rij;
		}
	}

    write_imagef(imageOUT, x, (float4)(resultOUT));
}
