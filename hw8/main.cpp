#include "FIR.h"
#include "sysc/communication/sc_signal_ports.h"
#include <iostream>
#include <sysc/kernel/sc_time.h>
#include <sysc/tracing/sc_trace.h>
#include <systemc.h>

int sc_main(int argc, char *argv[]) {
    sc_time period(5, SC_NS); // clock period
    sc_clock clk("clk", period); // clock
  
    sc_signal<bool> rst;
    sc_signal<sc_uint<32>> x;
    sc_signal<sc_uint<32>> y32;
    sc_signal<sc_uint<32>> y48;

    FIR fir32("fir32", 32);
    FIR fir48("fir48", 48); 
    
    // Binding
    fir32.clk(clk);
    fir32.rst(rst);
    fir32.x(x);
    fir32.y(y32);   
    
    fir48.clk(clk);
    fir48.rst(rst);
    fir48.x(x);
    fir48.y(y48);   
    
    // Create the trace file "RESULT.vcd"
    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");
    if (!tf) {
        std::cerr << "Error: Can't create VCD file." << std::endl;
        return 0;
    }   
    
    // Trace ports
    sc_trace(tf, clk, "clk");
    sc_trace(tf, rst, "rst");
    sc_trace(tf, x, "x");
    sc_trace(tf, fir32.y, "y32");
    sc_trace(tf, fir48.y, "y48");

    rst.write(0);
    sc_start(period);
    rst.write(1);

    // Open and read test file
    ifstream fir_data;
    fir_data.open("firData");
    uint fir_in;
    while (fir_data >> fir_in) {
        x.write(fir_in);
        sc_start(period);
        
        std::cout << "x: " << x << endl;
        std::cout << "y32: " << std::hex << fir32.y.read() << endl;
        std::cout << "y48: " << std::hex << fir48.y.read() << endl;
        std::cout << endl;
    }
    
    // Close test file
    fir_data.close();   
    
    // Close trace file
    sc_close_vcd_trace_file(tf);    
    
    return 0;
}