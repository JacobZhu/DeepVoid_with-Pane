/*************************************************************************************/
/* All stereo vision related functions are here, including dense matching algorithms.*/
/* Author: Johnny Zhu (朱肇昆)                         Date:2013.12.19 Melbourne     */
/*************************************************************************************/

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

namespace DeepVoid
{
// Quadratic curve fitting
void QuadCurveFit(double * x, double * f,					 // 输入：所有采样点的 x 和 f 值
				  int n,									 // 输入：采样点个数
				  double & a0, double & a1, double & a2		 // 输出：拟合出来的二次曲线的系数 f = a0 + a1x + a2x^2
				  );

// compute the matching cost, proposed by S. Birchfield and C. Tomasi, for a single pixel.
double ComputeMatchingCostforOnePixel_BT(int x, int y,		// input: the coordinates in base image of the checked pixel
										 int d,				// input: the disparity to be checked
										 int w, int h,		// input: the width and height of stereo images
										 BYTE ** imgb,		// input: h*w, the rectified grayscale base image
										 BYTE ** imgm		// input: h*w, the rectified grayscale matching image
										 );

// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
void GenerateDSI_BT(int w, int h,			// input: the width and height of stereo images
					BYTE ** imgb,			// input: h*w, the rectified grayscale base image
					BYTE ** imgm,			// input: h*w, the rectified grayscale matching image
					int dmin,				// input: scalar, the minimal disparity
					int dmax,				// input: scalar, the maximal disparity
					double *** DSI,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
					bool bMB = false		// input: is matching in the reverse direction, i.e. matching image is treated as base image and vice versa
					);

// Cost aggregation along one path for one given pixel based on scanline
// optimizaiton or dynamic programming?
void CostAggalongOnePathforOnePixel(int x, int y,			// input: the coordinates in base image of the checked pixel
									double ang,				// input: the angle between scanline and positive x-axis, starting from positive x-axis
									int w, int h, int nd,	// input: the dimensions of the Disparity Space Image
									double *** DSI,			// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
									double P1,				// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
									double P2,				// input: scalar, a larger constant penalty for all larger disparity changes
									double * Lr				// output:nd-vector,the aggregated cost vector along one path for given pixel (x, y)
									);

// Critical step in cost aggregation along one path for one given pixel based on scanline optimizaiton or dynamic programming?
void CostAggalongOnePathforOnePixel_Step(int x, int y,				// input: the coordinates in base image of the checked pixel
										 int nd,					// input: disparity range
										 double *** DSI,			// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
										 double * Lr,				// input & output: nd_vector, as input it's the recursively aggregated cost up to the previous pixel before current one along the path
																	// as output it's the recursively aggregated cost up to the current pixel along the path
										 double P1,					// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
										 double P2,					// input: scalar, a larger constant penalty for all larger disparity changes
										 BYTE * pGrayDiff = NULL	// input: the difference between the gray level of currently checked pixel and the previous one
										 );

// Critical step in cost aggregation along one path for one given pixel based on scanline optimizaiton or dynamic programming?
void CostAgg_Step(int x, int y,		// input: the coordinates in base image of the checked pixel i.e. Lr
				  int nd,			// input: disparity range
				  double *** DSI,	// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
				  int x_0, int y_0,	// input: the coordinates in base image of the previous pixel i.e.Lr_1 
				  double *** C_dir,	// input&output: h*w*nd, the 3D aggregated costs along each direction
				  double P1,		// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
				  double P2			// input: scalar, a larger constant penalty for all larger disparity changes
				  );

// Cost aggregation along multiple paths for one given pixel based on scanline
// optimizaiton or dynamic programming?
void CostAggalongMultiPathsforOnePixel(int x, int y,			// input: the coordinates in base image of the checked pixel
									   int nPaths,				// input: the amount of scanlines
									   double * angs,			// input: contains angles of each path
									   int w, int h, int nd,	// input: the dimensions of the Disparity Space Image
									   double *** DSI,			// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
									   double P1,				// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
									   double P2,				// input: scalar, a larger constant penalty for all larger disparity changes
									   double * S				// output:nd-vector, the aggregated cost vector along multiple paths for given pixel (x, y)
									   );

enum SGM_PATHS
{
	SGM_PATHS_8,
	SGM_PATHS_16
};

// Semi global matching algorithm, implemented based on <Stereo Processing by Semiglobal Matching and Mutual Information> 2008
void SemiGlobalMatching(int w, int h,					// input: the width and height of stereo images
						BYTE ** imgb,					// input: h*w, the rectified grayscale base image
						BYTE ** imgm,					// input: h*w, the rectified grayscale matching image
						int dmin,						// input: scalar, the minimal disparity
						int dmax,						// input: scalar, the maximal disparity
						double P1,						// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
						double P2,						// input: scalar, a larger constant penalty for all larger disparity changes
						double ** DI,					// output:h*w, the disparity image
						SGM_PATHS nDir = SGM_PATHS_8,	// input: the amount of scanlines*/
						int constcCheck = -1,			// input: if matching is conducted reversely, i.e. treat the match image as a base image and vice versa.
						bool bSubPix = true				// input: if sub pixel interpolation for sub pixel disparities is applied
						);

// Semi global matching algorithm, implemented based on <Stereo Processing by Semiglobal Matching and Mutual Information> 2008
void SemiGlobalMatching_givenDSI(int w, int h,					// input: the width and height of stereo images
								 BYTE ** imgb,					// input: h*w, the rectified grayscale base image
								 BYTE ** imgm,					// input: h*w, the rectified grayscale matching image
								 int dmin,						// input: scalar, the minimal disparity
								 int dmax,						// input: scalar, the maximal disparity
								 double *** DSI,				// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
								 double P1,						// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
								 double P2,						// input: scalar, a larger constant penalty for all larger disparity changes
								 double ** DI,					// output:h*w, the disparity image
								 SGM_PATHS nDir = SGM_PATHS_8,	// input: the amount of scanlines*/
								 bool bSubPix = true			// input: if sub pixel interpolation for sub pixel disparities is applied
								 );

// convert a int matrix to a image
void ConvertMtx2Image_int(int w, int h,		// input: the width and height of stereo images
						  int ** mtx,		// input: the matrix to be converted
						  BYTE ** img		// output:the output image
						  );

// convert a int matrix to a image
void ConvertMtx2Image_double(int w, int h,	// input: the width and height of stereo images
							 double ** mtx,	// input: the matrix to be converted
							 BYTE ** img	// output:the output image
							 );

// A = A + B
void Add3DArrays_double(int w, int h, int d,
						double *** A,
						double *** B
						);


/*------------------------------------------------------------------------------------*/
/* Graph related ---------------------------------------------------------------------*/

// Definition of class CEdge
class CEdge
{
public:
	CEdge();
	CEdge(const CEdge & edge);					
	virtual ~CEdge();

public:
	int from;						// 0-based index of the vertex at which this edge starts
	int to;							// 0-based index of the vertex to which this edge points
	double weight;					// weight assigned to this edge

public:
	CEdge & operator = (const CEdge & otherEdge);	// implementation of operator =, graph = othergraph

protected:
private:
};

// Definition of class CVertex
class CVertex
{
public:
	double key;						// key value of this vertex
	vector <int> in_v;				// 0-based indices of all vertices that have flows into this vertex
	vector <CEdge> out_e;			// all the edges that have flows out of this vertex

protected:
private:
};

// Definition of class CGraph
class CGraph
{
public:
	CGraph();
	CGraph(int n);						// create a graph with n vertices
	CGraph(const CGraph & graph);					
	virtual ~CGraph();

public:
	int n_v;							// the total number of vertices contained in this graph
	CVertex * v;						// all the vertices contained in this graph
	bool ** adjMat;						// n_v*n_v adjacency matrix, if there is an edge from vertex i to j, then adjMat[i][j]=TRUE, otherwise FALSE.

public:
	void create(int n);									// recreate a graph with n vertices
	void release();										// release all resources
	void addEdge(int from, int to, double weight);		// add an edge with a direction if it does not exit
	void deleteEdge(int from, int to);					// delete the designated edge if it exits
	BOOL isEdge(int from, int to) const;					// if there is a edge from from to to
	BOOL getEdgeValue(int from, int to, double & w) const;	// get the weight of the designated edge, if the edge exists return TRUE, otherwise FALSE
	BOOL setEdgeValue(int from, int to, double w);		// set the weight of the designated edge, if the edge exists return TRUE, otherwise FALSE
	BOOL addEdgeValue(int from, int to, double w);		// add the weight to the designated edge, if the edge exists return TRUE, otherwise FALSE

	// operator
	CGraph & operator = (const CGraph & otherGraph);	// implementation of operator =, graph = othergraph

protected:
private:
};

// The Breadth-first search algorithm finds the least number of edges between designated vertex a and b, neglecting the weight of each edge
// this function return the number of least edges found between vertex a and b if there is this path, otherwise return -1
int BFS(const CGraph & graph,		// input:  the graph to be searched in
		int v_a, int v_b,			// input:  indices of vertices a and b
		vector <int> & path,		// output: the shortest path, with indices of all vertices in this output if there is a path from a to b;
									//		   if there is no path from a to b this var indicates which vertices are visited from a which are not
		double * thresh_w = NULL,	// input:  threshold, if the edge weight between two vertices <= this threshold, then these two vertices will still not be considered connected
		double * min_w = NULL		// output: if there is this shortest path, then return the smallest weight
		);

// Classical maxflow/mincut algorithm that finds the maximum flow from source s to sink t
// The algorithm is identical to the Ford-Fulkerson algorithm,
// except that the search order when finding the augmenting path is defined.
// The path found must be a shortest path that has available capacity.
// This can be found by a breadth-first search, as we let edges have unit length.
// Complexity is O(VE^2)
double MaxFlowMinCut_EdmondsKarp(CGraph & graph,		// input & output: the residual graph, with all reversed paths already constructed
								 int s, int t,			// input: the indices of the source s and sink t
								 vector <int> & segs	// output: n-vector, n is the number of vertices in residual graph, this is the segmentation obtained when max flow is reached,
														//         segs[i] = 1 means that vertex i is segmented to Set S, segs[i] = 0 means that vertex j is segmented to Set T.
								 );

/*------------------------------------------------------------------------------------*/

// Matching costs related ////////////////////////////////////////////////////////////////////////

// 3*3 bit string of census transform, 8 bits = 1 Byte
uchar GetCensusTransform_BitString_33(const Mat & img,	// input: the original graylevel image
	                                  int x, int y		// input: the pixel to be checked
									  );

// 5*5 bit string of census transform, 24 bits = 3 Bytes, so int type is used, which is 4 Bytes
UINT GetCensusTransform_BitString_55(const Mat & img,	// input: the original graylevel image
	                                 int x, int y		// input: the pixel to be checked
								     );

// 9*7 bit string of census transform, 62 bits = 7.75 Bytes, so double type is used, which is 8 Bytes
uint64 GetCensusTransform_BitString_97(const Mat & img,	// input: the original graylevel image
									   int x, int y		// input: the pixel to be checked
									   );

uchar hamdist(uchar x, uchar y);
UINT hamdist(UINT x, UINT y);
uint64 hamdist(uint64 x, uint64 y);

// Semi global matching algorithm, implemented based on <Stereo Processing by Semiglobal Matching and Mutual Information> 2008
// 20150317, input the cv::Mat
void SemiGlobalMatching(const Mat & mImgB,				// input: the epipolar-rectified base image, should be graylevel
						const Mat & mImgM,				// input: the epipolar-rectified matching image, should be graylevel
						int dmin,						// input: scalar, the minimal disparity
						int dmax,						// input: scalar, the maximal disparity
						double P1,						// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
						double P2,						// input: scalar, a larger constant penalty for all larger disparity changes
						Mat & mDI,						// output:the disparity image
						bool bVertical = false,			// input: whether the layout is vertical or horizontal 
						SGM_PATHS nDir = SGM_PATHS_8,	// input: the amount of scanlines*/
						int constcCheck = -1,			// input: if matching is conducted reversely, i.e. treat the match image as a base image and vice versa.
						bool bSubPix = true				// input: if sub pixel interpolation for sub pixel disparities is applied
						);

//////////////////////////////////////////////////////////////////////////////////////////////////

}