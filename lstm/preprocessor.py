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
from abc import ABCMeta, abstractmethod, abstractproperty

import math
import numpy as np
import cv2 as cv2
from PIL import Image
import PIL.ImageOps 

FRAKTUR_PADDING = 16
FRAKTUR_MIN_CLIP = -1.75
FRAKTUR_MAX_CLIP = 3.75
DELTA = 0.1

class ImagePreprocessor(object):
    __metaclass__ = ABCMeta

    def __init__(self):
        pass
    
    @abstractmethod
    def preprocess(self, image):
        pass

class PlainImagePreprocessor(ImagePreprocessor):
    def __init__(self, target_height, input_bit_width):
        super(PlainImagePreprocessor, self).__init__()
        self.target_height = target_height
        self.input_bit_width = input_bit_width

    def greyscale(self, image):
        return image.convert('L')

    def invert(self, image):
        return PIL.ImageOps.invert(image)

    def resize_and_normalize(self, image):
        original_width, original_height = image.size
        scaling_ratio = (self.target_height + DELTA) / original_height
        target_width = int(math.ceil(original_width * scaling_ratio)/2)*2
        size = (int(target_width), int(self.target_height))
        resized_image = image.resize(size, Image.BILINEAR) 
        resized_image = resized_image 
        return resized_image

    def reshape(self, image):
        reshaped_image = np.array(image).T
        reshaped_image = np.expand_dims(reshaped_image, axis=0)
        reshaped_image = np.swapaxes(reshaped_image, 0, 1)
        reshaped_image = reshaped_image.reshape(-1, 1)
        return reshaped_image

    def shift_scale(self, image):
        return ((image.astype(np.float32) / 255.0) - 0.5) * 2 #put it between -1 and 1

    def quantize(self, image):
        prescale = 2 ** (self.input_bit_width - 1)
        postscale = 2 ** (- self.input_bit_width + 1) 
        return np.round(np.clip(image, a_min=float(-1), a_max=float(1 - postscale)) * prescale) * postscale

    def preprocess(self, image):
        preprocessed_image = self.greyscale(image)
        preprocessed_image = self.invert(preprocessed_image)
        preprocessed_image = self.resize_and_normalize(preprocessed_image)
        preprocessed_image = self.reshape(preprocessed_image)
        preprocessed_image = self.shift_scale(preprocessed_image)
        preprocessed_image = self.quantize(preprocessed_image)
        return preprocessed_image  
        
class FrakturImagePreprocessor(ImagePreprocessor):
    def __init__(self, mean, std_deviation):
        super(FrakturImagePreprocessor, self).__init__()
        self.mean = mean
        self.std_deviation = std_deviation

    def greyscale(self, image):
        return image.convert('L')

    def preprocess(self, image):
        preprocessed_image = self.greyscale(image)
        preprocessed_image = np.array(preprocessed_image)
        preprocessed_image = preprocessed_image * 1.0 / np.amax(preprocessed_image)
        preprocessed_image = np.amax(preprocessed_image) - preprocessed_image
        preprocessed_image = preprocessed_image.T   
        w = preprocessed_image.shape[1]
        preprocessed_image = np.vstack([np.zeros((FRAKTUR_PADDING, w)), preprocessed_image, np.zeros((FRAKTUR_PADDING, w))])
        preprocessed_image = (preprocessed_image - self.mean) / self.std_deviation
        preprocessed_image = preprocessed_image.reshape(-1,1)
        preprocessed_image = np.round(preprocessed_image * 4)/4
        preprocessed_image = np.clip(preprocessed_image, a_min=float(FRAKTUR_MIN_CLIP), a_max=float(FRAKTUR_MAX_CLIP))
        preprocessed_image = preprocessed_image.astype(np.float32)    
        return preprocessed_image
