#include "PkmnComponent.h"
#include "Scene.h"
#include "GameObject.h"

PkmnComponent::PkmnComponent(PkmnBlueprint blueprint, Vector2 initialPos)
	: m_initialPos(initialPos)
{
	m_state = CreatePkmn(blueprint, initialPos);
}

void PkmnComponent::Update()
{
	// Sync transform position into internal state
	if (gameObject) {
		m_state.position = gameObject->position;
	}

	// Use global player position provided by Scene (will be set in main loop later)
	UpdatePkmn(&m_state, gPlayerPosition);

	// Sync back to GameObject transform
	if (gameObject) {
		gameObject->position = m_state.position;
	}
}

void PkmnComponent::Draw()
{
	DrawPkmn(m_state);
}

void PkmnComponent::Reset()
{
	m_state = CreatePkmn(m_state.blueprint, m_initialPos);
	if (gameObject) gameObject->position = m_initialPos;
}

void PkmnComponent::StartBounce()
{
	m_state.state = PKMN_STATE_BOUNCE;
	m_state.speed = {0.0f, 0.0f};
}

[[nodiscard]] bool PkmnComponent::IsVisible() const { return m_state.isVisible; }

[[nodiscard]] bool PkmnComponent::IsActive() const { return m_state.isActive; }

[[nodiscard]] float PkmnComponent::GetRadius() const { return m_state.blueprint.radius; }

[[nodiscard]] const PkmnBlueprint& PkmnComponent::GetBlueprint() const { return m_state.blueprint; }

[[nodiscard]] PkmnType PkmnComponent::GetType() const { return m_state.blueprint.type; }

[[nodiscard]] Vector2 PkmnComponent::GetPosition() const { return m_state.position; }

void PkmnComponent::SetActive(bool active) { m_state.isActive = active; }
