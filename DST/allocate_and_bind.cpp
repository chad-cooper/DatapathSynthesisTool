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

template void allocateAndBind<Op>(vector<Op>, int);

template void allocateAndBind<Reg>(vector<Reg>, int);

//MARK: allocateAndBind definition
template <typename A>
void allocateAndBind(vector<A> vertices, int num_vertices){

    vec_mat comp;
    resizeAdjMatrix(comp, num_vertices);
    
    for(int i = 0; i < num_vertices; i++){
        for(int j = 0; j < num_vertices; j++) {
            if(vertices[i] != vertices[j]){
                if (!vertices[i].intersects(vertices[j])){
                    insertAdjMat(comp, i, j, 1);
                }
            }
        }
    }
    
} // end allocateAndBind


