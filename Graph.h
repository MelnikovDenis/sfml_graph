#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
using namespace sf;
const int NOVALUE = 2147483647; //отсутствие значения

//матрица смежности графа
class AdjacencyMatrix {
private:	
	static bool isNoEmpty(std::vector<std::vector<int>> matrix);
public:
	template<class T1, class T2> struct Tuple {
		T1 item1;
		T2 item2;
	};
	std::vector<Tuple<int, int>> getWayParts() const;
	std::vector<std::vector<int>> matrix; //матрица смежности

	AdjacencyMatrix();
	AdjacencyMatrix& operator=(const AdjacencyMatrix& adjMatrix);
	int getWayLength(std::vector<Tuple<int, int>>& wayParts)const;
	AdjacencyMatrix(std::vector<std::vector<int>>& matrix);
	AdjacencyMatrix(const AdjacencyMatrix& adjMatrix);
	unsigned int getVerticesCount() const;
	std::vector<int> getWay(int startPoint) const;
};

//вершина графа
class gVertex : public Drawable {
private:
	Text text;
	CircleShape circle;

	static float VERTEX_RADIUS; //радиус окружности вершин
	static int VERTEX_POINTS; //количество точек при отрисовке вершин
public:
	//коснструкторы, деструктор и оператор присваивания
	gVertex();
	gVertex(const sf::String& text, const Font& font, const int& textSize);
	gVertex& operator=(const gVertex& v);
	gVertex(const gVertex& v);
	~gVertex();

	void setPosition(float x, float y);
	static void getMoving(gVertex*& moving, std::vector<gVertex>& vertices, const Event& event, const Vector2i& cursorPos);
	static void move(gVertex*& moving, const Vector2i& cursorPos, const int& window_width, const int& window_height);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	FloatRect getGlobalBounds();
	FloatRect getLocalBounds();
	Vector2f getCenter();
	Vector2f getPosition();
	static void getPolygonSpread(std::vector<gVertex>& vertices, const Vector2f& SPREAD_CENTER, const int& SPREAD_RADIUS);
};

//ребро графа
class gEdge : public Drawable {
private:
	//главная линия
	Vertex mainLine[2];

	//стрелочки посередине главной линии
	Vertex fromStartArrow[3];
	Vertex fromEndArrow[3];

	//текст над линией (для весов ребёр)
	Text fromStartText;
	Text fromEndText;

	

	//значение высоты стрелочек посередине главной линии
	static float ARROW_HEIGHT;

	//получение длины вектора
	static float getLength(const Vector2f& vec);
	//получение единичного вектора
	static Vector2f getUnitVector(const Vector2f& vec);
	//получение координат вектора на основе точек начала и конца
	static Vector2f getVector(const Vector2f& startPoint, const Vector2f& endPoint);
	//получение координат конца высоты треугольника образующего первую стрелочку
	Vector2f getArrowHeightPoint1();
	//получение координат конца высоты треугольника образующего вторую стрелочку
	Vector2f getArrowHeightPoint2();
	//центр первой половины от старта (там будет размещаться текст)
	Vector2f getStartHalfCenter();
	//центр второй половины от старта (там будет размещаться текст)
	Vector2f getEndHalfCenter();
	static Vector2f getCenter(const Vector2f& startPoint, const Vector2f& endPoint);

public:
	//значение веса ребра
	int fromStartLength = NOVALUE;
	int fromEndLength = NOVALUE;
	void setFromStartArrowColor(const Color& c);
	void setFromEndArrowColor(const Color& c);

	gEdge(const int& fromStartLength, const int& fromEndLength, const Font& font, const int& textSize);	
	Vector2f getStart();
	Vector2f getEnd();
	void setPosition(const Vector2f& startPoint, const Vector2f& endPoint);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

class Graph{
public:
	AdjacencyMatrix adjMatrix;
	std::vector<gEdge> edges;
	std::vector<gVertex> vertices;
	Graph(std::vector<std::vector<int>>& matrix, const std::vector<std::wstring>& names, 
		const Font& font, const int& textSize, const Vector2f& SPREAD_CENTER, const int& SPREAD_RADIUS);
	void loadNewData(std::vector<std::vector<int>>& matrix, const std::vector<std::wstring>& names,
		const Font& font, const int& textSize, const Vector2f& SPREAD_CENTER, const int& SPREAD_RADIUS);
	void snapEdgesToVertices();
	void showHamiltoneCycle();
};


