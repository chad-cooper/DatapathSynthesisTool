//
//  DCS.cpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "DCS.hpp"

//MARK: Build compatibility graph from AUDI file
ADUIGraph readAUDI(string audi, bool print){
    
    ADUIGraph audiGraph;
    
    std::string discard, line, reg_name, op_name, op_type_str, in1, in2, out;
    int width;
    
    ifstream audiFile (audi);
    std::istringstream iss;
    
    getline(audiFile, line); // INPUTS
    
    if (print) (std::cout << line << std::endl);
    
    iss.str(line); // Extract information from line
    
    iss >> discard; // throwaway 'inputs'
    
    while (iss >> reg_name >> width){
        audiGraph.E.emplace_back(reg_name, Reg::input, width, true);
    }
    
    getline(audiFile, line); // OUTPUTS
    if (print) std::cout << line << std::endl;
    iss.clear();
    iss.str(line);
    
    iss >> discard; // throw away 'outputs'
    
    while (iss >> reg_name >> width){
        audiGraph.E.emplace_back(reg_name, Reg::output, width, false);
    }
    
    getline(audiFile, line); // Intermediate registers
    if (print) std::cout << line << std::endl;
    
    iss.clear();
    iss.str(line);
    
    iss >> discard; // throw away 'regs'
    
    while (iss >> reg_name >> width) {
        audiGraph.E.emplace_back(reg_name, Reg::intermediate, width, false);
    }
    
    getline(audiFile, line);
    while(line != "end"){
        
        iss.clear();
        iss.str(line);
        
        if (print) std::cout << iss.str() << std::endl;
        
        while(iss >> op_name >> op_type_str >> width >> in1 >> in2 >> out){
            
            Op::op_type op_type;
            switch (op_type_str[0]) {
                case 'A':
                    op_type = Op::ADD;
                    break;
                case 'S':
                    op_type = Op::SUB;
                    break;
                case 'M':
                    op_type = Op::MULT;
                    break;
                case 'D':
                    op_type = Op::DIV;
                    break;
            }
            
            array<Reg*, 2> inputs = {getXByName<Reg>(audiGraph.E, in1), getXByName<Reg>(audiGraph.E, in2)};
            Reg* output = getXByName<Reg>(audiGraph.E, out);
            
            audiGraph.V.emplace_back(op_name, op_type, width, inputs, output);
        }
        
        getline(audiFile, line);
        
        // If line contains end, make sure it is end
        if(line.find("end") != std::string::npos){
            line = "end";
        }
        
    }
    
    if (print) std::cout << "end" << std::endl;
    audiFile.close(); // graph read
    
    audiGraph.bit_width = width;
    
    return audiGraph;
}

//MARK: Schedulers
vector<int> LIST_L(ADUIGraph& G, array<int, 4>& a){
    
    // First, check if we have the necessary number of resources...
    array<int, 4> num_needed = {0};
    for(int op = 0; op < G.V.size(); op++){
        num_needed[G.V[op].type]++;
    }
    for(int i = 0; i < a.size(); i++){
        if(num_needed[i] != 0 && a[i] == 0){
            throw invalid_argument("\nThere are not enough resources to schedule this graph!\n\n");
        }
    }
    
    vector<int> t(G.V.size(), -1);
    
    // l is timestep
    int l = 1;
    
    do {
        for(int op = 0; op < G.V.size(); op++){
            
            // An operation is ready to begin when it has valid data on its
            // input registers and enough available resources.
            if (G.V[op].canStart(a) && t[op] == -1){
                
                // Schedule an operations start time
                t[op] = l;
                
                // Reduce the number of available resources
                a[G.V[op].type]--;
            }
            
            // If the current time step is later than op's start time + delay,
            // then it is finished and its resources can be released.
            if(t[op] != -1 && l >= G.V[op].delay + t[op]){
                G.V[op].finished();
                a[G.V[op].type]++;
            }
        }
        
        // Increment time step
        l += 1;
        
    } while (find(t.begin(), t.end(), -1) != t.end() ); // While an operation is unscheduled
    
    return t;
}

void assignStartTimes(ADUIGraph& G, vector<int>& t){
    for(int i = 0; i < G.V.size(); i++){
        G.V[i].start_time = t[i];
    }
}

void assignLifetime(ADUIGraph& G){
    // The lifetime of a register is time from the first write to the last read
    
    // Determine latest timestep...
    int latest_time = 0;
    for(auto op : G.V){
        if(op.start_time > latest_time) { latest_time = op.start_time; }
    }
    
    for(auto op = G.V.begin(); op != G.V.end(); op++){
        
        op->output_reg->lifetime[0] = op->start_time;
        
        for(auto input = op->input_reg.begin(); input != op->input_reg.end(); input ++){
            if ((*input)->type != Reg::output){
                (*input)->lifetime[1] = op->start_time;
            }
        }
        
        if (op->output_reg->type == Reg::output){
            op->output_reg->lifetime[1] = latest_time;
        }
        
    }
}

void printRegLifetimes(vector<Reg>& registers){
    for(auto reg : registers){
        cout << reg.name << ": (" << reg.lifetime[0] << ", " << reg.lifetime[1] << ")" << endl;
    }
}

//MARK: Overloaded << for Reg, Op, VHDLFU & VHDLReg
ostream& operator<<(ostream& os, const Reg& reg)
{
    os << "Name: " << reg.name << endl << "Type: " << reg.type;
    return os;
}

ostream& operator<<(ostream& os, const Op& op)
{
    os << "Name: " << op.name << endl << "Type: " << op.type
    << endl << "Start time: " << op.start_time << endl
    << "Input registers: " << op.input_reg[0]->name << ", " << op.input_reg[1]->name << endl
    << "Output register: " << op.output_reg->name;
    return os;
}


ostream& operator<<(ostream& os, const VHDLReg& reg){
    os << "Name: " << reg.name << endl << "Width: " << reg.width << endl
    << "Input mux: " << reg.input_mux->name << endl
    << "Logical outputs: ";
    for(const auto& out : reg.boundRegs){os << out->name << " ";}
    os << endl << "Output Muxes: ";
    for(const auto& mux : reg.output_muxes){os << mux->name << " ";}
    return os;
}


ostream& operator<<(ostream& os, const VHDLFU& FU){
    os << "Name: " << FU.name << endl << "Width: " << FU.width << endl
    << "Logical Operations: ";
    for(const auto& op : FU.boundOps){os << op->name << " ";}
    os << endl << "Input Muxes: " << FU.input_muxes[0]->name << " " << FU.input_muxes[1]->name << endl
    << "Output Muxes: ";
    for(const auto& mux : FU.output_muxes){os << mux->name << " ";}
    
    return os;
}


//MARK: Aux and helper functions

template VHDLFU* getXByName<VHDLFU>(vector<VHDLFU>& x, string& s);
template VHDLReg* getXByName<VHDLReg>(vector<VHDLReg>& x, string& s);

template <typename X>
X* getXByName(vector<X>& x, string& s){
    for(auto it = x.begin(); it != x.end(); it++){
        if (it->name == s){
            return &(*it);
        }
    }
    return nullptr;
}



void printMuxes(vector<Mux> muxes){
    
    cout << "\n\t\tMuxes\n";
    
    for(int i = 0; i < muxes.size(); i ++){
        cout << muxes[i].name << ": ";
        for(int j = 0; j < muxes[i].num_inputs; j++){
            cout << (muxes[i].log_in[j])->name << " ";
        }
        cout << endl;
    }
}

vector<Op*> subsetOpsByType(vector<Op>& ops, Op::op_type type){
    vector<Op*> subset;
    
    for(auto op = ops.begin(); op != ops.end(); op++){
        if (op->type == type){
            subset.push_back(&(*op));
        }
    }
    
    return subset;
}

string opTypeString(Op::op_type type){
    string str[] = {"ADD", "SUB", "MULT", "DIV"};
    
    return str[type];
}
