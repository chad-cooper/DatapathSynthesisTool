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
#include "./clique_partition.c"

//template <typename A>
void allocate(vector<Op>&);

//template <typename B>
void bind(vector<Reg>&);

#endif /* allocate_and_bind_hpp */
