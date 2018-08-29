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

#ifndef INPUT_HANDLING_HPP
#define INPUT_HANDLING_HPP

#include <stdexcept>
#include <string>       // std::string
#include <iostream>     // std::cout, std::cerr
#include <fstream>      // std::ifstream std::ofstream
#include <vector>
#include <math.h>       // tanh, log
#include <dirent.h>
#include <sys/types.h>
#include <algorithm>	// std::sort
#include <ctype.h>		// isspace()
#include <chrono>       // std::chrono::seconds, std::chrono::duration_cast

#include <cstdint>		// std::memcpy()
#include <cstring>		// std::memcpy()

#include <unistd.h>
#include <stdio.h>
#include <hls_stream.h>
#include <ap_int.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

#include "hw_config.h"

std::vector<float> ReadImageFromFile(std::string image_path);
std::vector<float> ReadImageFromArray(float* image_array, unsigned int flat_length); 
	
template
<
unsigned int DatawidthPacked,
typename t_pixel,
unsigned int PixelWidth,
unsigned int ColumnsHeight
>
ap_uint<DatawidthPacked> Pack(t_pixel x[ColumnsHeight]) {
	constexpr unsigned int mask = (1 << PixelWidth) - 1;
	ap_uint<DatawidthPacked> packed = 0;
	for (int count=ColumnsHeight-1; count >= 0;count--) {
		packed = packed << PixelWidth;
		ap_uint<PixelWidth> uValue = *reinterpret_cast<ap_uint<PixelWidth> *>(&x[count]);
		packed(PixelWidth-1,0) = (uValue&mask);
	}
	return packed;	
}
	
template
<
unsigned int DatawidthPacked,
typename t_pixel,
unsigned int PixelWidth,
unsigned int ColumnsHeight
>
void UnPack(ap_uint<DatawidthPacked> pack, t_pixel x[ColumnsHeight]) {
	ap_uint<DatawidthPacked> temp[ColumnsHeight];
	for(unsigned int i = 0; i < HEIGHT_IN_PIX; i++){
		temp[i] = (pack >> (i * PixelWidth));
		x[i] = *((t_pixel*)&temp[i]);
	}
}

class Alphabet {
  public:
	Alphabet();
	~Alphabet();
	std::vector<std::string> alphabet;
	void Init(std::string input_file_alphabet);
	std::string ReturnSymbol(unsigned int label);
	void Print();

  protected:
  private:
};

class InputImage {
  public:
	InputImage(std::vector<float> image);
	InputImage(float* image, unsigned int flat_length);
    InputImage(std::string image_path);
	~InputImage();
	float *image_fw_bw_;
	unsigned int width_;

  protected:
  private:
};

#endif
