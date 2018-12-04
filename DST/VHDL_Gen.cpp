//
//  VHDL_Gen.cpp
//  DST
//
//  Created by Chad Cooper on 11/29/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "VHDL_Gen.hpp"
#include <cmath>

void generateDataPath(string& filename, vector<VHDLFU>& FUs, vector<VHDLReg>& RUs, vector<Op>& LOp, vector<Reg>& LReg, vector<Mux<VHDLFU>>& FUMuxes, vector<Mux<VHDLReg>>& RUMuxes){
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
    
    for(const auto& FUMux : FUMuxes) {
        sel = ceil(log2(FUMux.num_inputs));
        if(sel < 1) sel = 1;
        
        cout  << "\t" << FUMux.name << ": entity work.c_multiplexer"
        << "\n\tgeneric map(width => " << FUMux.width << ", no_of_inputs => "<< FUMux.num_inputs
        << ", select_size => " << sel << ")\n" << "\tport map (";
        
        // Link internal registers here
        for(int i = 0; i < FUMux.num_inputs; i++){
            cout << "input(" << i*FUMux.width + FUMux.width - 1 << " downto " << i*FUMux.width<< ") => " << FUMux.log_in[i]->name << ", ";
        }
        
        cout << endl << endl;
    }
    
//    for(int i = 0; i < MUXs.size(); i++){
//        if(MUXs[i].type != REG){
//            cout  << "\t" << MUXs[i].nameA << "_MUX_A : entity work.c_multiplexer" << "\n\tgeneric map(width => " << MUXs[i].width << ", no_of_inputs => "<<
//            MUXs[i].MUX_inA.size() << ", select_size => " << get_size(MUXs[i].MUX_inA.size())  << ")\n" << "\tport map ("
//            << muxSplitInput(MUXs[i].width, MUXs[i].MUX_inA.size(), MUXs[i].MUX_inA, MUXs, this -> regs)
//            << "mux_select => "<< MUXs[i].nameA << "_MUX_A_SEL, output => " << MUXs[i].nameA << "_A_IN);\n\n";
//
//            cout  << "\t" << MUXs[i].nameB << "_MUX_B : entity work.c_multiplexer" << "\n\tgeneric map(width => " << MUXs[i].width << ", no_of_inputs => "<<
//            MUXs[i].MUX_inB.size() << ", select_size => " << get_size(MUXs[i].MUX_inB.size())  << ")\n" << "\tport map ("
//            << muxSplitInput(MUXs[i].width, MUXs[i].MUX_inB.size(), MUXs[i].MUX_inB, MUXs, this -> regs)
//            << "mux_select => "<< MUXs[i].nameA << "_MUX_B_SEL, output => " << MUXs[i].nameB << "_B_IN);\n\n";
//        }
//    }
//    cout  << "\n--BEGINNING OF MUXES FOR REGISTERS\n\n";
//    for(int i = 0; i < MUXs.size(); i++){
//        if(MUXs[i].type == REG){
//            cout  << "\t" << MUXs[i].nameA << "_MUX : entity work.c_multiplexer" << "\n\tgeneric map(width => " << MUXs[i].width << ", no_of_inputs => "<<
//            MUXs[i].MUX_inA.size() << ", select_size => " << get_size(MUXs[i].MUX_inA.size())  << ")\n" << "\tport map ("
//            << muxSplitInput(MUXs[i].width, MUXs[i].MUX_inA.size(), MUXs[i].MUX_inA, MUXs, this->regs)
//            << "mux_select => "<< MUXs[i].nameA << "_MUX_SEL, output => " << MUXs[i].nameA << "_IN);\n\n";
//        }
//    }
//
//    cout  << "end " << filename << "_arch;";
    
    
    
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
