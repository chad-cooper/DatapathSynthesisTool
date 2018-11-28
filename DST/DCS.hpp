//
//  DCS.hpp
//  DST
//
//  Created by Chad Cooper on 11/27/18.
//  Copyright © 2018 Chad Cooper. All rights reserved.
//

#ifndef DCS_hpp
#define DCS_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Reg {
public:
    enum reg_type {input, intermediate, output}; // A register can have one of these types
    
    // Constructor
    Reg(string init_name, reg_type init_type, int init_width, bool valid = true){
        name = init_name;
        type = init_type;
        width = init_width;
        dataValid = valid;
    }
    
    string name; // Register name
    reg_type type; // Register type
    int width; // Bit width of register
    array<int, 2> lifetime = {0, 0}; // lt[0] = first time written to, lt[1] = last time read from
    int lifespan = lifetime[1] - lifetime[0];
    
    bool isWriting = false;
    bool isReading = false;
    bool dataValid = false;
    
    friend ostream& operator<<(ostream& os, const Reg& reg);
    
};

ostream& operator<<(ostream& os, const Reg& reg);

class Op {
public:
    enum op_type {ADD,SUB,MULT,DIV};
    
    Op(string init_name, op_type init_type, int init_width, array<Reg*, 2> ins, Reg* out){
        name = init_name;
        type = init_type;
        width = init_width;
        input_reg = ins;
        output_reg = out;
    }
    
    string name;
    op_type type;
    int width;
    
    int start_time = -1;
    int delay = 1;
    
    // I/O Registers
    array<Reg*, 2> input_reg;
    Reg* output_reg;
    
    bool isReady() {return input_reg[0]->dataValid && input_reg[1]->dataValid;}
    bool canStart(array<int, 4>& resources) {return isReady() && resources[type] > 0;}
    
    void finished() {output_reg->dataValid = true;}
    
    
    
    friend ostream& operator<<(ostream& os, const Op& op);
    
};

ostream& operator<<(ostream& os, const Op& op);


template <class VT, class ET>
class CompatibilityGraph {
    
public:
    
    // Vertices and edges
    vector<VT> V;
    vector<ET> E;
    
    int** adj; // Adjacency list for clique partitioning
    
    void buildAdjMatrix(vector<VT>&);
    
    void printVertices() {for(auto v: V) {cout << v << endl << endl;}}
    void printEdges() {for(auto e: E) {cout << e << endl << endl;}}
};


// Return pointer to X based on X.name
template <typename X>
X* getXByName(vector<X>&, string&);

CompatibilityGraph<Op, Reg> readAUDI(string, bool);

vector<int> LIST_L(CompatibilityGraph<Op, Reg>&, array<int, 4>&);

void assignStartTimes(CompatibilityGraph<Op, Reg>&, vector<int>&);


#endif /* DCS_hpp */
