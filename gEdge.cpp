#include "Graph.h"
float gEdge::ARROW_HEIGHT = 15;

//получение длины вектора
float gEdge::getLength(const Vector2f& vec) {
	return sqrtf(pow(vec.x, 2) + pow(vec.y, 2));
}
//получение единичного вектора
Vector2f gEdge::getUnitVector(const Vector2f& vec) {
	return Vector2f(vec.x / getLength(vec), vec.y / getLength(vec));
}
//получение координат вектора на основе точек начала и конца
Vector2f  gEdge::getVector(const Vector2f& startPoint, const Vector2f& endPoint) {
	return Vector2f(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
}
//получение координат конца высоты треугольника образующего первую стрелочку
Vector2f gEdge::getArrowHeightPoint1() {
	Vector2f arrowHeightVector = getUnitVector(getVector(mainLine[0].position, mainLine[1].position));
	arrowHeightVector = Vector2f(arrowHeightVector.x * ARROW_HEIGHT, arrowHeightVector.y * ARROW_HEIGHT);
	return Vector2f(getCenter(mainLine[0].position, mainLine[1].position).x - arrowHeightVector.x, getCenter(mainLine[0].position, mainLine[1].position).y - arrowHeightVector.y);
}
//получение координат конца высоты треугольника образующего вторую стрелочку
Vector2f gEdge::getArrowHeightPoint2() {
	Vector2f arrowHeightVector = getUnitVector(getVector(mainLine[1].position, mainLine[0].position));
	arrowHeightVector = Vector2f(arrowHeightVector.x * ARROW_HEIGHT, arrowHeightVector.y * ARROW_HEIGHT);
	return Vector2f(getCenter(mainLine[0].position, mainLine[1].position).x - arrowHeightVector.x, getCenter(mainLine[0].position, mainLine[1].position).y - arrowHeightVector.y);
}
//центр первой половины от старта (там будет размещаться текст)
Vector2f gEdge::getStartHalfCenter() {
	return getCenter(getCenter(mainLine[0].position, mainLine[1].position), mainLine[0].position);
}
//центр второй половины от старта (там будет размещаться текст)
Vector2f gEdge::getEndHalfCenter() {
	return getCenter(getCenter(mainLine[0].position, mainLine[1].position), mainLine[1].position);
}

gEdge::gEdge(const int& fromStartLength, const int& fromEndLength, const Font& font, const int& textSize) {
	this->fromStartLength = fromStartLength;
	this->fromEndLength = fromEndLength;

	fromStartText.setFont(font);
	fromEndText.setFont(font);
	fromStartText.setFillColor(Color::Black);
	fromEndText.setFillColor(Color::Black);
	fromStartText.setCharacterSize(textSize);
	fromEndText.setCharacterSize(textSize);
	fromStartText.setString(std::to_string(fromStartLength));
	fromEndText.setString(std::to_string(fromEndLength));

	mainLine[0].color = Color::Black;
	mainLine[1].color = Color::Black;

	fromStartArrow[0].color = Color::Black;
	fromStartArrow[1].color = Color::Black;
	fromStartArrow[2].color = Color::Black;

	fromEndArrow[0].color = Color::Black;
	fromEndArrow[1].color = Color::Black;
	fromEndArrow[2].color = Color::Black;
}
Vector2f gEdge::getCenter(const Vector2f& startPoint, const Vector2f& endPoint) {
	return Vector2f((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
}
void gEdge::setPosition(const Vector2f& startPoint, const Vector2f& endPoint) {
	const float ARROW_HALF_SIDE = ARROW_HEIGHT / sqrtf(3); //половина длины стороны стрелки

	mainLine[0].position = startPoint;
	mainLine[1].position = endPoint;

	Vector2f center = getCenter(mainLine[0].position, mainLine[1].position);
	Vector2f mainVector = getVector(mainLine[0].position, mainLine[1].position);

	float tan_a = mainVector.x / mainVector.y; //тангенс угла наклона основания стрелки
	float atan_a = atanf(tan_a); //арктангенс угла наклона основания стрелки	
	float yOffset = abs(sinf(atan_a)) * ARROW_HALF_SIDE; //смещение для вершины по оси y
	float xOffset = cosf(atan_a) * ARROW_HALF_SIDE; //смещение для вершины по оси x
	if (tan_a > 0) xOffset *= -1;
	if (fromStartLength != NOVALUE) {
		Vector2f arrowHeightPoint1 = getArrowHeightPoint1();
		//получение координат первой вершины первой стрелки
		fromStartArrow[0].position = Vector2f(arrowHeightPoint1.x - xOffset, arrowHeightPoint1.y - yOffset);
		//получение координат второй вершины первой стрелки
		fromStartArrow[1].position = center;
		//получение координат третьей вершины первой стрелки
		fromStartArrow[2].position = Vector2f(arrowHeightPoint1.x + xOffset, arrowHeightPoint1.y + yOffset);
		fromStartText.setPosition(getStartHalfCenter());
	}
	if (fromEndLength != NOVALUE) {
		Vector2f arrowHeightPoint2 = getArrowHeightPoint2();
		//получение координат первой вершины второй стрелки
		fromEndArrow[0].position = Vector2f(arrowHeightPoint2.x - xOffset, arrowHeightPoint2.y - yOffset);
		//получение координат второй вершины второй стрелки
		fromEndArrow[1].position = center;
		//получение координат третьей вершины второй стрелки
		fromEndArrow[2].position = Vector2f(arrowHeightPoint2.x + xOffset, arrowHeightPoint2.y + yOffset);
		fromEndText.setPosition(getEndHalfCenter());
	}
}
void gEdge::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mainLine, 2, Lines, states);
	if (fromStartLength != NOVALUE) {
		target.draw(fromStartArrow, 3, LinesStrip, states);
		target.draw(fromStartText, states);
	}
	if (fromEndLength != NOVALUE) {
		target.draw(fromEndArrow, 3, LinesStrip, states);
		target.draw(fromEndText, states);
	}
}