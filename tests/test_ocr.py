#   Copyright (c) 2018, TU Kaiserslautern
#   Copyright (c) 2018, Xilinx, Inc.
#   All rights reserved.
# 
#   Redistribution and use in source and binary forms, with or without 
#   modification, are permitted provided that the following conditions are met:
#
#   1.  Redistributions of source code must retain the above copyright notice, 
#       this list of conditions and the following disclaimer.
#
#   2.  Redistributions in binary form must reproduce the above copyright 
#       notice, this list of conditions and the following disclaimer in the 
#       documentation and/or other materials provided with the distribution.
#
#   3.  Neither the name of the copyright holder nor the names of its 
#       contributors may be used to endorse or promote products derived from 
#       this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os
import lstm
from PIL import Image
import numpy as np
import pytest

def test_fraktur_ocr():
    test_dir = os.path.dirname(os.path.realpath(__file__))
    hw_ocr = lstm.PynqFrakturOCR(runtime=lstm.RUNTIME_HW)
    im = Image.open(os.path.join(test_dir, 'Test_images', 'fraktur', '010001.raw.lnrm.png'))
    with open(os.path.join(test_dir, 'Test_images', 'fraktur', '010001.txt'), 'r') as f:
        gt = f.read().replace('\n', '')
    hw_result = hw_ocr.inference(im)
    _, _, hw_recognized_text = hw_result
    hw_ocr.cleanup()
    assert gt == hw_recognized_text

def test_plain_ocr():
    networks = ["W2A2", "W2A4", "W4A4", "W4A8"]
    test_dir = os.path.dirname(os.path.realpath(__file__))
    for network in networks:
        hw_ocr = lstm.PynqPlainOCR(runtime=lstm.RUNTIME_HW, network=network)
        im = Image.open(os.path.join(test_dir, 'Test_images', 'plain', network, '010077.bin.png'))
        with open(os.path.join(test_dir, 'Test_images', 'plain', network, 'test_image_gt.txt'), 'r') as f:
            gt = f.read().replace('\n', '')
            hw_result = hw_ocr.inference(im)
            _, _, hw_recognized_text = hw_result
            hw_ocr.cleanup()
            assert gt == hw_recognized_text
