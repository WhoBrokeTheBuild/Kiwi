#ifndef KIWI_GAMEBOY_SETTINGS_WINDOW_H
#define KIWI_GAMEBOY_SETTINGS_WINDOW_H

#include <QFrame>
#include <QString>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QFrame
{
    Q_OBJECT

public:

    explicit SettingsWindow(QWidget * parent = nullptr);

    ~SettingsWindow();

    inline QString defaultROMDirectory() const {
        return _defaultROMDirectory;
    }

    inline QString bootstrapROM() const {
        return _bootstrapROM;
    }

    void load();

    void save();

public slots:

    void btnDefaultROMDirectoryTriggered();

    void btnBootstrapROMTriggered();

    void saveTriggered();

    void cancelTriggered();

private:

    inline std::string filename() {
        return "Kiwi.Settings.GameBoy.json";
    }

    Ui::SettingsWindow * _ui;

    QString _defaultROMDirectory;

    QString _bootstrapROM;

};

#endif // KIWI_GAMEBOY_SETTINGS_WINDOW_H
