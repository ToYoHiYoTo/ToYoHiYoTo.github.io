#include "Mymath.h"

//==============================================================================================================================
//	�֐�
//==============================================================================================================================
/*---- �x���@����ʓx�@�� ----*/
float ToRadian(float degree) {
	return (degree*(float)PI) * DIVISION_180;
}
/*---- �ʓx�@����x���@�� ----*/
float ToDegree(float radian) {
	return radian * (180.0f / (float)PI);
}
/*---- ���b�v�A���E���h�֐�(int) ----*/
int wrap(int x, int low, int high) {
	assert(low < high);
	const int n = (x - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
/*---- ���b�v�A���E���h�֐�(float) ----*/
float wrap(float x, float low, float high) {
	assert(low < high);
	const float n = std::fmod(x - low, high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
/*---- ���b�v�A���E���h�֐�(double) ----*/
double wrap(double x, double low, double high) {
	assert(low < high);
	const double n = std::fmod(x - low, high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
/*---- ���b�v�A���E���h�֐�(long double) ----*/
long double wrap(long double x, long double low, long double high) {
	assert(low < high);
	const long double n = std::fmod(x - low, high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
