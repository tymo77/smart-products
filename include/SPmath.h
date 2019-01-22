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
	VecF_t initv(const int n, const float val = 0.0);
	VecF_t addv(const VecF_t vec_a, const VecF_t  vec_b);
	VecF_t subv(const VecF_t vec_a, const VecF_t  vec_b);
	VecF_t multv(const VecF_t vec_a, const VecF_t vec_b);
	float dotv(const VecF_t vec_a, const VecF_t vec_b);

	MatF_t initIdent(const int n, const float val);
	MatF_t initm(const int n, const int m, const float val = 0);
	void transposeRef(MatF_t& mat_a);
	MatF_t invertm(MatF_t& mat_a);
	void resizem(MatF_t* p_mat, const int n_new, const int m_new);
	void printm(const MatF_t& mat_a);
	void swapRows(MatF_t & mat_a, const int n1, const int n2);
	MatF_t addm(const MatF_t mat_a, const MatF_t mat_b);
	MatF_t subm(const MatF_t mat_a, const MatF_t mat_b);
	MatF_t multm(const MatF_t mat_a, const MatF_t mat_b);
	float detm(const MatF_t mat_a);
	void rref(MatF_t & mat_a);

}
#endif
