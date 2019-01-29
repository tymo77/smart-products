#ifndef SMART_PRODUCTS_MATH
#define SMART_PRODUCTS_MATH

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

	// Declare data structure types for matrices and vectors.
	typedef std::vector<float> VecF_t;
	typedef std::vector<VecF_t> MatF_t;

	// Initialization of vector.
	VecF_t initv(const unsigned int, const float val = 0.0f);

	// Vector arithmetic operations.
	VecF_t addv(const VecF_t, const VecF_t);
	VecF_t subv(const VecF_t, const VecF_t);
	VecF_t multv(const VecF_t, const VecF_t);
	float dotv(const VecF_t, const VecF_t);

	// Initialization of matrix.
	MatF_t initIdent(const unsigned int);
	MatF_t initm(const unsigned int, const unsigned int, const float val = 0.0f);

	// Matrix operations.
	MatF_t transposem(MatF_t);
	MatF_t invertm(MatF_t);
	void transposemRef(MatF_t&);
	void invertmRef(MatF_t&);
	void swapRows(MatF_t&, const int, const int);
	float detm(const MatF_t);
	void rref(MatF_t&);
	int rankm(const MatF_t);

	// Non-mathematical matrix operations.
	void resizem(MatF_t*, const unsigned int, const unsigned int);
	void printm(const MatF_t&);

	// Matrix arithmetic operations.
	MatF_t addm(const MatF_t, const MatF_t);
	MatF_t subm(const MatF_t, const MatF_t);
	MatF_t multm(MatF_t, MatF_t);

}
#endif