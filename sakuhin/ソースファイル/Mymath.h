//==============================================================================================================================
//	インクルードファイル
//
//	個人的に欲しかった計算用のマクロや関数
//									name : yuuya ooyuki
//									dey	 : 2019 / 01 / 06
//
//	※注意ごとをここに書く
//
//==============================================================================================================================
#ifndef __MY_MATH_H__
#define __MY_MATH_H__



//==============================================================================================================================
//	インクルードファイル
//==============================================================================================================================
#include <cmath>
#include <cassert>
#include <DirectXMath.h>

//==============================================================================================================================
//	マクロ(定数)
//==============================================================================================================================
#define DIVISION_180 (0.0055556f) // これ掛けると大体÷１８０と一緒
#define DIVISION_60	 (0.0166666f)	// これ掛けると大体÷６０と一緒
#define DIVISION_30  (0.0333333f)	// これ掛けると大体÷３０と一緒
#define DIVISION_15  (0.0666666f)	// これ掛けると大体÷１５と一緒
#define PI			 (3.14)			// 円周率
#define PI_SUPER	 (3.14159265358979323846264338327950288419)	// 円周率（遊びだから使うな）


//==============================================================================================================================
//	関数
//==============================================================================================================================
/*---- 度数法から弧度法へ ----*/
float ToRadian(float degree);
/*---- 弧度法から度数法へ ----*/
float ToDegree(float radian);
/*---- クランプ関数 ----*/
template <class T>
T clamp(T x, T low, T high) {
	assert(low <= high);
	return min(max(x, low), high);
};
/*---- ラップアラウンド関数(int) ----*/
int wrap(int x, int low, int high);
/*---- ラップアラウンド関数(float) ----*/
float wrap(float x, float low, float high);
/*---- ラップアラウンド関数(double) ----*/
double wrap(double x, double low, double high);
/*---- ラップアラウンド関数(long double) ----*/
long double wrap(long double x, long double low, long double high);




struct Float2 {
	float x;
	float y;
	Float2() = default;
	Float2(float x, float y) : x(x), y(y) {}

	Float2(const Float2&) = default;
	Float2& operator=(const Float2&) = default;

	Float2(Float2&&) = default;
	Float2& operator=(Float2&&) = default;
};

struct Float3 {
	float x;
	float y;
	float z;
	Float3() = default;
	Float3(float x, float y,float z) : x(x), y(y), z(z) {}

	Float3(const Float3&) = default;
	Float3& operator=(const Float3&) = default;

	Float3(Float3&&) = default;
	Float3& operator=(Float3&&) = default;
};

struct Float4 {
	float x;
	float y;
	float z;
	float w;
	Float4() = default;
	Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Float4(const Float4&) = default;
	Float4& operator=(const Float4&) = default;

	Float4(Float4&&) = default;
	Float4& operator=(Float4&&) = default;
};
#endif // !__MY_MATH_H__
