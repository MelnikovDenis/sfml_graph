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
void showWayInfo(Graph& g, std::vector<std::wstring>& names) {
	std::vector<AdjacencyMatrix::Tuple<int, int>> wayParts = g.adjMatrix.getWayParts();
	std::cout << "Отрезки пути наименьшего гамильтонова цикла:\n";
	for (int i = 0; i < wayParts.size(); ++i) {
		std::wcout << names[wayParts[i].item1] << L"  ->  " << names[wayParts[i].item2] << '\n';
	}
	std::cout << "Длина наименьшего гамильтонова цикла: " << g.adjMatrix.getWayLength(wayParts);
	std::cout << '\n' << "Наименьший замкнутый путь с началом в точке ";
	std::wcout << names[0];
	std::cout << ": ";
	std::vector<int> shortCut = g.adjMatrix.getWay(0);
	for (int i = 0; i < shortCut.size(); ++i) {
		if (i != shortCut.size() - 1) std::wcout << names[shortCut[i]] << " -> ";
		else std::wcout << names[shortCut[i]] << '\n';
	}
}
void showMatrix(std::vector<std::vector<int>>& replica, std::vector<std::wstring>& names) {
	std::cout << "Исходная матрица смежности:";
	std::cout << '\n' << '\t';
	for (int i = 0; i < replica.size(); ++i) {
		std::wcout << names[i] << '\t';
	}
	std::cout << '\n';
	for (unsigned int i = 0; i < replica.size(); ++i) {
		std::wcout << names[i] << '\t';
		for (unsigned int j = 0; j < replica.size(); ++j) {
			if (replica[i][j] == NOVALUE) {
				std::cout << 'N' << '\t';
			}
			else  std::cout << replica[i][j] << '\t';
		}
		std::cout << '\n';
	}
}
std::vector<std::vector<int>> inputMatrix(std::vector<std::wstring>& names) {
	bool badInput = false;	
	int verticiesCount = getUserInputAsInt("Введите кол-во вершин: ", 3, 8);	
	std::vector<std::vector<int>> matrix(verticiesCount);
	for (int i = 0; i < verticiesCount; ++i) {
		matrix[i] = std::vector<int>(verticiesCount);
	}
	names.erase(names.begin(), names.end());
	names.resize(verticiesCount);
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
		names.push_back(std::to_wstring(i + 1));
	}
	return matrix;
}
void showBasicInfo() {
	std::cout << "В графическом окне зелёными стрелочками отмечается наименьший\n";
	std::cout << "гамильтонов цикл(замкнутый путь, проходящий через все веришны).\n";
	std::cout << "Между стрелочкой, указывающей путь и противоположенной этому направлению вершиной\n";
	std::cout << "указывается длина пути из этой вершины.\n\n";
}
void addVertex(std::vector<std::vector<int>>& matrix, std::vector<std::wstring>& names) {
	for (int i = 0; i < matrix.size(); ++i) {
		matrix[i].resize(matrix[i].size() + 1);
	}
	matrix.resize(matrix.size() + 1);
	matrix.back() = std::vector<int>(matrix.size());
	names.push_back(std::to_wstring(std::stoi(names.back()) + 1));
	int temp;
	for (int i = 0; i < matrix.size() - 1; ++i) {
		std::cout << "Введите длину дороги из ";
		std::wcout << names[i];
		std::cout << " в ";
		std::wcout << names.back();
		std::cout << ": ";
		temp = getUserInputAsInt("", 0);
		if (temp <= 0) matrix[i].back() = NOVALUE;
		else matrix[i].back() = temp;
		
	}
	for (int i = 0; i < matrix.size() - 1; ++i) {
		std::cout << "Введите длину дороги из ";
		std::wcout << names.back();
		std::cout << " в ";
		std::wcout << names[i];
		std::cout << ": ";
		temp = getUserInputAsInt("", 0);
		if (temp <= 0) matrix.back()[i] = NOVALUE;
		else matrix.back()[i] = temp;
	}
	matrix.back().back() = NOVALUE;
}
void removeVertex(std::vector<std::vector<int>>& matrix, std::vector<std::wstring>& names) {
	bool isFind = false;
	int rVer;
	do {	
		rVer = getUserInputAsInt("Введите вершину, которую нужно удалить: ", 1);
		for (int i = 0; i < names.size() && !isFind; ++i) {
			if (std::to_wstring(rVer) == names[i]) {
				isFind = true;
				rVer = i;
			}
		}
		if (!isFind) std::cout << "Такой вершины нет, повторите ввод!\n";
	} while (!isFind);
	
	std::vector<std::vector<int>> copy(matrix.size() - 1);
	for (unsigned int i = 0; i < copy.size(); ++i)
		copy[i] = std::vector<int>(copy.size());
	for (unsigned int i = 0; i < matrix.size(); ++i) {
		for (unsigned int j = 0; j < matrix.size(); ++j) {
			if (i < rVer && j < rVer) copy[i][j] = matrix[i][j];
			else if (i < rVer && j > rVer) copy[i][j - 1] = matrix[i][j];

			if (i > rVer && j < rVer) copy[i - 1][j] = matrix[i][j];
			else if (i > rVer && j > rVer) copy[i - 1][j - 1] = matrix[i][j];
		}
	}
	matrix = copy;
	int i = 0;
	for (auto iter = names.begin(); i <= rVer; ++i, ++iter) {
		if (i == rVer) {
			names.erase(iter);
			break;
		}
	}
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

	std::vector<std::wstring> names = { L"1", L"2", L"3", L"4", L"5", L"6"};
	Graph g(matrix, names, font, 12, Vector2f(260.f, 260.f), 200);	
	gVertex* moving = nullptr;
	Button enterButton(L"Enter matrix", font, 100.f, 37.f);
	enterButton.setPosition(65.f, 550.f);
	Button addButton(L"Add vertex", font, 100.f, 37.f);
	addButton.setPosition(200.f, 550.f);
	Button delButton(L"Delete vertex", font, 110.f, 37.f);
	delButton.setPosition(335.f, 550.f);
	Clock timer;


	
	g.showHamiltoneCycle();

	showBasicInfo();
	showMatrix(matrix, names);
	showWayInfo(g, names);
	
	
	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		Vector2i cursorPos = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			enterButton.buttonEv(cursorPos, timer, event, [&g, &names]()->void {
				system("cls");
				showBasicInfo();
				std::vector<std::vector<int>> matrix = inputMatrix(names);
				showMatrix(matrix, names);
				g.loadNewData(matrix, names, font, 12, Vector2f(260.f, 260.f), 200);
				g.showHamiltoneCycle();
				showWayInfo(g, names);
			});
			addButton.buttonEv(cursorPos, timer, event, [&g, &names]()->void {
				if (g.adjMatrix.matrix.size() < 9) {
					system("cls");
					showBasicInfo();
					showMatrix(g.adjMatrix.matrix, names);
					addVertex(g.adjMatrix.matrix, names);
					showMatrix(g.adjMatrix.matrix, names);
					g.loadNewData(g.adjMatrix.matrix, names, font, 12, Vector2f(260.f, 260.f), 200);
					g.showHamiltoneCycle();
					showWayInfo(g, names);
				}
				else {
					std::cout << "Максимальное кол-во вершин, добавление новых невозможно!\n";
				}
			});
			delButton.buttonEv(cursorPos, timer, event, [&g, &names]()->void {
				if (g.adjMatrix.matrix.size() > 3) {
					try {
						system("cls");
						showBasicInfo();
						showMatrix(g.adjMatrix.matrix, names);
						removeVertex(g.adjMatrix.matrix, names);
						showMatrix(g.adjMatrix.matrix, names);
						g.loadNewData(g.adjMatrix.matrix, names, font, 12, Vector2f(260.f, 260.f), 200);
						g.showHamiltoneCycle();
						showWayInfo(g, names);
					}
					catch (std::exception& ex) {
						std::cout << ex.what() << '\n';
					}
				}
				else std::cout << "Дальнейшее удаление невозможно!\n";
			});
			gVertex::getMoving(moving, g.vertices, event, cursorPos);
			
		}
		window.clear(Color::White);
		
		g.snapEdgesToVertices();
		gVertex::move(moving, cursorPos, WINDOW_WIDTH, WINDOW_HEIGHT - 100);
		for (unsigned int i = 0; i < g.edges.size(); ++i) {
			window.draw(g.edges[i]);
		}
		for (unsigned int i = 0; i < g.vertices.size(); ++i) {
			window.draw(g.vertices[i]);
		}
		window.draw(enterButton);
		window.draw(delButton);
		window.draw(addButton);
		window.display();
	}
	return 0;
}