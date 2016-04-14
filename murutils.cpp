#include "murutils.h"
#include "murmagics.h"

namespace prvt {

uint16_t updateCRC(uint16_t acc, const uint8_t input)
{
    // Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
    // with no carries)
    acc ^= (input << 8);

    // "Divide" the poly into the dividend using CRC XOR subtraction
    // CRC_acc holds the "remainder" of each divide
    // Only complete this division for 8 bits since input is 1 byte

    for (uint8_t i = 0; i < 8; i++) {
        // Check if the MSB is set (if MSB is 1, then the POLY can "divide"
        // into the "dividend")

        if ((acc & 0x8000) == 0x8000) {
            acc <<= 1;
            acc ^= ::poly_val;
        }
        else {
            acc <<= 1;
        }
    }

    return acc;
}

}

uint16_t calculateCRC(const char* data, const size_t len)
{
    uint16_t crcout = ::seed_val;

    for (size_t i = 0; i < len; ++i) {
        crcout = prvt::updateCRC(crcout, data[i]);
    }

    return crcout;
}

