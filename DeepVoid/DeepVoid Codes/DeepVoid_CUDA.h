#pragma once

template <typename T> 
/*extern*/ void
CUDA_PatchMatch_template(const unsigned char * h_imgb,	// input: the base gray image
				const unsigned char * h_imgm,	// input: tha matching gray image
				int w_b, int h_b,				// input: the width and height of base image
				int w_m, int h_m,				// input: the width and height of matching image
				T * h_depth,					// output:the estimated depth map of the base image
				T * h_alpha,					// output:the estimated surface normal map of the base image
				T * h_beta,						// output:the estimated surface normal map of the base image
				int nThreads_w,					// input: the number of threads per row of the thread block
				int nThreads_h,					// input: the number of threads per column of the thread block
				unsigned long long randSeed,	// input: the random seed
				T min_d, T max_d,				// input: the minimum and maximum depth
				T min_a, T max_a,				// input: the minimum and maximum alpha
				T min_b, T max_b				// input: the minimum and maximum beta
				);