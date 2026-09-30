#pragma once
#include "Pkmn.h"
#include  <vector>

typedef struct {
	PkmnBlueprint blueprint;
	Vector2 initialPos;
} PkmnSpawnData;

typedef struct {
	std::vector<PkmnSpawnData> pkmnSpawns;
} StageData;

StageData GetStageData(int stageIndex);
void LoadStage(int stageIndex);