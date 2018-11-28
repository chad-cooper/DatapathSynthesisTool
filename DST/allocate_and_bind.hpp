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

//template <typename A>
//void allocateAndBind(vector<A>, int);

template <typename A>
void allocateAndBind(vector<A> vertices, int loop_size){
/*
for the size of the loop do
    G(V) <- NULL  // G(V,E) is a compatibility graph
    G(E) <- NULL
    // Build the compatibility graph
    for each operation op1 of type T do
        if(A1 is not in G) then
            G(V) <- G(V) U {A1}
        end if
        for each operation op2 of type T do
            if(A2 is not in G) then
                G(V) <- G(V) U {A2}
            end if
            if( A1 != A2) then
                if( A1 and A2 don't intersect) then
                        G(E) <- G(E) U {(A1, A2)}
                end if
            end if
        end for
    end for
    C <- clique partition(G)
    for each clique c in C  do
        // Call Clique Partitioner
        Bind all the vertices A in c to
        an unbound resource of type A
    end for
end for
*/
    
    
    
    for(int i = 0; i < loop_size; i++){
        vec_mat comp;
        
    } // end for all op_types
    
    
    
}

// Resizes and inserts a value into the adj matrix
void insertAdjMat(vec_mat&, int, int, int);

// Expands a matrix (vector of vectors) to maintain a square shape
void resizeAdjMatrix(vec_mat&, int&);

void printMat(vec_mat&);

#endif /* allocate_and_bind_hpp */
