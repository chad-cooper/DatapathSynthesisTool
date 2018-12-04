//
//  allocate_and_bind.cpp
//  DST
//
//  Created by Chad Cooper on 11/28/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "allocate_and_bind.hpp"
#include "clique_partition.h"


void insertAdjMat(vec_mat& mat, int i, int j, int val){
    int max = (i > j ? i : j) + 1; //
    
    if (mat.size() < max){
        resizeAdjMatrix(mat, max);
    }
    
    mat[i][j] = val;
    mat[j][i] = val;
    
}

void resizeAdjMatrix(vec_mat& mat, int& n){
    while (mat.size() < n){
        vector<int> temp{0};
        mat.push_back(temp);
    }
    for(int i = 0; i < n; i++){
        while (mat[i].size() < n) {
            mat[i].push_back(0);
        }
    }
    
}

void printMat(vec_mat& mat){
    for(auto rows : mat){
        for(auto cols : rows){
            cout << cols << " ";
        }
        cout << endl;
    }
}

int** vec_mat2c_mat(vec_mat& v_mat){
    int dim = int(v_mat.size());
    
    // Create C style matrix for use with clique_partition.
    int** c_mat;
    c_mat = new int* [dim];
    for (int i = 0; i < dim; i++){
        c_mat[i] = new int[dim];
    }
    
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            c_mat[i][j] = v_mat[i][j];
        }
    }
    
    return c_mat;
}

template vec_mat allocateAndBind<Op>(vector<Op>, int);

template vec_mat allocateAndBind<Reg>(vector<Reg>, int);

//MARK: allocateAndBind definition
template <typename A>
vec_mat allocateAndBind(vector<A> vertices, int num_vertices){
    
    vec_mat ret_clique;

    vec_mat comp;
    resizeAdjMatrix(comp, num_vertices);
    
    for(int i = 0; i < num_vertices; i++){
        for(int j = 0; j < num_vertices; j++) {
            if(vertices[i] != vertices[j]){
                if (!vertices[i].intersects(vertices[j])){
                    comp[i][j] = 1;
                }
            }
        }
    }
    
    clique_partition(vec_mat2c_mat(comp), int(comp.size()));
    
    // For each clique in the clique set
    for (int i = 0; clique_set[i].size != UNKNOWN; i++){
        
        vector<int> members;
        
        for( int j = 0; j < clique_set[i].size; j++){
            members.push_back(clique_set[i].members[j]);
        }
            
        ret_clique.push_back(members);
    }
    
    return ret_clique;
} // end allocateAndBind



//MARK: Generate and bind muxes and physical VHDL units
vector<vector<Mux>> generateFUMux(vec_mat& FUsForType, Op::op_type type, int width, int num_inputs) {
    // Number of inputs defaults to two.
    
    // There can be several physical functional units for each logical operation type.
    // We need muxes that determine the data that are fed into the physical FUs.
    vector<vector<Mux>> muxes_for_type;
    
    int n = 0;
    
    // Create a new pair of muxes for each physical FU in this clickset
    for (int i = 0; i < FUsForType.size(); i++) {
        string FUname = opTypeString(type) + to_string(i);
        
        // n is the number of operations bound to this FU
        n = int(FUsForType[i].size());
        
        vector<Mux> FUMux;
        
        muxes_for_type.push_back(FUMux);
        
        // Create a mux with n inputs for both/each inputs into the funcitonal unit
        for(int j = 0; j < num_inputs; j++){
            
            muxes_for_type[i].emplace_back(FUname + "_in"+to_string(j+1),n, width);
            (muxes_for_type[i].end()-1)->type = Mux::FU;
            
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

void bindVHDLFUMux(vector<Mux>& FUMuxes, vector<VHDLFU>& FUs){
    // bindVHDLFUMux binds the multiplexers to functional units.
    
    // There are two muxes for every functional unit
    for(int i = 0; i < FUMuxes.size(); i++){
        
        // Link logical inputs of functional unit to correct mux
        FUMuxes[i].log_in = FUs[i/2].logical_inputs[i%2];
        FUMuxes[i].phys_name = FUs[i/2].name;
//        FUMuxes[i].phys = &FUs[i/2];
        
        // Link the functional unit mux to the input muxes of a functional unit
        FUs[i/2].input_muxes[i%2] = &FUMuxes[i];

    }
}


vector<VHDLFU> byTypeToList(vector<vector<VHDLFU>> FUsByType){
    vector<VHDLFU> FUList;
    
    for(const auto& FUofType: FUsByType){
        for(const auto& FU: FUofType){
            FUList.push_back(FU);
        }
    }
    
    return FUList;
}

vector<Mux> byTypeToList(vector<vector<vector<Mux>>> FUMuxesByType){
    vector<Mux> FUMuxList;
    
    for(const auto& FUMuxofType : FUMuxesByType){
        for(const auto& FUMuxPair : FUMuxofType){
            for(const auto& FUMux: FUMuxPair){
                FUMuxList.push_back(FUMux);
            }
        }
    }
    
    return FUMuxList;
}

vector<Mux> generateREGMux(vec_mat& clickset, int width, vector<Reg>& E){
    // Create vector of muxes to return
    vector<Mux> muxes;
    
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
        muxes.emplace_back("Reg" + mux_num + "Mux" , n, width, logical_regs);
        (muxes.end()-1)->type = Mux::RU;
        
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

void bindVHDLRegMux(vector<Mux>& REGMuxes, vector<VHDLReg>& phys_reg){
    for(int r = 0; r < REGMuxes.size(); r++){
        phys_reg[r].input_mux = &REGMuxes[r];
        REGMuxes[r].phys_name = phys_reg[r].name;
//        REGMuxes[r].phys = &(phys_reg[r]);
    }
}

template void linkLogicalOut<VHDLFU>(vector<VHDLFU>&, vector<Mux>&);
template void linkLogicalOut<VHDLReg>(vector<VHDLReg>&, vector<Mux>&);

template <typename VHDLU>
void linkLogicalOut(vector<VHDLU>& Us, vector<Mux>& Muxes){
    // Link FU outputs to register units based on logical outputs/inputs
    for(auto& U : Us){
        
        for(auto& log_out : U.log_out){
            
            for(auto& mux : Muxes){
                if(find(mux.log_in.begin(), mux.log_in.end(), log_out) != mux.log_in.end() ){
                    //FU.output_muxes.push_back(mux);
                    U.output_muxes.push_back(&mux);
                }
            }
        }
    }
    
}
