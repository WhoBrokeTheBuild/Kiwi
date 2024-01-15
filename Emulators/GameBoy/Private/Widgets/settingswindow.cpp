#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <Kiwi/Utility/Log.hpp>

#include <QFileDialog>
#include <QMessageBox>

#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

SettingsWindow::SettingsWindow(QWidget * parent) :
    QFrame(parent),
    _ui(new Ui::SettingsWindow)
{
    _ui->setupUi(this);

    load();
}

SettingsWindow::~SettingsWindow()
{
    delete _ui;
}

void SettingsWindow::load()
{
    std::ifstream file(filename());
    if (!file) {
        return;
    }

    json data = json::parse(file);

    if (data.contains("defaultROMDirectory")) {
        _defaultROMDirectory = QString::fromStdString(data["defaultROMDirectory"].get<std::string>());
        _ui->txtDefaultROMDirectory->setText(_defaultROMDirectory);
    }

    if (data.contains("bootstrapROM")) {
        _bootstrapROM = QString::fromStdString(data["bootstrapROM"].get<std::string>());
        _ui->txtBootstrapROM->setText(_bootstrapROM);
    }
}

void SettingsWindow::save()
{
    std::ofstream file(filename());
    if (!file) {
        QMessageBox::critical(
            this,
            "Failed to save settings",
            QString::fromStdString(fmt::format("Unable to save settings file {}", filename())),
            QMessageBox::Ok
        );
    }

    json data;
    data["defaultROMDirectory"] = _defaultROMDirectory.toStdString();
    data["bootstrapROM"] = _bootstrapROM.toStdString();
    file << data.dump(4);
}

void SettingsWindow::btnDefaultROMDirectoryTriggered()
{
    auto path = QFileDialog::getExistingDirectory(
        this,
        "Choose Default ROM Directory",
        _ui->txtDefaultROMDirectory->text()
    );

    if (!path.isEmpty()) {
        _ui->txtDefaultROMDirectory->setText(path);
    }
}

void SettingsWindow::btnBootstrapROMTriggered()
{
    auto filename = QFileDialog::getOpenFileName(
        this,
        "Load GameBoy Bootstrap ROM",
        _ui->txtBootstrapROM->text(),
        "ROM files (*.*)"
    );

    if (!filename.isEmpty()) {
        _ui->txtBootstrapROM->setText(filename);

        // try {
            // DMG_CPU.loadBootstrapROM(filename.toStdString());
        // }
        // catch (const RuntimeError& e) {
        //     QMessageBox::critical(
        //         _mainWindow,
        //         "Failed to load Bootstrap ROM",
        //         e.what(),
        //         QMessageBox::Ok
        //     );
        // }
    }
}

void SettingsWindow::saveTriggered()
{
    _defaultROMDirectory = _ui->txtDefaultROMDirectory->text();
    _bootstrapROM = _ui->txtBootstrapROM->text();

    save();
    hide();
}

void SettingsWindow::cancelTriggered()
{
    _ui->txtDefaultROMDirectory->setText(_defaultROMDirectory);
    _ui->txtBootstrapROM->setText(_bootstrapROM);

    hide();
}