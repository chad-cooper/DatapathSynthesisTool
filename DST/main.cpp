//
//  main.cpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//


#include "VHDL_Gen.hpp"

#define NUM_RES_TYPES 4

int main(int argc, const char * argv[]) {
    
    bool print = true;
    
    string filename = "toyexample";
    
    ADUIGraph audi = readAUDI(filename + ".aif", print);
    
    array<int, NUM_RES_TYPES> available_resources = {2, 2, 2, 2};
    
    //    cout << "Enter the number of available resources in the order of [ADD SUB MULT DIV]:\n";
    //    cin >> num_resources[0] >> num_resources[1] >> num_resources[2] >> num_resources[3];
    
    vector<int> t;
    try {
        t = LIST_L(audi, available_resources);
    } catch (invalid_argument& e) {
        cout << e.what();
        return 1;
    }
    
    // Print start times
    cout << endl;
    for (int i = 0; i < t.size(); i++){
        cout << "Operation: " << audi.V[i].name << ", start time: " << t[i] << endl;
    }
    
    cout << endl;
    
    assignStartTimes(audi, t);
    
    
    // Allocate and bind functional units
    vector<Op> res_type;
    vector<vec_mat> FUCliquesByType;
    for (int i = 0; i < NUM_RES_TYPES; i++){
        // Clear mat
        res_type.clear();
        
        for(Op res : audi.V) {if (res.type == i){res_type.push_back(res);}}
        
        FUCliquesByType.push_back(allocateAndBind(res_type, int(res_type.size())));
    }
    
    string type;
    for(int i = 0; i < NUM_RES_TYPES; i++){
        switch (i) {
            case 0:
                type = "ADD";
                break;
            case 1:
                type = "SUB";
                break;
            case 2:
                type = "MULT";
                break;
            case 3:
                type = "DIV";
                break;
        }
        cout << "\n\nCliques for " << type << "...\n\n";
        printMat(FUCliquesByType[i]);
        
    }
    
    cout << endl;
    
    // Determine the lifetime of all the edges
    assignLifetime(audi);
    
    vec_mat reg_cliques = allocateAndBind(audi.E, int(audi.E.size()));
    
    printRegLifetimes(audi.E);
    
    vector<vector<vector<Mux<VHDLFU>>>> FUMuxesByType;
    vector<vector<VHDLFU>> FUsByType;
    for(int i = 0; i < NUM_RES_TYPES; i++){
        FUMuxesByType.push_back(generateFUMux(FUCliquesByType[i], Op::op_type(i), audi.bit_width));
        
        FUsByType.push_back(generateVHDLFUs(audi.V, FUCliquesByType[i], Op::op_type(i), audi.bit_width));
    }
    
    vector<Mux<VHDLReg>> REGMuxes = generateREGMux(reg_cliques, audi.bit_width, audi.E);
    vector<VHDLReg> RegList = generateVHDLRegs(reg_cliques, audi.bit_width, audi.E);
    
    vector<Mux<VHDLFU>> FUMuxes = byTypeToList(FUMuxesByType);
    
    vector<VHDLFU> FUList = byTypeToList(FUsByType);
    
    bindVHDLFUMux(FUMuxes, FUList);
    
    bindVHDLRegMux(REGMuxes, RegList);
    
    linkLogicalOut(FUList, REGMuxes);
    
    
    cout << "\n\nFU MUXES...\n\n";
    for(auto mux : FUMuxes){
        cout << mux << endl << endl;
    }
    
    cout << "\n\nReg MUXES...\n\n";
    
    for(const auto& mux : REGMuxes){
        cout << mux << endl << endl;
    }
    
    cout << "\n\nFunctional Units\n\n";
    
    for(const auto& FU : FUList ){
        cout << FU << endl << endl;
    }
    
    
        cout << "\n\nRegister Units\n\n";
    for(const auto& R : RegList ){
        cout << R << endl << endl;
    }

    generateDataPath(filename, FUList, RegList, audi.V, audi.E, FUMuxes, REGMuxes);
    
    return 0;
}
