//
//  VHDL_Gen.cpp
//  DST
//
//  Created by Chad Cooper on 11/29/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "VHDL_Gen.hpp"
#include <cmath>

void generateDataPath(string& filename, vector<VHDLFU>& FUs, vector<VHDLReg>& RUs, vector<Op>& LOp, vector<Reg>& LReg, vector<Mux>& FUMuxes, vector<Mux>& RUMuxes){
//      A data path consists of three parts:
//          (1) entity: primary inputs/outputs + reset, start & clock ports
//          (2) component declaration: assign componenets from AUDI library
//          (3) architecture: structural netlist of registers, adders, subtractors, multipliers, dividers, and multiplexors
    
    cout << "\n\n\n\nVHDL FILE\n\n\n";
    
    cout << "library IEEE;\n";
    cout << "use IEEE.STD_LOGIC_1164.ALL;\n";
    cout << "use ieee.numeric_std.all;\n\n\n";
    cout << "entity datapath_" << filename << " is\n";
    cout << "port\n(\n";
    
    // Assign inputs and outputs
    for(const auto& reg : LReg){
        
        string IO;
        if(reg.type == Reg::input){
            IO = "in";
        } else if (reg.type == Reg::output){
            IO = "out";
        }
            cout  << "\t" << reg.name << " : " << IO << " std_logic_vector(" << reg.width - 1 << " downto 0);\n";

    }
    
    cout  << "\tWR_INPUT : in std_logic;\n";
    
    int sel;
    
    // Create select signals for multiplexors
    for(const auto& FUMux : FUMuxes ){
        sel = ceil(log2(FUMux.num_inputs));
        if(sel < 1) sel = 1;
        cout << "\t" << FUMux.name << "_SEL" << " : in std_logic_vector(" << sel << " downto 0);\n";
    }
    
    for(const auto& RUMux : RUMuxes ){
        sel = ceil(log2(RUMux.num_inputs));
        if(sel < 1) sel = 1;
        cout << "\t" << RUMux.name << "_SEL" << " : in std_logic_vector(" << sel << " downto 0);\n";
    }
    
    cout  << "\tclear : in std_logic;\n\tclock : in std_logic);\nend datapath_" << filename << ";\n";
    cout  << "\narchitecture " << filename << "_arch of datapath_" << filename << " is\n";

    
    // Generate internal signals.
    // These are the signals coming from and going to the multiplexors
    cout  << "\n--BEGINNING OF SIGNALS\n\n";

    for(const auto& RU : RUs ){
        cout  << "\tsignal " << RU.name << "_IN" << " : std_logic_vector(" << RU.width - 1 << " downto 0);\n";
        cout  << "\tsignal " << RU.name << "_OUT" << " : std_logic_vector(" << RU.width - 1 << " downto 0);\n";
    }
    
    for(const auto& FU : FUs ){
        cout  << "\tsignal " << FU.name << "_IN1" << " : std_logic_vector(" << FU.width - 1 << " downto 0);\n";
        cout  << "\tsignal " << FU.name << "_IN2" << " : std_logic_vector(" << FU.width - 1 << " downto 0);\n";
        cout  << "\tsignal " << FU.name << "_OUT" << " : std_logic_vector(" << FU.width - 1 << " downto 0);\n";
    }

    // Instantiate functional units
    cout  << "\nbegin\n\n";
    cout  << "\n--BEGINNING OF FUNCTIONAL UNITS\n\n";
    for(const auto& FU : FUs){
        cout << "\t" << FU.name << FUEntity(FU.type) << "\n\tgeneric map(width => " << FU.width << ")\n"
        << "\tport map(input1 => " << FU.name << "_IN1 , input2 => " << FU.name << "_IN2, output => " << FU.name << "_OUT);\n\n";
    }
    
    cout  << "\n--BEGINNING OF REGS\n\n";
    
    for(const auto& RU : RUs){
        cout << "\t" << RU.name << ": entity work.c_register" << "\n\tgeneric map(width => " << RU.width << ")\n"
        << "\tport map(input => " << RU.name << "_IN, WR => WR_INPUT, output => " << RU.name << "_OUT, clear => clear, clock => clock);\n\n";
    }

    cout  << "\n--BEGINNING OF MUXES FOR FUNCTIONAL UNITS\n\n";

    for(int i = 0; i < FUMuxes.size(); i++){
        sel = ceil(log2(FUMuxes[i].num_inputs));
        if(sel < 1) sel = 1;
        
        cout  << "\t" << FUMuxes[i].name << ": entity work.c_multiplexer"
        << "\n\tgeneric map(width => " << FUMuxes[i].width << ", no_of_inputs => "<< FUMuxes[i].num_inputs
        << ", select_size => " << sel << ")\n" << "\tport map (";
        
        // Link internal registers here
        for(int j = 0; j < FUMuxes[i].num_inputs; j++){
            string input_signal_name;
            if (FUMuxes[i].log_in[j]->type == Reg::intermediate){
                
                // Find FU with this intermediate register as ouptut
                for(const auto& FU : FUs){ // for each FU,
                    for (const auto& FU_output : FU.log_out){ // check if its output
                            if (FU_output == FUMuxes[i].log_in[j]){ // is an input to the current mux
                                input_signal_name = FU.name + "_OUT";
                        }
                    }
                }
            } else {
                input_signal_name = FUMuxes[i].log_in[j]->name;
            }
            
            cout << "input(" << j*FUMuxes[i].width + FUMuxes[i].width - 1 << " downto " << j*FUMuxes[i].width
            << ") => " << input_signal_name << ", ";
        }
        
        cout << "mux_select => " << FUMuxes[i].name << "_SEL, ";
        
        string output_signal_name = (getXByName(FUs, FUMuxes[i].phys_name))->input_muxes[i%2]->name;
        
        // Convert output signal name to uppercase to match signal name
        transform(output_signal_name.begin(), output_signal_name.end(), output_signal_name.begin(), ::toupper);
        
        cout << "output <= " << output_signal_name << ");\n\n"; //FUMuxes[i].phys->input_muxes[i%2]->name;
    }
    

    cout  << "\n--BEGINNING OF MUXES FOR REGISTERS\n\n";
    
    for(int i = 0; i < RUMuxes.size(); i++){
        sel = ceil(log2(RUMuxes[i].num_inputs));
        if(sel < 1) sel = 1;
        
        cout  << "\t" << RUMuxes[i].name << ": entity work.c_multiplexer"
        << "\n\tgeneric map(width => " << RUMuxes[i].width << ", no_of_inputs => "<< RUMuxes[i].num_inputs
        << ", select_size => " << sel << ")\n" << "\tport map (";
        
        // Link internal registers here
        for(int j = 0; j < RUMuxes[i].num_inputs; j++){
            string input_signal_name;
            if (RUMuxes[i].log_in[j]->type == Reg::intermediate){
                
                // Find FU with this intermediate register as ouptut
                for(const auto& FU : FUs){ // for each FU,
                    for (const auto& FU_output : FU.log_out){ // check if its output
                        if (FU_output == RUMuxes[i].log_in[j]){ // is an input to the current mux
                            input_signal_name = FU.name + "_OUT";
                        }
                    }
                }
            } else {
                input_signal_name = RUMuxes[i].log_in[j]->name;
            }
            
            cout << "input(" << j*RUMuxes[i].width + RUMuxes[i].width - 1 << " downto " << j*RUMuxes[i].width
            << ") => " << input_signal_name << ", ";
        }
        
        cout << "mux_select => " << RUMuxes[i].name << "_SEL, ";
        
        string output_signal_name = RUs[i].name + "_IN";

        cout << "output <= " << output_signal_name << ");\n\n"; //FUMuxes[i].phys->input_muxes[i%2]->name;
    }
    
    cout  << "end " << filename << "_arch;";
    
    
    
}

string FUEntity(Op::op_type type){
    switch (type) {
        case Op::ADD:
            return ": entity work.c_adder";
            break;
        case Op::SUB:
            return ": entity work.c_subtractor";
            break;
        case Op::MULT:
            return ": entity work.c_multiplier";
            break;
        case Op::DIV:
            return ": entity work.c_divider";
            break;
    }
}
