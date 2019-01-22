#include "../include/SPmath.h"
#include <algorithm>

namespace sp {

	/*
		simple initialize function
	*/
	VecF_t initv(const int n, const float val)
	{
		return VecF_t(n, val);
	}

	/*
		Add two vectors, checking for correct dimensions.
	*/
	VecF_t addv(const VecF_t vec_a, const VecF_t vec_b)
	{

		int n_a = vec_a.size();
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("addv(): Cannot add vectors of different dimensions!");
		VecF_t vec_c = sp::initv(n_b);
		for (int i = 0; i < n_b; i++)
		{
			// adding vectors element by element
			vec_c[i] = vec_a[i] + vec_b[i];
		}
		return vec_c;
	}

	/*
		Subtract two vectors, checking for correct dimensions.
	*/
	VecF_t subv(const VecF_t vec_a, const VecF_t vec_b)
	{
		int n_a = vec_a.size();
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("subv(): Cannot add vectors of different dimensions!");
		VecF_t vec_c = sp::initv(n_b);
		for (int i = 0; i < n_b; i++)
		{
			// subtracting vectors element by element
			vec_c[i] = vec_a[i] - vec_b[i];
		}
		return vec_c;
	}

	/*
	Dot product of two vectors, checking for correct dimensions.
	*/
	float dotv(const VecF_t vec_a, const VecF_t vec_b)
	{
		int n_a = vec_a.size();
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("dotv(): Cannot add vectors of different dimensions!");

		float result = 0.0;
		for (int i = 0; i < n_b; i++)
		{
			// multiplying and summing vectors element by element
			result += vec_a[i] * vec_b[i];
		}
		return result;
	}

	/*
	Scalar product of two vectors, checking for correct dimensions.
	*/
	VecF_t multv(const VecF_t vec_a, const VecF_t vec_b){
		int n_a = vec_a.size();
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("multv(): Cannot add vectors of different dimensions!");
		VecF_t vec_c = sp::initv(n_b);

		for (int i = 0; i < n_b; i++)
		{
			// multiplying and summing vectors element by element
			vec_c[i] = vec_a[i] * vec_b[i];
		}
		return vec_c;
	}

	/*
	simple initialize function
	*/
	MatF_t initm(const int n, const int m, const float val)
	{
		return MatF_t(n, VecF_t(m, val));
	}

	/*
	Initialize an identity matrix.
	*/
	MatF_t initIdent(const int n, const float val)
	{
		MatF_t result = MatF_t(n, VecF_t(n, val));

		for (int i = 0; i < n; i++) {
			result[i][i] = 1.0;
		}
		return result;
	}

	/*
	Compute deteriminant of a matrix.
	*/
	float detm(MatF_t mat_a) {

		return 0.0;
	}

	/*
	transpose matrix
	*/
	void transposeRef(MatF_t & mat_a)
	{
		int rows = mat_a.size();
		int cols = mat_a[0].size();
		bool is_square = cols == rows ? true : false;
		bool is_fat = cols > rows ? true : false;
		if (is_fat)
		{
			mat_a.resize(cols, VecF_t((is_fat ? rows : cols), 0.0));
		}
		for (int i = 0; i < (is_fat ? rows : cols); i++)
		{
			if (!is_fat & !is_square)
			{
				mat_a[i].resize(rows, 0.0);
			}
			for (int j = i + 1; j < (is_fat ? cols : rows); j++)
			{
				float temp_ij = mat_a[i][j];
				mat_a[i][j] = mat_a[j][i];
				mat_a[j][i] = temp_ij;
			}
		}
		if (!is_fat & !is_square)
		{
			mat_a.resize(cols);
		}
		else
		{
			for (int i = 0; i<rows; i++)
			{
				mat_a[i].resize(rows);
			}
		}
	}

	/*
	Swap two rows in a matrix.
	*/
	void swapRows(MatF_t & mat_a, const int n1, const int n2) {
		VecF_t temp = mat_a[n1];
		mat_a[n1] = mat_a[n2];
		mat_a[n2] = temp;
	}

	/*
	Get reduced row echelon form of a matrix.
	*/
	void rref(MatF_t & mat_a){
		int m = mat_a.size();
		int n = mat_a[0].size();
		// FORWARD ELIMINATION. Implemented from the algorithm on wikipedia XD
		int h = 0;
		int k = 0;
		int i_max;
		float temp_comp1, temp_comp2, f;
		while (h < m && k < n) {

			// Find the pivot in the k-th column.
			i_max = 0;
			temp_comp1 = 0.0; temp_comp2 = 0.0;

			// Get the index of the maximum row in the column.
			for (int i = h; i < m; i++) {
				temp_comp2 = abs(mat_a[i][k]);
				if (temp_comp2 > temp_comp1) {
					i_max = i;
					temp_comp1 = temp_comp2;
				}
			}

			if (abs(mat_a[i_max][k]) == 0.0) {
				// No pivot in this column. Pass to next column.
				k = k + 1;
			}
			else {
				swapRows(mat_a, h, i_max); // Swap the pivot row with the current row.

				// For all the rows below the pivot.
				for (int i = h + 1; i < m; i++) {
					f = mat_a[i][k] / mat_a[h][k]; // Get the factor to cancel the column under the pivot.

					mat_a[i][k] = 0.0; // Fill with zeros.

					// For all elements right of the pivot in this row.
					for (int j = k + 1; j < n; j++) {
						mat_a[i][j] = mat_a[i][j] - mat_a[h][j] * f; // Cancelation with the current pivot row.
					}
				}
				h = h + 1;
				k = k + 1;
			}
		}

		//printm(mat_a);
		// BACKWARD ELIMINATION.
		// Start in the bottom left. Search right for a pivot. Then move up row by row.
		h = m - 1;
		k = 0;
		while (h >= 0) {//Run until we get to the top of the matrix.
			//printf("(%d, %d)", h, k);
			// If the current position is non-zero. (A pivot)
			f = mat_a[h][k];
			if (f != 0.0) {
				//printf("\n");
				// Divide through the row by the pivot.
				for (int j = k; j < n; j++) {
					mat_a[h][j] = mat_a[h][j] / f;
				}

				// For all the column above the pivot.
				for (int i = h - 1; i >= 0; i--) {
					f = mat_a[i][k];
					if (f != 0.0) {
						mat_a[i][k] = 0.0;
						for (int j = k + 1; j < n; j++) {
							mat_a[i][j] = mat_a[i][j] - mat_a[h][j] * f;
						}
					}
				}
				k = 0; h = h - 1;
				//printm(mat_a);
				//printf("(%d, %d)\n", h, k);
			}
			else {
				//printf(" is a zero.\n");
				// The current element is a zero
				// Keep checking to the right.
				k = k + 1;
				// If we run out of columns on the R. Move up 2 the next row & start from the L.
				if (k > n) { k = 0; h = h - 1; }
			}
		}
	}

	/*
	Invert a matrix.
	*/
	MatF_t invertm(MatF_t & mat_a)
	{
		int rows = mat_a.size();
		int cols = mat_a[0].size();
		if (rows != cols) {
			throw std::runtime_error("invertm(): mat_a must be square.!");
		}
		int N = rows;

		// Resize matrix to twice to width.
		resizem(&mat_a, N, 2*N);

		// Add ones on diagonal of new section.
		for (int i = 0; i < N; i++) {
			mat_a[i][i + N] = 1.0;
		}

		// Get reduced-row echelon form of the new matrix.
		rref(mat_a);

		// Get the right side of the matrix.
		MatF_t mat_b;
		mat_b = initm(N, N, 0.0);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				mat_b[i][j] = mat_a[i][j + N];
			}
		}
		return mat_b;
	}

	/*
	Adjust matrix size. If expanding, uses 0.0 as default.
	*/
	void resizem(MatF_t * p_mat, const int n_new, const int m_new)
	{
		p_mat->resize(n_new, VecF_t(m_new, 0.0));
		int rows = p_mat->size();
		int cols = p_mat->at(0).size();

		for (int i = 0; i < rows; i++)
		{
			p_mat->at(i).resize(m_new, 0.0);
		}
	}

	/*
	Print out the matrix.
	*/
	void printm(const MatF_t & mat_a)
	{
		for (auto& row : mat_a)
		{
			for (auto& elem : row)
			{
				printf("% -#8.4g   ", elem);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	/*
	Add two matrices, checking for correct dimensions... to do in class.
	*/
	MatF_t addm(const MatF_t mat_a, const MatF_t mat_b){

		// Get sizes from matrices.
		int a_Nrows = mat_a.size();
		int b_Nrows = mat_b.size();
		int a_Ncols = mat_a[0].size();
		int b_Ncols = mat_b[0].size();

		// Test that the matrices are of the same dimensions.
		if ( a_Nrows != b_Nrows || a_Ncols != b_Ncols){
			throw std::runtime_error("addm(): Cannot add matrices of different dimensions!");
		}

		MatF_t result;
		result = initm(a_Nrows, a_Ncols, 0.0);
		for ( int i = 0; i < a_Nrows; i++ ){
			result[i] = addv (mat_a[i], mat_b[i]);			
		}
		return MatF_t();
	}

	/*
	Subctract two matrices, checking for correct dimensions... to do in class.
	*/
	MatF_t subm(const MatF_t mat_a, const MatF_t mat_b) {

		// Get sizes from matrices.
		int a_Nrows = mat_a.size();
		int b_Nrows = mat_b.size();
		int a_Ncols = mat_a[0].size();
		int b_Ncols = mat_b[0].size();

		// Test that the matrices are of the same dimensions.
		if (a_Nrows != b_Nrows || a_Ncols != b_Ncols) {
			throw std::runtime_error("addm(): Cannot add matrices of different dimensions!");
		}

		MatF_t result;
		result = initm(a_Nrows, a_Ncols, 0.0);
		for (int i = 0; i < a_Nrows; i++) {
			result[i] = subv(mat_a[i], mat_b[i]);
		}
		return MatF_t();
	}

	/*
	Multiply two matrices, checking for correct dimensions.
	*/
	MatF_t multm(const MatF_t mat_a, const MatF_t mat_b){
		// Get sizes from matrices.
		int a_Nrows = mat_a.size();
		int b_Nrows = mat_b.size();
		int a_Ncols = mat_a[0].size();
		int b_Ncols = mat_b[0].size();

		// Test that the matrices are of the same dimensions.
		if ( a_Nrows != b_Nrows || a_Ncols != b_Ncols){
			throw std::runtime_error("multm(): Cannot add matrices of different dimensions!");
		}

		MatF_t result;
		result = initm(a_Nrows, a_Ncols, 0.0);
		for ( int i = 0; i < a_Nrows; i++ ){
			result[i] = multv (mat_a[i], mat_b[i]);			
		}
		return result;
	}
}
