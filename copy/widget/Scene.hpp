#ifndef SCHENE_HPP
#define SCHENE_HPP

#include <QWidget>

class SceneBase : public QWidget
{
    Q_OBJECT
public:
    explicit SceneBase(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

    virtual ~SceneBase() = default;
};

#endif
