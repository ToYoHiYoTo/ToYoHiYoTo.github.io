#include "Mymath.h"

//==============================================================================================================================
//	関数
//==============================================================================================================================
/*---- 度数法から弧度法へ ----*/
float ToRadian(float degree) {
	return (degree*(float)PI) * DIVISION_180;
}
/*---- 弧度法から度数法へ ----*/
float ToDegree(float radian) {
	return radian * (180.0f / (float)PI);
}
/*---- ラップアラウンド関数(int) ----*/
int wrap(int x, int low, int high) {
	assert(low < high);
	const int n = (x - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
/*---- ラップアラウンド関数(float) ----*/
float wrap(float x, float low, float high) {
	assert(low < high);
	const float n = std::fmod(x - low, high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
/*---- ラップアラウンド関数(double) ----*/
double wrap(double x, double low, double high) {
	assert(low < high);
	const double n = std::fmod(x - low, high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
/*---- ラップアラウンド関数(long double) ----*/
long double wrap(long double x, long double low, long double high) {
	assert(low < high);
	const long double n = std::fmod(x - low, high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
