#ifndef SCENE1_HPP
#define SCENE1_HPP

#include "Scene.hpp"
#include <QLabel>

class Scene1 final : public SceneBase
{
    Q_OBJECT
public:
    explicit Scene1(QWidget* parent = nullptr)
        : SceneBase(parent)
    {
        label = new QLabel("This is Scheme 1", this);
        label->setAlignment(Qt::AlignCenter);
        setStyleSheet("background-color: lightblue;");
    }

    QLabel* label;
};

#endif // SCENE1_HPP
