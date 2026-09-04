#pragma once
#include "Pkmn.h"

typedef struct {
	PkmnBlueprint blueprint;
	Vector2 initialPos;
} PkmnSpawnData;

typedef struct {
	int pkmnCount;
	PkmnSpawnData pkmnSpawns[MAX_ACTIVE_POKEMON];
} StageData;

StageData GetStageData(int stageIndex);
void LoadStage(int stageIndex, PkmnManager* manager);