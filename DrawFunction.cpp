﻿#include"DrawFunction.h"

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision=24;//分割数
	const float kLonEvery= 2.0f * PI / static_cast<float>(kSubdivision);
	const float kLatEvery=PI / static_cast<float>(kSubdivision);
	///緯度に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = -PI/2.0f+kLatEvery*latIndex;//現在の緯度
		float latD = PI / static_cast<float>(kSubdivision);//緯度の分割幅
		//経度に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++)
		{
			float lon=lonIndex*kLonEvery;//現在の経度
			float lonD = PI * 2.0f / static_cast<float>(kSubdivision);//経度の分割幅
			//world
			Vector3 a,b,c;
			a={
				 sphere.center.x + sphere.radius * cosf(lat)*cosf(lon),
				 sphere.center.y + sphere.radius *sinf(lat),
				 sphere.center.z + sphere.radius *cosf(lat)*sinf(lon)
			};
			b={
				 sphere.center.x + sphere.radius * cosf(lat+latD)*cosf(lon),
				 sphere.center.y + sphere.radius *sin(lat+latD),
				 sphere.center.z + sphere.radius * cosf(lat + latD) * sinf(lon)
			};
			c={ 
				 sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + lonD),
				sphere.center.y + sphere.radius *sinf(lat),
				sphere.center.z + sphere.radius *cosf(lat) * sinf(lon + lonD) };
			//緯度線
			Vector3 screen[3] = {};
			for (uint32_t i = 0; i < 3; i++)
			{
				Vector3 nbc = Transform(i==0?a:i==1?b:c, viewProjectionMatrix);
				screen[i] = Transform(nbc, viewportMatrix);
			}
			//描画
			Novice::DrawLine(static_cast<int>(screen[0].x), static_cast<int>(screen[0].y),
								static_cast<int>(screen[1].x), static_cast<int>(screen[1].y), color);
			Novice::DrawLine(static_cast<int>(screen[0].x), static_cast<int>(screen[0].y),
				static_cast<int>(screen[2].x), static_cast<int>(screen[2].y), color);

			
			
			
		}

	}

}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth=2.0f;//グリッドの半分
	const uint32_t kSubdivision=10;//分割数
	//一つ分の長さ
	const float kGridEvery =(kGridHalfWidth*2.0f)/static_cast<float>(kSubdivision);

	//奥から引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		Vector3 kGridLocalVertices[2]={};
			kGridLocalVertices[0]={-kGridHalfWidth+static_cast<float>(xIndex)*static_cast<float>(kGridEvery),0.0f,kGridHalfWidth};
			kGridLocalVertices[1]={-kGridHalfWidth+static_cast<float>(xIndex)*static_cast<float>(kGridEvery),0.0f,-kGridHalfWidth};

		Vector3 screen[2]={};
		for (uint32_t i = 0; i < 2; i++)
		{


			Vector3 nbc = Transform(kGridLocalVertices[i], viewProjectionMatrix);
			screen[i] = Transform(nbc, viewportMatrix);
		}

		

		if (xIndex==kSubdivision/2)
		{
			Novice::DrawLine(static_cast<int>(screen[0].x),static_cast<int>(screen[0].y),static_cast<int>(screen[1].x),static_cast<int>(screen[1].y),0x000000FF);

		}else{
			Novice::DrawLine(static_cast<int>(screen[0].x),static_cast<int>(screen[0].y),static_cast<int>(screen[1].x),static_cast<int>(screen[1].y),0xAAAAAAFF);
		
		}
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++)
	{
		Vector3 kGridLocalVertices[2]={};
			kGridLocalVertices[0]={kGridHalfWidth,0.0f,-kGridHalfWidth+static_cast<float>(zIndex)*static_cast<float>(kGridEvery)};
			kGridLocalVertices[1]={-kGridHalfWidth,0.0f,-kGridHalfWidth+static_cast<float>(zIndex)*static_cast<float>(kGridEvery)};

		Vector3 screen[2]={};
		for (uint32_t i = 0; i < 2; i++)
		{


			Vector3 nbc = Transform(kGridLocalVertices[i], viewProjectionMatrix);
			screen[i] = Transform(nbc, viewportMatrix);
		}

		
		if (zIndex==kSubdivision/2)
		{
			Novice::DrawLine(static_cast<int>(screen[0].x),static_cast<int>(screen[0].y),static_cast<int>(screen[1].x),static_cast<int>(screen[1].y),0x000000FF);

		}else{
			Novice::DrawLine(static_cast<int>(screen[0].x),static_cast<int>(screen[0].y),static_cast<int>(screen[1].x),static_cast<int>(screen[1].y),0xAAAAAAFF);
		
		}
		

	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1]={-perpendiculars[0].x ,-perpendiculars[0].y,-perpendiculars[0].z};
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3]={-perpendiculars[2].x ,-perpendiculars[2].y,-perpendiculars[2].z};

	Vector3 points[4];
	for (int i = 0; i < 4; ++i){
		Vector3 extend =Multiply(2.0f,perpendiculars[i]);
		Vector3 point = Add(center, extend);
		points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
		Novice::ScreenPrintf(0,15*i,"point%d:x%0.0f:y%0.0f:z%0.0f",i,points[i].x,points[i].y,points[i].z);
	}
	Novice::DrawLine (
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
			color);
	Novice::DrawLine (
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
			color);
	Novice::DrawLine (
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[3].x),
		static_cast<int>(points[3].y),
			color);
	Novice::DrawLine (
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		static_cast<int>(points[3].x),
		static_cast<int>(points[3].y),
			color);

}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 point = Add(segment.origin, segment.diff);
	Vector3 points[2];
	for (int i = 0; i < 2; ++i){
		

		if (i==0)
		{
			points[i] = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);

		} else
		{
			points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);

		}
	}
	Novice::DrawLine(
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{

	Vector3 points[3];
	for (int i = 0; i < 3; ++i){
		points[i] = Transform(Transform(triangle.vertces[i], viewProjectionMatrix), viewportMatrix);

		Novice::ScreenPrintf(0,15*i,"point%d:x%0.0f:y%0.0f:z%0.0f",i,points[i].x,points[i].y,points[i].z);
	}
	Novice::DrawLine(
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		color);
	Novice::DrawLine(
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
		color);
	Novice::DrawLine(
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		color);

}

/// <summary>
/// 正射影ベクトル
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	float dot = Dot(v1, v2);
	float length = Length(v2);
	if (length == 0.0f) {return Vector3();} // v2がゼロベクトルの場合は投影できない

	
	return Multiply((dot / powf(length ,2)),v2);
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 project = Project(Subtract(point, segment.origin),segment.diff);
	return Add(segment.origin,project);
}

// sphereとsphereの衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2)
{
	float distance= Length(Subtract(s2.center,s1.center));
	if (distance<=s1.radius+s2.radius)
	{
		return true;
	}
	return false;
}
//SphereとPlaneの衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	float distance = Dot(sphere.center, plane.normal) - plane.distance;
	if (fabs(distance) <= sphere.radius)
	{
		return true;
	}
	return false;
}

//SegmentとPlaneの衝突判定
bool isCollision(const Segment& segment, const Plane& plane)
{
	float dot = Dot(segment.diff, plane.normal);
	if (dot==0.0f)
	{
		return false;
	}
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	
	if (t<0.0f || t > 1.0f)
	{
		return false;
	}




	
	
	
	return true;

}
//LineとPlaneの衝突判定
bool isCollision(const Line& line, const Plane& plane)
{
	float dot = Dot(line.diff, plane.normal);
	if (dot==0.0f)
	{
		return false;
	}
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	if (t<=0.0f || t > 1.0f)
	{
		return false;
	}
	
	
	return true;
}
//RayとPlaneの衝突判定
bool isCollision(const Ray& ray, const Plane& plane)
{
	float dot = Dot(ray.diff, plane.normal);
	if (dot==0.0f)
	{
		return false;
	}
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	if (t<=0.0f)
	{
		return false;
	}
	
	
	return true;
}

bool isCollision(const Triangle triangle,const Segment& segment )
{

	
	 // 三角形の頂点
    const Vector3& v0 = triangle.vertces[0];
    const Vector3& v1 = triangle.vertces[1];
    const Vector3& v2 = triangle.vertces[2];
	 
//三角形の辺を求める
	Vector3 v01 = Subtract(v1,v0);
	Vector3 v12 = Subtract(v2, v1);
	Vector3 v21 = Subtract(v1, v2);
	Vector3 v20 = Subtract(v0, v2);
	
	Vector3 normal = Cross(v01, v12);

	//法線ベクトルを正規化
	normal = Normalize(normal);
// 線分の始点と終点
   
    Vector3 p0 = segment.origin;
    Vector3 p1 = Add(segment.origin, segment.diff);
  
	
	float d= Dot(v0,normal);
	float dot= Dot(segment.diff,normal);
	if (dot==0.0f)
	{
		return false;
	}
	float t = (d - Dot(p0, normal)) / dot;

	if (t<0.0f || t>1.0f)
	{
		return false;
	}
	//セグメントの始点と終点を求める
	Vector3 p = Add(p0, Multiply(t, segment.diff));
	

	//三角形の法線ベクトルを求める	
	//v01とv12の外積を求める
	
	

	//三角形の頂点からセグメントの始点までのベクトルを求める
	Vector3 v0p = Subtract(p, v0);
	Vector3 v1p = Subtract(p, v1);
	Vector3 v2p = Subtract(p, v2);

	

	//セグメントの始点と終点を求める

	Vector3 cross01=Cross(v01,v1p);
	Vector3 cross12=Cross(v12,v2p);
	Vector3 cross20=Cross(v20,v0p);

	if(	Dot(cross01, normal) >= 0.0f &&
	   Dot(cross12, normal) >= 0.0f &&
		Dot(cross20, normal) >= 0.0f){
		return true;
	}




	return false;
}
