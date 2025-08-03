#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Animation
{
	sf::Sprite		sprite;
	size_t			frameCount;
	int				gameFrame;
	size_t			duration;
	Vec2f			size;
	std::string		name;


public:

	Animation() = default;

	Animation(const sf::Texture& tex, const std::string& aname, size_t aframes, size_t aduration)
		:name(aname), frameCount(aframes), duration(aduration)
	{
		gameFrame = 0;
		sprite.setTexture(tex);
		size = Vec2f(sprite.getTexture()->getSize().x / frameCount, sprite.getTexture()->getSize().y);
		sprite.setOrigin(size.x / 2, size.y / 2);

		update();		
	}
	
	void update()
	{
		gameFrame++;
		gameFrame = gameFrame >= duration * frameCount ? 0 : gameFrame;
		int animFrame = (gameFrame / duration) % frameCount;
		

		sf::IntRect rectangle = sf::IntRect(animFrame * size.x, 0, size.x, size.y);
		sprite.setTextureRect(rectangle);
	}
	
		

	const std::string& getName() const
	{
		return name;
	}

	std::string& getName() 
	{
		return name;
	}

	const Vec2f& getSize() const
	{
		return size;
	}

	Vec2f& getSize()
	{
		return size;
	}
	
	const sf::Sprite& getSprite() const
	{
		return sprite;
	}
	
	sf::Sprite& getSprite()
	{
		return sprite;
	}
};

