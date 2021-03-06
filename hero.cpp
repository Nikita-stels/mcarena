#include "inc/hero.h"
#include "inc/effect.h"

Hero::Hero(HeroTemplate *_templ)
    : heroTemplate(_templ),
      effectList(),
      showedDead(false)
{
    if(heroTemplate != nullptr)
    {
        stats = new Stats(heroTemplate->getStats());
        stats->actions.setSender(this);
    }
    else
    {
        stats = nullptr;
    }
}

Hero::~Hero()
{
    if(stats != nullptr)
    {
        delete stats;
    }
}

Stats& Hero::getStats()
{
    return *stats;
}

Stats& Hero::getTemplateStats()
{
    return heroTemplate->getStats();
}

Resources& Hero::getResources()
{
    return heroTemplate->getResources();
}

HeroTemplate *Hero::getTemplate() const
{
    return heroTemplate;
}

Action* Hero::getAttack() const
{
    return stats->actions.getAttack();
}

Action* Hero::getSkill() const
{
    return stats->actions.getSkill();
}

void Hero::add(Effect *effect)
{
    effectList.push_back(effect);
    effect->onAdd(this);
}

bool Hero::onTurnEffects()
{
    bool res = false;
    for(auto it = effectList.begin(); it != effectList.end();)
    {
        res |= (*it)->onTurn(this);

        if( !(*it)->isRemoveOnTurn() )  // if remove after turn
        {
            ++it;
            continue;
        }

        if( (*it)->updateDuration() == 0)
        {
            auto itOld = it;
            ++it;
            remove(*itOld);
        }
        else
        {
            ++it;
        }
    }

    return res;
}

void Hero::afterTurnEffects()
{
    for(auto it = effectList.begin(); it != effectList.end();)
    {
        if( (*it)->isRemoveOnTurn() )  // if remove on turn
        {
            ++it;
            continue;
        }

        if( (*it)->updateDuration() == 0)
        {
            auto itOld = it;
            ++it;
            remove(*itOld);
        }
        else
        {
            ++it;
        }
    }
}

void Hero::removeAllEffects()
{
    while(effectList.size() != 0)
    {
        remove(effectList.front());
    }
}

void Hero::remove(Effect *effect)
{
    effectList.remove(effect);
    effect->onRemove(this);
    delete effect;
}

bool Hero::isAlive() const
{
    return stats->hp.curr > 0;
}

bool Hero::updateDead()
{
    if(showedDead == true)
    {
        return false;
    }

    return showedDead = !isAlive();
}
