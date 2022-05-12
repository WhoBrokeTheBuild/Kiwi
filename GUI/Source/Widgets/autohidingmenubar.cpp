#include "autohidingmenubar.hpp"

#include <Kiwi/Log.hpp>

void AutoHidingMenuBar::focusChanged(QWidget * from, QWidget * to)
{
    bool inFocus = hasFocus() || isAncestorOf(to) || hasFocusedChild();
    if (inFocus && !isVisible()) {
        setVisible(true);
    }
    else if (!inFocus && isVisible() && _autoHiding) {
        setVisible(false);
    }
}

bool AutoHidingMenuBar::hasFocusedChild()
{
    QObjectList queue(children());
    while (!queue.isEmpty()) {
        QObject * child = queue.takeFirst();
        QWidget * widget = dynamic_cast<QWidget *>(child);
        if (widget && widget->hasFocus()) {
            return true;
        }

        queue.append(child->children());
    }

    return false;
}

void AutoHidingMenuBar::altPressed()
{
    if (!isVisible()) {
        setVisible(true);
    }

    QObject * firstChild = children().front();
    QWidget * firstWidget = dynamic_cast<QWidget *>(firstChild);
    if (firstWidget) {
        firstWidget->focusWidget();
        QMenu * firstMenu = dynamic_cast<QMenu *>(firstWidget);
        if (firstMenu) {
            firstMenu->exec();
        }
        else {
            kiwi::Log(KIWI_ANCHOR, "Double fuck");
        }
    }
    else {
        kiwi::Log(KIWI_ANCHOR, "Fuck");
    }
}