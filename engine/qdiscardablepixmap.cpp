#include "qdiscardablepixmap.hpp"
#include <QImageReader>
#include <QImage>
#include <iostream>

using namespace std;

QDiscardablePixmap:: QDiscardablePixmap(const QString fileName, int width, int height) {
    loaded = false;
    aFileName = QString("pixmaps/") + fileName;
    aWidth = width;
    aHeight = height;
}

QDiscardablePixmap:: ~QDiscardablePixmap() {
}

void QDiscardablePixmap:: discard() {
    if (!loaded) return;
    aPixmap = QPixmap();
    loaded = false;
}

void QDiscardablePixmap:: load() {
    if (loaded) return;
    QImageReader reader(aFileName);
    reader.setScaledSize(QSize(aWidth, aHeight));
    QImage image = reader.read();
    aPixmap = QPixmap::fromImage(image);
    loaded = true;
}

QPixmap& QDiscardablePixmap:: pixmap() {
    load();
    return aPixmap;
}

void QDiscardablePixmap:: resize(int width, int height) {
    discard();
    aWidth = width;
    aHeight = height;
}
