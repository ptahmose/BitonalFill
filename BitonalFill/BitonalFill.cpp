// BitonalFill.cpp : Defines the entry point for the application.
//

#include "BitonalFill.h"
#include "FillFromBitonal.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	uint8_t bitonalSrc[] = { 0x88,0x44 };
	uint8_t gray8Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

	FillFromBitonalFromOnes_Gray8_AVX2(16, 1, bitonalSrc, 2, gray8Bitmap, 16, 0xff);
	return 0;
}
