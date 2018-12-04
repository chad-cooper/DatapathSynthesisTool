//
//  allocate_and_bind.hpp
//  DST
//
//  Created by Chad Cooper on 11/28/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#ifndef allocate_and_bind_hpp
#define allocate_and_bind_hpp

#include <stdio.h>
#include "DCS.hpp"

// Resizes and inserts a value into the adj matrix
void insertAdjMat(vec_mat&, int, int, int);

// Expands a matrix (vector of vectors) to maintain a square shape
void resizeAdjMatrix(vec_mat&, int&);

void printMat(vec_mat&);

int** vec_mat2c_mat(vec_mat&);

//MARK: allocateAndBind definition
template <typename A>
vec_mat allocateAndBind(vector<A> vertices, int num_vertices);

//MARK: Generate and bind functions
vector<vector<Mux>> generateFUMux(vec_mat& FUsForType, Op::op_type type, int width, int num_inputs = 2);

vector<VHDLFU> generateVHDLFUs(vector<Op>& V, vec_mat& FUsForType, Op::op_type type, int width, int num_inputs= 2);

vector<Mux> generateREGMux(vec_mat&, int, vector<Reg>&);

vector<VHDLReg> generateVHDLRegs(vec_mat& clickset, int width, vector<Reg>& E);

void bindVHDLFUMux(vector<Mux>& FUMuxes, vector<VHDLFU>& FUs);

void bindVHDLRegMux(vector<Mux>& REGMuxes, vector<VHDLReg>& phys_reg);

template <typename VHDLU>
void linkLogicalOut(vector<VHDLU>&, vector<Mux>&);

vector<Mux> byTypeToList(vector<vector<vector<Mux>>> FUMuxesByType);

vector<VHDLFU> byTypeToList(vector<vector<VHDLFU>> FUsByType);
#endif /* allocate_and_bind_hpp */
