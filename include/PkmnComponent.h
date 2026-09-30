#pragma once
#include "Component.h"
#include "Pkmn.h"
#include "raylib.h"

class PkmnComponent : public Component {
public:
	PkmnComponent() = delete;
	PkmnComponent(PkmnBlueprint blueprint, Vector2 initialPos);

	void Update() override;
	void Draw() override;

	void Reset();

	void StartBounce();
	[[nodiscard]] bool IsVisible() const;
	[[nodiscard]] bool IsActive() const;
	[[nodiscard]] float GetRadius() const;
	[[nodiscard]] const PkmnBlueprint& GetBlueprint() const;
	[[nodiscard]] PkmnType GetType() const;
	[[nodiscard]] Vector2 GetPosition() const;
	void SetActive(bool active);

private:
	Pkmn m_state; // 内部で既存の Pkmn のロジックを使うため構造体を保持
	Vector2 m_initialPos;
};
