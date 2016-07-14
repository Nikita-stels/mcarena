QT += core
QT -= gui

CONFIG += c++11

TARGET = mcarena
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    battlegui.cpp \
    battlemodel.cpp \
    menugui.cpp \
    menumodel.cpp \
    selectiongui.cpp \
    selectionmodel.cpp \
    game.cpp \
    hp.cpp \
    damage.cpp \
    element.cpp \
    initiative.cpp \
    stats.cpp \
    herotemplate.cpp \
    hero.cpp \
    mainwindow.cpp \
    player.cpp \
    herogroup.cpp \
    resources.cpp \
    cost.cpp \
    heroqueue.cpp \
    action.cpp \
    targets.cpp \
    actions.cpp \
    spritesfield.cpp \
    attack.cpp \
    attackrandom.cpp \
    heal.cpp \
    lifesteal.cpp \
    attackall.cpp \
    attackfrontline.cpp \
    attacksingle.cpp

HEADERS += \
    inc/menugui.h \
    inc/selectiongui.h \
    inc/battlegui.h \
    inc/menumodel.h \
    inc/selectionmodel.h \
    inc/battlemodel.h \
    inc/hero.h \
    inc/heroqueue.h \
    inc/herogroup.h \
    inc/player.h \
    inc/action.h \
    inc/game.h \
    inc/stats.h \
    inc/hp.h \
    inc/damage.h \
    inc/kind.h \
    inc/element.h \
    inc/initiative.h \
    inc/actions.h \
    inc/resources.h \
    inc/herotemplate.h \
    inc/mainwindow.h \
    inc/heroposition.h \
    inc/herovector.h \
    inc/cost.h \
    inc/targets.h \
    inc/spritesfield.h \
    inc/attack.h \
    inc/actions_list.h \
    inc/attackrandom.h \
    inc/heal.h \
    inc/lifesteal.h \
    inc/attackall.h \
    inc/attackfrontline.h \
    inc/attacksingle.h

LIBS += \
    -lsfml-graphics \
    -lsfml-window \
    -lsfml-system \
    -lsfml-audio \
    -lsfgui \
    -ltinyxml
