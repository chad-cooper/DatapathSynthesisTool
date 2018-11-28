//
//  main.cpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#include "DCS.hpp"

int main(int argc, const char * argv[]) {
    
    CompatibilityGraph<Op, Reg> comp = readAUDI("toyexample.aif", true);
    
    cout << "\nPrinting vertices...\n" <<  endl;
    
    comp.printVertices();
    
    cout << "\nPrinting edges...\n" << endl;
    
    comp.printEdges();

    return 0;
}
