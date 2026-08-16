#pragma once
#include "raylib.h"
#include  "GameState.h"
#include "Player.h"
#include "Ball.h"
#include  "pkmn.h"

void InitializeRule(Player* player, Ball* ball, PkmnManager* pkmnManager);
void UpdateRule(Player* player, Ball* ball, PkmnManager* pkmnManager, GameState* gameState);
void DrawRule(const Player* player,const Ball* ball, const PkmnManager* pkmnManager);