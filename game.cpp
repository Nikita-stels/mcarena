#include "inc/game.h"
#include <tinyxml.h>
#include <iostream>
#include "inc/actions_list.h"

using namespace std;

Game::Game(int argc, char *argv[]) : app(argc, argv), heroes()
{
    mainWindow = new MainWindow();
    menuGUI = new MenuGUI(*mainWindow, this);
    menuModel = new MenuModel(this);
    selectionGUI = new SelectionGUI(*mainWindow, this);
    selectionModel = new SelectionModel(this);
    battleGUI = new BattleGUI(*mainWindow, this);
    battleModel = new BattleModel(this);

    // Signals Menu GUI -> Model
    connect(menuGUI, SIGNAL(clickedPlay()), menuModel, SLOT(clickedPlay()));
    connect(menuGUI, SIGNAL(clickedPlayOnline()), menuModel, SLOT(clickedPlayOnline()));
    connect(menuGUI, SIGNAL(clickedPlayCPU()), menuModel, SLOT(clickedPlayCPU()));
    connect(menuGUI, SIGNAL(clickedExit()), menuModel, SLOT(clickedExit()));
    connect(menuGUI, SIGNAL(clickedSound()), menuModel, SLOT(clickedSound()));
    connect(menuGUI, SIGNAL(clickedMusic()), menuModel, SLOT(clickedMusic()));
    connect(menuGUI, SIGNAL(closed()), menuModel, SLOT(closed()));
    // Signals Menu Model -> GUI
    connect(menuModel, SIGNAL(show()), menuGUI, SLOT(show()));
    connect(menuModel, SIGNAL(hide()), menuGUI, SLOT(hide()));
    connect(menuModel, SIGNAL(setSounds(bool)), menuGUI, SLOT(setSounds(bool)));
    connect(menuModel, SIGNAL(setMusic(bool)), menuGUI, SLOT(setMusic(bool)));

    // Signals Selection GUI -> Model
    connect(selectionGUI, SIGNAL(clickedHero(HeroTemplate*)), selectionModel, SLOT(clickedHero(HeroTemplate*)));
    connect(selectionGUI, SIGNAL(clickedPlace(HeroPosition)), selectionModel, SLOT(clickedPlace(HeroPosition)));
    connect(selectionGUI, SIGNAL(clickedCross(HeroPosition)), selectionModel, SLOT(clickedCross(HeroPosition)));
    connect(selectionGUI, SIGNAL(clickedDiscard()), selectionModel, SLOT(clickedDiscard()));
    connect(selectionGUI, SIGNAL(clickedStart()), selectionModel, SLOT(clickedStart()));
    connect(selectionGUI, SIGNAL(clickedMenu()), selectionModel, SLOT(clickedMenu()));
    connect(selectionGUI, SIGNAL(clickedPlayer1()), selectionModel, SLOT(clickedPlayer1()));
    connect(selectionGUI, SIGNAL(clickedPlayer2()), selectionModel, SLOT(clickedPlayer2()));
    connect(selectionGUI, SIGNAL(closed()), selectionModel, SLOT(closed()));
    // Signals Selection Model -> GUI
    connect(selectionModel, SIGNAL(show()), selectionGUI, SLOT(show()));
    connect(selectionModel, SIGNAL(hide()), selectionGUI, SLOT(hide()));
    connect(selectionModel, SIGNAL(setHeroVector(HeroVector*)), selectionGUI, SLOT(setHeroVector(HeroVector*)));
    connect(selectionModel, SIGNAL(setActiveHero(HeroTemplate*)), selectionGUI, SLOT(setActiveHero(HeroTemplate*)));
    connect(selectionModel, SIGNAL(setHeroGroup(HeroGroup*)), selectionGUI, SLOT(setHeroGroup(HeroGroup*)));
    connect(selectionModel, SIGNAL(setCost(Cost*)), selectionGUI, SLOT(setCost(Cost*)));

    // Signals Battle GUI -> Model
    connect(battleGUI, SIGNAL(selectedAction(Action*)), battleModel, SLOT(selectedAction(Action*)));
    connect(battleGUI, SIGNAL(selectedTarget(Hero*)), battleModel, SLOT(selectedTarget(Hero*)));
    connect(battleGUI, SIGNAL(beginTurn()), battleModel, SLOT(beginTurn()));
    connect(battleGUI, SIGNAL(finished()), battleModel, SLOT(finished()));
    connect(battleGUI, SIGNAL(closed()), battleModel, SLOT(closed()));
    // Signals Battel Model -> GUI
    connect(battleModel, SIGNAL(show()), battleGUI, SLOT(show()));
    connect(battleModel, SIGNAL(hide()), battleGUI, SLOT(hide()));
    connect(battleModel, SIGNAL(setPlayers(Player*, Player*)), battleGUI, SLOT(setPlayers(Player*, Player*)));
    connect(battleModel, SIGNAL(setActiveHero(Hero*)), battleGUI, SLOT(setActiveHero(Hero*)));
    connect(battleModel, SIGNAL(setQueue(HeroQueue*)), battleGUI, SLOT(setQueue(HeroQueue*)));
    connect(battleModel, SIGNAL(showTargets(Action*)), battleGUI, SLOT(showTargets(Action*)));
    connect(battleModel, SIGNAL(playAction(Action*)), battleGUI, SLOT(playAction(Action*)));
    connect(battleModel, SIGNAL(updateHPBars()), battleGUI, SLOT(updateHPBars()));
    connect(battleModel, SIGNAL(showDead(Hero*)), battleGUI, SLOT(showDead(Hero*)));
    connect(battleModel, SIGNAL(winPlayer1()), battleGUI, SLOT(winPlayer1()));
    connect(battleModel, SIGNAL(winPlayer2()), battleGUI, SLOT(winPlayer2()));

    // Signals MenuModel -> Game
    connect(menuModel, SIGNAL(clickedPlaySignal()), SLOT(clickedPlay()));
    connect(menuModel, SIGNAL(clickedPlayOnlineSignal()), SLOT(clickedPlayOnline()));
    connect(menuModel, SIGNAL(clickedPlayCPUSignal()), SLOT(clickedPlayCPU()));
    connect(menuModel, SIGNAL(clickedExitSignal()), SLOT(clickedExit()));
    connect(menuModel, SIGNAL(closedSignal()), SLOT(closedMenu()));

    // Signals SelectionModel -> Game
    connect(selectionModel, SIGNAL(clickedStartSignal(Player*, Player*)), SLOT(clickedStart(Player*, Player*)));
    connect(selectionModel, SIGNAL(clickedMenuSignal()), SLOT(clickedMenu()));
    connect(selectionModel, SIGNAL(closedSignal()), SLOT(closedSelection()));

    // Signals BattleModel -> Game
    connect(battleModel, SIGNAL(finishedSignal()), SLOT(finished()));
    connect(battleModel, SIGNAL(closedSignal()), SLOT(closedBattle()));
}

void Game::clickedPlay()
{
    menuModel->hideGUI();
    selectionModel->beginPlay(&heroes);
}

void Game::clickedPlayOnline()
{
    menuModel->hideGUI();
    selectionModel->beginPlayOnline(&heroes);
}

void Game::clickedPlayCPU()
{
    menuModel->hideGUI();
    selectionModel->beginPlayCPU(&heroes);
}

void Game::clickedExit()
{
    menuModel->hideGUI();
    app.quit();
}

void Game::closedMenu()
{
    menuModel->hideGUI();
    app.quit();
}

void Game::clickedStart(Player *player1, Player *player2)
{
    selectionModel->hideGUI();
    battleModel->beginBattle(player1, player2);
}

void Game::clickedMenu()
{
    selectionModel->hideGUI();
    menuModel->showGUI();
}

void Game::closedSelection()
{
    selectionModel->hideGUI();
    app.quit();
}

void Game::finished()
{
    battleModel->hideGUI();
    menuModel->showGUI();
}

void Game::closedBattle()
{
    battleModel->hideGUI();
    app.quit();
}

int Game::exec()
{
	srand(time(0));
	
    loadHeroes();
    menuModel->showGUI();

    return app.exec();
}

void Game::loadHeroes()
{
    TiXmlDocument *document = new TiXmlDocument("res/Heroes.xml");
    if(!document->LoadFile(TIXML_ENCODING_UTF8))
    {
        std::cout << "Error! file .... ";
        return;
    }

    TiXmlElement *xml_resources = document->FirstChildElement("resources");
    if (xml_resources == nullptr)
        return;

    TiXmlElement *xml_hero = xml_resources->FirstChildElement("hero");
    if (xml_hero == nullptr)
        return;

    while(xml_hero != nullptr)
    {
        HeroTemplate *hero = new HeroTemplate();

        // resource load
        Resources resource;
        hero->setName(xml_hero->Attribute("name"));
        string atk_snd = xml_hero->Attribute("atk_snd");
        string skl_snd = xml_hero->Attribute("skl_snd");
        string texture = xml_hero->Attribute("texture");
        string image = xml_hero->Attribute("image");
        string image2 = xml_hero->Attribute("image2");
        resource.loadAttackSound(atk_snd);
        resource.loadSkillSound(skl_snd);
        resource.loadTexture(texture);
        resource.loadImage(image);
        resource.loadImage2(image2);

        hero->setResources(resource);

        // stats load
        int hp, dmg_min, dmg_max, init, cost;
        Kind kind;
        Element element;
        Actions actions;

        xml_hero->QueryIntAttribute("hp", &hp);
        xml_hero->QueryIntAttribute("dmg_min", &dmg_min);
        xml_hero->QueryIntAttribute("dmg_max", &dmg_max);
        kind = strToKind(xml_hero->Attribute("kind"));
        element = strToElement(xml_hero->Attribute("elem"));
        xml_hero->QueryIntAttribute("init", &init);
        strToActions(xml_hero->Attribute("skill"), &actions);
        xml_hero->QueryIntAttribute("cost", &cost);

        hero->setStats(Stats(HP(hp), Damage(dmg_min, dmg_max), Kind(kind), Element(element),
                             Initiative(init), actions, cost));

        heroes.push_back(hero);

        xml_hero = xml_hero->NextSiblingElement("hero");
    }
}

Kind Game::strToKind(const string &str) const
{
    if(!str.compare("range"))
        return Kind::range;

    if(!str.compare("melee"))
        return Kind::melee;

    return Kind(-1);
}

Element Game::strToElement(const string &str) const
{
    if(!str.compare("neutral"))
        return Element::neutral;

    if(!str.compare("fire"))
        return Element::fire;

    if(!str.compare("water"))
        return Element::water;

    if(!str.compare("earth"))
        return Element::earth;

    return Element(-1);
}

void Game::strToActions(const string &str, Actions *actions) const
{
    actions->setAttack(new Attack());

    if(!str.compare("attackrandom"))
    {
        actions->setSkill(new AttackRandom(3, Damage(3, 3)));
        return;
    }

    if(!str.compare("damagedecrease"))
    {
        actions->setSkill(new AttackWithDamageDecrease(Damage(0, 0), 0.5, 1));
        return;
    }

    if(!str.compare("heal"))
    {
        actions->setSkill(new Heal(Damage(5, 7)));
        return;
    }

    if(!str.compare("transferhealth"))
    {
        actions->setSkill(new TransferHealth(Damage(5, 5)));
        return;
    }

    if(!str.compare("attackwithdamagedecrease"))
    {
        actions->setSkill(new AttackWithDamageDecrease(Damage(1, 2), 0.33, 1));
        return;
    }

    if(!str.compare("lifesteal"))
    {
        actions->setSkill(new Lifesteal(Damage(2, 3)));
        return;
    }

    if(!str.compare("dissipateeffects"))
    {
        actions->setSkill(new DissipateEffects());
        return;
    }

    if(!str.compare("attackall"))
    {
        actions->setSkill(new AttackAll(Damage(2, 3)));
        return;
    }

    if(!str.compare("attackfrontline"))
    {
        actions->setSkill(new AttackFrontLine(Damage(4, 6)));
        return;
    }

    if(!str.compare("attacktargetandself"))
    {
        actions->setSkill(new AttackTargetAndSelf(Damage(8, 8), Damage(4, 4)));
        return;
    }

    if(!str.compare("applyperiodicdamage"))
    {
        actions->setSkill(new ApplyPeriodicDamage(Damage(4, 4), 2));
        return;
    }

    if(!str.compare("damageincreaseall"))
    {
        actions->setSkill(new DamageIncreaseAll(0.5, 1));
        return;
    }

    if(!str.compare("attackwithstun"))
    {
        actions->setSkill(new AttackWithStun(Damage(2, 3), 0.5, 1));
        return;
    }

    if(!str.compare("stun"))
    {
        actions->setSkill(new AttackWithStun(Damage(0, 0), 1.0, 1));
        return;
    }

    if(!str.compare("attackwithchance"))
    {
        actions->setSkill(new AttackWithChance(Damage(6, 6), 0.5));
        return;
    }

    if(!str.compare("attacksingle"))
    {
        actions->setSkill(new AttackSingle(Damage(2, 12)));
        return;
    }
}
