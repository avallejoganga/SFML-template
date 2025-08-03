#pragma once
#include <map>
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include "Animation.h"
#include <SFML/Graphics.hpp>

class Assets
{
	int x;
	
	std::map<std::string, Animation> m_animationMap;
	std::map<std::string, sf::Texture> m_textureMap;
	std::map<std::string, sf::Font> m_fontMap;

	void addTexture(const std::string& name, const std::string& path)
	{
		m_textureMap[name] = sf::Texture();

		if (!m_textureMap[name].loadFromFile(path))
		{
			std::cerr << "Could not load texture file: " << path << std::endl;
			m_textureMap.erase(name);
		}
		else
		{
			auto sz = m_textureMap[name].getSize();
			m_textureMap[name].setSmooth(true);
			std::cout << "Loaded texture '" << name << "' size: " << sz.x << "x" << sz.y << std::endl;
		}
	}

	void addAnimation(const std::string& name, const std::string& texture, size_t frames, size_t speed)
	{
		m_animationMap[name] = Animation(m_textureMap[texture], name, frames, speed);
	}


	void addFont(const std::string& name, const std::string& path)
	{
		m_fontMap[name] = sf::Font();

		if (!m_fontMap[name].loadFromFile(path))
		{
			std::cerr << "Could not load from file: " << path << std::endl;
			m_fontMap.erase(name);
		}
		else
		{
			std::cout << "Loaded Font: " << path << std::endl;
		}
	}

public:

	void loadFromFile(const std::string& path)
	{
		std::ifstream file(path);
		std::string str;

		while (file.good())
		{
			file >> str;			

			if (str == "Texture")
			{
				std::string name, path;
				file >> name >> path;
				addTexture(name, path);
			}
			else if (str == "Animation")
			{
				std::string name, texture;
				size_t frames, speed;
				file >> name >> texture >> frames >> speed;
				addAnimation(name, texture, frames, speed);
			}
			else if (str == "Font")
			{
				std::string name, path;
				file >> name >> path;
				addFont(name, path);
			}
			else
			{
				std::cerr << "Unknown Asset Type: " << str << std::endl;
			}
		}
	}


	const sf::Texture& getTexture(const std::string& textureName)
	{
		assert(m_textureMap.find(textureName) != m_textureMap.end());
		return m_textureMap.at(textureName);
	}

	const Animation& getAnimation(const std::string& animationName)
	{
		assert(m_animationMap.find(animationName) != m_animationMap.end());
		return m_animationMap.at(animationName);
	}

	const sf::Font& getFont(const std::string& fontName) const
	{
		assert(m_fontMap.find(fontName) != m_fontMap.end());
		return m_fontMap.at(fontName);
	}

	const std::map<std::string, sf::Texture>& getTextures() const
	{
		return m_textureMap;
	}

	const std::map<std::string, Animation>& getAnimations() const
	{
		return m_animationMap;
	}
};

