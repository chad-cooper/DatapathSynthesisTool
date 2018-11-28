//
//  DCS.cpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "DCS.hpp"

//MARK: Build compatibility graph from AUDI file
CompatibilityGraph<Op, Reg> readAUDI(string audi, bool print){
    
    CompatibilityGraph<Op, Reg> comp;
    
    std::string discard, line, reg_name, op_name, op_type_str, in1, in2, out;
    int width;
    
    ifstream audiFile (audi);
    std::istringstream iss;
    
    getline(audiFile, line); // INPUTS
    
    if (print) (std::cout << line << std::endl);
    
    iss.str(line); // Extract information from line
    
    iss >> discard; // throwaway 'inputs'
    
    while (iss >> reg_name >> width){
        comp.E.emplace_back(reg_name, Reg::input, width, true);
    }
    
    getline(audiFile, line); // OUTPUTS
    if (print) std::cout << line << std::endl;
    iss.clear();
    iss.str(line);
    
    iss >> discard; // throw away 'outputs'
    
    while (iss >> reg_name >> width){
        comp.E.emplace_back(reg_name, Reg::output, width, false);
    }
    
    getline(audiFile, line); // Intermediate registers
    if (print) std::cout << line << std::endl;
    
    iss.clear();
    iss.str(line);
    
    iss >> discard; // throw away 'regs'
    
    while (iss >> reg_name >> width) {
        comp.E.emplace_back(reg_name, Reg::intermediate, width, false);
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
            
            array<Reg*, 2> inputs = {getXByName<Reg>(comp.E, in1), getXByName<Reg>(comp.E, in2)};
            Reg* output = getXByName<Reg>(comp.E, out);
            
            comp.V.emplace_back(op_name, op_type, width, inputs, output);
        }
        
        getline(audiFile, line);
        
        // If line contains end, make sure it is end
        if(line.find("end") != std::string::npos){
            line = "end";
        }
        
    }
    
    if (print) std::cout << "end" << std::endl;
    audiFile.close(); // graph read
    
    return comp;
}

//MARK: LIST_L implementation
vector<int> LIST_L(CompatibilityGraph<Op, Reg>& G, array<int, 4>& a){
    
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


//MARK: Overloaded << for Reg and Op
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

//MARK: Aux and helper functions
template <typename X>
X* getXByName(vector<X>& x, string& s){
    for(auto it = x.begin(); it != x.end(); it++){
        if (it->name == s){
            return &(*it);
        }
    }
    return nullptr;
}

void assignStartTimes(CompatibilityGraph<Op, Reg>& G, vector<int>& t){
    for(int i = 0; i < G.V.size(); i++){
        G.V[i].start_time = t[i];
    }
}
