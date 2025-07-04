#pragma once
#include <KamataEngine.h>
#include "MassFunction.h"
#include <Novice.h>
using namespace KamataEngine;
struct Sphere{
	Vector3 center;//中心
	float radius;
};
struct Line{
	Vector3 origin;//起点
	Vector3 diff;//方向

};
struct Ray{
	Vector3 origin;//起点
	Vector3 diff;//方向
};
struct  Segment{
	Vector3 origin;//起点
	Vector3 diff;//方向
};
struct Plane
{
	Vector3 normal;//法向量
	float distance;//距離
};
struct Triangle
{
	Vector3 vertces[3];//頂点1

};




void DrawSphere(
	const Sphere& sphere,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color
	
);
void DrawGrid(
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix
);
void DrawPlane(
	const Plane& plane,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color
);
void DrawSegment(
	const Segment& segment,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color
);
void DrawTriangle(
	const Triangle& triangle,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color
);

Vector3 Project(const Vector3& v1,const Vector3& v2);
Vector3 ClosestPoint(const Vector3& point,const Segment& segment);

bool IsCollision(const Sphere& si,const Sphere& s2);
bool IsCollision(const Sphere& sphere,const Plane& plane);
bool isCollision(const Segment& segment, const Plane& plane);
bool isCollision(const Line& line, const Plane& plane);
bool isCollision(const Ray& ray, const Plane& plane);
bool isCollision(const Triangle triangle , const  Segment& segment );

