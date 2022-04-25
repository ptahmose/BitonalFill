// BitonalFill.cpp : Defines the entry point for the application.
//

#include "BitonalFill.h"
#include "FillFromBitonal.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	FillFromBitonalFromOnes_Gray8_AVX2(0, 0, nullptr, 0, nullptr, 0, 0);
	return 0;
}
