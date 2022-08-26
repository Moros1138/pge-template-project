#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"
#include "AssetManager.h"

using am = AssetManager;


class OneLoneCoder_Asteroids : public olc::PixelGameEngine
{
public:
    OneLoneCoder_Asteroids()
    {
        sAppName = "Asteroids";
    }

private:
    struct sSpaceObject
    {
        int nSize;
        float x;
        float y;
        float dx;
        float dy;
        float angle;
    };

    std::vector<sSpaceObject> vecAsteroids;
    std::vector<sSpaceObject> vecBullets;
    sSpaceObject player;
    bool bDead = false;
    int nScore = 0;

    std::vector<std::pair<float, float>> vecModelShip;
    std::vector<std::pair<float, float>> vecModelAsteroid;
    
    olc::sound::WaveEngine waveEngine;
    
public:

    bool OnUserCreate() override
    {
        waveEngine.InitialiseAudio();
        am::LoadGraphic("background", "assets/gfx/space.png");
        
        am::LoadSound("bg-music",     "assets/sounds/bg-music.wav");
        am::LoadSound("laser",        "assets/sounds/Laser_Shoot11.wav");
        am::LoadSound("explosion",    "assets/sounds/Explosions1.wav");
        am::LoadSound("lose",         "assets/sounds/lose9.wav");
        am::LoadSound("thruster",     "assets/sounds/thruster.wav");

        vecModelShip = 
        {
            { 0.0f, -5.0f},
            {-2.5f, +2.5f},
            {+2.5f, +2.5f}
        }; // A simple Isoceles Triangle

        // Create a "jagged" circle for the asteroid. It's important it remains
        // mostly circular, as we do a simple collision check against a perfect
        // circle.
        int verts = 20;
        for (int i = 0; i < verts; i++)
        {
            float noise = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
            vecModelAsteroid.push_back(std::make_pair(noise * sinf(((float)i / (float)verts) * 6.28318f), 
                                                 noise * cosf(((float)i / (float)verts) * 6.28318f)));
        }

        backgroundLayer = CreateLayer();
        EnableLayer(backgroundLayer, true);
        
        SetDrawTarget(backgroundLayer);
        DrawSprite(0,0, am::GetSprite("background"));
        SetDrawTarget(nullptr);

        ResetGame();
        waveEngine.PlayWaveform(am::GetSound("bg-music"), true);

        return true;
    }
    int backgroundLayer = -1;

    void ResetGame()
    {
        // Initialise Player Position
        player.x = ScreenWidth() / 2.0f;
        player.y = ScreenHeight() / 2.0f;
        player.dx = 0.0f;
        player.dy = 0.0f;
        player.angle = 0.0f;

        vecBullets.clear();
        vecAsteroids.clear();

        // Put in two asteroids
        vecAsteroids.push_back({ (int)16, player.x - 80.0f, player.y,  10.0f,  40.0f, 0.0f });
        vecAsteroids.push_back({ (int)16, player.x + 80.0f, player.y, -10.0f, -40.0f, 0.0f });

        // Reset game
        bDead = false;
        nScore = false;
    }

    // Implements "wrap around" for various in-game sytems
    void WrapCoordinates(float ix, float iy, float &ox, float &oy)
    {
        ox = ix;
        oy = iy;
        if (ix < 0.0f)	ox = ix + (float)ScreenWidth();
        if (ix >= (float)ScreenWidth())	ox = ix - (float)ScreenWidth();
        if (iy < 0.0f)	oy = iy + (float)ScreenHeight();
        if (iy >= (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
    }

    // Overriden to handle toroidal drawing routines
    bool Draw(int x, int y, olc::Pixel col = olc::WHITE) override
    {
        float fx, fy;
        WrapCoordinates(x, y, fx, fy);		
        return olc::PixelGameEngine::Draw(fx, fy, col);
    }

    bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
    {
        return sqrt((x-cx)*(x-cx) + (y-cy)*(y-cy)) < radius;
    }

    olc::sound::PlayingWave thruster;

    // Called by olcConsoleGameEngine
    bool OnUserUpdate(float fElapsedTime) override
    {
        if (bDead)
            ResetGame();
        
        // Clear Screen
        Clear(olc::BLANK);

        // Steer Ship
        if (GetKey(olc::LEFT).bHeld)
            player.angle -= 5.0f * fElapsedTime;
        if (GetKey(olc::RIGHT).bHeld)
            player.angle += 5.0f * fElapsedTime;

        // Thrust? Apply ACCELERATION
        if (GetKey(olc::UP).bHeld)
        {
            // ACCELERATION changes VELOCITY (with respect to time)
            player.dx += sin(player.angle) * 20.0f * fElapsedTime;
            player.dy += -cos(player.angle) * 20.0f * fElapsedTime;
        }

        if(GetKey(olc::UP).bPressed)
            thruster = waveEngine.PlayWaveform(am::GetSound("thruster"), true);
        
        if(GetKey(olc::UP).bReleased)
            waveEngine.StopWaveform(thruster);

        // VELOCITY changes POSITION (with respect to time)
        player.x += player.dx * fElapsedTime;
        player.y += player.dy * fElapsedTime;

        // Keep ship in gamespace
        WrapCoordinates(player.x, player.y, player.x, player.y);

        // Check ship collision with asteroids
        for (auto &a : vecAsteroids)
            if (IsPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y))
            {
                bDead = true; // Uh oh...
                waveEngine.PlayWaveform(am::GetSound("lose"));
            }
                

        // Fire Bullet in direction of player
        if (GetKey(olc::SPACE).bReleased)
        {
            vecBullets.push_back({ 0, player.x, player.y, 150.0f * sinf(player.angle), -150.0f * cosf(player.angle), 100.0f });
            waveEngine.PlayWaveform(am::GetSound("laser"));
        }
            

        // Update and draw asteroids
        for (auto &a : vecAsteroids)
        {
            // VELOCITY changes POSITION (with respect to time)
            a.x += a.dx * fElapsedTime;
            a.y += a.dy * fElapsedTime;
            a.angle += 0.5f * fElapsedTime; // Add swanky rotation :)

            // Asteroid coordinates are kept in game space (toroidal mapping)
            WrapCoordinates(a.x, a.y, a.x, a.y);

            // Draw Asteroids
            DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, (float)a.nSize, olc::YELLOW);	
        }

        // Any new asteroids created after collision detection are stored
        // in a temporary vector, so we don't interfere with the asteroids
        // vector iterator in the for(auto)
        std::vector<sSpaceObject> newAsteroids;

        // Update Bullets
        for (auto &b : vecBullets)
        {
            b.x += b.dx * fElapsedTime;
            b.y += b.dy * fElapsedTime;
            WrapCoordinates(b.x, b.y, b.x, b.y);
            b.angle -= 1.0f * fElapsedTime;

            // Check collision with asteroids
            for (auto &a : vecAsteroids)
            {
                //if (IsPointInsideRectangle(a.x, a.y, a.x + a.nSize, a.y + a.nSize, b.x, b.y))
                if(IsPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y))
                {
                    // Asteroid Hit - Remove bullet
                    // We've already updated the bullets, so force bullet to be offscreen
                    // so it is cleaned up by the removal algorithm. 
                    b.x = -100;

                    // Create child asteroids
                    if (a.nSize > 4)
                    {
                        float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                        float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                        newAsteroids.push_back({ (int)a.nSize >> 1 ,a.x, a.y, 30.0f * sinf(angle1), 30.0f * cosf(angle1), 0.0f });
                        newAsteroids.push_back({ (int)a.nSize >> 1 ,a.x, a.y, 30.0f * sinf(angle2), 30.0f * cosf(angle2), 0.0f });
                    }

                    // Remove asteroid - Same approach as bullets
                    a.x = -100;
                    nScore += 100; // Small score increase for hitting asteroid
                    waveEngine.PlayWaveform(am::GetSound("explosion"));
                }
            }
        }

        // Append new asteroids to existing vector
        for(auto a:newAsteroids)
            vecAsteroids.push_back(a);

        // Clear up dead objects - They are out of game space

        // Remove asteroids that have been blown up
        if (vecAsteroids.size() > 0)
        {
            auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](sSpaceObject o) { return (o.x < 0); });
            if (i != vecAsteroids.end())
                vecAsteroids.erase(i);
        }

        if (vecAsteroids.empty()) // If no asteroids, level complete! :) - you win MORE asteroids!
        {
            // Level Clear
            nScore += 1000; // Large score for level progression
            vecAsteroids.clear();
            vecBullets.clear();

            // Add two new asteroids, but in a place where the player is not, we'll simply
            // add them 90 degrees left and right to the player, their coordinates will
            // be wrapped by th enext asteroid update
        vecAsteroids.push_back({ (int)16, 80.0f * sinf(player.angle - 3.14159f/2.0f) + player.x,
                                            80.0f * cosf(player.angle - 3.14159f/2.0f) + player.y,
                                            60.0f * sinf(player.angle), 60.0f*cosf(player.angle), 0.0f });

        vecAsteroids.push_back({ (int)16, 80.0f * sinf(player.angle + 3.14159f/2.0f) + player.x,
                                            80.0f * cosf(player.angle + 3.14159f/2.0f) + player.y,
                                            60.0f * sinf(-player.angle), 60.0f*cosf(-player.angle), 0.0f });
        }

        // Remove bullets that have gone off screen
        if (vecBullets.size() > 0)
        {
            auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](sSpaceObject o) { return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1); });
            if (i != vecBullets.end())
                vecBullets.erase(i);
        }

        // Draw Bullets
        for (auto b : vecBullets)
            Draw(b.x, b.y);

        // Draw Ship
        DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

        // Draw Score
        DrawString(2, 2, "SCORE: " + std::to_string(nScore));
        
        return !GetKey(olc::ESCAPE).bPressed;
    }

    void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, olc::Pixel col = olc::WHITE)
    {
        // pair.first = x coordinate
        // pair.second = y coordinate
        
        // Create translated model vector of coordinate pairs
        std::vector<std::pair<float, float>> vecTransformedCoordinates;
        int verts = vecModelCoordinates.size();
        vecTransformedCoordinates.resize(verts);

        // Rotate
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
            vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
        }

        // Scale
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
        }

        // Translate
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
        }

        // Draw Closed Polygon
        for (int i = 0; i < verts + 1; i++)
        {
            int j = (i + 1);
            DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, 
                vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, col);
        }
    }
};


int main()
{
    OneLoneCoder_Asteroids game;
    if(game.Construct(320, 180, 4, 4))
        game.Start();

    return 0;
}

