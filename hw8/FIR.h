#ifndef FIR_H
#define FIR_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(FIR) {
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_in<sc_uint<32>> x;
    sc_out<sc_uint<32>> y;

    void run_fir();

    sc_uint<32> order_;
    std::vector<sc_signal<sc_uint<32>>> taps_;

    SC_CTOR(FIR, uint order = 16) : order_(order), taps_(order + 1) {
        SC_CTHREAD(run_fir, clk.pos());
        reset_signal_is(rst, false);
    }

    ~FIR() {}
};

#endif