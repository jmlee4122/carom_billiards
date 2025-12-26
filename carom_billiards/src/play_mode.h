#pragma once

#include "game_mode.h"

class PlayMode : public GameMode
{
public:
    void Init() override;
    void Cleanup() override;
    void HandleEvents(unsigned char key, int x, int y) override;
    void Update(float dt) override;
    void Draw() override;
};