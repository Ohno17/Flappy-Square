#include "game.h"

#define SPEEDCAP 40
#define JUMPSPEED -15
#define OBSTACLESPEED 8
#define GRAVITY 2
#define PLAYERX 60

// Max y value player can be to survive obstacle.
uint16 CurrentTop;
// Minimum y value player can be to survive obstacle.
uint16 CurrentBottom;

uint64 Score;
Game Game;

// Class representing the player.
class Square
{
private:
  int16 y;
  int16 yspeed;

public:
  Square(void);

  void Jump(void);

  void Draw();
  void Update(void);

  void setY(int16 y) { this->y = y; }
  void setSpeed(int16 yspeed) { this->yspeed = yspeed; }
  int16 getY() { return this->y; }
};

Square::Square(void)
{
  this->y = WINDOWHEIGHT / 2;
  this->yspeed = 0;
}

// Changes the Y speed, and preforms some checks before doing so.
void Square::Jump(void)
{
  if (abs(this->yspeed) < SPEEDCAP)
	{
    this->yspeed = JUMPSPEED;
  }
}

// Draws the player.
void Square::Draw()
{
  Game.Screen_Putrect(PLAYERX, this->y - 10, 20, 20);
}

// Checks for player collisions, and adds y speed to the y position.
void Square::Update(void)
{
  this->setY(this->y + this->yspeed);
  this->setSpeed(this->yspeed + GRAVITY);

  // Cap speed
  if (this->yspeed > SPEEDCAP) 
	{
    this->yspeed = SPEEDCAP;
  }

  if (this->yspeed < (-1 * SPEEDCAP))
	{
    this->yspeed = (-1 * SPEEDCAP);
  }

  // Check if out of screen bounds.
  if (this->y < 1 || this->y > WINDOWHEIGHT)
	{
    CurrentGameState = GAMEOVER;
  }
}

// Check player's collision to the obstacles.
void CheckObstacleCollision(Square player)
{
  int16 playery = player.getY();
  if (playery > CurrentBottom || playery < CurrentTop)
	{
    CurrentGameState = GAMEOVER;
  }
}

Square Player;

// Obstacle which the player will be avoiding.
class Obstacle
{
	private:
  	uint16 x;

	public:
  	Obstacle();

  	void Draw();
  	void Update(void);

  	void RandomizeHeights(void);
};

Obstacle::Obstacle()
{
  this->x = WINDOWWIDTH;
  this->RandomizeHeights();
}

void Obstacle::RandomizeHeights(void)
{
  int32 random = std::rand() % 100;
  if (random > 50)
	{
		random -= 50;
    random = random * -1;
  }
  CurrentTop = 50 + random;
  CurrentBottom = WINDOWHEIGHT - (120 - random);
}

void Obstacle::Draw()
{
  Game.Screen_Putrect(x, 0, 20, CurrentTop);
  Game.Screen_Putrect(x, CurrentBottom, 20, (WINDOWHEIGHT - CurrentBottom));
}

void Obstacle::Update(void)
{
  if (this->x < PLAYERX + 10 && this->x > PLAYERX - 10)
	{
    CheckObstacleCollision(Player);
  }
  if (this->x < 1 + OBSTACLESPEED)
	{
    Score++;
    this->RandomizeHeights();
    this->x = WINDOWWIDTH;
  }
  this->x -= OBSTACLESPEED;
}

Obstacle Obstacle;

// Draw the obstacle, then the player.
void Draw()
{
  Game.Screen_Putrect(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
  Obstacle.Draw();
  Player.Draw();
}

// Update the obstacle, then the player.
void Update()
{
  Obstacle.Update();
  Player.Update();
}

// Main loop

int main(void)
{

  CurrentGameState = PLAYING;
  Score = 0;

  Game.Screen_Putrect(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
  Game.Screen_Putstring(10, 10, "Flappy Square");
  Game.Screen_Putstring(10, 30, "Press any key to play!");
	Game.Screen_Putstring(10, 40, "Press ESCAPE to quit.");
  Game.Screen_Show();

	uint32 key = Game.Game_Getinput();
  while (key == 0)
	{
		key = Game.Game_Getinput();
    SDL_Delay(20);
  }

	if (key == SDLK_ESCAPE)
	{
		CurrentGameState = GAMEOVER;
	}

  while (CurrentGameState == PLAYING)
	{
    Game.Screen_Clear();
    Game.Screen_Putstring(10, 10, "Score: " + std::to_string(Score));
    Update();
    Draw();
    Game.Screen_Show();

    uint32 key = Game.Game_Getinput();
    switch (key)
		{
    	case SDLK_SPACE:
      	Player.Jump();
      	break;
			case SDLK_ESCAPE:
				CurrentGameState = GAMEOVER;
				break;
    	case 0:
    	default:
      	break;
    }

    SDL_Delay(60);
  }

  Game.Screen_Clear();

  Game.Screen_Putrect(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
  Game.Screen_Putstring(10, 10, "Score: " + std::to_string(Score));
  Game.Screen_Putstring(10, 40, "Press any key to quit.");
  Game.Screen_Show();

  SDL_Delay(100);

  while (Game.Game_Getinput() == 0)
	{
    SDL_Delay(20);
  }

	// Nothing is allocated dynamically, so no calls to free() are needed.

  SDL_Quit();
	return 0;
}