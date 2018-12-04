//
//  VHDL_Gen.hpp
//  DST
//
//  Created by Chad Cooper on 11/29/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#ifndef VHDL_Gen_hpp
#define VHDL_Gen_hpp

#include "allocate_and_bind.hpp"

// Generate the data path for the AUDI file
void generateDataPath(string&, vector<VHDLFU>&, vector<VHDLReg>&, vector<Op>&, vector<Reg>&, vector<Mux>&, vector<Mux>&);

void generateController(void);

void generateDesgin(void);

string FUEntity(Op::op_type type);

#endif /* VHDL_Gen_hpp */
