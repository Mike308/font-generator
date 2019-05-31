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
    font.setPointSize(12);
    ui->previewField->setFont(font);
    ui->previewField->clear();
    ui->previewField->appendPlainText("Font test");
}


Font MainWindow::prepareFont(const QString &asciiChar){
    QFont font;
    font.setPointSize(ui->fontSizeSpinner->value());
    font.setFamily(ui->fontComboBox->currentText());
    font.setBold(ui->boldFont->isChecked());
    font.setItalic(ui->italicFont->isChecked());
    font.setUnderline(ui->underLine->isChecked());

    QSize size;
    QFontMetrics fm(font);
    qDebug () << "Font: " <<font.family() << " Size: " << fm.height()
              << "Ascent: " << fm.ascent() << "Descent: " << fm.descent()
              << "Leading: " << fm.leading() << "wide: " << fm.width(asciiChar)
              << "Max Width: " << fm.maxWidth() << "Cap height: " << fm.capHeight();
    QImage image(fm.width("W"), fm.ascent() + fm.leading(), QImage::Format_Mono);
    image.fill(0);
    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.setFont(font);
    size.setWidth(fm.width("W"));
    size.setHeight(fm.ascent() + fm.leading());
    painter.drawText((image.width() - fm.width(asciiChar))/2, (fm.ascent() + fm.leading()), asciiChar);
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
    for (int r = 0; r < size.height(); r++) {
        for (int c = 0; c < size.width(); c++) {
            bin.append(QString::number(mainArray[r][c]));
        }
        list.append(extraZeros(bin));
        bin = "";
    }


    for (int r = 0; r < size.height(); r++) {
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
    fonts.setAscent(fm.ascent() + fm.leading());
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
            fonts.append(QString(" //").append(QString(char(i))).append("(").append(QString::number(calculateOffset(font.getAscent(), font.getWidth(), char(i)))).append(")"));
            fonts.append(font.getFonts());
        }

    }

    ui->outputField->clear();
    ui->outputField->appendPlainText("#include ""font.h""");
    ui->outputField->appendPlainText("const uint8_t " +  ui->fontComboBox->currentText().replace(" ","_").toLower() + "_" + QString::number(ui->fontSizeSpinner->value())
                                     + "_table[]" + "={ ");
    foreach (QString item, fonts) {
        ui->outputField->appendPlainText(item);
    }
    ui->outputField->appendPlainText("};");
    ui->outputField->appendPlainText("font_t " + ui->fontComboBox->currentText().replace(" ","_").toLower() + "_" + QString::number(ui->fontSizeSpinner->value()) + " = {");
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
    uint32_t decValue = std::stoi(binValue.toStdString(), nullptr, 2);
    qDebug () << "Bin lenght: " << binValue.length();
    if (binValue.length() > 8 && binValue.length() < 17){
        uint8_t high = decValue >> 8;
        uint8_t low = decValue;
        return "0x" + QString::number(high, 16).append(",").append("0x").append(QString::number(low, 16));

    }else if (binValue.length() > 16 && binValue.length() < 25){

        return "0x" + QString::number((uint8_t)(decValue >> 16), 16).append(",0x").append(QString::number((uint8_t)(decValue >> 8), 16)).append(",0x").append(QString::number((uint8_t)decValue, 16));
    }
    else if (binValue.length() > 24 && binValue.length() <= 32) {
        return "0x" + QString::number((uint8_t)(decValue >> 24), 16).append(",0x").append(QString::number((uint8_t)(decValue >> 16), 16)).append(",0x")
                .append(QString::number((uint8_t)(decValue >> 8), 16)).append(",0x").append(QString::number((uint8_t)(decValue), 16));
    }
    else if (binValue.length() > 32) {
        return "0x" + QString::number((uint8_t)(decValue >> 32), 16).append(",0x").append(QString::number((uint8_t)(decValue >> 24), 16)).append(",0x")
                .append(QString::number((uint8_t)(decValue >> 16), 16)).append(",0x").append(QString::number((uint8_t)(decValue >> 8), 16)).append(",0x")
                .append(QString::number((uint8_t)decValue));
    }
    else{
        return "0x" + QString::number(decValue, 16);
    }
}

void MainWindow::on_fontSizeSpinner_valueChanged(int arg1)
{
    QFont font;
    font.setPixelSize(ui->fontSizeSpinner->value());
    font.setFamily(ui->fontComboBox->currentText());
    font.setBold(ui->boldFont->isChecked());
    font.setItalic(ui->italicFont->isChecked());
    font.setUnderline(ui->underLine->isChecked());
    font.setFamily(ui->fontComboBox->currentText());
    font.setPixelSize(arg1);
    QFontMetrics fontMetrics(font);
    ui->previewField->setFont(font);
    ui->ascentValue->setText(QString("Asccent: ").append(QString::number(fontMetrics.ascent())));
    ui->widthValue->setText(QString("Width: ").append(QString::number(fontMetrics.width("W"))));
}

uint32_t MainWindow::calculateOffset(int height, int width, char asciiChar){
    uint32_t result = (asciiChar - ' ') * height * (width / 8 + (width % 8 ? 1 : 0));
    return result;
}

void MainWindow::on_boldFont_stateChanged(int arg1) {
    QFont font;
    font.setPixelSize(ui->fontSizeSpinner->value());
    font.setFamily(ui->fontComboBox->currentText());
    font.setBold(ui->boldFont->isChecked());
    font.setItalic(ui->italicFont->isChecked());
    font.setUnderline(ui->underLine->isChecked());
    font.setFamily(ui->fontComboBox->currentText());
    font.setPixelSize(ui->fontSizeSpinner->value());
    QFontMetrics fontMetrics(font);
    ui->previewField->setFont(font);
    ui->widthValue->setText(QString("Width: ").append(QString::number(fontMetrics.width("W"))));
    ui->ascentValue->setText(QString("Asccent: ").append(QString::number(fontMetrics.ascent())));
}

void MainWindow::on_italicFont_stateChanged(int arg1)
{
    QFont font;
    font.setPointSize(ui->fontSizeSpinner->value());
    font.setFamily(ui->fontComboBox->currentText());
    font.setBold(ui->boldFont->isChecked());
    font.setItalic(ui->italicFont->isChecked());
    font.setUnderline(ui->underLine->isChecked());
    font.setFamily(ui->fontComboBox->currentText());
    font.setPixelSize(ui->fontSizeSpinner->value());
    QFontMetrics fontMetrics(font);
    ui->previewField->setFont(font);
    ui->widthValue->setText(QString("Width: ").append(QString::number(fontMetrics.width("W"))));
    ui->ascentValue->setText(QString("Asccent: ").append(QString::number(fontMetrics.ascent())));
}

void MainWindow::on_underLine_stateChanged(int arg1)
{
    QFont font;
    font.setPixelSize(ui->fontSizeSpinner->value());
    font.setFamily(ui->fontComboBox->currentText());
    font.setBold(ui->boldFont->isChecked());
    font.setItalic(ui->italicFont->isChecked());
    font.setUnderline(ui->underLine->isChecked());
    font.setFamily(ui->fontComboBox->currentText());
    font.setPixelSize(ui->fontSizeSpinner->value());
    QFontMetrics fontMetrics(font);
    ui->previewField->setFont(font);
    ui->widthValue->setText(QString("Width: ").append(QString::number(fontMetrics.width("W"))));
    ui->ascentValue->setText(QString("Asccent: ").append(QString::number(fontMetrics.ascent())));
}

void MainWindow::on_calcOffset_clicked()
{
    int height = ui->spinHeight->value();
    int width = ui->spinWidth->value();
    QString asciiStr = ui->lineEdit->text();
    qDebug () << "Ascii code: " << (int) asciiStr.at(0).toLatin1();
    ui->offsetValue->setText(QString::number(calculateOffset(height, width, asciiStr.at(0).toLatin1())));
}
