//
//  main.cpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

//#include "DCS.hpp"
#include "allocate_and_bind.hpp"

int main(int argc, const char * argv[]) {
    
    bool print = true;
    
    CompatibilityGraph<Op, Reg> comp = readAUDI("toyexample.aif", print);
    
    array<int, 4> num_resources = {1, 1, 1, 1};
    
//    cout << "Enter the number of available resources in the order of [ADD SUB MULT DIV]:\n";
//    cin >> num_resources[0] >> num_resources[1] >> num_resources[2] >> num_resources[3];
    
    vector<int> t;
    try {
         t = LIST_L(comp, num_resources);
    } catch (invalid_argument& e) {
        cout << e.what();
        return 1;
    }
    
    cout << endl;
    for (int i = 0; i < t.size(); i++){
        cout << "Operation: " << comp.V[i].name << ", start time: " << t[i] << endl;
    }
    
    cout << endl;
    
    assignStartTimes(comp, t);
    
    if(print) comp.printVertices();
    
    // Allocate and bind funcitonal units
    vector<Op> res_type;
    vector<vec_mat> FUs;
    for (int i = 0; i < num_resources.size(); i++){
        // Clear mat
        res_type.clear();
        
        for(Op res : comp.V) {if (res.type == i){res_type.push_back(res);}}
        
        FUs.push_back(allocateAndBind(res_type, int(res_type.size())));
    }
    
    for(auto FU : FUs){
        cout << "\n\nClique...\n\n";
        printMat(FU);
        
    }
    
    cout << endl;
    
    return 0;
}
