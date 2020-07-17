/************************************************************************************/
/* All header files are included in this file, all you need to do is just including */
/* this file in stdafx.h, job's done.                                               */
/* Author: Johnny Zhu (DeepVoid) in Melbourne                      Date:2014.03.02  */
/************************************************************************************/

#pragma once

#include <vector>
#include <queue>
#include <deque>
#include <set> // 20160317

// 20160313
#ifndef _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#endif

#include <hash_set>

#include <map> // 20150508
#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric> // 20200715, in order to use std::accumulate()
#include <omp.h> // 20150211, to use OpenMP

#include "DeepVoid Codes/DeepVoid_DataType.h"
#include "DeepVoid Codes/DeepVoid_Calib3D.h"

// zhaokunz, 20140305, add levmar and sba lib
// #include "levmar and sba/levmar.h"
// #include "levmar and sba/sba.h"

#include "DeepVoid Codes/DeepVoid_Levmar_Wrapper.h"
#include "DeepVoid Codes/DeepVoid_SBA_Wrapper.h"

#include "DeepVoid Codes/DeepVoid_StereoVision.h"
#include "DeepVoid Codes/DeepVoid_Derivatives.h"

// 20170430
#include "DeepVoid Codes/DeepVoid_CUDA.h"