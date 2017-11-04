#pragma once

#define COLMANAGER	cGeometry3D::GetInstance()

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
	D3DXVECTOR3 Start;	// ������
	D3DXVECTOR3 End;	// ����

	inline Line() {}
	inline Line(D3DXVECTOR3& s, D3DXVECTOR3& e) : Start(s), End(e) {}
}Line;

typedef struct Ray
{
	D3DXVECTOR3		Origin;		// Ray�� �� ��ġ
	D3DXVECTOR3		Direction;	// Ray�� �� ����

	inline Ray() : Direction(0.0f, 0.0f, 1.0f) {}
	inline Ray(D3DXVECTOR3& o, D3DXVECTOR3& d) : Origin(o), Direction(d) {	NomalizeDirection();}
	inline void NomalizeDirection() { D3DXVec3Normalize(&Direction, &Direction); } // ������ �Ÿ��� �ʿ���⋚���� ��ֶ������Ѵ�.
}Ray;

typedef struct Sphere
{
	D3DXVECTOR3 Position;	// ����
	float		Radius;		// ������

	inline Sphere() : Radius(1.0f) {}
	inline Sphere(D3DXVECTOR3& p, float r) : Position(p), Radius(r) {}
}Sphere;

typedef struct AABB
{
	D3DXVECTOR3		Origin;	// ����
	D3DXVECTOR3		Size;	// �ڽ� ũ��

	inline	AABB() : Size(1, 1, 1) {}
	inline	AABB(D3DXVECTOR3& o,  D3DXVECTOR3& s) : Origin(o), Size(s) {}
}AABB;

typedef struct OBB
{
	D3DXVECTOR3					Position;		// ����
	D3DXVECTOR3					Size;			// �ڽ� ũ��
	D3DXMATRIXA16				Orientation;	// �ڽ� ȸ�� ��Ʈ����
	std::vector<D3DXVECTOR3>	vecVertex;		// �ڽ��� ���ؽ�

	inline	OBB() : Size(1, 1, 1) {}
	inline  OBB(D3DXVECTOR3& p,  D3DXVECTOR3& s,  D3DXMATRIXA16 o)
		: Position(p), Size(s), Orientation(o) {}
}OBB;

typedef struct Plane
{
	D3DXVECTOR3		Normal;		// ����
	float			Distance;	// ����

	inline  Plane() : Normal(1, 0, 0) {}
	inline  Plane( D3DXVECTOR3& n, float d) : Normal(n), Distance(d) {}
}Plane;

typedef struct Triangle
{
	union 
	{
		struct				// �ﰢ���� �̷� �� ��
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

typedef struct Interval // min �� max �� float������ ������ �ִ� ����ü
{
	float min;
	float max;
}Interval;

class cGeometry3D
{
	SINGLETONE(cGeometry3D);
private:
public:
		
	float		Length(IN  Line& line);				// �� �� ������ ���̸� ���Ѵ�.
	float		LengthSq(IN  Line& line);			// �� �� ������ ������ �������� ���Ѵ�.
	float		Magnitude(IN  D3DXVECTOR3& v);		// ���� ���Ѵ�.
	float		MagnitudeSq(IN  D3DXVECTOR3& v);	// ���� ������ ���Ѵ�.

	Ray			FromPoints(IN  D3DXVECTOR3& from, IN  D3DXVECTOR3& to);	// Ray�� ������ ��ȯ���ش�.

	D3DXVECTOR3 GetMin(IN  AABB& aabb);	// AABB�� min ���� ��ȯ.
	D3DXVECTOR3	GetMax(IN  AABB& aabb); // AABB�� max ���� ��ȯ.
	AABB		FromMinMax(IN  D3DXVECTOR3& min, IN  D3DXVECTOR3& max); // AABB ���ڸ� ����

	float		PlaneEquation(IN  D3DXVECTOR3& pt, IN  Plane& plane);

	// point test || bool �Լ��� �浹�ߴ��� ���ߴ��� Ȯ���ϴ� �Լ�
	// point in sphere
	bool		PointInSphere(IN  D3DXVECTOR3& point, IN  Sphere& sphere);
	D3DXVECTOR3	ClosestPoint(IN  Sphere& sphere, IN  D3DXVECTOR3& point);
	// point in aabb(Axis Aligned Bounding Box)
	bool		PointInAABB(IN  D3DXVECTOR3& point, IN  AABB& aabb);
	D3DXVECTOR3	ClosestPoint(IN  AABB& aabb, IN  D3DXVECTOR3& point);
	// point in obb(Oriented Bounding Box)
	bool		PointInOBB(IN  D3DXVECTOR3& point, IN  OBB& obb);
	D3DXVECTOR3	ClosestPoint(IN  OBB& obb, IN  D3DXVECTOR3& point);
	// point in Plane
	bool		PointOnPlane(IN  D3DXVECTOR3& point, IN  Plane& plane);
	D3DXVECTOR3	ClosestPoint(IN  Plane& plane, IN  D3DXVECTOR3& point);
	// point On line
	bool		PointOnLine(IN  D3DXVECTOR3& point, IN  Line& line);
	D3DXVECTOR3	ClosestPoint(IN  Line& line, IN  D3DXVECTOR3& point);
	// point On Ray
	bool		PointOnRay(IN  D3DXVECTOR3& point, IN  Ray& ray);
	D3DXVECTOR3	ClosestPoint(IN  Ray& ray, IN  D3DXVECTOR3& point);

	// sphere test
	// sphere to Sphere
	bool		SphereSphere(IN  Sphere& s1, IN  Sphere& s2);
	// sphere to aabb
	bool		SphereAABB(IN  Sphere& sphere, IN  AABB& aabb);
	// sphere to obb
	bool		SphereOBB(IN  Sphere& sphere, IN  OBB& obb);
	// sphere to plane
	bool		SpherePlane(IN  Sphere& sphere, IN  Plane& plane);
	// aabb to aabb
	bool		AABBAABB(IN  AABB& aabb1, IN  AABB& aabb2);
	// aabb to obb
	Interval	GetInterval(IN  AABB& rect, IN  D3DXVECTOR3& axis);
	Interval	GetInterval(IN  OBB& rect, IN  D3DXVECTOR3& axis);
	bool		OverlapOnAxis(IN  AABB& aabb, IN  OBB& obb, IN  D3DXVECTOR3 & axis);
	bool		AABBOBB(IN  AABB& aabb, IN  OBB& obb);
	// aabb to plane
	bool		AABBPlane(IN  AABB& aabb, IN  Plane& plane);
	// obb to obb
	bool		OverlapOnAxis(IN  OBB& obb1, IN  OBB& obb2, IN  D3DXVECTOR3& axis);
	bool		OBBOBB(IN  OBB& obb1, IN  OBB& obb2);
	// obb to plane
	bool		OBBPlane(IN  OBB& obb, IN  Plane& plane);
	// plane to plane
	bool		PlanePlane(IN  Plane& plane1, IN  Plane& plane2);
};

