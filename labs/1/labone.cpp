#include<iostream>
#include<stdlib.h>
#include<vector>
#include"..\..\include\SPmath.h"

int main()
{
	/*
		Main Code
	*/
	//sp::VecF_t A(10, 2.0), B(10, 3.0);
	//sp::VecF_t C = sp::addv(A, B);
	//std::cout << "Adding Vectors (C=A+B): C = " << std::endl;
	//for (float elem : C)
	//{
	//	std::cout << elem << std::endl;
	//}

	/*
	Demonstrating use of runtime_error for 
	vector dimensional mismatch.
	*/
	//sp::VecF_t D(9, 2.0), E(10, 3.0);
	//sp::VecF_t F;
	//try
	//{
	//	F = sp::addv(D, B);
	//}
	//catch (const std::exception& e)
	//{
	//	std::cout <<"error!: "<< e.what() << std::endl<<std::endl;
	//}


	//std::cout << "Fat Matrix: Am = " << std::endl;
	//sp::MatF_t Am = { A, B, C };
	//sp::printm(Am);
	//std::cout << "transposed: Am = " << std::endl;
	//sp::transposeRef(Am);
	//sp::printm(Am);

	//std::cout << "Skinny Matrix: Bm = " << std::endl;
	//sp::MatF_t Bm = { {1,1,1},{2,2,2},{3,3,3},{4,4,4},{5,5,5} };
	//sp::printm(Bm);
	//std::cout << "transposed: Bm = " << std::endl;
	//sp::transposeRef(Bm);
	//sp::printm(Bm);

	std::cout << "Square Matrix: Cm = " << std::endl;
	sp::MatF_t Cm = { { 0,0,1 },{ 0,1,0 },{ 1,0,0 }};
	sp::printm(Cm);
	//std::cout << "transposed: Cm = " << std::endl;
	//sp::transposeRef(Cm);
	//sp::printm(Cm);
	std::cout << "Inverse: Cminv = " << std::endl;
	sp::MatF_t Cminv = sp::invertm(Cm);
	sp::printm(Cminv);
	//std::cout << "Cm resized with pointer" << std::endl;
	//sp::resizem(&Cm, 5, 3);
	//sp::printm(Cm);
}