#pragma once

#include "herotemplate.h"

class Hero
{
private:
    HeroTemplate *heroTemplate;
    Stats *stats;

public:
    Hero(HeroTemplate *_templ = nullptr);
    ~Hero();

    Stats& getStats();  // return stats
    Stats& getTemplateStats();  // return heroTemplate->getStats
    Resources& getResources();
    HeroTemplate* getTemplate() const;

    Action* getAttack() const;
    Action* getSkill() const;

    bool isAlive() const;
};

#include "stats.h"
