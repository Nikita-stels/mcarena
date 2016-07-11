#pragma once

#include <list>

#include "hero.h"
#include "herogroup.h"

class HeroQueue
{
private:
    std::list<Hero*> heroList;

public:
    HeroQueue(const HeroGroup &group1, const HeroGroup &group2);

    void setHeroes(const HeroGroup &group1, const HeroGroup &group2);
    std::list<Hero*>::iterator begin();
    std::list<Hero*>::iterator end();
    Hero* first() const;
    void rotate();  // pop_front and put on place

};
