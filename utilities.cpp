#include <cstdio>
#include <QString>
#include <QStringList>
#include "utilities.h"

void drawLine() {
    for (int i = 0 ; i < 80 ; i++)
        printf("*");
    printf("\n");

}

void displayResults(QStringList & results) {
    drawLine();
    for (auto line : results) {
        printf(">> %s\n", line.toLocal8Bit().data());
    }
    drawLine();
}
