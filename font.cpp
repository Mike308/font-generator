#include "font.h"

Font::Font()
{

}

int Font::getSize() const
{
    return size;
}

void Font::setSize(int value)
{
    size = value;
}

int Font::getWidth() const
{
    return width;
}

void Font::setWidth(int value)
{
    width = value;
}

int Font::getAscent() const
{
    return ascent;
}

void Font::setAscent(int value)
{
    ascent = value;
}

QStringList Font::getFonts() const
{
    return fonts;
}

void Font::setFonts(const QStringList &value)
{
    fonts.append(value);
}
