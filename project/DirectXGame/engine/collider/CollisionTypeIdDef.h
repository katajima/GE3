#pragma once

#include <cstdint>

// コリジョン種別ID定義
enum class CollisionTypeIdDef : uint32_t {
	kdefault,
	kPlayer,
	kPlayerWeapon,
	kEnemy,
	kEnemyWeapon,
	kEnemyWeapon2,
};

enum class ColliderType
{
	Sphere,
	Capsule,
	Box,
	Mesh,
};
