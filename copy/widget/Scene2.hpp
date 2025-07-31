#ifndef SCENE2_HPP
#define SCENE2_HPP

#include "Scene.hpp"
#include <QPushButton>

class Scene2 final : public SceneBase
{
    Q_OBJECT
public:
    explicit Scene2(QWidget* parent = nullptr)
        : SceneBase(parent)
    {
        button = new QPushButton("Button in Scheme 2", this);
        button->move(50, 50);
        setStyleSheet("background-color: lightgreen;");
    }
    QPushButton* button;
};

#endif
