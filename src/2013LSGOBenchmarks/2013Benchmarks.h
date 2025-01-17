#ifndef _2013Benchmarks_
#define _2013Benchmarks_


#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>

// 2013 LSGO benchmark set header files
#include "F1.h"
#include "F2.h"
#include "F3.h"
#include "F4.h"
#include "F5.h"
#include "F6.h"
#include "F7.h"
#include "F8.h"
#include "F9.h"
#include "F10.h"
#include "F11.h"
#include "F12.h"
#include "F13.h"
#include "F14.h"
#include "F15.h"
#include "Benchmarks.h"

using namespace std;

Benchmarks* generateFuncObj(int funcID);
Benchmarks* generateFuncObj(int funcID, int min_value, int max_value, int dim);

#endif
