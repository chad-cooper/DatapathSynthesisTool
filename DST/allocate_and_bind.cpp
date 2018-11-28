//
//  allocate_and_bind.cpp
//  DST
//
//  Created by Chad Cooper on 11/28/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//


#include "allocate_and_bind.hpp"
#include "./clique_partition.c"

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
