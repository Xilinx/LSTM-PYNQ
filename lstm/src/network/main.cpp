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

int main(int argc, const char* argv[]) {
	std::string current_exec_name = argv[0]; // Name of the current exec program
  	std::string label;
	float compute_time;
  	std::ifstream label_file;

  	if (argc != 4)
	{
		std::cout << "3 parameters are needed: " << endl;
		std::cout << "1 - Alphabet path " << endl;
		std::cout << "2 - Quantized input image" << endl;
		std::cout << "3 - Expected result" << endl;
		return 1;
	}

	std::string alphabet_path = argv[1];
	std::string predicted = lstm_ocr_from_file_path(argv[2], alphabet_path.c_str(), &compute_time);
  	label_file.open(argv[3]);
  	std::getline(label_file, label); 
	printf("Label = %s\n", label.c_str());
	printf("Pred  = %s\n", predicted.c_str());
	if (strcmp(predicted.c_str(), label.c_str()) != 0) 
		return 1;
	return 0;
}

