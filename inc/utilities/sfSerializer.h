#pragma once

#include "SFML/Graphics.hpp"
#include <QString>


extern QString sfVector2fToString(const sf::Vector2f &v);
extern sf::Vector2f stringToSfVector2f(const QString &str);

