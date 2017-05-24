#pragma once

#include <vector>
#include <string>

using namespace std;
typedef vector<vector<vector<string> > > Ad_Mat;

class zusammenfassen
{
public:
	Ad_Mat Adjacenzmatrix;
	bool seriell();
	bool paralle();
	bool dreieck2stern();
	bool stern2dreieck();
	void operator ()(Ad_Mat& Matrix);
};