//
//  main.cpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "DCS.hpp"

int main(int argc, const char * argv[]) {
    
    CompatibilityGraph<Op, Reg> comp = readAUDI("lattice.aif", true);
    
    cout << "\nPrinting vertices...\n" <<  endl;
    
    comp.printVertices();
    
    cout << "\nPrinting edges...\n" << endl;
    
    comp.printEdges();
    
    array<int, 4> num_resources;
    
    cout << "Enter the number of available resources in the order of [ADD SUB MULT DIV]:\n";
    cin >> num_resources[0] >> num_resources[1] >> num_resources[2] >> num_resources[3];
    
    vector<int> t;
    try {
         t = LIST_L(comp, num_resources);
    } catch (invalid_argument& e) {
        cout << e.what();
        return 1;
    }
    

    for (int i = 0; i < t.size(); i++){
        cout << "Operation: " << comp.V[i].name << ", start time: " << t[i] << endl;
    }
    
    return 0;
}
