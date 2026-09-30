#include "SceneManager.h"
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
	obj->scale = { 1.0f, 1.0f };
	obj->AddComponent<PkmnComponent>(blueprint, pos);
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
