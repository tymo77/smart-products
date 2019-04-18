#ifndef SMART_PRODUCTS_MATRIX
#define SMART_PRODUCTS_MATRIX

#include<vector>
#include<math.h>
#include<iostream>
#include<stdlib.h>
#include<stdexcept>
#include<exception>
#include "SPmath.h"

/*
	Namespace Smart Products (sp)
*/
namespace sp {
	class MatF {

	protected:
		MatF_t mat_data_;
		int n_rows_;
		int n_cols_;
		bool init_;

	public:
		// Constructors
		MatF();
		MatF(int,int,float);
		MatF(const MatF_t);

		// Getters
		inline MatF_t getMatData() const { return mat_data_; }
		inline int getNRows() const { return n_rows_; }
		inline int getNCols() const { return n_cols_; }

		// Self operations
		MatF inv();
		MatF t();

		// Operators
		MatF& operator= (const MatF&);
		MatF operator+ (const MatF&);
		MatF operator- (const MatF&);
		MatF operator* (const MatF&);
		friend std::ostream& operator<< (std::ostream&, const MatF&);	
	};
}
#endif
