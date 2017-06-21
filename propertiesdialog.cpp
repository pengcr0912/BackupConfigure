#include "propertiesdialog.h"
#include "baseitem.h"
#include <QColorDialog>

PropertiesDialog::PropertiesDialog(BaseItem *baseItem, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    myItem = baseItem;
    widthSpinBox->setValue(int(baseItem->rect().width()));
    heightSpinBox->setValue(int(baseItem->rect().height()));
    outlineColor = baseItem->pen().color();
    fillColor = baseItem->brush().color();

    updateColorLabel(outlineColLabel, outlineColor);
    updateColorLabel(fillColorLabel, fillColor);
}
void PropertiesDialog::on_buttonBox_accepted()
{
    QRectF rect = QRectF(myItem->rect().topLeft(),
                         QSize(widthSpinBox->value(),heightSpinBox->value()));
    myItem->setRect(rect);

    myItem->setPen(outlineColor);
    myItem->setBrush(fillColor);
    myItem->update();
    QDialog::accept();
}

void PropertiesDialog::on_outlineColButton_clicked()
{
    chooseColor(outlineColLabel, &outlineColor);
}

void PropertiesDialog::on_fillColorButton_clicked()
{
    chooseColor(fillColorLabel, &fillColor);
}

void PropertiesDialog::updateColorLabel(QLabel *label,const QColor &color)
{
    QPixmap pixmap(80, 20);
    pixmap.fill(color);
    label->setPixmap(pixmap);
}

void PropertiesDialog::chooseColor(QLabel *label, QColor *color)
{
    QColor newColor = QColorDialog::getColor(*color, this);
    if (newColor.isValid())
    {
        *color = newColor;
        updateColorLabel(label, *color);
    }
}
