#pragma once
#include "raylib.h"
#include  "GameState.h"
#include "Player.h"
#include "Ball.h"
#include  "Pkmn.h"

void InitializeRule(GameObject* playerObject, Ball* ball);
void UpdateRule(GameObject* playerObject, Ball* ball, GameState* gameState);
void DrawRule(GameObject* playerObject,const Ball* ball);