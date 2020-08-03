//==============================================================================================================================
//	�C���N���[�h�t�@�C��
//
//	�l�I�ɗ~���������v�Z�p�̃}�N����֐�
//									name : yuuya ooyuki
//									dey	 : 2019 / 01 / 06
//
//	�����ӂ��Ƃ������ɏ���
//
//==============================================================================================================================
#ifndef __MY_MATH_H__
#define __MY_MATH_H__



//==============================================================================================================================
//	�C���N���[�h�t�@�C��
//==============================================================================================================================
#include <cmath>
#include <cassert>
#include <DirectXMath.h>

//==============================================================================================================================
//	�}�N��(�萔)
//==============================================================================================================================
#define DIVISION_180 (0.0055556f) // ����|����Ƒ�́��P�W�O�ƈꏏ
#define DIVISION_60	 (0.0166666f)	// ����|����Ƒ�́��U�O�ƈꏏ
#define DIVISION_30  (0.0333333f)	// ����|����Ƒ�́��R�O�ƈꏏ
#define DIVISION_15  (0.0666666f)	// ����|����Ƒ�́��P�T�ƈꏏ
#define PI			 (3.14)			// �~����
#define PI_SUPER	 (3.14159265358979323846264338327950288419)	// �~�����i�V�т�����g���ȁj


//==============================================================================================================================
//	�֐�
//==============================================================================================================================
/*---- �x���@����ʓx�@�� ----*/
float ToRadian(float degree);
/*---- �ʓx�@����x���@�� ----*/
float ToDegree(float radian);
/*---- �N�����v�֐� ----*/
template <class T>
T clamp(T x, T low, T high) {
	assert(low <= high);
	return min(max(x, low), high);
};
/*---- ���b�v�A���E���h�֐�(int) ----*/
int wrap(int x, int low, int high);
/*---- ���b�v�A���E���h�֐�(float) ----*/
float wrap(float x, float low, float high);
/*---- ���b�v�A���E���h�֐�(double) ----*/
double wrap(double x, double low, double high);
/*---- ���b�v�A���E���h�֐�(long double) ----*/
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
