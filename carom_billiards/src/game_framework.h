#pragma once

class GameMode;

//namespace Framework
//{
//	void ChangeMode(GameMode* mode);
//	void PushMode(GameMode* mode);
//	void PopMode();
//
//	void quit();
//	void Run(GameMode* startMode);
//}

class Framework
{
public:
	void Run(int argc, char** argv, GameMode* startMode);
	void ChangeMode(GameMode* mode);
	void PushMode(GameMode* mode);
	void PopMode();
	void quit();

private:
	bool Initialize(int argc, char** argv);
};