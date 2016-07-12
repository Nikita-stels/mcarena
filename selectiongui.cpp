#include "inc/selectiongui.h"
#define YINFO 180
#define XFIELD 166
#define YFIELD 300
#define XPOINTS 180
#define YPOINTS 130
#define XSCROLL 166
#define YBUTTONS 80
#define XPLAYERS 190
#define YPLAYERS 200
#define FRAME 10
#define XSCROLLBAR 30 //default scroll width of sfg::ScrollWindow
#define N 16 //debug

SelectionGUI::SelectionGUI(MainWindow& _app_window, QObject *parent)
    :
      QObject(parent),

      app_window(_app_window),

      //Info
      infoWindow(sfg::Window::Create(sfg::Window::Style::BACKGROUND)),
      infoBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL,FRAME)),
      infoTable(sfg::Table::Create()),

      pointsWindow(sfg::Window::Create(sfg::Window::Style::BACKGROUND)),
      pointsBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, FRAME)),
      pointsLabel(sfg::Label::Create("POINTS: ")),

      infoLabelWindow(sfg::Window::Create(sfg::Window::Style::BACKGROUND)),
      label(sfg::Label::Create("Choose a hero!!!")),

      infoPic(sfg::Image::Create()),
      pendingImage(sfg::Image::Create()),
      pendingImage2(sfg::Image::Create()),

      activeHeroNumber(0),

      //Left heroes list
      scrollwin(sfg::Window::Create(sfg::Window::Style::BACKGROUND)),
      scrollwinbox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, FRAME)),
      scroll(sfg::ScrolledWindow::Create()),
      table(sfg::Table::Create()),
      tablebox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL)),

      //Central hero pick list
      fieldWindow(sfg::Window::Create(sfg::Window::Style::NO_STYLE)),
      fieldBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, FRAME)),
      fieldTable(sfg::Table::Create()),

      heroesGroup(),

      //Buttons
      playerWindow(sfg::Window::Create(sfg::Window::Style::NO_STYLE)),
      playerBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20)),
      player1Button(sfg::Button::Create( "PLAYER1" )),
      player2Button(sfg::Button::Create( "PLAYER2" )),

      buttonsWindow(sfg::Window::Create(sfg::Window::Style::NO_STYLE)),
      buttonsBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 15)),
      menuButton(sfg::Button::Create( "MAIN MENU" )),
      discardButton(sfg::Button::Create( "DISCARD" )),
      startButton(sfg::Button::Create("START"))

{
    app_window.resetGLStates();
    backgroundT.loadFromFile("src/selectBACK.jpg");
    background.setTexture(backgroundT);

    //Info
    desktop.Add(infoWindow);
    infoWindow->Add(infoBox);
    infoBox->Pack(infoTable);
    infoWindow->SetAllocation(sf::FloatRect(XSCROLL+FRAME*2+XSCROLLBAR, 0, app_window.getX()-XSCROLL-FRAME*2-XSCROLLBAR, YINFO));

    desktop.Add(infoLabelWindow);
    infoLabelWindow->Add(label);
    infoLabelWindow->SetAllocation(infoWindow->GetAllocation());
    label->SetId("l1");
    sfg::Context::Get().GetEngine().SetProperty("Label#l1", "FontSize", 20);

    desktop.Add(pointsWindow);
    pointsWindow->Add(pointsBox);
    pointsBox->Pack(pointsLabel);
    pointsWindow->SetAllocation(sf::FloatRect(app_window.getX()-XPOINTS, YINFO, XPOINTS, YPOINTS));

    for (int i=0;i<7;i++) infoLabels[i]= sfg::Label::Create("");
    for (int i=0;i<5;i++)
    {
        sf::Image icoImg;
        icoImg.loadFromFile("src/ico.jpg");
        infoPics[i]= sfg::Image::Create(icoImg);
    }

    infoTable->Attach(infoPics[0],sf::Rect<sf::Uint32>(1, 1, 1, 1),sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoPics[1],sf::Rect<sf::Uint32>(1, 2, 1, 1),sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoPics[2],sf::Rect<sf::Uint32>(3, 1, 1, 1),sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoPics[3],sf::Rect<sf::Uint32>(3, 2, 1, 1),sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoPics[4],sf::Rect<sf::Uint32>(3, 0, 1, 1),sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[0],sf::Rect<sf::Uint32>(1, 0, 2, 1),sfg::Table::EXPAND | sfg::Table::FILL , sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[1],sf::Rect<sf::Uint32>(2, 1, 1, 1),sfg::Table::EXPAND , sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[2],sf::Rect<sf::Uint32>(2, 2, 1, 1),sfg::Table::EXPAND , sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[3],sf::Rect<sf::Uint32>(4, 1, 1, 1),sfg::Table::EXPAND , sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[4],sf::Rect<sf::Uint32>(4, 2, 1, 1),sfg::Table::EXPAND , sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[5],sf::Rect<sf::Uint32>(4, 0, 1, 1),sfg::Table::EXPAND , sfg::Table::EXPAND | sfg::Table::FILL);
    infoTable->Attach(infoLabels[6],sf::Rect<sf::Uint32>(0, 3, 7, 1),sfg::Table::EXPAND | sfg::Table::FILL , sfg::Table::EXPAND | sfg::Table::FILL);

    //Left heroes list
    desktop.Add(scrollwin);
    scrollwin->Add(scrollwinbox);
    scrollwinbox->Pack(scroll, false, true);
    scroll->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER| sfg::ScrolledWindow::VERTICAL_ALWAYS);
    tablebox->Pack(table);
    scroll->AddWithViewport(tablebox);
    scroll->SetRequisition( sf::Vector2f( XSCROLL - FRAME*2, app_window.getY() - FRAME*2) );

    //Central hero pick list
    desktop.Add(fieldWindow);
    fieldWindow->Add(fieldBox);
    fieldBox->Pack(fieldTable);
    fieldWindow->SetAllocation(sf::FloatRect((app_window.getX()+XSCROLL+FRAME*2+XSCROLLBAR-XPOINTS-XFIELD)/2,
                                             (app_window.getY()-FRAME*2-YBUTTONS+YINFO-YFIELD)/2, XFIELD, YFIELD));

    plusImg.loadFromFile("res/img/icons/plus.png");
    crossImg.loadFromFile("res/img/icons/cross.png");
    for (int i=0;i<6;i++)
    {
        imageArray[i] = sfg::Image::Create(plusImg);
        imageType[i] = Image::DEFAULT;

        crossImageArray[i] = sfg::Image::Create(crossImg);

        //Signals connection
        imageArray[i]->GetSignal( sfg::Widget::OnMouseEnter ).Connect(
                    std::bind( &SelectionGUI::mouseEvent, this, Mouse::ENTER, i ) );
        imageArray[i]->GetSignal( sfg::Widget::OnMouseLeave ).Connect(
                    std::bind( &SelectionGUI::mouseEvent, this, Mouse::LEAVE, i ) );
        connectSignals(i);

        fieldTable->Attach(imageArray[i],sf::Rect<sf::Uint32>( i%2, floor(i/2+0.5), 1, 1),sfg::Table::FILL, sfg::Table::FILL);
        fieldTable->Attach(crossImageArray[i], sf::Rect<sf::Uint32>( i%2, floor(i/2+0.5), 1, 1));
        crossImageArray[i]->Show(false);
    }

    //Buttons
    desktop.Add(buttonsWindow);
    buttonsWindow->Add(buttonsBox);
    buttonsWindow->SetAllocation(sf::FloatRect( XSCROLL + FRAME + XSCROLLBAR, app_window.getY()-YBUTTONS,
                                                app_window.getX()-XSCROLL - FRAME - XSCROLLBAR, YBUTTONS));

    buttonsBox->Pack(menuButton);
    buttonsBox->Pack(discardButton);
    buttonsBox->Pack(startButton);


    desktop.Add(playerWindow);
    playerWindow->Add(playerBox);
    playerWindow->SetAllocation(sf::FloatRect( app_window.getX()-XPLAYERS, app_window.getY()-YPLAYERS-YBUTTONS, XPLAYERS, YPLAYERS));
    playerBox->Pack(player1Button);
    playerBox->Pack(player2Button);

    player1Button->SetId("p1");
    player2Button->SetId("p2");
    sfg::Context::Get().GetEngine().SetProperty("Button#p1", "BackgroundColor", sf::Color(96,26,67));
    player1Button->SetState(sfg::Widget::State::INSENSITIVE);

    //Signals
    startButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(  std::bind( &SelectionGUI::clickedButton, this, ButtonPressed::START ) );
    discardButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &SelectionGUI::clickedButton, this, ButtonPressed::DISCARD ) );
    menuButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(  std::bind( &SelectionGUI::clickedButton, this, ButtonPressed::MENU ) );
    player1Button->GetSignal( sfg::Widget::OnLeftClick ).Connect(  std::bind( &SelectionGUI::clickedButton, this, ButtonPressed::PLAYER1 ) );
    player2Button->GetSignal( sfg::Widget::OnLeftClick ).Connect(  std::bind( &SelectionGUI::clickedButton, this, ButtonPressed::PLAYER2 ) );
}

void SelectionGUI::clickedButton(ButtonPressed Button)
{
    switch (Button)
    {
        case START: clickedStart(); break;
        case DISCARD: clickedDiscard(); break;
        case MENU: clickedMenu(); break;
        case PLAYER1:
        {
            sfg::Context::Get().GetEngine().SetProperty("Button#p1", "BackgroundColor", sf::Color(96,26,67));
            sfg::Context::Get().GetEngine().SetProperty("Button#p2", "BackgroundColor", sf::Color(85,87,82));
            player1Button->SetState(sfg::Widget::State::INSENSITIVE);
            player2Button->SetState(sfg::Button::State::NORMAL);
            clickedPlayer1();
            break;
        }
        case PLAYER2:
        {
            sfg::Context::Get().GetEngine().SetProperty("Button#p2", "BackgroundColor", sf::Color(96,26,67));
            sfg::Context::Get().GetEngine().SetProperty("Button#p1", "BackgroundColor", sf::Color(85,87,82));
            player2Button->SetState(sfg::Widget::State::INSENSITIVE);
            player1Button->SetState(sfg::Button::State::NORMAL);
            clickedPlayer2();
            break;
        }
    }
}

void SelectionGUI::_clickedHero(int i)
{
    if(i != activeHeroNumber)
    {
        clickedHero(heroesList->at(i));
    }
}

void SelectionGUI::_clickedPlace(HeroPosition pos, int i)
{
    if(imageType[i] == Image::DEFAULT && activeHeroNumber != -1)
        clickedPlace(pos);
}

void SelectionGUI::_clickedCross(HeroPosition pos, int i)
{
    crossImageArray[i]->Show(false);
    clickedCross(pos);
}

void SelectionGUI::show()
{
   infoWindow->Show(true);
   pointsWindow->Show(true);
   fieldWindow->Show(true);
   buttonsWindow->Show(true);
   playerWindow->Show(true);
   infoLabelWindow->Show(true);
   scrollwin->Show(true);
   timer = app_window.newTimer();
   connect(timer, SIGNAL(timeout()), this, SLOT(update()));
   timer->start(10);
}

void SelectionGUI::hide()
{
    infoWindow->Show(false);
    pointsWindow->Show(false);
    fieldWindow->Show(false);
    buttonsWindow->Show(false);
    playerWindow->Show(false);
    infoLabelWindow->Show(false);
    scrollwin->Show(false);
    app_window.deleteTimer();
    app_window.clear(sf::Color::Black);
    app_window.display();
}

void SelectionGUI::update()
{
    if (app_window.isOpen())
    {
        sf::Event event;
        while (app_window.pollEvent(event))
        {
            desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
            {
               app_window.close();
               closed();
            }
        }
        desktop.Update( 10 );
        app_window.clear();
        app_window.draw(background);
        sfgui.Display(app_window);
        app_window.display();
    }
}

void SelectionGUI::setHeroVector(HeroVector *heroVector)
{
    heroesList = heroVector;
    int i = 0;
    for(auto it = heroesList->begin(), end = heroesList->end(); it != end; it++, i++)
    {
        sfg::Image::Ptr finalImage = sfg::Image::Create((*it)->getResources().getImage());
        table->Attach(finalImage,sf::Rect<sf::Uint32>( i%2, floor(i/2+0.5), 1, 1),sfg::Table::FILL, sfg::Table::FILL);
        //Signal setting
        finalImage->GetSignal( sfg::Widget::OnLeftClick ).Connect(  std::bind( &SelectionGUI::_clickedHero, this, i));
    }
}

void SelectionGUI::setActiveHero(HeroTemplate *hero)
{
    //Heroes highlighting in left menu
    int i = 0, pos = -1;
    for(auto it = heroesList->begin(), end = heroesList->end(); it != end; it++)
    {
        if(*it == hero)
        {
            pendingImage->SetImage((hero)->getResources().getImage2());
            table->Attach(pendingImage,sf::Rect<sf::Uint32>( i%2, floor(i/2+0.5), 1, 1),sfg::Table::FILL, sfg::Table::FILL);
            pos = i;
        }
        else if(i == activeHeroNumber)
        {
            pendingImage2->SetImage((*it)->getResources().getImage());
            table->Attach(pendingImage2,sf::Rect<sf::Uint32>( i%2, floor(i/2+0.5), 1, 1),sfg::Table::FILL, sfg::Table::FILL);
        }
        i++;
    }
    activeHeroNumber = pos;

    //Active hero info on the top
    if(activeHeroNumber == -1)
    {
        infoLabelWindow->Show(true);
        infoWindow->Show(false);
    }
    else
    {
        infoLabelWindow->Show(false);
        infoWindow->Show(true);
        infoLabels[0]->SetText("Pretty Long Hero Name");
        infoLabels[1]->SetText(std::to_string( hero->getStats().hp.max ) + " HP");
        infoLabels[2]->SetText(std::to_string( hero->getStats().damage.max ) + " DMG");
        infoLabels[3]->SetText(std::to_string( hero->getStats().initiative.val ) + " INIT");
        switch(hero->getStats().element)
        {
            case Element::neutral: infoLabels[4]->SetText("Neutral element");
            case Element::fire: infoLabels[4]->SetText("Fire element");
            case Element::water: infoLabels[4]->SetText("Water element");
            case Element::earth: infoLabels[4]->SetText("Earth element");
        }
        infoLabels[5]->SetText(std::to_string( hero->getStats().cost ) + " COINS");
        switch(hero->getStats().kind)
        {
            case Kind::melee: infoLabels[6]->SetText("Melee");
            case Kind::range: infoLabels[6]->SetText("Range");
        }

        infoPic->SetImage(hero->getResources().getImage());
        infoTable->Attach(infoPic,sf::Rect<sf::Uint32>(0, 0, 1, 3),sfg::Table::EXPAND | sfg::Table::FILL , sfg::Table::EXPAND );
    }
}

void SelectionGUI::setHeroGroup(HeroGroup *heroGroup)
{
        for (int i = 0; i < 6; i++)
        {
            switch(i)
            {
                case 0: setHeroImage(heroGroup->at(HeroPosition::back1), heroesGroup.at(HeroPosition::back1), i); break;
                case 1: setHeroImage(heroGroup->at(HeroPosition::front1), heroesGroup.at(HeroPosition::front1), i); break;
                case 2: setHeroImage(heroGroup->at(HeroPosition::back2), heroesGroup.at(HeroPosition::back2), i); break;
                case 3: setHeroImage(heroGroup->at(HeroPosition::front2), heroesGroup.at(HeroPosition::front2), i); break;
                case 4: setHeroImage(heroGroup->at(HeroPosition::back3), heroesGroup.at(HeroPosition::back3), i); break;
                case 5: setHeroImage(heroGroup->at(HeroPosition::front3), heroesGroup.at(HeroPosition::front3), i); break;
            }
        }
        heroesGroup = *heroGroup;
}

void SelectionGUI::setHeroImage(Hero *hero, Hero *myhero, int pos)
{
    if(hero != myhero)
    {
        if (hero != nullptr)
        {
            imageArray[pos]->SetImage(hero->getResources().getImage());
            imageType[pos] = Image::NORMAL;
        }
        else
        {
            imageArray[pos]->SetImage(plusImg);
            imageType[pos] = Image::DEFAULT;
        }
    }
}

void SelectionGUI::mouseEvent(Mouse mouse, int i)
{
    if(imageType[i] == Image::NORMAL)
    {
        switch(mouse)
        {
            case ENTER:
                crossImageArray[i]->Show(true);
                break;
            case LEAVE:
                crossImageArray[i]->Show(false);
                break;
        }
    }
}

void SelectionGUI::connectSignals(int pos)
{
    switch(pos)
    {
        case 0: imageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                    std::bind( &SelectionGUI::_clickedPlace, this, HeroPosition::back1, pos ) );
                crossImageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                                std::bind( &SelectionGUI::_clickedCross, this, HeroPosition::back1, pos) );
                break;
        case 1: imageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                    std::bind( &SelectionGUI::_clickedPlace, this, HeroPosition::front1, pos ) );
                crossImageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                                std::bind( &SelectionGUI::_clickedCross, this, HeroPosition::front1, pos) );
                break;
        case 2: imageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                    std::bind( &SelectionGUI::_clickedPlace, this, HeroPosition::back2, pos ) );
                crossImageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                                std::bind( &SelectionGUI::_clickedCross, this, HeroPosition::back2, pos) );
                break;
        case 3: imageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                    std::bind( &SelectionGUI::_clickedPlace, this, HeroPosition::front2, pos ) );
                crossImageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                                std::bind( &SelectionGUI::_clickedCross, this, HeroPosition::front2, pos) );
                break;
        case 4: imageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                   std::bind( &SelectionGUI::_clickedPlace, this, HeroPosition::back3, pos ) );
                crossImageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                                std::bind( &SelectionGUI::_clickedCross, this, HeroPosition::back3, pos) );
                break;
        case 5: imageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                    std::bind( &SelectionGUI::_clickedPlace, this, HeroPosition::front3, pos ) );
                crossImageArray[pos]->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                                std::bind( &SelectionGUI::_clickedCross, this, HeroPosition::front3, pos) );
                break;
    }
}

void SelectionGUI::setCost(Cost *)
{

}

void SelectionGUI::showCross(HeroPosition)
{

}
