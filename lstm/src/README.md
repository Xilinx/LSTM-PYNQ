# Software sources

This folder contains the source files required to build:
 - For the hardware accelerated implementation, the host side of the OCR engine, used to expose an interface to python level.
 - For the software accelerated implementation, both the interface to the python level code and the BiLSTM implementation itself, which consists of the same HLS code used to generate the hardware implementation, but compiled for the ARM processor.

Because of the limited compute resources of the ARM processor on the PYNQ-Z1 board, the steps described here perform a cross compilation from a more powerful machine.

## Requirements 
 - An *arm-linux-gnueabihf-g++* cross-compiler.
 - A copy of the *include* folder from your Vivado installation.
 - A copy of the header *libxlnk_cma.h* from your PYNQ-Z1 board, located on the board at */usr/include/libxlnk_cma.h*.
 - A copy of the *libsds_lib.so* shared library from your PYNQ-Z1 board, located on the board at */usr/lib/libsds_lib.so*.
 
## Steps
1. From a bash command line run in this directory, create a folder for the build: `mkdir build`.
2. Export the path to the Vivado include folder: `export VIVADOHLS_INCLUDE_PATH=/path/to/include`, replacing */path/to/include* with the appropriate absolute path.
3. Export the path to the libsds_lib.so file: `export LIBSDS_ABS_FILE_PATH=/path/to/include`, replacing */path/to/include* with the appropriate absolute path.
4. Put a copy of *libxlnk_cma.h* into the sub-folder (with respect to the current one) *library/driver/*.
5. For the cross compiled hardware accelerated version, from the *build* folder, run `NETWORK=WxAy DATASET=plain cmake -DCROSS_COMPILE=TRUE -DON_BOARD=TRUE ..` replacing *WxAy*  the chosen precision. For the software version, set *-DON_BOARD=FALSE* instead.
6. Run `make && make install` to put the generated library in the appropriate folder.


