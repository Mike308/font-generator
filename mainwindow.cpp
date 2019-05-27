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


Font MainWindow::prepareFont(const QString &asciiChar){
    QFont font;
    font.setPixelSize(ui->fontSizeSpinner->value());
    font.setFamily(ui->fontComboBox->currentText());
    font.setBold(ui->)
    QSize size;
    QFontMetrics fm(font);
    qDebug () << "Font: " <<font.family() << " Size: " << fm.height()
              << "Ascent: " << fm.ascent() << "Descent: " << fm.descent()
              << "Leading: " << fm.leading() << "wide: " << fm.width(asciiChar);
    QImage image(fm.width("W"), fm.ascent(), QImage::Format_Mono);
    image.fill(0);
    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.setFont(font);
    size.setWidth(fm.width("W"));
    size.setHeight(fm.ascent());
    painter.drawText((image.width() - fm.width(asciiChar))/2, fm.ascent() - fm.descent(), asciiChar);
    ui->label->setPixmap(QPixmap::fromImage(image).scaled(100, 100));
    QVector <QVector <int> > mainArray(size.height(), QVector < int> (size.width(), 0));
    QVector <QVector <char> > previewArray(size.height(), QVector <char> (size.width(), ' '));
    for (int r = 0; r < size.height(); r++) {
        for (int c = 0; c < size.width(); c++) {
            mainArray[r][c] = (image.pixelColor(c,r) == Qt::white ? 1 : 0);
            previewArray[r][c] = (image.pixelColor(c, r) == Qt::white ? '*' : '.');
        }
    }
    QString bin = "";
    QStringList list;
    int start = ui->checkBox->isChecked() == true ? (fm.descent() * 2) : 0;
    for (int r = start; r < size.height(); r++) {
        for (int c = 0; c < size.width(); c++) {
            bin.append(QString::number(mainArray[r][c]));
        }
        list.append(extraZeros(bin));
        bin = "";
    }


    for (int r = start; r < size.height(); r++) {
        for (int c = 0; c < size.width(); c++) {
            std::cout << previewArray[r][c];
        }
        std::cout << std::endl;
    }

    QStringList newFont;

    int i = 0;
    //    newFont.append("//"+asciiChar);
    foreach (QString item, list) {
        //        qDebug () << "I: " << i <<"Item: " << item;
        //                  <<"Hex: " << convertToHex(item);
        newFont.append(convertToHex(item).append(","));
        i++;
    }

    Font fonts;
    qDebug () << "Lenght: " << newFont.length();
    fonts.setSize(fm.height());
    fonts.setWidth(fm.width("W"));
    fonts.setAscent(fm.ascent());
    fonts.setFonts(newFont);
    ui->ascentValue->setText(QString::number(fm.ascent()));
    ui->widthValue->setText(QString::number(fm.width("W")));

    return fonts;
}

void MainWindow::on_generateBtn_clicked()
{
    QStringList fonts;
    Font font;
    if (ui->checkBox_2->isChecked()){
        font = prepareFont(ui->lineEdit->text());
        fonts.append(font.getFonts());
    }else{

        for (int i = 32; i < 128; i++) {
            font = prepareFont(QString(char(i)));
            fonts.append(QString(" //").append(QString(char(i))));
            fonts.append(font.getFonts());
        }

    }

    ui->outputField->clear();
    ui->outputField->appendPlainText("const uint8_t " +  ui->fontComboBox->currentText().replace(" ","_").toLower() + "_" + QString::number(ui->fontSizeSpinner->value())
                                     + "_table[]" + "={ ");
    foreach (QString item, fonts) {
        ui->outputField->appendPlainText(item);
    }
    ui->outputField->appendPlainText("};");
    ui->outputField->appendPlainText("font_t " + ui->fontComboBox->currentText().replace(" ","_").toLower() + "_" + QString::number(ui->fontSizeSpinner->value()) + " {");
    ui->outputField->appendPlainText(ui->fontComboBox->currentText().replace(" ","_").toLower() + "_" + QString::number(ui->fontSizeSpinner->value())
                                     + "_table,");
    ui->outputField->appendPlainText(QString::number(font.getWidth()).append(","));
    ui->outputField->appendPlainText(QString::number(font.getAscent()));
    ui->outputField->appendPlainText("};");


}

QString MainWindow::extraZeros(QString binValue) {
    QString temp;
    while ((binValue.length() % 8) != 0) {
        temp = binValue.append("0");
    }
    if ((binValue.length() % 8) == 0) {
        return binValue;
    }
    else {
        return temp;
    }
}

QString MainWindow::convertToHex(QString binValue) {
    if (binValue.length() > 8){
        uint16_t decValue = std::stoi(binValue.toStdString(), nullptr, 2);
        uint8_t high = decValue >> 8;
        uint8_t low = decValue;
        return "0x" + QString::number(high, 16).append(",").append("0x").append(QString::number(low, 16));
    }else{
        int decValue = std::stoi(binValue.toStdString(), nullptr, 2);
        return "0x" + QString::number(decValue, 16);
    }
}

void MainWindow::on_fontSizeSpinner_valueChanged(int arg1)
{
    QFont font;
    font.setFamily(ui->fontComboBox->currentText());
    font.setPixelSize(arg1);
    QFontMetrics fontMetrics(font);
    ui->previewField->setFont(font);
    ui->ascentValue->setText(QString("Asccent: ").append(QString::number(fontMetrics.ascent())));

}

uint32_t MainWindow::calculateOffset(int height, int width){
    uint32_t result = ('!' - ' ') * height * (width / 8 + (width % 8 ? 1 : 0));
    return result;
}
