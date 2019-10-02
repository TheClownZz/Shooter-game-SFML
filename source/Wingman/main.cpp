#include"Game.h"
#include"GameMapMaker.h"
#include<Windows.h>

int main()
{
	srand(time(NULL));
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd,SW_HIDE);
	RenderWindow window(VideoMode(1120, 630), "Wingman Game", Style::Default);

	Clock clock;
	float dt = 0.f;

	Game game(&window);

	//Game loop
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				exit(0);
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				window.close();
				exit(0);
			}
		}

		dt = clock.restart().asSeconds();

			game.update(dt);
			game.draw();
	}

	return 0;
}