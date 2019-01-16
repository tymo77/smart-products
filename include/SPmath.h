#ifndef SMART_PRODUCTS_MATH
#define SMART_PRODUCTS_MATH
/*
	The statement ifndef... define...endif is a preprocessor directive and is used
	so that a header file is only loaded into the compiler once. 
	This prevents redefinition errors when you include this header
	file in mulitple locations.
*/
#include<vector>
#include<math.h>
#include<iostream>
#include<stdlib.h>
#include<stdexcept>
#include<exception>

/*
	Namespace Smart Products (sp)
*/
namespace sp {

	typedef std::vector<float> VecF_t;
	typedef std::vector<VecF_t> MatF_t;
	VecF_t initv(int n, float val = 0.0);
	VecF_t addv(VecF_t vec_a, VecF_t  vec_b);
	VecF_t subv(VecF_t vec_a, VecF_t  vec_b);
	VecF_t multv(VecF_t vec_a, VecF_t vec_b);
	float dotv(VecF_t vec_a, VecF_t vec_b);


	MatF_t initm(int n, int m, float val = 0);
	void transposeRef(MatF_t& mat_a);
	void resizem(MatF_t* p_mat, int n_new, int m_new);
	void printm(MatF_t& mat_a);
	MatF_t addm(MatF_t mat_a, MatF_t mat_b);
	MatF_t multm(MatF_t mat_a, MatF_t mat_b);

}
#endif
