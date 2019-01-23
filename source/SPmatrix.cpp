#include "../include/SPmath.h"
#include "../include/SPmatrix.h"
#include <iomanip>
#include <iostream>

namespace sp {

		// Constructors
		// Default
		MatF::MatF() : n_rows_(0), n_cols_(0), init_(false) { mat_data_ = initm(0, 0); }
		// Initialized to a value
		MatF::MatF(int n_rows, int n_cols, float f = 0.0f) : n_rows_(n_rows), n_cols_(n_cols), init_(true) {
			mat_data_ = initm(n_rows, n_cols, f); 
		}
		// Copy
		MatF::MatF(const MatF_t m) {
			init_ = true;
			mat_data_ = m;
			n_rows_ = m.size();
			n_cols_ = m[0].size();
		}

		// Getters
		inline MatF_t MatF::getMatData() const { return mat_data_; }
		inline int MatF::getNRows() const { return n_rows_; }
		inline int MatF::getNCols() const { return n_cols_; }

		// Self-operations
		MatF MatF::inv() {
			MatF B(invertm(mat_data_));
			return B;
		}
		MatF MatF::t() {
			MatF B(transposem(mat_data_));
			return B;
		}

		// Operators
		MatF& MatF::operator= (const MatF& m) {
			mat_data_ = m.getMatData();
			n_rows_ = m.getNRows();
			n_cols_ = m.getNCols();
			return *this;
		}
		MatF MatF::operator+ (const MatF& m) {
			MatF C(addm(this->mat_data_, m.mat_data_));
			return C;
		}
		MatF MatF::operator- (const MatF& m) {
			MatF C(subm(mat_data_, m.mat_data_));
			return C; 
		}
		MatF MatF::operator* (const MatF& m) {
			MatF C(multm(this->mat_data_, m.mat_data_));
			return C; 
		}

	std::ostream& operator<< (std::ostream& out, const MatF& m) {
		MatF_t mat_data = m.getMatData();

		for (int i = 0; i < m.getNRows(); i++) {
			for (int j = 0; j < m.getNCols(); j++) {
				out << std::setw(10) << mat_data[i][j] << "\t";
			}
			out << "\n";
		 }

		return out;
	}
	

}
