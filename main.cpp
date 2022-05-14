#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Graph.h"
#include "Button.h"
#include "climits"
using namespace sf;

const int WINDOW_WIDTH = 600; //ширина главного окна
const int WINDOW_HEIGHT = 600; //высота главного окна
Font font; //шрифт текста


//функции для работы с консолью
int getUserInputAsInt(const std::string& message, int min = INT32_MIN, int max = INT32_MAX)
{
	using namespace std;
	int number = 0;
	while (true)
	{
		cout << message;

		string input;
		getline(cin, input);

		if (input.length() == 0)
		{
			continue;
		}

		const int digitsCount = count_if(input.begin(), input.end(),
			[](char ch) { return isdigit(ch); });

		if (static_cast<size_t>(digitsCount) == input.length())
		{
			const int temp = stoll(input);
			if (temp <= numeric_limits<int>::max())
			{
				number = static_cast<int>(temp);
				if (number >= min && number <= max)
				{
					break;
				}
			}
		}
	}
	return number;
}
void showWayInfo(Graph& g) {
	std::vector<AdjacencyMatrix::Tuple<int, int>> wayParts = g.adjMatrix.getWayParts();
	std::cout << "Отрезки пути наименьшего гамильтонова цикла:\n";
	for (int i = 0; i < wayParts.size(); ++i) {
		std::cout << wayParts[i].item1 + 1 << "  ->  " << wayParts[i].item2 + 1 << '\n';
	}
	std::cout << "Длина наименьшего гамильтонова цикла: " << g.adjMatrix.getWayLength(wayParts);
	std::cout << '\n' << "Наименьший замкнутый путь с началом в точке 1: ";
	std::vector<int> shortCut = g.adjMatrix.getWay(0);
	for (int i = 0; i < shortCut.size(); ++i) {
		if (i != shortCut.size() - 1) std::cout << shortCut[i] + 1 << " -> ";
		else std::cout << shortCut[i] + 1 << '\n';
	}
}
void showMatrix(std::vector<std::vector<int>>& replica) {
	std::cout << "Исходная матрица смежности:";
	std::cout << '\n' << '\t';
	for (int i = 0; i < replica.size(); ++i) {
		std::cout << i + 1 << '\t';
	}
	std::cout << '\n';
	for (unsigned int i = 0; i < replica.size(); ++i) {
		std::cout << i + 1 << '\t';
		for (unsigned int j = 0; j < replica.size(); ++j) {
			if (replica[i][j] == NOVALUE) {
				std::cout << 'N' << '\t';
			}
			else  std::cout << replica[i][j] << '\t';
		}
		std::cout << '\n';
	}
}
std::vector<std::vector<int>> inputMatrix() {
	bool badInput = false;	
	int verticiesCount = getUserInputAsInt("Введите кол-во вершин: ", 3, 8);	
	std::vector<std::vector<int>> matrix(verticiesCount);
	for (int i = 0; i < verticiesCount; ++i) {
		matrix[i] = std::vector<int>(verticiesCount);
	}

	int temp = 0;
	for (int i = 0; i < verticiesCount; ++i) {
		for (int j = 0; j < verticiesCount; ++j) {
			if (i != j) {
				temp = getUserInputAsInt("Введите длину дороги из " + std::to_string(i + 1) + " в " + std::to_string(j + 1) + ": ");
				if (temp <= 0) matrix[i][j] = NOVALUE;
				else matrix[i][j] = temp;
			}
			else matrix[i][j] = NOVALUE;
			
		}
	}
	return matrix;
}
void showBasicInfo() {
	std::cout << "В графическом окне зелёными стрелочками отмечается наименьший\n";
	std::cout << "гамильтонов цикл(замкнутый путь, проходящий через все веришны).\n";
	std::cout << "Между стрелочкой, указывающей путь и противоположенной этому направлению вершиной\n";
	std::cout << "указывается длина пути из этой вершины.\n\n";
}

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");
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
	std::vector<std::wstring> names = { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8"};
	Graph g(matrix, names, font, 12, Vector2f(260.f, 260.f), 200);	
	gVertex* moving = nullptr;
	Button b(L"Enter matrix", font, 100.f, 37.f);
	Clock timer;


	b.setPosition(65.f, 550.f);
	g.showHamiltoneCycle();

	showBasicInfo();
	showMatrix(matrix);
	showWayInfo(g);

	
	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		Vector2i cursorPos = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			b.buttonEv(cursorPos, timer, event, [&g, &names]()->void {
				system("cls");
				showBasicInfo();
				std::vector<std::vector<int>> matrix = inputMatrix();
				showMatrix(matrix);
				g.loadNewData(matrix, names, font, 12, Vector2f(260.f, 260.f), 200);
				g.showHamiltoneCycle();
				showWayInfo(g);
			});
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
		window.draw(b);
		window.display();
	}
	return 0;
}