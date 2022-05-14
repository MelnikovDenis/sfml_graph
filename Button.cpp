#include "Button.h"
Button::Button(const std::wstring& text, const Font& font, const float& width, const float& height) {
	this->text.setString(text);
	this->text.setFont(font);
	this->text.setCharacterSize(10);
	this->text.setFillColor(Color::Black);
	rect.setSize(Vector2f(width, height));
	rect.setOutlineThickness(1.f);
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color(180, 180, 180));
}
void Button::buttonEv(const Vector2i& cursorPos, Clock& timer, const Event& event, std::function<void()> action) {
	if (rect.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) {
		float time = timer.getElapsedTime().asMilliseconds();
		bool isActive = true;
		if (time < 5.f) {
			isActive = false;
		}
		else {
			timer.restart();
			isActive = true;
		}
		rect.setOutlineThickness(2.f);
		if (event.type == Event::MouseButtonPressed) {
			if (event.key.code == Mouse::Left && isActive) {
				rect.setFillColor(Color(140, 140, 140));
			}
		}
		else if (event.type == Event::MouseButtonReleased && isActive) {
			if (event.key.code == Mouse::Left) {
				rect.setFillColor(Color(180, 180, 180));
				action();
			}
		}
	}
	else {
		rect.setOutlineThickness(1.f);
		rect.setFillColor(Color(180, 180, 180));
	}
}
void Button::setPosition(const float& x, const float& y) {
	rect.setPosition(x - rect.getLocalBounds().width / 2, y - rect.getLocalBounds().height / 2);

	text.setPosition(x - text.getLocalBounds().width / 2,
		y - text.getLocalBounds().height);
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rect, states);
	target.draw(text, states);
}