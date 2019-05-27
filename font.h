#ifndef FONT_H
#define FONT_H

#include <QObject>
#include <QStringList>

class Font
{
public:
    explicit Font();

    int getSize() const;
    void setSize(int value);

    int getWidth() const;
    void setWidth(int value);

    int getAscent() const;
    void setAscent(int value);

    QStringList getFonts() const;
    void setFonts(const QStringList &value);

private:
    int size;
    int width;
    int ascent;
    QStringList fonts;
};

#endif // FONT_H
