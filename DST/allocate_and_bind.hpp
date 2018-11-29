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

//MARK: allocateAndBind definition
template <typename A>
void allocateAndBind(vector<A> vertices, int num_vertices){
/*
for the size of the loop do
    G(V) <- NULL  // G(V,E) is a compatibility graph
    G(E) <- NULL
    // Build the compatibility graph
    for each vertex A1 do
        if(A1 is not in G) then
            G(V) <- G(V) U {A1}
        end if
        for each vertex A2
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

#endif /* allocate_and_bind_hpp */
