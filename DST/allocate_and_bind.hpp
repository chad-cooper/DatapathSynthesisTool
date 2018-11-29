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


typedef vector<vector<int>> vec_mat;

// Resizes and inserts a value into the adj matrix
void insertAdjMat(vec_mat&, int, int, int);

// Expands a matrix (vector of vectors) to maintain a square shape
void resizeAdjMatrix(vec_mat&, int&);

void printMat(vec_mat&);

int** vec_mat2c_mat(vec_mat&);

//MARK: allocateAndBind definition
template <typename A>
vec_mat allocateAndBind(vector<A> vertices, int num_vertices);




#endif /* allocate_and_bind_hpp */
