#pragma once

struct ST_SPPT_DATA
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	float speed;
	float life;
	float age;
	float roty;
	D3DXCOLOR color;
	D3DXCOLOR colorfade;
	bool isAlive;

	ST_SPPT_DATA()
		: pos(0, 0, 0)
		, dir(0, 0, 0)
		, speed(0.f)
		, life(0.f)
		, age(0.f)
		, isAlive(true)
	{
		ZeroMemory(&color, sizeof(D3DXCOLOR));
		ZeroMemory(&colorfade, sizeof(D3DXCOLOR));
	}
};

struct ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle) = 0;
};

struct ST_EFX_LTURN : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		particle.roty -= 0.2f;
		D3DXMATRIXA16 matr;
		D3DXMatrixRotationY(&matr, particle.roty);
		D3DXVec3TransformNormal(&particle.dir, &particle.dir, &matr);
	}
};

struct ST_EFX_RTURN : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		particle.roty += 0.2f;
		D3DXMATRIXA16 matr;
		D3DXMatrixRotationY(&matr, particle.roty);
		D3DXVec3TransformNormal(&particle.dir, &particle.dir, &matr);
	}
};

struct ST_EFX_MOVE : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		particle.pos += particle.dir * particle.speed;
	}
};