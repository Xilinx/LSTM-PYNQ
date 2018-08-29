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

#include "lstm_inference.hpp"

void lstm_ocr_wrapper(float* image, int flat_length, char* out_buffer, const char* alphabet_path, float* compute_time) {
	InputImage input_image(image, flat_length);
	std::string predicted = lstm_ocr(input_image.image_fw_bw_, input_image.width_, alphabet_path, compute_time);
	std::strcpy(out_buffer, predicted.c_str());
}

void lstm_ocr_from_file_path_wrapper(const char* file_path, char* out_buffer, const char* alphabet_path, float* compute_time) {
	std::string predicted = lstm_ocr_from_file_path(file_path, alphabet_path, compute_time);
	std::strcpy(out_buffer, predicted.c_str());
}

std::string lstm_ocr_from_file_path(const char* file_path, const char* alphabet_path, float* compute_time) {
	std::string image_path(file_path);
	InputImage input_image(image_path);
	return lstm_ocr(input_image.image_fw_bw_, input_image.width_, alphabet_path, compute_time);
}

std::string lstm_ocr(float* image_fw_bw, int columns, const char* alphabet_path, float* compute_time) {
	#if ON_BOARD == 1
		DonutDriver* platform;
		void* accel_buf_in, *accel_buf_out;
		platform = initPlatform();
    	accel_buf_in = platform->allocAccelBuffer(1024 * 64);
    	if (!accel_buf_in) {
    		throw std::runtime_error("Failed to allocate accel buffer");
    	}
	#endif

	Alphabet alphabet;
	std::string input_file_alphabet_dir(alphabet_path);
	alphabet.Init(input_file_alphabet_dir);

	std::vector<ap_uint<PACKEDWIDTH> > v_all_images;
	for (unsigned int j = 0; j < columns * 2; j++) {
		t_fixed_image fpix[HEIGHT_IN_PIX];		
		for(unsigned int l = 0; l < HEIGHT_IN_PIX; l++) {		
			fpix[l] = (t_fixed_image) image_fw_bw[j * HEIGHT_IN_PIX + l];	
		}		
		ap_uint<PACKEDWIDTH> pack = Pack<PACKEDWIDTH,t_fixed_image,PIXELWIDTH,HEIGHT_IN_PIX>(fpix);
		v_all_images.push_back(pack);
	}

	#if ON_BOARD == 1
		void *input_buffer_address_virt = cma_alloc(v_all_images.size() * DATAWIDTH, false);
    	void *input_buffer_address_phys = reinterpret_cast<void*>(cma_get_phy_addr(input_buffer_address_virt));
		void *output_buffer_address_virt = cma_alloc((128 * 8), false);
    	void *output_buffer_address_phys = reinterpret_cast<void*>(cma_get_phy_addr(output_buffer_address_virt));
		memory_write<ap_uint<PACKEDWIDTH>>(input_buffer_address_virt, v_all_images);
	#else
		ap_uint<DATAWIDTH> *sw_input = (ap_uint<DATAWIDTH> *) v_all_images.data();
		ap_uint<DATAWIDTH> *sw_output = (ap_uint<DATAWIDTH> *) malloc(128 * 8);
		if (!sw_output) {
			throw std::runtime_error("Failed to allocate output buffer");
		}
	#endif
		
	unsigned int bytes_factor = ceil((float)(PIXELWIDTH * HEIGHT_IN_PIX * 8) / DATAWIDTH);
	ap_uint<32> number_bytes_read = 2 * columns * bytes_factor;

	auto start = std::chrono::high_resolution_clock::now();

	#if ON_BOARD == 1
	platform->writeJamRegAddr(0x10, columns);
	platform->writeJamRegAddr(0x18, columns * 2);
	platform->writeJamRegAddr(0x20, number_bytes_read);
	platform->write64BitJamRegAddr(0x28, (AccelDblReg)input_buffer_address_phys);
	platform->write64BitJamRegAddr(0x34, (AccelDblReg)output_buffer_address_phys);
	platform->writeJamRegAddr(0x00, 1);

	while ((platform->readJamRegAddr(0x00) & 0x2) == 0) {}

	#else	
	topLevel_BLSTM_CTC(columns, columns * 2, number_bytes_read, sw_input, sw_output);
	#endif	

	auto finish = std::chrono::high_resolution_clock::now();
	auto ms_compute_time = std::chrono::duration<float, std::chrono::milliseconds::period>(finish - start);
	*compute_time = ms_compute_time.count();

	std::vector<uint64_t> label(128);
	#if ON_BOARD == 1
		memory_read<uint64_t>(output_buffer_address_virt, label);
	#else
		memory_read<uint64_t>(sw_output, label);
	#endif

	std::string predicted_string;
	for (unsigned int j = 1; j < label.at(0) + 1; j++) {		
		std::string symbol = alphabet.ReturnSymbol(label.at(j));
		predicted_string.insert(predicted_string.end(), symbol.begin(), symbol.end());
	}
	return predicted_string;
}

