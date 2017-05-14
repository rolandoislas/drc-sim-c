//
// Created by rolando on 5/14/17.
//

#include "BitUtil.h"

unsigned BitUtil::reverse(unsigned number, int bits) {
    unsigned reversed = 0;
    for(int i = 0; i < bits; i++)
        reversed |= ((number >> i) & 1) << (bits - 1 - i);
    return reversed;
}
