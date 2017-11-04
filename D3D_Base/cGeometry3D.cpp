#include "stdafx.h"
#include "cGeometry3D.h"
#include <cmath>
#include <cfloat>

cGeometry3D::cGeometry3D()
{
}

cGeometry3D::~cGeometry3D()
{
}

// sq �� �������� ��ȯ����
// ����
float cGeometry3D::Length(IN  Line & line)
{
	// ���� �� ������ �Ÿ�
	return D3DXVec3Length(&(line.Start - line.End));
}

float cGeometry3D::LengthSq(IN  Line & line)
{
	// ���� �� ������ �Ÿ� ����
	return D3DXVec3LengthSq(&(line.Start - line.End));
}

// ��
float cGeometry3D::Magnitude(IN  D3DXVECTOR3 & v)
{
	// ��
	return sqrtf(D3DXVec3Dot(&v, &v));
}

float cGeometry3D::MagnitudeSq(IN  D3DXVECTOR3 & v)
{
	// ���� ����
	return D3DXVec3Dot(&v, &v);
}

// ��
Ray cGeometry3D::FromPoints(IN  D3DXVECTOR3 & from, IN  D3DXVECTOR3 & to)
{
	D3DXVECTOR3 out;
	// ����ȭ(ũ��� ���ְ� ���⸸ ����� �۾�)
	D3DXVec3Normalize(&out, &(to - from));
	return Ray(from, out);
}

// AABB
D3DXVECTOR3 cGeometry3D::GetMin(IN  AABB & aabb)
{
	D3DXVECTOR3 p1 = aabb.Origin + aabb.Size;
	D3DXVECTOR3 p2 = aabb.Origin - aabb.Size;

	return D3DXVECTOR3(fminf(p1.x,p2.x), fminf(p1.y, p2.y), fminf(p1.z,p2.z));
}

D3DXVECTOR3 cGeometry3D::GetMax(IN  AABB & aabb)
{
	D3DXVECTOR3 p1 = aabb.Origin + aabb.Size;
	D3DXVECTOR3 p2 = aabb.Origin - aabb.Size;

	return D3DXVECTOR3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
}

AABB cGeometry3D::FromMinMax(IN  D3DXVECTOR3 & min, IN  D3DXVECTOR3 & max)
{
	// �ش� ��ü�� �ִ���ġ�� �ּ���ġ�� ���Ѵ�.
	return AABB((min + max) * 0.5f, (max - min) * 0.5f);
}

// Plane
float cGeometry3D::PlaneEquation(IN  D3DXVECTOR3 & pt, IN  Plane & plane)
{
	// ������ ���� ���� �˾Ƴ���.
	return D3DXVec3Dot(&pt, &plane.Normal);
}

// point test
// point in sphere
bool cGeometry3D::PointInSphere(IN  D3DXVECTOR3 & point, IN  Sphere & sphere)
{
	// ���� �� ������ �Ÿ� ������ ã��
	float magSq = D3DXVec3LengthSq(&(point - sphere.Position));
	// �������� ����
	float radSq = sphere.Radius * sphere.Radius;
	// ���� �� ������ �Ÿ� ������ ������ �������� ������ �浹
	return magSq < radSq;
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Sphere & sphere, IN  D3DXVECTOR3 & point)
{
	// ���� ���� �������� �Ÿ��� ���ؼ� �����Ѵ�.
	D3DXVECTOR3 SphereToPoint = point - sphere.Position;
	// ��ֶ���� ����� ũ��� ���ְ� ���⸸ ���ܳ��´�.
	D3DXVec3Normalize(&SphereToPoint, &SphereToPoint);
	// ���� * �������� ���� ��´�. 
	SphereToPoint = SphereToPoint * sphere.Radius;
	return SphereToPoint + sphere.Position;
}

// point in aabb
bool cGeometry3D::PointInAABB(IN  D3DXVECTOR3 & point, IN  AABB & aabb)
{
	// �浹�˻��� aabb�� min point �� max point�� ���Ѵ�.
	D3DXVECTOR3 Min = GetMin(aabb);
	D3DXVECTOR3 Max = GetMax(aabb);

	// aabb �� �˻��� point�� ��ġ�� aabb in�� �ȵȴٸ� false
	if (point.x < Min.x || point.y < Min.y || point.z < Min.z) { return false; }
	if (point.x > Max.x || point.y > Max.y || point.z > Max.z) { return false; }

	// aabb�� �˻��� point�� ��ġ�� aabb in�� �ȴٸ� true
	return true;
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  AABB & aabb, IN  D3DXVECTOR3 & point)
{
	D3DXVECTOR3 Result = point;		// ��ȯ�� �༮
	D3DXVECTOR3 Min = GetMin(aabb); 
	D3DXVECTOR3 Max = GetMax(aabb);

	// point�� aabb������ ���� aabb��ġ�� , �ȵ����� result���� ��ȯ�ؼ� 
	// ���� �߽ɰ� ���� ����� �Ÿ��� ���� ã��.
	Result.x = (Result.x < Min.x) ? Min.x : Result.x;
	Result.y = (Result.y < Min.y) ? Min.y : Result.y;
	Result.z = (Result.z < Min.z) ? Min.z : Result.z;

	Result.x = (Result.x > Max.x) ? Max.x : Result.x;
	Result.y = (Result.y > Max.y) ? Max.y : Result.y;
	Result.z = (Result.z > Max.z) ? Max.z : Result.z;

	return Result;
}

// point in obb
bool cGeometry3D::PointInOBB(IN  D3DXVECTOR3 & point, IN  OBB & obb)
{
	// ������ ������ ��ġ�� ���� ������ ����.
	D3DXVECTOR3 Dir = point - obb.Position;

	// for���� 3�� ������. (x��, y��, z��)
	// OBB ������ ���������� ���� �����ϰ� �ش� ���� OBB���ڿ� ���������� �Ÿ��� ����
	for (int i = 0; i < 3; ++i)
	{
		// XYZ ���� ��Ÿ�� ���͸� ����.
		 float* Orientation = &obb.Orientation[i * 4]; 
		D3DXVECTOR3	Axis
		(
			Orientation[0],
			Orientation[1],
			Orientation[2]
		);

		// ���� ����� �� �࿡ ���� �����ؼ� �� ���� �����ؼ� OBB���ڰ� ������ �󸶳� �������ִ��� ����.
		// normalize �Ŀ� �����ϸ� ���� ���� �� �ְ�, normalize�����ʰ� �����ϸ� ���� �ȴ�.
		float Distance = D3DXVec3Dot(&Dir, &Axis);

		// Distance�� 1���� ũ�ų� ������� ���� OBB���� �ȿ� ����. �� �浹�� �ƴϴ�.
		if (Distance > obb.Size[i]) { return false; }
		if (Distance < -obb.Size[i]) { return false; }
	}

	return true;
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  OBB & obb, IN  D3DXVECTOR3 & point)
{
	// point ���� obb�� ���������� ����
	D3DXVECTOR3	Result = obb.Position;
	D3DXVECTOR3 Dir = point - obb.Position;

	// for���� 3�� ������. (x��, y��, z��)
	for (int i = 0; i < 3; ++i)
	{
		// XYZ ���� ��Ÿ�� ���͸� ����.
		 float* Orientation = &obb.Orientation[i * 4]/*.asArray*/;

		D3DXVECTOR3	Axis
		(
			Orientation[0],
			Orientation[1],
			Orientation[2]
		);

		// ���� ����� �� �࿡ ���� �����ؼ� �� ���� �����ؼ� OBB���ڰ� ������ �󸶳� �������ִ��� ����.
		// normalize �Ŀ� �����ϸ� ���� ���� �� �ְ�, normalize�����ʰ� �����ϸ� ���� �ȴ�.
		float Distance = D3DXVec3Dot(&Dir, &Axis);

		// Distance�� 1���� ũ�ų� ������ ���� �������ش�.
		if (Distance > obb.Size[i]) { Distance = obb.Size[i]; }
		if (Distance < -obb.Size[i]) { Distance = -obb.Size[i]; }

		Result = Result + (Axis * Distance);
	}
	return Result;
}

// point in plane
bool cGeometry3D::PointOnPlane(IN  D3DXVECTOR3 & point, IN  Plane & plane)
{
	float Dot = D3DXVec3Dot(&point, &plane.Normal);
	return Dot - plane.Distance == 0.0f; // ���� ����� ǥ�鿡 �ִ��� Ȯ�� �Ѵ�.
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Plane & plane, IN  D3DXVECTOR3 & point)
{
	float Dot = D3DXVec3Dot(&plane.Normal,&point);
	float Distance = Dot - plane.Distance;
	return point - plane.Normal * Distance;
}

// point on line
bool cGeometry3D::PointOnLine(IN  D3DXVECTOR3 & point, IN  Line & line)
{
	// ������ ����� ���� �� ���� ���Ѵ�.
	D3DXVECTOR3 Closest = ClosestPoint(line, point);
	float		DistanceSq = MagnitudeSq(Closest - point);
	return DistanceSq == 0.0f; // ���� ���� ���� �ִ��� Ȯ���Ѵ�.
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Line & line, IN  D3DXVECTOR3 & point)
{
	D3DXVECTOR3	LVec = line.End - line.Start; // Line Vector

	float t = D3DXVec3Dot(&(point - line.Start), &(LVec / D3DXVec3Dot(&LVec, &LVec))); 
	t = fmaxf(t, 0.0f);	// Clamp to 0
	t = fmaxf(t, 1.0f);	// Clamp to 1

	return line.Start + LVec * t;
}

// point on ray
bool cGeometry3D::PointOnRay(IN  D3DXVECTOR3 & point, IN  Ray & ray)
{
	if (point == ray.Origin) { return true; }

	// A - B = B���� A�� ���� ������ ���´�.
	D3DXVECTOR3 Norm = point - ray.Origin;

	// ũ��� �ʿ�����ϱ� ���⸸ ���ܳ��´�.
	D3DXVec3Normalize(&Norm, &Norm);

	float Diff = D3DXVec3Dot(&Norm, &ray.Direction);

	// ������ ray�� ���� ����� ray�� ������ ������ 1�� �����µ� �׷��� �浹.
	return Diff == 1.0f; // Consider using epsilon!
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Ray & ray, IN  D3DXVECTOR3 & point)
{
	float t = D3DXVec3Dot(&(point - ray.Origin), &ray.Direction);

	// x �Ǽ�, y �Ǽ� <- ���� ū �༮�� ��ȯ����.
	// t���� ����� �ޱ����� 0�� ���ؼ� Ŭ ��쿡 t�� �ް� �ƴҰ�� 0�� �޴´�.
	t = fmaxf(t, 0.0f); 

	return D3DXVECTOR3(ray.Origin + ray.Direction * t);
}

// sphere test
// sphere to Sphere
bool cGeometry3D::SphereSphere(IN  Sphere & s1, IN  Sphere & s2)
{
	float radiisum = s1.Radius + s2.Radius; // �� ���� �������� ���� ����
	float sqDistance = MagnitudeSq(s1.Position - s2.Position); // �� ���� ������ ���� ���� ������ ������ ����

	return sqDistance < radiisum * radiisum; // ���� ������ ������ ���� �������� �պ��� ������ �浹��.
}

// sphere to aabb
bool cGeometry3D::SphereAABB(IN  Sphere & sphere, IN  AABB & aabb)
{
	// AABB�ڽ����� ������ ���� ����� ���� ã�Ƽ� ����.
	D3DXVECTOR3 closestPoint = ClosestPoint(aabb, sphere.Position);

	// sphere�� aabb�� �Ÿ��� ����
	float distSq = MagnitudeSq(sphere.Position - closestPoint);
	// sphere�� �������� ����
	float radiusSq = sphere.Radius * sphere.Radius;

	return distSq < radiusSq;
}

// sphere to obb
bool cGeometry3D::SphereOBB(IN  Sphere & sphere, IN  OBB & obb)
{
	// obb�ڽ����� sphere�������� ����� �Ÿ��� �ִ� ���� ����.
	D3DXVECTOR3 closestPoint = ClosestPoint(obb, sphere.Position);

	float distSq = MagnitudeSq(sphere.Position - closestPoint); // ���� ������ sphere������ �Ÿ� ������ ����.
	float radiusSq = sphere.Radius * sphere.Radius; // sphere�� �������� ������ ����.

	return distSq < radiusSq; // �ΰ��� ���ؼ� �������� �������� ������ �浹��.
}

// sphere to plane
bool cGeometry3D::SpherePlane(IN  Sphere & sphere, IN  Plane & plane)
{ 
	// plane���� sphere�������� ����� �Ÿ��� �ִ� ���� ����.
	D3DXVECTOR3 closestPoint = ClosestPoint(plane, sphere.Position);

	float distSq = MagnitudeSq(sphere.Position - closestPoint); // ���� ������ sphere������ �Ÿ� ������ ����.
	float radiusSq = sphere.Radius * sphere.Radius; // sphere�� �������� ������ ����.

	return distSq < radiusSq; // �ΰ��� ���ؼ� �������� �������� ������ �浹��.
}

// aabb to aabb
bool cGeometry3D::AABBAABB(IN  AABB & aabb1, IN  AABB & aabb2)
{
	// first AABB�� �ִ� �ּ� �������� ����.
	D3DXVECTOR3 aMin = GetMin(aabb1);
	D3DXVECTOR3 aMax = GetMax(aabb1);

	// second AABB�� �ִ� �ּ� �������� ����.
	D3DXVECTOR3 bMin = GetMin(aabb2);
	D3DXVECTOR3 bMax = GetMax(aabb2);

	// �׸��� �׷��� Ȯ���ϸ� ����.
	return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
}

// aabb to obb
Interval cGeometry3D::GetInterval(IN  AABB & rect, IN  D3DXVECTOR3 & axis)
{
	// AABB�� �ִ� �ּ� �������� ����.
	D3DXVECTOR3 min = GetMin(rect);
	D3DXVECTOR3 max = GetMax(rect);

	// AABB �ڽ��� vertex �������� ����.
	D3DXVECTOR3 vertex[8] =
	{
		D3DXVECTOR3(min.x, max.y, max.z),
		D3DXVECTOR3(min.x, max.y, min.z),
		D3DXVECTOR3(min.x, min.y, max.z),
		D3DXVECTOR3(min.x, min.y, min.z),
		D3DXVECTOR3(max.x, max.y, max.z),
		D3DXVECTOR3(max.x, max.y, min.z),
		D3DXVECTOR3(max.x, min.y, max.z),
		D3DXVECTOR3(max.x, min.y, min.z)
	};

	// �ִ� �ּҸ� float ������ ������ �ִ� ����ü
	Interval result;

	result.min = result.max = D3DXVec3Dot(&axis, &vertex[0]);

	for (int i = 1; i < 8; ++i)
	{
		// ���ؽ����� ������� ���̸� ����.
		float projection = D3DXVec3Dot(&axis, &vertex[i]);

		// AABB�ڽ��� �ִ� �ּ� ���ؽ��� �������� ������.
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}

	return result;
}

Interval cGeometry3D::GetInterval(IN  OBB & rect, IN  D3DXVECTOR3 & axis)
{
	D3DXVECTOR3	vertex[8];

	// obb center ����
	D3DXVECTOR3 C = rect.Position;
	// obb extent ����
	D3DXVECTOR3 E = rect.Size;

	 float* o = rect.Orientation;
	//rect.Orientation._11;
	// obb axis ��
	// ���� 3*3 ���¥���� ����ؾ� ������ ���������ֱ⶧���� 4*4����� ���.
	// 0   1   2   3                                   0    1    2
	// 4   5   6   7                                   4    5    6
	// 8   9  10  11                                   8    9   10  << �̰͵鸸 �����.
    //12  13  14  15 << 3*3 ����� ����ؾ��ϱ⋚����    
	// ȸ���� ���Ͽ� ����ϱ⶧���� ����� 12~15�� �̵� �����̶� ����.
	// ����� 3 7 11 15�� ����ġ�� ����. xyz�� ����ϴ� �� 3*3�� ����ϴ°���.
	D3DXVECTOR3 A[] = 
	{
		D3DXVECTOR3(o[0],o[1],o[2]),
		D3DXVECTOR3(o[4],o[5],o[6]),
		D3DXVECTOR3(o[8],o[9],o[10])
	};

	// ���ý� ��ǥ = ���� (+-) �� * ���� (+-) �� * ���� (+-) �� * ����
	vertex[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2]; // +++
	vertex[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2]; // -++
	vertex[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2]; // +-+
	vertex[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2]; // ++-
	vertex[4] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2]; // --+
	vertex[5] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2]; // -+-
	vertex[6] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2]; // +--
	vertex[7] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2]; // ---

	rect.vecVertex.resize(8);

	for (int i = 0; i < rect.vecVertex.size(); ++i)
	{
		D3DXVECTOR3 temp = vertex[i];
		rect.vecVertex[i] = temp;
	}

	Interval result;
	result.min = result.max = D3DXVec3Dot(&axis, &vertex[0]);

	for (int i = 0; i < 8; ++i)
	{
		float projection = D3DXVec3Dot(&axis, &vertex[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}
		
	return result;
}

// �񱳿� ����� obb�ڽ��� aabb�ڽ��� �ӽ÷� �������.
bool cGeometry3D::OverlapOnAxis(IN  AABB & aabb, IN  OBB & obb, IN  D3DXVECTOR3 & axis)
{
	Interval a = GetInterval(aabb, axis);
	Interval b = GetInterval(obb, axis);

	return ((b.min <= a.max) && (a.min <= b.max));
}

bool cGeometry3D::AABBOBB(IN  AABB & aabb, IN  OBB & obb)
{
	 float* o = obb.Orientation;

	// 3D�󿡼� ��ü���� �浹�� Ȯ���ϱ����ؼ� �ּ� 15���� ���⿡�� üũ�� �ؾ��ϱ� ������
	// aabb�ڽ��� �� xyz 3���� obb�ڽ��� �� 3��. �׸��� �� ���� �����ؼ� ���� ���� 9���� �˻縦 �Ѵ�.
	D3DXVECTOR3 test[15] = {
		D3DXVECTOR3(1,0,0), // AABB axis 1
		D3DXVECTOR3(0,1,0), // AABB axis 2
		D3DXVECTOR3(0,0,1), // AABB aixs 3
		D3DXVECTOR3(o[0], o[1], o[2]), // OBB axis 1
		D3DXVECTOR3(o[4], o[5], o[6]), // OBB axis 2
		D3DXVECTOR3(o[8], o[9], o[10]) // OBB axis 3
	};

	for (int i = 0; i < 3; ++i) // aabb�ڽ��� ��3���� obb�ڽ��� ��3���� �����ؼ� test[6]���� ������ �߰����ش�.
	{
		D3DXVec3Cross(&test[6 + i * 3 + 0], &test[i], &test[3]);
		D3DXVec3Cross(&test[6 + i * 3 + 1], &test[i], &test[4]);
		D3DXVec3Cross(&test[6 + i * 3 + 2], &test[i], &test[5]);
	}

	for (int i = 0; i < 15; ++i)
	{
		if (!OverlapOnAxis(aabb, obb, test[i])) { return false;	}// seperating aixs found 
	}
		return true; // seperating aixs not  found;
}

// aabb to plane
bool cGeometry3D::AABBPlane(IN  AABB & aabb, IN  Plane & plane)
{
	float pLen = aabb.Size.x * fabsf(plane.Normal.x) +
		aabb.Size.y * fabsf(plane.Normal.y) +
		aabb.Size.z * fabsf(plane.Normal.z);

	float dot = D3DXVec3Dot(&plane.Normal, &aabb.Origin);
	float dist = dot - plane.Distance;

	return fabsf(dist) <= pLen;
}

// obb to obb
bool cGeometry3D::OverlapOnAxis(IN  OBB & obb1, IN  OBB & obb2, IN  D3DXVECTOR3 & axis)
{
	Interval a = GetInterval(obb1, axis);
	Interval b = GetInterval(obb2, axis);

	return ((b.min <= a.max) && (a.min <= b.max));
}

bool cGeometry3D::OBBOBB(IN  OBB & obb1, IN  OBB & obb2)
{
	 float* o1 = obb1.Orientation;
	 float* o2 = obb2.Orientation;

	D3DXVECTOR3 test[15] = {
		D3DXVECTOR3(o1[0],o1[1],o1[2]),
		D3DXVECTOR3(o1[4],o1[5],o1[6]),
		D3DXVECTOR3(o1[8],o1[9],o1[10]),

		D3DXVECTOR3(o2[0],o2[1],o2[2]),
		D3DXVECTOR3(o2[4],o2[5],o2[6]),
		D3DXVECTOR3(o2[8],o2[9],o2[10])
	};

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3Cross(&test[6 + i * 3 + 0], &test[i], &test[0]);
		D3DXVec3Cross(&test[6 + i * 3 + 1], &test[i], &test[1]);
		D3DXVec3Cross(&test[6 + i * 3 + 2], &test[i], &test[2]);
	}

	for (int i = 0; i < 15; ++i)
	{
		if (!OverlapOnAxis(obb1, obb2, test[i]))
		{
			return false; // seperating axis found
		}
	}

	return true; // seperating axis not found
}

// obb to plane
bool cGeometry3D::OBBPlane(IN  OBB & obb, IN  Plane & plane)
{
	// Local variables for readability only
	 float* o = obb.Orientation;
	D3DXVECTOR3 rot[] = {// rotation / orientation
		D3DXVECTOR3(o[0],o[1],o[2]),
		D3DXVECTOR3(o[4],o[5],o[6]),
		D3DXVECTOR3(o[8],o[9],o[10])
	};
	D3DXVECTOR3 normal = plane.Normal;

	float pLen =
		obb.Size.x * fabsf(D3DXVec3Dot(&normal, &rot[0])) +
		obb.Size.y * fabsf(D3DXVec3Dot(&normal, &rot[1])) +
		obb.Size.z * fabsf(D3DXVec3Dot(&normal, &rot[2]));

	float dist = D3DXVec3Dot(&plane.Normal, &obb.Position) - plane.Distance;

	return fabsf(dist) <= pLen;
}

// plane to plane
bool cGeometry3D::PlanePlane(IN  Plane & plane1, IN  Plane & plane2)
{
	// cross product returns 0 when used on parallel lines
	D3DXVECTOR3 d;
	D3DXVec3Cross(&d,&plane1.Normal, &plane2.Normal);

	return D3DXVec3Dot(&d, &d) != 0; // Consider using an epsilon;
}
