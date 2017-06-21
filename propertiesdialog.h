#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>
#include "ui_propertiesdialog.h"
class BaseItem;
class QColor;
class PropertiesDialog : public QDialog, public Ui::PropertiesDialog
{
    Q_OBJECT
public:
    PropertiesDialog(BaseItem *baseItem, QWidget *parent = 0);

signals:

public slots:
private slots:
    void on_buttonBox_accepted();
    void on_outlineColButton_clicked();
    void on_fillColorButton_clicked();
private:
    void updateColorLabel(QLabel *label, const QColor &color);
    void chooseColor(QLabel *label, QColor *color);

    BaseItem *myItem;
    QColor outlineColor;
    QColor fillColor;
};

#endif // PROPERTIESDIALOG_H
