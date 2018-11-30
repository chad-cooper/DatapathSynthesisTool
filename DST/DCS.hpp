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

typedef vector<vector<int>> vec_mat;

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
    int lifespan;
    
    // States
    bool isWriting = false;
    bool isReading = false;
    bool dataValid = false;
    
    void setLifespan(){lifespan = lifetime[1] - lifetime[0];}
    
    bool intersects(const Reg& other) const {
        
        // Checks if two
        bool disjoint = (lifetime[1] < other.lifetime[0] || lifetime[0] > other.lifetime[1]);
        
        return !disjoint;
        
    }
    
    
    bool operator==(const Reg &other) const {
        bool eq = true;
        
        eq = (name == other.name) && eq;
        
        eq = (type == other.type) && eq;
        
        eq = (width == other.width) && eq;
        
        eq = (lifetime[0] == other.lifetime[0] && lifetime[1] == other.lifetime[1]) && eq;
        
        eq = (lifetime == other.lifetime) && eq;
        
        return eq;
    }
    
    bool operator!=(const Reg &other) const {
        return !(*this == other);
    }
    
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
    
    bool intersects(const Op& other) const {return start_time == other.start_time;}
    
    
    bool operator==(const Op &other) const {
        // Compare the values, and return the result.
        bool eq = true;
        
        eq = (name == other.name) && eq;
        
        eq = (type == other.type) && eq;
        
        eq = (width == other.width) && eq;
        
        eq = (start_time == other.start_time) && eq;
        
        eq = (delay == other.delay) && eq;
        
        eq = (input_reg[0] == other.input_reg[0] && input_reg[1] == other.input_reg[1]) && eq;
        
        eq = (output_reg == other.output_reg) && eq;
        
        return eq;
        
    }
    
    bool operator!=(const Op &other) const {
        return !(*this == other);
    }
    
    friend ostream& operator<<(ostream& os, const Op& op);
    
};

ostream& operator<<(ostream& os, const Op& op);


template <class VT, class ET>
class CompatibilityGraph {
    
public:
    
    // Vertices and edges
    vector<VT> V;
    vector<ET> E;
    
    void printVertices() {for(auto v: V) {cout << v << endl << endl;}}
    void printEdges() {for(auto e: E) {cout << e << endl << endl;}}
};

class Mux {
public:
    int width;
    int num_inputs;
    
    Mux(int n_inputs, int bit_width){
        num_inputs = n_inputs;
        width = bit_width;
    }
};


//MARK: Aux and helper functions
// Return pointer to X based on X.name
template <typename X>
X* getXByName(vector<X>&, string&);

CompatibilityGraph<Op, Reg> readAUDI(string, bool);

vector<int> LIST_L(CompatibilityGraph<Op, Reg>&, array<int, 4>&);

void assignStartTimes(CompatibilityGraph<Op, Reg>&, vector<int>&);

void assignLifetime(CompatibilityGraph<Op, Reg>&);

void printRegLifetimes(vector<Reg>&);

//TODO: Add generateFUMux here
vector<Mux> generateFUMux(vec_mat&, int, int);

//TODO: Add genereateREGMux here
vector<Mux> generateREGMux(vec_mat&, int);


#endif /* DCS_hpp */
