#include "inc/attackfrontline.h"

AttackFrontLine::AttackFrontLine(Damage _damage)
    : Action(),
      damage(_damage)
{

}

Action *AttackFrontLine::clone() const
{
    return new AttackFrontLine(*this);
}

Targets AttackFrontLine::getAvaliableTargetsPlayer1() const
{
    if(!player1->has(sender))
    {
        return getAliveTargetsFrontLine(player1);
    }

    return Targets(false);
}

Targets AttackFrontLine::getAvaliableTargetsPlayer2() const
{
    if(!player2->has(sender))
    {
        return getAliveTargetsFrontLine(player2);
    }

    return Targets(false);
}

Targets AttackFrontLine::getTargetsPlayer1() const
{
    if(player1->has(target))
        return targets;

    return Targets(false);
}

Targets AttackFrontLine::getTargetsPlayer2() const
{
    if(player2->has(target))
        return targets;

    return Targets(false);
}

std::string AttackFrontLine::getName() const
{
    return "Attack front";
}

std::string AttackFrontLine::getDescription() const
{
    std::string dmgString;
    if(damage.min == damage.max)
        dmgString = std::to_string(damage.min);
    else
        dmgString = std::to_string(damage.min) + "-" + std::to_string(damage.max);

    return "deal " + dmgString + " damage to all enemies in front line";
}

void AttackFrontLine::doAction()
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

    targets = getAliveTargetsFrontLine(player);

    attackAllTargets(player, targets, damage);
}
