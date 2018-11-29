////
////  clique_partition.h
////  DST
////
////  Created by Chad Cooper on 11/29/18.
////  Copyright © 2018 Chad Cooper. All rights reserved.
////
//
//#ifndef clique_partition_h
//#define clique_partition_h
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//
//#define UNKNOWN -12345
//#define MAXCLIQUES 200
//
//#define CLIQUE_UNKNOWN -12345
//#define CLIQUE_TRUE 100
//#define CLIQUE_FALSE 110
//
//int input_sanity_check(int** compat, int array_dimension);
//
//int output_sanity_check(int array_dimension, int** local_compat, int** compat);
//
//void make_a_local_copy(int** local_compat, int** compat, int nodesize);
//
//int get_degree_of_a_node(int x, int nodesize, int** local_compat, int* node_set);
//
//int select_new_node(int** local_compat, int nodesize, int* node_set);
//
//int form_setY(int* setY, int* current_clique, int** local_compat, int nodesize, int* node_set);
//
//void print_setY(int* setY);
//
//void form_set_Y1(int nodesize, int* set_Y, int* set_Y1, int** sets_I_y, int* node_set);
//
//void form_set_Y2(int nodesize, int* set_Y2, int* set_Y1, int* sizes_of_sets_I_y);
//
//int pick_a_node_to_merge(int* setY, int** local_compat, int* node_set, int nodesize);
//
//void init_clique_set(void);
//
//void print_clique_set(void);
//
//int clique_partition(int** compat, int nodesize);
//
//#endif /* clique_partition_h */
