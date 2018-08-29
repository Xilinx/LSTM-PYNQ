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

from abc import ABCMeta, abstractmethod, abstractproperty
import numpy as np
import cffi
import os

from pynq import Overlay, PL, Xlnk


LSTM_ROOT_DIR = os.path.dirname(os.path.realpath(__file__))
LSTM_LIB_DIR = os.path.join(LSTM_ROOT_DIR, 'libraries')
LSTM_BIT_DIR = os.path.join(LSTM_ROOT_DIR, 'bitstreams')
LSTM_DATA_DIR = os.path.join(LSTM_ROOT_DIR, 'datasets')

RUNTIME_HW = "libhw"
RUNTIME_SW = "libsw"

class PynqLSTM(object):
    __metaclass__ = ABCMeta
    
    def __init__(self, runtime, dataset, network, load_overlay):
        self._ffi = cffi.FFI()
        self._libraries = {}
        if runtime == RUNTIME_HW:
            self.bitstream_name="{}-{}-pynq.bit".format(dataset, network)
            self.bitstream_path=os.path.join(LSTM_BIT_DIR, dataset, network, self.bitstream_name)
            if PL.bitfile_name != self.bitstream_path:
                if load_overlay:
                    Overlay(self.bitstream_path).download()
                else:
                    raise RuntimeError("Incorrect Overlay loaded")
        dllname = "{}-{}-{}-ocr-pynq.so".format(runtime, dataset, network)
        if dllname not in self._libraries:
            self._libraries[dllname] = self._ffi.dlopen(
		os.path.join(LSTM_LIB_DIR, dataset, network, dllname))
        self.interface = self._libraries[dllname]
        self._ffi.cdef(self.ffi_interface)

    @property
    def ops_per_seq_element(self):
        return self.lstm_ops_per_seq_element
    
    @property
    def lstm_ops_per_seq_element(self):
        gate_input_size = self.input_size + self.hidden_size + 1 if self.bias_enabled else self.input_size + self.hidden_size
        #2 accounts for mul and add separately, 4 is the number of gates
        ops = 2 * gate_input_size * 4 * self.hidden_size
        #element wise muls and peepholes
        ops = ops + 3 * self.hidden_size * 2 if self.peepholes_enabled else ops + 3 * self.hidden_size
        #directions
        return ops * 2 if self.bidirectional_enabled else ops

    def cleanup(self):
        xlnk = Xlnk()
        xlnk.xlnk_reset()

    @abstractproperty
    def input_size(self):
        pass

    @abstractproperty
    def hidden_size(self):
        pass

    @abstractproperty
    def peepholes_enabled(self):
        pass

    @abstractproperty
    def bias_enabled(self):
        pass

    @abstractproperty
    def bidirectional_enabled(self):
        pass

    @abstractproperty
    def ffi_interface(self):
        pass
        
    @abstractmethod
    def inference(self, input_data):
        pass


        

    
