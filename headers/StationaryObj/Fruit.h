//
// Created by jemek on 07.06.2024.
//

#ifndef PROJEKT_FRUIT_H
#define PROJEKT_FRUIT_H
#include "StationaryObj.h"

class Fruit : public StationaryObj {
public:
    Fruit(const int& size, const sf::Vector2<float>& position);
    std::pair<int, bool> interact() override;
};


#endif //PROJEKT_FRUIT_H
