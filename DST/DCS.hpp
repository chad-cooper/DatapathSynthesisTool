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
    array<int, 2> lifetime = {{0, 0}}; // lt[0] = first time written to, lt[1] = last time read from
    int lifespan;
    
    // States
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


class ADUIGraph {
    
public:
    
    // Vertices and edges
    vector<Op> V;
    vector<Reg> E;
    
    int bit_width;
    
    void printVertices() {for(auto v: V) {cout << v << endl << endl;}}
    void printEdges() {for(auto e: E) {cout << e << endl << endl;}}
};

template <class P>
class Mux {
public:
    string name;
    int width;
    int num_inputs;
    vector<Reg*> log_in;
    P* phys;
    
    Mux(){};
    
    Mux(string mux_name, int n_inputs, int bit_width){
        name = mux_name;
        num_inputs = n_inputs;
        width = bit_width;
    }
    
    Mux(string mux_name, int n_inputs, int bit_width, vector<Reg*> inputs){
        name = mux_name;
        num_inputs = n_inputs;
        width = bit_width;
        log_in = inputs;
    }
    
    
    friend ostream& operator<<(ostream& os, const Mux<P>& mux){
        os << "Name: " << mux.name << endl << mux.num_inputs << " inputs: ";
        for(const auto& in : mux.log_in){os << in->name << " ";}
        os << endl << "Width: " << mux.width << endl
        << "Input of: " << mux.phys->name;
        
        return os;
    }
    
    
};
//template <typename P>
//ostream& operator<<(ostream& os, const Mux<P>& mux);

class VHDLReg {
public:
    
    VHDLReg(string reg_name, int bit_width, vector<Reg*> bound){
        name = reg_name;
        width = bit_width;
        boundRegs = bound;
    }
    
    string name;
    int width;
    
    Mux<VHDLReg>* input_mux;
    
    // Pointer to logical registers bound to this physical register.
    // The logical registers are the edges in the audi graph ( G.E )
    vector<Reg*> boundRegs;
    
    friend ostream& operator<<(ostream& os, const VHDLReg& reg);
    
};

ostream& operator<<(ostream& os, const VHDLReg& reg);


class VHDLFU {
public:
    
    string name;
    int width;
    
    VHDLFU(string fu_name, int bit_width, vector<Op*> bound){
        name = fu_name;
        width = bit_width;
        boundOps = bound;
        
        bindOperations(boundOps);
        
    }
    
    // Pointer to logical operations that are bound to this physical functional unit.
    // This logical operations are the audi graph vertices ( G.V )
    vector<Op*> boundOps;
    
    array<vector<Reg*>, 2> logical_inputs;
    array<Mux<VHDLFU>*, 2> input_muxes;
    
    vector<Mux<VHDLReg>*> output_muxes;
    
    vector<Reg*> log_out;
    
    void bindOperations(vector<Op*> bound_ops){
        
        for (const auto& op : bound_ops){
            logical_inputs[0].push_back(op->input_reg[0]);
            logical_inputs[1].push_back(op->input_reg[1]);
            log_out.push_back(op->output_reg);
        }
    }
    
    
    friend ostream& operator<<(ostream& os, const VHDLFU& FU);
    
};

ostream& operator<<(ostream& os, const VHDLFU& FU);


//MARK: Graph manipulation functions
ADUIGraph readAUDI(string, bool);

vector<int> LIST_L(ADUIGraph&, array<int, 4>&);

void assignStartTimes(ADUIGraph&, vector<int>&);

void assignLifetime(ADUIGraph&);

//MARK: Print functions
void printRegLifetimes(vector<Reg>&);

template <typename M>
void printMuxes(vector<Mux<M>>);



//MARK: Aux and helper functions
// Return pointer to X based on X.name
template <typename X>
X* getXByName(vector<X>&, string&);

vector<Op*> subsetOpsByType(vector<Op>&, Op::op_type);

string opTypeString(Op::op_type);

#endif /* DCS_hpp */
