#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

struct Pixel
{
	Vector2 position;
	Color color;
};

const int WIDTH = 400;
const int HEIGHT = 400;

std::vector<Pixel> pixels;
int CurrentPixel;
Pixel CurrentPixelE;
Color CurrentColour;

std::vector<Color> colors = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE, BLACK};

std::vector<Vector2> PixelsPos;
std::vector<Vector2> tiles;

Vector2 SelectPos;

bool tiles_spawned = false;

bool save = false;

constexpr bool operator==(Vector2 a, Vector2 b) {
  return a.x == b.x && a.y == b.y;
}

constexpr bool operator==(Pixel i, Pixel key)
{
	return (i.position.x == key.position.x) && (i.position.y == key.position.y) && 
		(i.color.r == key.color.r) && (i.color.g == key.color.g) && (i.color.b == key.color.b);
}

int PIndexOf(std::vector<Pixel> v, Pixel K)
{
    auto it = find(v.begin(), v.end(), K);
  
    // If element was found
    if (it != v.end()) 
    {
      
        // calculating the index
        // of K
        int index = it - v.begin();
        return index;
    }
    return -1;
}
int VIndexOf(std::vector<Vector2> v, Vector2 K)
{
    auto it = find(v.begin(), v.end(), K);
  
    // If element was found
    if (it != v.end()) 
    {
      
        // calculating the index
        // of K
        int index = it - v.begin();
        return index;
    }
    return -1;
}

struct compare
{
    Pixel key;
    compare(Pixel i): key(i) {}
 
    bool operator()(Pixel i) {
        return (i.position.x == key.position.x) && (i.position.y == key.position.y) && 
		(i.color.r == key.color.r) && (i.color.g == key.color.g) && (i.color.b == key.color.b);
    }
};

struct ppcompare
{
    Pixel key;
    ppcompare(Pixel i): key(i) {}
 
    bool operator()(Pixel i) {
        return (i.position.x != key.position.x) && (i.position.y != key.position.y);
    }
};

struct pcompare
{
    Vector2 key;
    pcompare(Vector2 &i): key(i) {}
 
    bool operator() (Vector2 &i) {
        return (i.x == key.x) && (i.y == key.y);
    }
};

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT+50, "Pixel Art! :)");
	CurrentColour = RED;

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	std::ofstream art("art.txt", std::ofstream::trunc);

	for (Pixel pixel : pixels)
	{
		art << pixel.position.x << ' ' << pixel.position.y-50 << ' ' << (int)pixel.color.r << ' ' << (int)pixel.color.g << ' ' << (int)pixel.color.b << '\n';
	}

	art.close();

	system("py main.py");

	return 0;
}

void UpdateDrawFrame()
{
	CurrentPixel = pixels.size()-1;
	//std::cout << pixels.size() << '\n';
	//ccstd::cout << GetMouseY() << '\n';
	SelectPos = Vector2Subtract(GetMousePosition(), 
		Vector2 { 50/2, 50/2 }
	);
	if (pixels.size() > 0)
		CurrentPixelE = pixels.back();

	if (IsKeyPressed(KEY_E))
		save = true;

	BeginDrawing();
	
	ClearBackground(BLACK);

	SelectPos = Vector2Clamp(
		SelectPos, Vector2{0, 50}, Vector2{350, HEIGHT}
	);

	for (int x = 0; x < 8; x++)
	{
		for (int y = 1; y < 8+5; y++)
		{
			Vector2 new_pos = Vector2{(float)x*50, (float)y*50};

			DrawRectangleLines(new_pos.x, new_pos.y, 50, 50, DARKGRAY);

			if (!tiles_spawned)
			{
				tiles_spawned = true;
				tiles.push_back(new_pos);
			}

			if (CheckCollisionRecs(Rectangle{new_pos.x, new_pos.y, 50, 50}, Rectangle{(float)SelectPos.x+50/2, (float)SelectPos.y+50/2, 16, 16}))
			{
				SelectPos = new_pos;
			}
		}
	}

	for (int x = 0; x < (int)colors.size(); x++)
	{
		for (int y = 0; y < 1; y++)
		{
			Vector2 new_pos = Vector2{(float)x * 30 + (WIDTH/2-colors.size()/2*20), (float)y+20};
			if (colors.at(x).r == 0 && colors.at(x).g == 0 && colors.at(x).b == 0)
				DrawRectangleLines(new_pos.x, new_pos.y, 20, 20, DARKGRAY);
			else
				DrawRectangleV(new_pos, Vector2{20, 20}, colors.at(x));

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
				CheckCollisionPointRec(GetMousePosition(), Rectangle{new_pos.x, new_pos.y, 20, 20}))
			{
				CurrentColour = colors.at(x);
			}
		}
	}

	// std::cout << "X : " << SelectPos.x << ", Y : " << SelectPos.y << '\n';

	for (Pixel pixel : pixels)
	{
		DrawRectangleV(pixel.position, Vector2{50, 50}, pixel.color);
		pixel.position = Vector2Clamp(pixel.position, Vector2{0, 0}, Vector2{350, 350});

		for (Vector2 tile : tiles)
		{
			if (CheckCollisionRecs(Rectangle{tile.x, tile.y, 50, 50},
				Rectangle{pixel.position.x, pixel.position.y, 50, 50}
			))
			{
				pixel.position = tile;
			}
		}
	}

	std::cout << pixels.size() << '\n';

	DrawRectangleLinesEx(Rectangle{SelectPos.x, SelectPos.y, 50, 50}, 1, DARKGRAY);
	DrawRectangleV(SelectPos, Vector2{50, 50}, CurrentColour);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && GetMouseY() > 40)
	{
		if (!(SelectPos.x > 400 || SelectPos.x < 0 ||
			SelectPos.y > 400 || SelectPos.y < 0
		))
		{
			if (!std::any_of(pixels.begin(), pixels.end(), compare(Pixel{SelectPos, CurrentColour})))
			{
				pixels.push_back(Pixel{SelectPos, CurrentColour});
				PixelsPos.push_back(SelectPos);
			}
			if (std::any_of(pixels.begin(), pixels.end(), ppcompare(Pixel{SelectPos, CurrentColour})))
			{
				pixels.erase(pixels.begin() + PIndexOf(pixels, Pixel{SelectPos, CurrentColour}));
				pixels.push_back(Pixel{SelectPos, CurrentColour});
				PixelsPos.push_back(SelectPos);
			}
		}
		else
			std::cerr << "You cant place here !\n";
	}
	DrawFPS(10, HEIGHT+50-25);

	EndDrawing();
}