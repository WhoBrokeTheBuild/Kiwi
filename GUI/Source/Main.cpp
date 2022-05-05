
#include <QApplication>

#include <cflags.hpp>

#include <Kiwi/Log.hpp>

#include "Widgets/mainwindow.hpp"

#include <cstdlib>
#include <ctime>

int main(int argc, char * argv[])
{
    srand(time(0));

    try {
        QApplication app(argc, argv);

        cflags::cflags flags;

        std::string emulatorName;
        flags.add_string('e', "emulator", &emulatorName, "emulator library to load on launch");

        flags.parse(argc, argv);

        MainWindow window;
        window.show();

        if (!emulatorName.empty()) {
            window.loadEmulator(emulatorName);
        }

        return app.exec();
    }
    catch (const std::exception& e) {
        printf("Exception: %s\n", e.what());
    }

    return 1;
}