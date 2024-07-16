#include <Arduboy2.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Arduboy2 arduboy;

#define screenWidth 128      
#define screenHeight 64      

#define mapWidth 16                       
#define mapHeight 16                      
bool worldMap[mapHeight][mapWidth] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

float playerX = 5.0f;               
float playerY = 14.0f;              
float playerDir = M_PI;             
float playerSpeed = 3.0f;		       
float playerFOV = M_PI / 2.0f;

unsigned long timeBefore, timeAfter;
float frameTime = 0.0f;

void checkKeyState()
{
    if (arduboy.pressed(UP_BUTTON))
    {
        playerX += cosf(playerDir) * playerSpeed * frameTime;
        playerY += sinf(playerDir) * playerSpeed * frameTime;

        if (worldMap[(int)playerY][(int)playerX])
        {
            playerX -= cosf(playerDir) * playerSpeed * frameTime;
            playerY -= sinf(playerDir) * playerSpeed * frameTime;
        }
    }
    else if (arduboy.pressed(DOWN_BUTTON))
    {
        playerX -= cosf(playerDir) * playerSpeed * frameTime;
        playerY -= sinf(playerDir) * playerSpeed * frameTime;

        if (worldMap[(int)playerY][(int)playerX])
        {
            playerX += cosf(playerDir) * playerSpeed * frameTime;
            playerY += sinf(playerDir) * playerSpeed * frameTime;
        }
    }
    else if (arduboy.pressed(LEFT_BUTTON))
    {
        playerDir -= (playerSpeed * 0.75f) * frameTime;
    }
    else if (arduboy.pressed(RIGHT_BUTTON))
    {
        playerDir += (playerSpeed * 0.75f) * frameTime;
    }
}

void showLog()
{
  arduboy.setCursor(0,0);
  arduboy.print((int)(1.0f / frameTime));
  arduboy.print("FPS");
}
void renderFrame()
{
    for (int x = 0; x < screenWidth; x++)
    {
        float rayDir = (playerDir - playerFOV / 2.0f) + ((float)x / (float)screenWidth) * playerFOV;
        float rayX = cosf(rayDir);
        float rayY = sinf(rayDir);

        float deltaDistX = fabs(1 / cosf(rayDir));
        float deltaDistY = fabs(1 / sinf(rayDir));

        float sideDistX;
        float sideDistY;
        int side;

        int mapX = (int)playerX;
        int mapY = (int)playerY;

        int stepX;
        int stepY;

        bool hitWall = 0;
        bool hitEdge = 0;

        // Digital Differential Analysis (DDA)
        if (rayX < 0)
        {
            stepX = -1;
            sideDistX = (playerX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
        }

        if (rayY < 0)
        {
            stepY = -1;
            sideDistY = (playerY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
        }

        while (!hitWall)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (worldMap[mapY][mapX])
            {
                hitWall = 1;

                float edgeDistance[4] = {
                    hypotf(mapX - playerX, mapY - playerY),
                    hypotf(mapX + 1 - playerX, mapY - playerY),
                    hypotf(mapX - playerX, mapY + 1 - playerY),
                    hypotf(mapX + 1 - playerX, mapY + 1 - playerY)
                    };

                float dotProduct[4] = {
                    (rayX * (mapX - playerX) + rayY * (mapY - playerY)) / edgeDistance[0],
                    (rayX * (mapX + 1 - playerX) + rayY * (mapY - playerY)) / edgeDistance[1],
                    (rayX * (mapX - playerX) + rayY * (mapY + 1 - playerY)) / edgeDistance[2],
                    (rayX * (mapX + 1 - playerX) + rayY * (mapY + 1 - playerY)) / edgeDistance[3]
                    };

                for (int i = 0; i < 4; i++)
                {
                    for (int j = i + 1; j < 4; j++)
                    {
                        if (edgeDistance[j] < edgeDistance[i])
                        {
                            float temp = edgeDistance[j];
                            edgeDistance[j] = edgeDistance[i];
                            edgeDistance[i] = temp;

                            temp = dotProduct[j];
                            dotProduct[j] = dotProduct[i];
                            dotProduct[i] = temp;
                        }
                    }
                }

                float proximity = 0.004f;

                for (int i = 0; i < 2; i++)
                {
                    if (acosf(dotProduct[i]) < proximity)
                    {
                        hitEdge = 1;
                        break;
                    }
                }
            }
        }

        float perpWallDist;
        if (side == 0) 
          perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayX;
        else
          perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayY;
        
        int lineHeight = (int)(screenHeight / perpWallDist);
        
        int ceilingHeight = -lineHeight / 2 + screenHeight / 2;
        if (ceilingHeight < 0) 
          ceilingHeight = 0;

        int floorHeight = lineHeight / 2 + screenHeight / 2;
        if (floorHeight >= screenHeight) 
          floorHeight = screenHeight - 1;

        for (int y = ceilingHeight; y < floorHeight; y++)
        {
          if (!hitEdge)
            arduboy.drawPixel(x, y, WHITE);
        }
    }
}

void setup() 
{
    timeBefore = millis(); 
    timeAfter = millis();

    arduboy.begin(); 
}

void loop() 
{
    arduboy.clear();  

    timeAfter = millis(); 
    frameTime = (float)(timeAfter - timeBefore) / 1000.0f; 
    timeBefore = timeAfter;  

    checkKeyState();  

    renderFrame();  
    showLog();
    
    arduboy.display();
}
