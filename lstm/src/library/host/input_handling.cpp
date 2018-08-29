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

#include "input_handling.hpp"

Alphabet::Alphabet() {}

Alphabet::~Alphabet() {
	alphabet.clear();
}

void Alphabet::Init(std::string input_file_alphabet) {
	std::ifstream input_stream;
	input_stream.open(input_file_alphabet, std::ifstream::in);
	if(!input_stream.good()) {
		throw std::runtime_error("Could not open alphabet at: " + input_file_alphabet);
	}
	
	std::string symbol;
	while (getline(input_stream, symbol)) {
		alphabet.push_back(symbol);
	}
	input_stream.close();
	
	if(alphabet.size() != NUMBER_OF_CLASSES) {
		throw std::runtime_error("Wrong number of symbols in alphabet");
	}
}
	
std::string Alphabet::ReturnSymbol(unsigned int label) {
	if(label < NUMBER_OF_CLASSES) {
		return alphabet.at(label);
	} else {
		throw std::runtime_error("Symbol is out of range");
	}
}
	
void Alphabet::Print() {
	for(unsigned int s = 0; s < NUMBER_OF_CLASSES; s++) {
		std::cout << s << "   " << alphabet.at(s) << std::endl;
	}
}

std::vector<float> ReadImageFromArray(float* image_array, unsigned int flat_length) {
	std::vector<float> image;
	std::copy(&image_array[0], &image_array[flat_length], back_inserter(image));
	return image;
}

std::vector<float> ReadImageFromFile(std::string image_path) {
	std::vector<float> image;
	std::ifstream input_stream;

	input_stream.open(image_path, std::ifstream::in);
	if (!input_stream.good()) {
		throw std::runtime_error("No image file found at: " + image_path);
	}

	float pix;
	while(input_stream >> pix) {
		image.push_back(pix);
	}
	input_stream.close();

	return image;
}

InputImage::InputImage(std::string image_path) : InputImage(ReadImageFromFile(image_path)) {}
	
InputImage::InputImage(float* image, unsigned int flat_length) : InputImage(ReadImageFromArray(image, flat_length)) {}

InputImage::InputImage(std::vector<float> image) {
	if (image.size() % HEIGHT_IN_PIX != 0) {
		throw std::runtime_error("Incorrect number of pixels in input image");
	}

	// Check if the number of columns is even
	if (image.size() % 2 == 0) {
		width_ = image.size() / HEIGHT_IN_PIX;
	} else {
		float last_column[HEIGHT_IN_PIX];
		for(unsigned int pix = 0; pix < HEIGHT_IN_PIX; pix++) {
			last_column[pix] = image[image.size() - HEIGHT_IN_PIX + pix];
		}
		for(unsigned int pix = 0; pix < HEIGHT_IN_PIX; pix++) {
			image.push_back(last_column[pix]);
		}	
		width_ = image.size() / HEIGHT_IN_PIX;
	}

	// Check if the number of columns is mult. of 4
	if (width_ % 4 == 0) {
		image_fw_bw_ = new float[2 * width_ * HEIGHT_IN_PIX];
	} else {
		image_fw_bw_ = new float[2 * width_ * HEIGHT_IN_PIX + 4];
	}

	// Interleave FW columns with BW columns
	for (unsigned int col = 0; col < width_; col++) {
		for (unsigned int row = 0; row < HEIGHT_IN_PIX; row++) {
			image_fw_bw_[2 * col * HEIGHT_IN_PIX + row] = image[col * HEIGHT_IN_PIX + row];
		}
		for (unsigned int row = 0; row < HEIGHT_IN_PIX; row++) {
			image_fw_bw_[HEIGHT_IN_PIX + 2 * col * HEIGHT_IN_PIX + row] = image[(width_ - col - 1) * HEIGHT_IN_PIX + row];
		}
	}
	if (width_ % 4 != 0) {
		for (unsigned int pix = 0; pix < 4; pix++) {
			image_fw_bw_[width_ * 2 * HEIGHT_IN_PIX + pix] = 0.0;
		}
	}	
}

InputImage::~InputImage() {
	if (image_fw_bw_ != NULL) {
		delete[] image_fw_bw_;
		image_fw_bw_ = NULL;
	}
}

	
	
	
