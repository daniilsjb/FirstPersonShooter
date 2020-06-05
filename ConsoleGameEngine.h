#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <string>
#include <queue>
#include <chrono>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <list>

#define DEFAULT_COLOR 0xF0
#define DEFAULT_CHAR ' '

#define MAX_VOLUME 0xFFFF

class Sprite;

class ConsoleGameEngine
{
	///////////////////////////////////////// CORE /////////////////////////////////////////////////

private:
	HANDLE console, consoleInput;
	DWORD originalInput;

	SMALL_RECT screenArea;

	CHAR_INFO *screen;

	int screenWidth, screenHeight;

	static std::atomic<bool> running;
	static std::condition_variable finished;
	static std::mutex gameMutex;

public:
	ConsoleGameEngine();

	~ConsoleGameEngine();

	bool ConstructScreen(int width, int height, int pixelWidth, int pixelHeight);

private:
	int Error(const wchar_t* message);

public:
	void Start();

private:
	void GameThread();

	static BOOL CloseHandler(DWORD evt);

public:
	int GetScreenWidth();

	int GetScreenHeight();

protected:
	short GetScreenCharacter(int i);

	short GetScreenCharacter(int x, int y);

	short GetScreenColor(int i);

	short GetScreenColor(int x, int y);

	void SetApplicationTitle(LPCWSTR title);

	virtual bool OnStart() = 0;

	virtual bool OnUpdate(float elapsedTime) = 0;

	virtual bool OnDestroy();

	//////////////////////////////////////// RENDER ////////////////////////////////////////////////

protected:
	void Draw(int index, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawPoint(int x, int y, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawLine(int x0, int y0, int x1, int y1, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

private:
	void FillBottomFlatTriangle(int x0, int y0, int x1, int y1, int x2, int y2, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void FillTopFlatTriangle(int x0, int y0, int x1, int y1, int x2, int y2, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

protected:
	void DrawRectangle(int x0, int y0, int x1, int y1, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawFilledRectangle(int x0, int y0, int x1, int y1, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawCircle(int cx, int cy, int r, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawFilledCircle(int cx, int cy, int r, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void DrawSprite(int x, int y, Sprite& sprite);

	void DrawSpriteAlpha(int x, int y, Sprite& sprite, short transparencyCol);

	void DrawPartialSprite(int x, int y, Sprite& sprite, int ox, int oy, int w, int h);

	void DisplayText(int x, int y, std::wstring text, short color = DEFAULT_COLOR);

	void DisplayTextAlpha(int x, int y, std::wstring text, short color = DEFAULT_COLOR);

	void Fill(int x, int y, int length, short character = DEFAULT_CHAR, short color = DEFAULT_COLOR);

	void ClearScreen();

	void FloodFill(int x, int y, short color = DEFAULT_COLOR);

	void Clip(int &x, int &y);

	//////////////////////////////////////// INPUT /////////////////////////////////////////////////

private:
	struct KeyState {
		bool pressed = false;
		bool held = false;
		bool released = false;
	} keys[256];

	short mouseX = 0;
	short mouseY = 0;

	void ReadInput();

public:
	short GetMouseX();

	short GetMouseY();

	KeyState GetKey(short key);

	//////////////////////////////////////// AUDIO /////////////////////////////////////////////////

private:
	class AudioClip
	{
	public:
		WAVEFORMATEX format;
		float *data = nullptr;

		long length;

		bool isValid = false;

		AudioClip();

		~AudioClip();

		AudioClip(std::wstring fileName);

	};

	struct CurrentlyPlayingClip
	{
		int audioClipID = 0;

		long samplePosition = 0;

		bool looped = false;
		bool paused = false;
		bool finished = false;

		CurrentlyPlayingClip();

		CurrentlyPlayingClip(int id, bool looped);

		void Restart();
	};

	std::vector<AudioClip> audioClips;
	std::list<CurrentlyPlayingClip> currentlyPlayingClips;

	HWAVEOUT device = 0;

	int samplesPerSec = 0;
	int channels = 0;
	int blockCount = 0;
	int samplesPerBlock = 0;

	short *samplesMemory = nullptr;
	WAVEHDR *blocks = nullptr;

	int currentBlock = 0;
	std::atomic<int> freeBlocks = 0;

	std::mutex writingBlock;
	std::condition_variable blockWritten;

	std::atomic<bool> audioThreadActive = false;
	std::thread audioThread;

	std::atomic<float> globalTime = 0.0f;

	bool soundMuted = false;
	int currentVolume = MAX_VOLUME;

	//Static wrapper for the callback function because Microsoft
	//(callback cannot be method of the class itself, but it may be called via dwInstance)
	static void CALLBACK waveOutProcWrapper(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

	//The actual callback function
	void waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

	void AudioThread();

	float GetMixerOutput(float timeStep, int channel);

public:
	void PlayAudioClip(int id, bool loop = false);

protected:
	unsigned int LoadAudioClip(std::wstring fileName);

	bool StartAudio(int samplesPerSec = 44100, int channels = 1, int blockCount = 8, int samplesPerBlock = 512);

	void DestroyAudio();

	virtual float OnUserSoundSample(int channel, float globalTime, float timeStep);

	virtual float OnUserSoundFilter(int channel, float globalTime, float mixedSample);

	int GetVolume();

	void SetVolume(float percent);

	void MuteAudio();

	void UnmuteAudio();

	void PauseAudio(int id);

	void PauseAllAudio();

	void RestartAudio(int id);

	void RestartAllAudio();

	void StopAudio(int id);

	void StopAllAudio();

};

enum Color
{
	FG_BLACK = 0x00,
	FG_DARK_BLUE = 0x01,
	FG_DARK_GREEN = 0x02,
	FG_DARK_CYAN = 0x03,
	FG_DARK_RED = 0x04,
	FG_DARK_PINK = 0x05,
	FG_DARK_YELLOW = 0x06,
	FG_GRAY = 0x07,
	FG_DARK_GRAY = 0x08,
	FG_BLUE = 0x09,
	FG_GREEN = 0xA,
	FG_CYAN = 0xB,
	FG_RED = 0xC,
	FG_PINK = 0xD,
	FG_YELLOW = 0xE,
	FG_WHITE = 0xF,

	BG_BLACK = 0x00,
	BG_DARK_BLUE = 0x10,
	BG_DARK_GREEN = 0x20,
	BG_DARK_CYAN = 0x30,
	BG_DARK_RED = 0x40,
	BG_DARK_PINK = 0x50,
	BG_DARK_YELLOW = 0x60,
	BG_GRAY = 0x70,
	BG_DARK_GRAY = 0x80,
	BG_BLUE = 0x90,
	BG_GREEN = 0xA0,
	BG_CYAN = 0xB0,
	BG_RED = 0xC0,
	BG_PINK = 0xD0,
	BG_YELLOW = 0xE0,
	BG_WHITE = 0xF0
};

enum PIXEL_TYPE
{
	PIXEL_QUARTER = 0x2591,
	PIXEL_HALF = 0x2592,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_SOLID = 0x2588
};

class Sprite
{
private:
	int width = 0;
	int height = 0;

	CHAR_INFO *contents = nullptr;

public:
	Sprite();

	Sprite(int width, int height);

	Sprite(std::wstring fileName);

	~Sprite();

	int GetWidth();

	int GetHeight();

	bool Create(int width, int height);

	void Copy(const Sprite& sprite);

	void SetCharacter(int x, int y, short character);

	short GetCharacter(int x, int y);

	const short GetCharacter(int x, int y) const;

	void SetColor(int x, int y, short color);

	short GetColor(int x, int y);

	const short GetColor(int x, int y) const;

	short SampleCharacter(float x, float y);

	short SampleColor(float x, float y);

	bool Save(std::wstring fileName);

	bool Load(std::wstring fileName);

};