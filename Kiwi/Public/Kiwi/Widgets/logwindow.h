#ifndef KIWI_GAMEBOY_LOG_WINDOW_H
#define KIWI_GAMEBOY_LOG_WINDOW_H

#include <QFrame>
#include <QString>

#include <Kiwi/Utility/Containers.hpp>

using namespace kiwi;

namespace Ui {
class LogWindow;
}

class LogWindow : public QFrame
{
    Q_OBJECT

public:

    inline static const unsigned LOG_LINE_LENGTH = 128;

    explicit LogWindow(QWidget * parent = nullptr);

    virtual ~LogWindow();

    inline void setAutoScroll(bool enable) {
        _autoScroll = enable;
    }

    inline bool autoScroll() const {
        return _autoScroll;
    }

    inline void setMaxLines(unsigned count) {
        _maxLines = count;
    }

    inline unsigned maxLines() const {
        return _maxLines;
    }

public slots:


private:

    Ui::LogWindow * _ui;

    bool _autoScroll;

    unsigned _maxLines;

    List<Array<char, LOG_LINE_LENGTH>> _lineList;

};

#endif // KIWI_GAMEBOY_LOG_WINDOW_H
