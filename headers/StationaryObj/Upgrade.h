//
// Created by jemek on 07.06.2024.
//

#ifndef PROJEKT_UPGRADE_H
#define PROJEKT_UPGRADE_H
#include "StationaryObj.h"

class Upgrade : public StationaryObj {
public:
    Upgrade(const int& size, const sf::Vector2<float>& position);
    std::pair<int, bool> interact() override;
};




#endif //PROJEKT_UPGRADE_H
