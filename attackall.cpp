#include "inc/attackall.h"

AttackAll::AttackAll(Damage _damage)
    : Action(),
      damage(_damage)
{

}

Action *AttackAll::clone() const
{
    return new AttackAll(*this);
}

Targets AttackAll::getAvaliableTargetsPlayer1() const
{
    if(!player1->has(sender))
    {
        return getAliveTargets(player1);
    }

    return Targets(false);
}

Targets AttackAll::getAvaliableTargetsPlayer2() const
{
    if(!player2->has(sender))
    {
        return getAliveTargets(player2);
    }

    return Targets(false);
}

Targets AttackAll::getTargetsPlayer1() const
{
    if(player1->has(target))
        return targets;

    return Targets(false);
}

Targets AttackAll::getTargetsPlayer2() const
{
    if(player2->has(target))
        return targets;

    return Targets(false);
}

const std::string &AttackAll::getName() const
{
    return "Attack all";
}

const std::string &AttackAll::getDescription() const
{
    return "Attack all enemies for " + std::to_string(damage.min) + "-" +
            std::to_string(damage.max) + " damage";
}

void AttackAll::doAction()
{
    Player *player;
    if(player1->has(target))
    {
        player = player1;
    }
    else
    {
        player = player2;
    }

    targets = getAliveTargets(player);

    attackAllTargets(player, targets, damage);
}
