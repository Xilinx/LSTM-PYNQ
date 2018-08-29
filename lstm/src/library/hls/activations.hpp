/*
 *  Copyright (c) 2018, TU Kaiserslautern
 *	Copyright (c) 2018, Xilinx
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2.  Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *  3.  Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

template
<
unsigned int NUMBER_OF_LUT_ENTRIES,
typename Input_t,
typename Limit_t,
typename RecipStep_t,
typename Output_t
>
Output_t sigmoid_lut(Input_t & input, Output_t lut_sigmoid[NUMBER_OF_LUT_ENTRIES])
{
	Limit_t lower_limit = -5.0;
	Limit_t upper_limit = 5.0;
	RecipStep_t recip_step = 25.5;

	Input_t input_temp = input;
	Output_t output;

	// If we are outside of LUT range
	if (input_temp <= lower_limit)
	{
		output = lut_sigmoid[0];
	}
	else if (input_temp >= upper_limit)
	{
		output = lut_sigmoid[NUMBER_OF_LUT_ENTRIES-1];
	}
	else
	{
		// Scale from [lower, upper] to [0, N]
		Input_t t = input_temp - lower_limit;
		uint16_t index = t * recip_step;

		output = lut_sigmoid[index];
	}

	return output;
}

template
<
unsigned int NUMBER_OF_LUT_ENTRIES,
typename Input_t,
typename Limit_t,
typename RecipStep_t,
typename Output_t
>
Output_t tanh_lut(Input_t & input, Output_t lut_tanh[NUMBER_OF_LUT_ENTRIES])
{
	Limit_t lower_limit = -3.0;
	Limit_t upper_limit = 3.0;
	RecipStep_t recip_step = 42.5;

	Input_t input_temp = input;
	Output_t output;

	// If we are outside of LUT range
	if (input_temp <= lower_limit)
	{
		output = lut_tanh[0];
	}
	else if (input_temp >= upper_limit)
	{
		output = lut_tanh[NUMBER_OF_LUT_ENTRIES-1];
	}
	else
	{
		// Scale from [lower, upper] to [0, N]
		Input_t t = input_temp - lower_limit;
		uint16_t index = t * recip_step;

		output = lut_tanh[index];
	}

	return output;
}
