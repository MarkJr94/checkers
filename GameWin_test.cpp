/*
 * GameWin_test.cpp
 *
 *  Created on: Sep 9, 2012
 *      Author: markjr
 */

#include "GameWin.hpp"

void testDrag() {
	// Initialization
	sf::RenderWindow window(sf::VideoMode(800, 600), "DragBox");
	const sf::Input &input = window.GetInput();

	sf::Vector2i mouseDownPosition;

	// Program loop
	while (window.IsOpened()) {
		// Event handling
		sf::Event event;
		while (window.GetEvent(event)) {
			if (event.Type == sf::Event::Closed)
				window.Close();
			else if (event.Type == sf::Event::MouseButtonPressed
					&& event.MouseButton.Button == sf::Mouse::Left) {
				mouseDownPosition.x = event.MouseButton.X;
				mouseDownPosition.y = event.MouseButton.Y;
			} else if ((event.Type == sf::Event::KeyPressed)
					&& (event.Key.Code == sf::Key::Escape)) {
				window.Close();
			}
		}

		// Drawing
		window.Clear();

		if (input.IsMouseButtonDown(sf::Mouse::Left)) {
			sf::Shape box = sf::Shape::Rectangle(mouseDownPosition.x,
					mouseDownPosition.y, input.GetMouseX(), input.GetMouseY(),
					sf::Color(0, 0, 0, 0), -1.f, sf::Color::Yellow);
			window.Draw(box);
		}

		window.Display();
	}
}

int main() {
//	testDrag();

	GameWin G(800,800);

	G.bindGame(new Game(false,false));
//	G.createGame(false, false);

	G.loop();
//	while (G.IsOpened()) {
//		G.sfHandleEvents();
//		G.drawGame();
//	}
}

