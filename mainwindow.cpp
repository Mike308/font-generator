#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFontDatabase fonts;
    QStringList list = fonts.families();
    ui->fontComboBox->addItems(list);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fontComboBox_currentTextChanged(const QString &arg1)
{
    QFont font;
    font.setFamily(arg1);
    font.setPixelSize(12);
    ui->previewField->setFont(font);
    ui->previewField->clear();
    ui->previewField->appendPlainText("Font test");
}


QStringList MainWindow::prepareFont(const QString &asciiChar){
    QFont font;
    font.setPixelSize(12);
    font.setFamily(ui->fontComboBox->currentText());
    QSize size;
    QImage image(40,40, QImage::Format_Mono);
    image.fill(0);
    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.setFont(font);
    QFontMetrics fm(painter.font());
    qDebug () << "Font: " << painter.font().family() << " Size: " << fm.height() << "Ascent: " << fm.ascent() << "Descent: " << fm.descent() << "Leading: " << fm.leading();
    size.setWidth(fm.width('A'));
    size.setHeight(fm.height());
    painter.drawText(0, fm.height(), asciiChar);
    QVector <QVector <int> > mainArray(size.height(), QVector < int> (size.width(), 0));
    QVector <QVector <char> > previewArray(size.height(), QVector <char> (size.width(), ' '));
    for (int r = 0; r < size.height(); r++) {
        for (int c = 0; c < size.width(); c++) {
            mainArray[r][c] = (image.pixelColor(c,r) == Qt::white ? 1 : 0);
            previewArray[r][c] = (image.pixelColor(c, r) == Qt::white ? '*' : '-');
        }
    }
    QString bin = "";
    QStringList list;
    for (int r = fm.descent() * 2; r < size.height(); r++) {
        for (int c = 0; c < size.width(); c++) {
            bin.append(QString::number(mainArray[r][c]));
        }
        list.append(extraZeros(bin));
        bin = "";
    }

    for (int i = 0; i < fm.descent(); i++) {
        list.append(extraZeros("0"));
    }

    QStringList newFont;

    int i = 0;
    foreach (QString item, list) {
        int decValue = std::stoi(item.toStdString(), nullptr, 2);
        qDebug () << "I: " << i <<"Item: " << item <<"Hex: " << "0x" + QString::number(decValue, 16);
        newFont.append("0x" + (QString::number(decValue, 16)).append("    ").append(item.replace("0","-").replace("1","*")));
        i++;
    }

    return newFont;
}

void MainWindow::on_generateBtn_clicked()
{
    QStringList newFont = prepareFont("A");
    foreach (QString item, newFont) {
        //qDebug () << item;
    }
}

QString MainWindow::extraZeros(QString value) {
    QString temp;
    while ((value.length() % 8) != 0) {
        temp = value.append("0");
    }
    return temp;
}
