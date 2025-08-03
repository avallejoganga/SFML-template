#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include "Animation.h"

class Component
{
public:
	bool exists = false;
};

class CTransform : public Component
{
public:
	Vec2f pos{ 0, 0 };
	Vec2f prevPos{ 0, 0 };
	Vec2f scale{ 1.0, 1.0 };
	Vec2f velocity{ 0, 0 };
	float angle = 0;

	CTransform() = default;
	CTransform(const Vec2f& p, const Vec2f& v, float a)
		: pos(p), velocity(v), angle(a)	{ }
};


class CBoundingBox : public Component
{
public:
	Vec2f size;
	Vec2f halfSize;

	CBoundingBox() = default;
	CBoundingBox(const Vec2f& s) :
		size(s), halfSize(Vec2f(s.x / 2, s.y / 2)){ }

};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation() = default;
	CAnimation(const Animation& animation, bool r) :
		animation(animation), repeat(r) { }
};

class CGravity : public Component
{
public:
	float gravity = 0;

	CGravity() = default;
	CGravity(float g) 
		: gravity(g) { }
};

class CState : public Component
{
public:
	std::string state = "jumping";
	
	CState() = default;
	CState(const std::string & s) : state(s) { }
};


class CLifespan : public Component
{
public:
	int lifespan = 0;
	int remaining = 0;

	CLifespan() = default;
	CLifespan(int time) :
		lifespan(time),
		remaining(time){ }

};

class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;
	bool grounded = true;

	CInput() = default;
};

