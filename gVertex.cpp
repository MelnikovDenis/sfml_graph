#include "Graph.h"
float gVertex::VERTEX_RADIUS = 30.f; //радиус окружности вершин
int gVertex::VERTEX_POINTS = 90; //количество точек при отрисовке вершин

gVertex::gVertex() {

}
gVertex& gVertex::operator=(const gVertex& v) {
	this->text = v.text;
	this->circle = v.circle;
	return *this;
}
gVertex::gVertex(const gVertex& v) {
	this->text = v.text;
	this->circle = v.circle;
}
gVertex::gVertex(const sf::String& text, const Font& font, const int& textSize = 15) {
	this->text = Text(L"" + text, font, textSize);
	this->text.setFillColor(Color::Black);
	circle = CircleShape(VERTEX_RADIUS, VERTEX_POINTS);
	circle.setOutlineThickness(2.f);
	circle.setOutlineColor(Color::Black);
}
gVertex::~gVertex() {

}

void gVertex::setPosition(float x, float y) {
	circle.setPosition(x, y);
	text.setPosition(x + (circle.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2),
		y + (circle.getLocalBounds().height / 2) - (text.getLocalBounds().height));
}
void gVertex::getMoving(gVertex*& moving, std::vector<gVertex>& vertices, const Event& event, const Vector2i& cursorPos) {
	if (event.type == Event::MouseButtonPressed) {
		if (event.key.code == Mouse::Left) {
			for (unsigned int i = 0; i < vertices.size(); ++i) {
				if (vertices[i].getGlobalBounds().contains(cursorPos.x, cursorPos.y)) {
					moving = &vertices[i];
				}
			}
		}
	}
	else if (event.type == Event::MouseButtonReleased) {
		if (event.key.code == Mouse::Left) {
			moving = nullptr;
		}
	}
}
void gVertex::move(gVertex*& moving, const Vector2i& cursorPos, const int& window_width, const int& window_height) {
	if (moving != nullptr) {
		float xM = cursorPos.x - moving->getLocalBounds().width / 2;
		float yM = cursorPos.y - moving->getLocalBounds().height / 2;
		if (xM < 0) xM = 2;
		if (yM < 0) yM = 2;
		if (xM + moving->getLocalBounds().width > window_width) xM = window_width - moving->getLocalBounds().width;
		if (yM + moving->getLocalBounds().height > window_height) yM = window_height - moving->getLocalBounds().height;
		moving->setPosition(xM, yM);
	}
}
void  gVertex::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(circle, states);
	target.draw(text, states);
}
FloatRect gVertex::getGlobalBounds() {
	return circle.getGlobalBounds();
}
FloatRect gVertex::getLocalBounds() {
	return circle.getLocalBounds();
}
Vector2f gVertex::getCenter() {
	return Vector2f(circle.getPosition().x + (circle.getLocalBounds().width / 2),
		circle.getPosition().y + (circle.getLocalBounds().height / 2));
}
Vector2f gVertex::getPosition() {
	return Vector2f(circle.getPosition().x, circle.getPosition().y);
}
void gVertex::getPolygonSpread(std::vector<gVertex>& vertices, const Vector2f& SPREAD_CENTER, const int& SPREAD_RADIUS) {
	const float Pi = acosf(-1); //число пи
	for (int i = 0; i < vertices.size(); ++i) {
		vertices[i].setPosition(SPREAD_CENTER.x + SPREAD_RADIUS * cosf(2 * Pi * i / vertices.size()),
			SPREAD_CENTER.y + SPREAD_RADIUS * sinf(2 * Pi * i / vertices.size()));
	}
}