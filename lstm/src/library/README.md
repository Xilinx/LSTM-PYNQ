# lstm-library

This is the FINN library for LSTMs and contains four folders:

- "hls" contains the LSTM hardware library (the templatized streaming components in HLS).

- "host" contains C++ host code to launch the accelerator(s).  

- "script" contains a set of Vivado scripts to generate the block design and bitstream generation of the HW accelerators. Those scripts are used when executing make-hw.

- "driver" contains a set of source files to handle the communication with the HW accelerator.

