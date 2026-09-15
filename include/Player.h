#pragma once
#include "Component.h"
#include "raylib.h"
#include "Mewtwo.h"
#include "Pkmn.h"

class Player : public Component {
public:
	Player();
	void Reset();
	void Update() override;
	void Draw() override;

	void CheckPlayerHurt(ProjectileManager* manager, PkmnManager* pkmnManager);

	bool IsDead() const;
	bool IsInvincible() const;
	
private:
	// プレイヤーの状態を表す列挙型
	enum class STATE {
		FINE,
		DEAD
	}m_STATE;

	Vector2 m_speed = { 0.0f, 0.0f };
	int m_life = 1;
	bool m_isInvincible = false;
	int m_invincibleFrame = 0;
	Color m_color = RED;	
};