#include<iostream>
#include<stdlib.h>
#include<vector>
#include"../../include/SPmath.h"
#include"../../include/SPmatrix.h"

int main()
{
	sp::MatF_t data = { { 5.0f, 3.3f, 2.34f },{ 6.7f, 7.32f, 9.3f },{ 8.123f, 7.345f, 22.34f } };
	sp::MatF A(data);
	std::cout << "Matrix A = " << std::endl << A << std::endl;
	sp::MatF Ainv = A.inv();
	std::cout << "Matrix Ainv = inverse(A) " << std::endl << Ainv << std::endl;
	sp::MatF B = A * Ainv;
	std::cout << "Matrix B = A * Ainv " << std::endl << B << std::endl;
	sp::MatF C = ((A.t() * Ainv + A - B) * A).inv();
	std::cout << "Matrix C = inverse((transpose(A) * Ainv + A - B) * A)" << std::endl << C << std::endl;
}