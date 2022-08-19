#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		engine.InitialiseAudio();
		w.LoadAudioWaveform("assets/SampleA.wav");
		ship.Load("assets/ship.png");
		// w.file.data()[23] = 8;
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if(GetKey(olc::SPACE).bPressed)
		{
			engine.PlayWaveform(&w);
		}
			
		Clear(olc::BLACK);
		DrawSprite(0,0, ship.Sprite());
		
		return true;
	}

	bool OnUserDestroy() override
	{
		engine.DestroyAudio();
		return true;
	}
	
	olc::Renderable ship;
	olc::sound::WaveEngine engine;
	olc::sound::Wave w;
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}

#if defined(_WIN32)
    int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
    {
        return main();
    }
#endif
