#ifndef QDISCARDABLEPIXMAP_HPP_
#define QDISCARDABLEPIXMAP_HPP_

#include <QObject>
#include <QPixmap>
#include <QString>

/** \brief QPixmap wrapper
 * 
 * Autoloads when needed, can be discarded, resized. Proxy design pattern.
 */
class QDiscardablePixmap : public QObject {
    Q_OBJECT
    protected:
        QPixmap aPixmap;
        bool loaded;
        QString aFileName;
        int aWidth;
        int aHeight;
    public:
        QDiscardablePixmap(const QString fileName, int width, int height);
        ~QDiscardablePixmap();
        void discard();
        void load();
        QPixmap& pixmap();
        void resize(int width, int height);
};

#endif
