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

#ifndef HOST_DRIVER_H
#define HOST_DRIVER_H

#include <iostream>
#include <math.h>
#include <ap_int.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <signal.h>

#if ON_BOARD == 1
#include "platform.hpp"
#include "xlnkdriver.hpp"
#endif

using namespace std;

typedef unsigned long long ExtMemWord;

const unsigned int bitsPerExtMemWord = sizeof(ExtMemWord) * 8;


template<typename LowPrecType>
void memory_write(void* buf, std::vector<LowPrecType> w_data) {
    LowPrecType * lpbuf = (LowPrecType *) buf;
    for (unsigned int i = 0; i < w_data.size(); i++) {
        lpbuf[i] = (LowPrecType) w_data[i];
    }
}

template<typename LowPrecType>
void memory_read(void* buf, std::vector<uint64_t> &r_data) {
    LowPrecType * lpbuf = (LowPrecType *) buf;
    for(unsigned int i = 0; i < r_data.size(); i++) {
        r_data[i] = (uint64_t)lpbuf[i];
    }
}

#if ON_BOARD == 1

static XlnkDriver* platform = 0;

void platformSIGINTHandler(int signum) {
    std::cout << "Caught SIGINT, forcing exit" << std::endl;
    if (platform) {
        platform->detach();
    }
    delete platform;
    exit(1);
}

DonutDriver* initPlatform(bool cleanSIGINTExit) {
    if (!platform) {
        platform = new XlnkDriver(0x43c00000, 64 * 1024);
    }
    if (cleanSIGINTExit) {
        struct sigaction action;
        std::memset(&action, 0, sizeof(struct sigaction));
        action.sa_handler = &platformSIGINTHandler;
        int res = sigaction(SIGINT, &action, NULL);
    }
    return static_cast<DonutDriver*>(platform);
}

void deinitPlatform(DonutDriver* driver) {
    delete platform;
    platform = 0;
}

void loadBitFile(const char* accelName) {
    // Dummy function to keep the linker happy
}

#endif

#endif
