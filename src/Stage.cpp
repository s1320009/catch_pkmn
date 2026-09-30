#include "Stage.h"
#include "SceneManager.h"

StageData GetStageData(int stageIndex) {
	StageData stage = {};

	PkmnBlueprint pika = {};
	pika.type = PKMN_PIKACHU;
	pika.radius = 15.0f;
	pika.color = YELLOW;
	pika.dashSpeed = 9.0f;
	pika.attackduration = 0.5f;
	pika.thinkduration = 1.0f;
	pika.stayduration = 1.5f;
	pika.moveduration = 0.5f;

	PkmnBlueprint m2 = {};
	m2.type = PKMN_MEWTWO;
	m2.radius = 20.0f;
	m2.color = PURPLE;
	m2.dashSpeed = 7.0f;
	m2.attackduration = 1.0f;
	m2.thinkduration = 0.5f;
	m2.stayduration = 1.0f;
	m2.moveduration = 0.5f;

	switch (stageIndex) {
	case 0:
		stage.pkmnSpawns.push_back({ pika, { 700, 300 } });
		break;
	case 1:
		stage.pkmnSpawns.push_back({ pika, { 400, 300 } });
		stage.pkmnSpawns.push_back({ m2, { 100, 300 } });
		break;
	case 2:
		stage.pkmnSpawns.push_back({ m2, { 500, 100 } });
		stage.pkmnSpawns.push_back({ m2, { 100, 300 } });
		stage.pkmnSpawns.push_back({ m2, { 900, 600 } });
		break;
	}
	return stage;
}

void LoadStage(int stageIndex) {
	SceneManager::Instance().ClearAll();
	StageData stage = GetStageData(stageIndex);
	for (const auto& spawnData : stage.pkmnSpawns) {
		SceneManager::Instance().CreatePkmnObject(
			spawnData.blueprint, 
			spawnData.initialPos, 
			"Pkmn"
		);
	}
}