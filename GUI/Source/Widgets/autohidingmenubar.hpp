#ifndef AUTOHIDINGMENUBAR_HPP
#define AUTOHIDINGMENUBAR_HPP

#include <Kiwi/Config.hpp>

#include <QMenuBar>
#include <QApplication>

class AutoHidingMenuBar : public QMenuBar
{
    Q_OBJECT
    
public:

    AutoHidingMenuBar(QWidget * parent = nullptr)
        : QMenuBar(parent)
    {
        connect(
            QApplication::instance(),
            SIGNAL(focusChanged(QWidget*, QWidget*)),
            this,
            SLOT(focusChanged(QWidget*, QWidget*))
        );
    }

    inline bool isAutoHiding() const {
        return _autoHiding;
    }

    inline void setAutoHiding(bool autoHiding) {
        _autoHiding = autoHiding;

        setVisible(!_autoHiding);
    }

    void altPressed();

private slots:

    void focusChanged(QWidget * from, QWidget * to);

private:

    bool _autoHiding = false;

    bool hasFocusedChild();

};

#endif // AUTOHIDINGMENUBAR_HPP