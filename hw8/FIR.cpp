#include "FIR.h"
#include <sysc/datatypes/fx/sc_fixed.h>

void FIR::run_fir() {
    sc_fixed<32, 16> fb;
    sc_uint<32> b;
    sc_uint<32> sum_;
    
    // Use sc_fixed to get the fixed-point value
    fb = (float)1 / (order_ + 1);
    for (int i = 0; i < 32; i++) { // copy bit-by-bit
        b[i] = fb[i];
    }

    // Reset taps
    std::fill(taps_.begin(), taps_.end(), 0);

    while (1) {
        wait();

        // Shift previous taps
        for (int i = order_; i > 0; i--) {
            taps_[i] = taps_[i - 1];
        }
        taps_[0] = x.read();

        // Compute output result
        sum_ = 0;
        for (uint i = 0; i <= order_; i++) {
            sum_ += taps_[i].read() * b;
        }

        y.write(sum_);
    }
}