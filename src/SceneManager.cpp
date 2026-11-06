#include "SceneManager.h"
#include "TextureAnimeComponent.h"
#include "Texture.h"
#include <algorithm>

SceneManager& SceneManager::Instance() {
	static SceneManager inst;
	return inst;
}

void SceneManager::AddObject(std::shared_ptr<GameObject> obj) {
	m_objects.push_back(obj);
}

std::shared_ptr<GameObject> SceneManager::CreatePkmnObject(PkmnBlueprint blueprint, Vector2 pos, const std::string& name) {
	auto obj = std::make_shared<GameObject>(m_nextId++, name, "Pkmn");
	obj->position = pos;
	obj->scale = { blueprint.radius * 3.0f, blueprint.radius * 3.0f };
	obj->AddComponent<PkmnComponent>(blueprint, pos);

	if (blueprint.type == PKMN_PIKACHU) {
		obj->AddComponent<TextureAnimeComponent>(dogAnime);
	}
	else if (blueprint.type == PKMN_MEWTWO) {
		obj->AddComponent<TextureAnimeComponent>(eyeAnime);
	}

	m_objects.push_back(obj);
	return obj;
}

void SceneManager::UpdateAll() {
	for (auto& o : m_objects) {
		if (o) o->Update();
	}
}

void SceneManager::DrawAll() {
	for (auto& o : m_objects) {
		if (o) o->Draw();
	}
}

void SceneManager::ResetAll() {
	for (auto& o : m_objects) {
		if (!o) continue;
		if (auto p = o->GetComponent<PkmnComponent>()) p->Reset();
	}
}

void SceneManager::ClearAll() {
	m_objects.clear();
}

std::vector<PkmnComponent*> SceneManager::GetPkmnComponents() {
	std::vector<PkmnComponent*> list;
	for (auto& o : m_objects) {
		if (!o) continue;
		if (auto p = o->GetComponent<PkmnComponent>()) list.push_back(p);
	}
	return list;
}

int SceneManager::GetActivePkmnCount() {
	int cnt = 0;
	for (auto p : GetPkmnComponents()) {
		if (p && p->IsActive()) cnt++;
	}
	return cnt;
}
