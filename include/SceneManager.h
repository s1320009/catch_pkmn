#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "PkmnComponent.h"

class SceneManager {
public:
	static SceneManager& Instance();

	void AddObject(std::shared_ptr<GameObject> obj);
	std::shared_ptr<GameObject> CreatePkmnObject(PkmnBlueprint blueprint, Vector2 pos, const std::string& name = "Pkmn");

	void UpdateAll();
	void DrawAll();
	void ResetAll();
	void ClearAll();

	std::vector<PkmnComponent*> GetPkmnComponents();
	int GetActivePkmnCount();

private:
	std::vector<std::shared_ptr<GameObject>> m_objects;
	int m_nextId = 1000; // starting id for scene m_objects
};
