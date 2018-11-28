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
#include <map>

using namespace std;

class Reg {
    enum reg_type {input, intermediate, output}; // A register can have one of these types

public:
    
    // Constructor
    Reg(string init_name, reg_type init_type, int init_width){
        name = init_name;
        type = init_type;
        width = init_width;
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

ostream& operator<<(ostream& os, const Reg& reg)
{
    os << "Name: " << reg.name << endl << "Type: " << reg.type;
    return os;
}

class Op {
    enum op_type {ADD,SUB,MULT,DIV};
    
public:
    
    Op(string init_name, op_type init_type, int init_width){
        name = init_name;
        type = init_type;
        width = init_width;
    }
    
    string name;
    op_type type;
    int width;
    
    
    int start_time = -1;
    int delay = 1;
    
    // I/O Registers
    array<Reg*, 2> input_reg;
    Reg* output_reg;
    
    friend ostream& operator<<(ostream& os, const Op& op);
    
};

ostream& operator<<(ostream& os, const Op& op)
{
    os << "Name: " << op.name << endl << "Type: " << op.type
    << endl << "Start time: " << op.start_time << endl
    << "Input registers: " << op.input_reg[0]->name << ", " << op.input_reg[1]->name << endl
    << "Output register: " << op.output_reg->name;
    return os;
}


template <class VT, class ET>
class CompatibilityGraph {
    
public:
    
    // Vertices and edges
    vector<VT> V;
    vector<ET> E;
    
    int** adj; // Adjacency list for clique partitioning
    
    void buildAdjMatrix(vector<VT>&);
    
    void printVertices();
    void printEdges();
};





#endif /* DCS_hpp */
