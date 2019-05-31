#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QFont>
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <string>
#include <iostream>

#include "font.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fontComboBox_currentTextChanged(const QString &arg1);
    void on_generateBtn_clicked();
    void on_fontSizeSpinner_valueChanged(int arg1);

    void on_boldFont_stateChanged(int arg1);

    void on_italicFont_stateChanged(int arg1);

    void on_underLine_stateChanged(int arg1);

    void on_calcOffset_clicked();

private:
    Ui::MainWindow *ui;
    Font prepareFont(const QString &asciiChar);
    QString extraZeros(QString binValue);
    QString convertToHex(QString binValue);
    uint32_t calculateOffset(int height, int width, char asciiChar);
};

#endif // MAINWINDOW_H
