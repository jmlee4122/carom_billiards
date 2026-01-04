#pragma once

#include <string>
#include "game_mode.h"

class PlayMode : public GameMode
{
public:
    void Init() override;
    void Cleanup() override;
    void HandleKey(unsigned char key, int x, int y) override;
    void HandleSpecialKey(int key, int x, int y) override;
    void HandleSpecialKeyUp(int key, int x, int y) override;
    void Update(float dt) override;
    void Draw(GLuint shaderID) override;
    void DrawUI();

private:
    void RenderText(float x, float y, const std::string& text, void* font);
};