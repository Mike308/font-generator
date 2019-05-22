#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QFont>
#include <QPainter>
#include <QImage>
#include <QDebug>

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
    QStringList prepareFont(const QString &asciiChar);
    QString extraZeros(QString binValue);
    void on_generateBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
