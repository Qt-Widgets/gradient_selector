#include "gradient_selector.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GradientSelector w;
    w.show();

    return a.exec();
}
