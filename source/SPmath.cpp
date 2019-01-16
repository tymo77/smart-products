#include "..\include\SPmath.h"

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
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("addv(): Cannot add vectors of different dimensions!");
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
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("addv(): Cannot add vectors of different dimensions!");

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
		int n_b = vec_b.size() == n_a ? vec_b.size() : throw std::runtime_error("addv(): Cannot add vectors of different dimensions!");
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

	MatF_t initm(int n, int m, float val)
	{
		return MatF_t(n, VecF_t(m, val));
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
				std::cout << elem << "\t";
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
			throw std::runtime_error("addv(): Cannot add matrices of different 				dimensions!");
		}

		MatF_t result;
		result = initm(a_Nrows, a_Ncols, 0.0);
		for ( int i = 0; i < a_Nrows; i++ ){
			result[i] = addv (mat_a[i], mat_b[i]);			
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
			throw std::runtime_error("addv(): Cannot add matrices of different 				dimensions!");
		}

		MatF_t result;
		result = initm(a_Nrows, a_Ncols, 0.0);
		for ( int i = 0; i < a_Nrows; i++ ){
			result[i] = multv (mat_a[i], mat_b[i]);			
		}

		return result;
	}

}
