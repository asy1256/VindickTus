#pragma once

#define g_pCOLLISIONMANAGER	cGeometry3D::GetInstance()

#define AABBSphere(aabb, sphere) \
	SphereAABB(Sphere, AABB)
#define OBBSphere(obb, sphere) \
	SphereOBB(Sphere, OBB)
#define PlaneSphere(plane, sphere) \
	SpherePlane(Sphere, Plane)
#define OBBAABB(obb,aabb) \
	AABBOBB(AABB,OBB)
#define PlaneAABB(plane, aabb) \
	AABBPlane(AABB,Plane)
#define PlaneOBB(plane, obb) \
	OBBPlane(OBB,Plane)

typedef struct Line
{
	D3DXVECTOR3 Start;	// 시작점
	D3DXVECTOR3 End;	// 끝점

	inline Line() {}
	inline Line(D3DXVECTOR3& s, D3DXVECTOR3& e) : Start(s), End(e) {}
}Line;

typedef struct Ray
{
	D3DXVECTOR3		Origin;		// Ray를 쏠 위치
	D3DXVECTOR3		Direction;	// Ray를 쏠 방향

	inline Ray() : Direction(0.0f, 0.0f, 1.0f) {}
	inline Ray(D3DXVECTOR3& o, D3DXVECTOR3& d) : Origin(o), Direction(d) {	NomalizeDirection();}
	inline void NomalizeDirection() { D3DXVec3Normalize(&Direction, &Direction); } // 방향은 거리가 필요없기떄문에 노멀라이즈한다.
}Ray;

typedef struct Sphere
{
	D3DXVECTOR3 Position;	// 중점
	float		Radius;		// 반지름

	inline Sphere() : Radius(1.0f) {}
	inline Sphere(D3DXVECTOR3& p, float r) : Position(p), Radius(r) {}
}Sphere;

typedef struct AABB
{
	D3DXVECTOR3		Origin;	// 중점
	D3DXVECTOR3		Size;	// 박스 크기

	inline	AABB() : Size(1, 1, 1) {}
	inline	AABB(D3DXVECTOR3& o,  D3DXVECTOR3& s) : Origin(o), Size(s) {}
}AABB;

typedef struct OBB
{
	D3DXVECTOR3					Position;		// 중점
	D3DXVECTOR3					Size;			// 박스 크기
	D3DXMATRIXA16				Orientation;	// 박스 회전 메트릭스
	std::vector<D3DXVECTOR3>	vecVertex;		// 박스의 버텍스

	inline	OBB() : Size(1, 1, 1) {}
	inline  OBB(D3DXVECTOR3& p,  D3DXVECTOR3& s,  D3DXMATRIXA16 o)
		: Position(p), Size(s), Orientation(o) {}
}OBB;

typedef struct Plane
{
	D3DXVECTOR3		Normal;		// 중점
	float			Distance;	// 길이

	inline  Plane() : Normal(1, 0, 0) {}
	inline  Plane( D3DXVECTOR3& n, float d) : Normal(n), Distance(d) {}
}Plane;

typedef struct Triangle
{
	union 
	{
		struct				// 삼각형을 이룰 세 점
		{
			D3DXVECTOR3 a;
			D3DXVECTOR3 b;
			D3DXVECTOR3 c;
		};
		D3DXVECTOR3	m_vPoints[3];
		float		m_fValues[9];
	};
	inline Triangle() {}
	inline Triangle(D3DXVECTOR3& p1, D3DXVECTOR3& p2, D3DXVECTOR3& p3) {}
}Triangle;

typedef struct Interval // min 과 max 를 float형으로 가지고 있는 구조체
{
	float min;
	float max;
}Interval;

class cGeometry3D
{
	SINGLETONE(cGeometry3D);
private:
public:
		
	float		Length(Line& line);				// 두 점 사이의 길이를 구한다.
	float		LengthSq(Line& line);			// 두 점 사이의 길이의 제곱값을 구한다.
	float		Magnitude(D3DXVECTOR3& v);		// 각을 구한다.
	float		MagnitudeSq(D3DXVECTOR3& v);	// 각의 제곱을 구한다.

	Ray			FromPoints(D3DXVECTOR3& from, D3DXVECTOR3& to);	// Ray의 방향을 반환해준다.

	D3DXVECTOR3 GetMin(AABB& aabb);	// AABB의 min 점을 반환.
	D3DXVECTOR3	GetMax(AABB& aabb); // AABB의 max 점을 반환.
	AABB		FromMinMax(D3DXVECTOR3& min, D3DXVECTOR3& max); // AABB 상자를 만들어냄

	float		PlaneEquation(D3DXVECTOR3& pt, Plane& plane);

	// point test || bool 함수는 충돌했는지 안했는지 확인하는 함수
	// point in sphere
	bool		PointInSphere(D3DXVECTOR3& point, Sphere& sphere);
	D3DXVECTOR3	ClosestPoint(Sphere& sphere, D3DXVECTOR3& point);
	// point in aabb(Axis Aligned Bounding Box)
	bool		PointInAABB(D3DXVECTOR3& point, AABB& aabb);
	D3DXVECTOR3	ClosestPoint(AABB& aabb, D3DXVECTOR3& point);
	// point in obb(Oriented Bounding Box)
	bool		PointInOBB(D3DXVECTOR3& point, OBB& obb);
	D3DXVECTOR3	ClosestPoint(OBB& obb, D3DXVECTOR3& point);
	// point in Plane
	bool		PointOnPlane(D3DXVECTOR3& point, Plane& plane);
	D3DXVECTOR3	ClosestPoint(Plane& plane, D3DXVECTOR3& point);
	// point On line
	bool		PointOnLine(D3DXVECTOR3& point, Line& line);
	D3DXVECTOR3	ClosestPoint(Line& line, D3DXVECTOR3& point);
	// point On Ray
	bool		PointOnRay(D3DXVECTOR3& point, Ray& ray);
	D3DXVECTOR3	ClosestPoint(Ray& ray,D3DXVECTOR3& point);

	// sphere test
	// sphere to Sphere
	bool		SphereSphere(Sphere& s1, Sphere& s2);
	// sphere to aabb
	bool		SphereAABB(Sphere& sphere, AABB& aabb);
	// sphere to obb
	bool		SphereOBB(Sphere& sphere, OBB& obb);
	// sphere to plane
	bool		SpherePlane(Sphere& sphere, Plane& plane);
	// aabb to aabb
	bool		AABBAABB(AABB& aabb1, AABB& aabb2);
	// aabb to obb
	Interval	GetInterval(AABB& rect, D3DXVECTOR3& axis);
	Interval	GetInterval(OBB& rect, D3DXVECTOR3& axis);
	bool		OverlapOnAxis(AABB& aabb, OBB& obb, D3DXVECTOR3 & axis);
	bool		AABBOBB(AABB& aabb, OBB& obb);
	// aabb to plane
	bool		AABBPlane(AABB& aabb, Plane& plane);
	// obb to obb
	bool		OverlapOnAxis(OBB& obb1, OBB& obb2, D3DXVECTOR3& axis);
	bool		OBBOBB(OBB& obb1, OBB& obb2);
	// obb to plane
	bool		OBBPlane(OBB& obb, Plane& plane);
	// plane to plane
	bool		PlanePlane(Plane& plane1, Plane& plane2);
};

