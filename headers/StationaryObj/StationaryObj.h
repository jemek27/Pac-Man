//
// Created by jemek on 07.06.2024.
//

#ifndef PROJEKT_STATIONARYOBJ_H
#define PROJEKT_STATIONARYOBJ_H
#include <SFML/Graphics.hpp>

//todo Png
class StationaryObj {
protected:
    sf::RenderTexture Texture;
    sf::Texture texturePng;
    sf::Vector2<float> Position;
    int ScoreNumber;
    int Size;
    sf::Sprite Sprite;
public:
    StationaryObj(const int& size, const sf::Vector2<float>& position, const int& scoreNumber = 0);

    sf::Sprite getSprite();
    sf::Vector2<float> getPosition();
    int getSize();

    virtual std::pair<int, bool> interact() = 0;
    virtual ~StationaryObj() = default;
};


#endif //PROJEKT_STATIONARYOBJ_H
