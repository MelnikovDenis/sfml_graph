#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
using namespace sf;
class Button : public Drawable{
private:
	RectangleShape rect;
	Text text;
	
public:	
	Button(const std::wstring& text, const Font& font, const float& width, const float& height);
	void buttonEv(const Vector2i& cursorPos, Clock& timer ,const Event& event, std::function<void()> action);
	//ставится на эти координаты центром
	void setPosition(const float& x, const float& y);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

