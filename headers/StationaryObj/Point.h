//
// Created by jemek on 07.06.2024.
//

#ifndef PROJEKT_POINT_H
#define PROJEKT_POINT_H
#include "StationaryObj.h"

class Point : public StationaryObj {
public:
    Point(const int& size, const sf::Vector2<float>& position);
    std::pair<int, bool> interact() override;
};


#endif //PROJEKT_POINT_H
