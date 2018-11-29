//
//  allocate_and_bind.hpp
//  DST
//
//  Created by Chad Cooper on 11/28/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#ifndef allocate_and_bind_hpp
#define allocate_and_bind_hpp

#include "DCS.hpp"
//#include "clique_partition.h"
#include <stdio.h>


//MARK: clique_partition.h
#include <stdlib.h>
#include <assert.h>

#define UNKNOWN -12345
#define MAXCLIQUES 200

#define CLIQUE_UNKNOWN -12345
#define CLIQUE_TRUE 100
#define CLIQUE_FALSE 110

int input_sanity_check(int** compat, int array_dimension);

int output_sanity_check(int array_dimension, int** local_compat, int** compat);

void make_a_local_copy(int** local_compat, int** compat, int nodesize);

int get_degree_of_a_node(int x, int nodesize, int** local_compat, int* node_set);

int select_new_node(int** local_compat, int nodesize, int* node_set);

int form_setY(int* setY, int* current_clique, int** local_compat, int nodesize, int* node_set);

void print_setY(int* setY);

void form_set_Y1(int nodesize, int* set_Y, int* set_Y1, int** sets_I_y, int* node_set);

void form_set_Y2(int nodesize, int* set_Y2, int* set_Y1, int* sizes_of_sets_I_y);

int pick_a_node_to_merge(int* setY, int** local_compat, int* node_set, int nodesize);

void init_clique_set(void);

void print_clique_set(void);

int clique_partition(int** compat, int nodesize);

int call_partition(int**, int);


//MARK: allocate_and_bind.hpp
typedef vector<vector<int>> vec_mat;

// Resizes and inserts a value into the adj matrix
void insertAdjMat(vec_mat&, int, int, int);

// Expands a matrix (vector of vectors) to maintain a square shape
void resizeAdjMatrix(vec_mat&, int&);

void printMat(vec_mat&);

int** vec_mat2c_mat(vec_mat&);

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
    
    cout << "Printing compatibility graph...\n" << endl;
    printMat(comp);
    
    clique_partition(vec_mat2c_mat(comp), int(comp.size()));
    
    print_clique_set();
    
} // end allocateAndBind

#endif /* allocate_and_bind_hpp */
