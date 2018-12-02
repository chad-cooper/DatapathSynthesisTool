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

vector<vector<Mux<VHDLFU>>> generateFUMux(vec_mat& FUsForType, Op::op_type type, int width, int num_inputs) {
    // Number of inputs defaults to two.
    
    // There can be several physical functional units for each logical operation type.
    // We need muxes that determine the data that are fed into the physical FUs.
    vector<vector<Mux<VHDLFU>>> muxes_for_type;
    
    int n = 0;
    
    // Create a new pair of muxes for each physical FU in this clickset
    for (int i = 0; i < FUsForType.size(); i++) {
        string FUname = opTypeString(type) + to_string(i);
        
        // n is the number of operations bound to this FU
        n = int(FUsForType[i].size());
        
        vector<Mux<VHDLFU>> FUMux;
        
        muxes_for_type.push_back(FUMux);
        
        // Create a mux with n inputs for both/each inputs into the funcitonal unit
        for(int j = 0; j < num_inputs; j++){
            
            muxes_for_type[i].emplace_back(FUname + "_in"+to_string(j+1),n, width);
            
        }
    }
    
    return muxes_for_type;
}

vector<VHDLFU> generateVHDLFUs(vector<Op>& V, vec_mat& FUsForType, Op::op_type type, int width, int num_inputs){

    vector<VHDLFU> phys_FU;

    vector<Op*> ops_of_this_type = subsetOpsByType(V, type);
    
    // Create a physical function unit for each clickset for this operation
    for (int i = 0; i < FUsForType.size(); i++){
        string FUname = opTypeString(type) + to_string(i);
        
        vector<Op*> bound_ops;
        
        for(auto idx : FUsForType[i]){
            bound_ops.push_back(ops_of_this_type[idx]);
        }
        
        phys_FU.emplace_back(FUname, width, bound_ops);
        
    }
    return phys_FU;
}

vector<VHDLFU> bindVHDLFUMux(vector<vector<vector<Mux<VHDLFU>>>>& FUMuxes, vector<vector<VHDLFU>>& FUs){
    // bindVHDLFUMux binds the multiplexers to functional units.
    
    vector<VHDLFU> boundFUmux;
    
    // For each operation type
    for(int t = 0; t < FUMuxes.size(); t++){
        // for each functional unit of this type
        for(int f = 0; f < FUMuxes[t].size(); f++){
            
            FUMuxes[t][f][0].log_in = FUs[t][f].logical_inputs[0];
            FUMuxes[t][f][1].log_in = FUs[t][f].logical_inputs[1];
            
            FUs[t][f].input_muxes[0] = &FUMuxes[t][f][0];
            FUs[t][f].input_muxes[1] = &FUMuxes[t][f][1];
            
            boundFUmux.push_back(FUs[t][f]);
            
            FUMuxes[t][f][0].phys = &(*(boundFUmux.end()-1));
            FUMuxes[t][f][1].phys = &(*(boundFUmux.end()-1));
            
        }
    }
    
    return boundFUmux;
}

vector<Mux<VHDLReg>> generateREGMux(vec_mat& clickset, int width, vector<Reg>& E){
    // Create vector of muxes to return
    vector<Mux<VHDLReg>> muxes;
    
    int n = 0;
    
    // Generate a new mux for each clickset
    for (int R = 0; R < clickset.size(); R++){
        
        // n is the number of inputs
        n = int(clickset[R].size());
        
        vector<Reg*> logical_regs;
        for (auto idx : clickset[R]) {
            logical_regs.push_back(&(E[idx]));
        }
        
        string mux_num = to_string(R);
        
        // Create a mux with name MuxR, n inputs, width w and logical register inputs.
        muxes.emplace_back("RegMux" + mux_num, n, width, logical_regs);
        
    }
    
    return muxes;
}

vector<VHDLReg> generateVHDLRegs(vec_mat& clickset, int width, vector<Reg>& E){
    // Create vector of registers to return
    vector<VHDLReg> phys_regs;
    
    // Generate a new physical register for each clickset
    for (int R = 0; R < clickset.size(); R++){
        
        vector<Reg*> logical_regs;
        for (auto idx : clickset[R]) {
            logical_regs.push_back(&(E[idx]));
        }
        
        string reg_num = to_string(R);
        
        // Create a register with name RegR, width w and list of attached logical registers.
        phys_regs.emplace_back("Reg" + reg_num, width, logical_regs);
    }
    
    return phys_regs;
}

void bindVHDLRegMux(vector<Mux<VHDLReg>>& REGMuxes, vector<VHDLReg>& phys_reg){
    for(int r = 0; r < REGMuxes.size(); r++){
        phys_reg[r].input_mux = &REGMuxes[r];
        REGMuxes[r].phys = &(phys_reg[r]);
    }
}


template void printMuxes<VHDLFU>(vector<Mux<VHDLFU>> muxes);

template void printMuxes<VHDLReg>(vector<Mux<VHDLReg>> muxes);

template <class M>
void printMuxes(vector<Mux<M>> muxes){
    
    cout << "\n\t\tMuxes\n";
    
    for(int i = 0; i < muxes.size(); i ++){
        cout << muxes[i].name << ": ";
        for(int j = 0; j < muxes[i].num_inputs; j++){
            cout << (muxes[i].log_in[j])->name << " ";
        }
        cout << "\n";
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
