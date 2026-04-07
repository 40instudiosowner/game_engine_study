#pragma once

class UI
{

public:
	void Update();

private:
    void UpdateUserInput();
    void UpdateLogic();
    void UpdateGui();
    void Render();
};