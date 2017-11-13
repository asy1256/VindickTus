#pragma once

struct ST_SPPT_DATA
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	float speed;
	float power, powerlimit;
	float life;
	float age;
	float roty;
	D3DXCOLOR color;
	D3DXCOLOR colorfade;
	bool isAlive;
	bool isTriger;

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
		D3DXMATRIXA16 matR, matT;
		D3DXMatrixRotationY(&matR, -particle.roty);
		D3DXMatrixTranslation(&matT, particle.pos.x, particle.pos.y, particle.pos.z);
		matR = matT * matR;
		particle.pos.x = matR._41;
		particle.pos.y = matR._42;
		particle.pos.z = matR._43;
	}
};

struct ST_EFX_RTURN : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		D3DXMATRIXA16 matR, matT;
		D3DXMatrixRotationY(&matR, particle.roty);
		D3DXMatrixTranslation(&matT, particle.pos.x, particle.pos.y, particle.pos.z);
		matR = matT * matR;
		particle.pos.x = matR._41;
		particle.pos.y = matR._42;
		particle.pos.z = matR._43;
	}
};

struct ST_EFX_DIRMOVE : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		particle.pos += particle.dir * particle.speed;
	}
};

struct ST_EFX_UP : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		if (particle.pos.y > 3.0f) { return; }
		particle.pos.y += 1.0f / 60.0f;
	}
};

struct ST_EFX_UP_DOWN : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		if (particle.pos.y > 1.0f) { particle.isTriger = true; }
		if (particle.pos.y < 0) { particle.isTriger = false; }
		if (particle.isTriger) { particle.pos.y -= 1.0f / 60.0f; }
		else { particle.pos.y += 1.0f / 40.0f; }
	}
};

struct ST_EFX_FALL : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		if (particle.power > particle.powerlimit) { particle.isTriger = true; }
		if (particle.isTriger)
		{
			particle.power -= particle.powerlimit / 40;
			particle.pos.y = particle.power;
		}
		else
		{
			particle.power += particle.powerlimit / 40;
			particle.pos.y = particle.power;
		}
	}
};

struct ST_EFX_INLINE : public ST_EFX_WORK
{
public:
	virtual void Work(ST_SPPT_DATA& particle)
	{
		particle.pos.x -= particle.pos.x / 45.0f;
	}
};