#include <SFML/Graphics.hpp>
#include "Graph.h"
using namespace sf;

const int WINDOW_WIDTH = 600; //ширина главного окна
const int WINDOW_HEIGHT = 600; //высота главного окна
Font font; //шрифт текста

int main() {
	setlocale(LC_ALL, "ru");

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	font.loadFromFile("vcrosdmonorusbyd.ttf");

	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Graph", sf::Style::Close | sf::Style::Titlebar, settings);

	std::vector<std::vector<int>> matrix = {
		std::vector<int>{NOVALUE, 10, 15, 11, 2, 55},
		std::vector<int>{17, NOVALUE, 16, 18, 21, 13},
		std::vector<int>{10, 50, NOVALUE, 39, 22, 3},
		std::vector<int>{28, 29, 24, NOVALUE, 28, 25},
		std::vector<int>{27, 9, 32, 9, NOVALUE, 2},
		std::vector<int>{43, 48, 40, 43, 21, NOVALUE}
	};
	/*std::vector<std::vector<int>> matrix = {
		std::vector<int>{NOVALUE, 5, 11, 9},
		std::vector<int>{10, NOVALUE, 8, 7},
		std::vector<int>{7, 14, NOVALUE, 8},
		std::vector<int>{12, 6, 15, NOVALUE}
		
	};*/
	std::vector<std::wstring> names = { L"1", L"2", L"3", L"4", L"5", L"6" };
	Graph g(matrix, names, font, 12, Vector2f(260.f, 260.f), 200);
	

	std::vector<AdjacencyMatrix::Tuple<int, int>> wayParts = g.adjMatrix.getWayParts();
	for (int i = 0; i < wayParts.size(); ++i) {
		std::cout << wayParts[i].item1 + 1 << '\t' << wayParts[i].item2 + 1<< '\n';
	}
	std::cout << '\n';
	std::vector<int> shortCut = g.adjMatrix.getWay(2);
	for (int i = 0; i < shortCut.size(); ++i) {
		std::cout << shortCut[i] + 1 << '\t';
	}

	gVertex* moving = nullptr;

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		Vector2i cursorPos = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			gVertex::getMoving(moving, g.vertices, event, cursorPos);
		}
		window.clear(Color::White);
		g.snapEdgesToVertices();
		gVertex::move(moving, cursorPos, WINDOW_WIDTH, WINDOW_HEIGHT);
		for (unsigned int i = 0; i < g.edges.size(); ++i) {
			window.draw(g.edges[i]);
		}
		for (unsigned int i = 0; i < g.vertices.size(); ++i) {
			window.draw(g.vertices[i]);
		}
		
		window.display();
	}
	return 0;
}