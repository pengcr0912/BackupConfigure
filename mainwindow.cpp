
#include <QtWidgets>
#include "mainwindow.h"
#include "diagramscene.h"
#include <QtDebug>
#include <QtGlobal>
#include "groupitem.h"
/*#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QFontComboBox>
#include <QColorDialog>
#include <QFontDialog>*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createToolBox();
    createMenus();
    createToolbars();


    myDiagramScene = new DiagramScene;
    myDiagramScene->setSceneRect(QRect(0, 0, 2000, 1500));

    connect(myDiagramScene,SIGNAL(itemSelected(QGraphicsItem*)),
            this,SLOT(itemSelected(QGraphicsItem*)));
    view = new QGraphicsView(myDiagramScene, this);
    view->setAutoFillBackground(true);
    view->setDragMode(QGraphicsView::RubberBandDrag);

//    QPalette pal = palette();
//    pal.setBrush(QPalette::Base, QPixmap(":/images/background.png"));
//    pal.setColor(QPalette::HighlightedText, Qt::red);
//    setPalette(pal);


    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(toolBox);
    layout->addWidget(view);

    layout->setStretch(0,1);
    layout->setStretch(1,4);


    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    setWindowTitle("可重构“射频-数据”一体化平台  集同管理软件");

    setBackgroundRole(QPalette::Base);
//    setCentralWidget(view);
//    resize(500, 500);
//    setCurrentFile("");
    connect(myDiagramScene, SIGNAL(selectionChanged()),
            this, SLOT(updateActions()));
    updateActions();
}

MainWindow::~MainWindow()
{

}
void MainWindow::createActions()
{
   newAction = new QAction(tr("&New"), this);
   newAction->setShortcuts(QKeySequence::New);
   newAction->setStatusTip(tr("Creat a new file"));
   newAction->setIcon(QIcon(":/images/new.png"));
   connect(newAction, SIGNAL(triggered()),
          this, SLOT(newFile()));

   openAction = new QAction(tr("&Open"), this);
   openAction->setShortcuts(QKeySequence::Open);
   openAction->setStatusTip(tr("Open an existing file"));
   openAction->setIcon(QIcon(":/images/open.png"));
   connect(openAction, SIGNAL(triggered()),
          this, SLOT(open()));

   saveAction = new QAction(tr("&Save"), this);
   saveAction->setShortcuts(QKeySequence::Save);
   saveAction->setStatusTip(tr("Save the document to disk"));
   saveAction->setIcon(QIcon(":/images/save.png"));
   connect(saveAction, SIGNAL(triggered()),
          this, SLOT(save()));

   saveAsAction = new QAction(tr("Save &As..."), this);
   saveAsAction->setShortcuts(QKeySequence::SaveAs);
   saveAsAction->setStatusTip(tr("Save the file under a new "
                                 "name"));
   connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

   for (int i = 0; i != MaxRecentFiles; ++i)
   {
       recentFileActions[i] = new QAction(this);
       recentFileActions[i]->setVisible(false);
       connect(recentFileActions[i], SIGNAL(triggered()),
               this, SLOT(openRecentFile()));
   }

   cutAction = new QAction(tr("&Cut"), this);
   cutAction->setShortcuts(QKeySequence::Cut);
   cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
   cutAction->setIcon(QIcon(":/images/cut.png"));
   connect(cutAction, SIGNAL(triggered()),
          this, SLOT(cut()));

   copyAction = new QAction(tr("&Copy"), this);
   copyAction->setShortcuts(QKeySequence::Copy);
   copyAction->setStatusTip(tr("copy the current selection's contents to the clipboard"));
   copyAction->setIcon(QIcon(":/images/copy.png"));
   connect(copyAction, SIGNAL(triggered()),
          this, SLOT(copy()));

   pasteAction = new QAction(tr("&Paste"), this);
   pasteAction->setShortcuts(QKeySequence::Paste);
   pasteAction->setStatusTip(tr(" Paste the clipboard's contents into the current "
                                "selection"));
   pasteAction->setIcon(QIcon(":/images/paste.png"));
   connect(pasteAction, SIGNAL(triggered()),
          this, SLOT(paste()));

   deleteAction = new QAction(tr("delete Item"),this);
   deleteAction->setShortcut(QKeySequence::Delete);
   deleteAction->setStatusTip(tr("Delete item from diagram"));
   deleteAction->setIcon(QIcon(":/images/deleteItem.png"));
   connect(deleteAction,SIGNAL(triggered()),
           this,SLOT(deleteItem()));

   exitAction = new QAction(tr("&Exit"), this);
   exitAction->setShortcuts(QKeySequence::Quit);
   exitAction->setStatusTip(tr("Exit the application"));
   exitAction->setIcon(QIcon(":/images/undo.png"));
   connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

   addCircleAction = new QAction(tr("actionAddCircle"),this);
   addCircleAction->setIcon(QPixmap(":/images/circle.png").scaled(30,30));
   connect(addCircleAction,SIGNAL(triggered()),
           this, SLOT(addCircle()));

   addRectangleAction = new QAction(tr("actionAddRectangle"),this);
   addRectangleAction->setIcon(QIcon(":/images/rectangle.png"));
   connect(addRectangleAction,SIGNAL(triggered()),
           this, SLOT(addRect()));

   addTriangleAction = new QAction(tr("actionAddTriangle"),this);//no signal and slot, no use
   addTriangleAction->setIcon(QIcon(":/images/triangle.png"));

   addLineAction = new QAction(tr("actionAddLine"),this);
   addLineAction->setIcon(QIcon(":/images/line.png"));
   connect(addLineAction,SIGNAL(triggered()),
           this, SLOT(addLine()));

   addTextAction = new QAction(tr("actionAddText"),this);
   addTextAction->setIcon(QIcon(":/images/text.png"));
   connect(addTextAction,SIGNAL(triggered()),
           this,SLOT(addText()));

   toFrontAction = new QAction(tr("Bring to &Front"), this);
   toFrontAction->setShortcut(tr("Ctrl+F"));
   toFrontAction->setStatusTip(tr("Bring item to front"));
   toFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    connect(toFrontAction, SIGNAL(triggered()),
          this, SLOT(bringToFront()));

   sendBackAction = new QAction(tr("send item to back"),this);
   sendBackAction->setIcon(QIcon(":/images/sendtoback.png"));
   connect(sendBackAction,SIGNAL(triggered()),
           this,SLOT(sendToBack()));

   boldAction = new QAction("Bold",this);
   boldAction->setShortcut(QKeySequence::Bold);
   boldAction->setCheckable(true);//使其具有on和off两种状态
   boldAction->setIcon(QIcon(":/images/bold.png"));
   connect(boldAction,SIGNAL(triggered()),
           this,SLOT(handleFontChange()));

   underlineAction = new QAction(tr("Underline"),this);
   underlineAction->setShortcut(QKeySequence::Underline);
   underlineAction->setCheckable(true);//使其具有on和off两种状态
   underlineAction->setIcon(QIcon(":/images/underline.png"));
   connect(underlineAction,SIGNAL(triggered()),
           this,SLOT(handleFontChange()));

   italicAction = new QAction(tr("Italic"),this);
   italicAction->setShortcut(QKeySequence::Italic);
   italicAction->setCheckable(true);//使其具有on和off两种状态
   italicAction->setIcon(QIcon(":/images/italic.png"));
   connect(italicAction,SIGNAL(triggered()),
           this,SLOT(handleFontChange()));

   propertiesAction = new QAction(tr("properties"),this);//no use
   connect(propertiesAction,SIGNAL(triggered()),
           this,SLOT(properties()));

   textColorAction = new QAction(tr("textcolor"),this);
   textColorAction->setIcon(QIcon(creatTextColorIcon(Qt::black)));
   connect(textColorAction,SIGNAL(triggered()),
           this,SLOT(textColor()));

   groupAction = new QAction(tr("group"),this);
   connect(groupAction,SIGNAL(triggered()),
           this,SLOT(group()));

   ungroupAction = new QAction(tr("ungroup"),this);
   connect(ungroupAction, SIGNAL(triggered()),
           this, SLOT(ungroup()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    editMenu->addAction(propertiesAction);
    editMenu->addSeparator();

    editMenu->addAction(toFrontAction);
    editMenu->addAction(sendBackAction);
    editMenu->addSeparator();

    editMenu->addAction(addRectangleAction);
    editMenu->addAction(addCircleAction);
    editMenu->addAction(addTriangleAction);
    editMenu->addAction(addTextAction);

    viewMenu = menuBar()->addMenu(tr("&View"));

    toolMenu = menuBar()->addMenu(tr("&Tool"));

    helpMenu = menuBar()->addMenu(tr("&Help"));


}
void MainWindow::createToolbars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(groupAction);
    editToolBar->addAction(ungroupAction);

    ItemToolBar = addToolBar(tr("shape"));
    ItemToolBar->addAction(addRectangleAction);
    ItemToolBar->addAction(addCircleAction);
    ItemToolBar->addAction(addTriangleAction);
    ItemToolBar->addAction(addLineAction);
    ItemToolBar->addAction(addTextAction);

    ItemToolBar->setOrientation(Qt::Vertical);
    addToolBar(Qt::RightToolBarArea, ItemToolBar);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i != 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *fontSizeValidator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(fontSizeValidator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));


    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->addAction(textColorAction);

    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(fillColorChanged()),
                         Qt::black));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
    ":/images/floodfill.png", Qt::black));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillColorButtonTriggered()));

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(outlineColorChanged()),
                                                 Qt::blue));
    lineColorAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
    ":/images/linecolor.png", Qt::blue));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(outlineColorButtonTriggered()));

    lineWidthToolButton =  new QToolButton;
    lineWidthToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineWidthToolButton->setMenu(createLineMenu(SLOT(outlineWidthChanged())));
    lineWidthAction = lineWidthToolButton->menu()->defaultAction();
    lineWidthToolButton->setIcon(createLineToolButtonIcon());
    connect(lineWidthToolButton, SIGNAL(clicked()),
            this, SLOT(outlineWidthButtonTriggered()));

    QToolButton *pointerToolButton = new QToolButton;
    pointerToolButton->setCheckable(true);
    pointerToolButton->setChecked(true);
    pointerToolButton->setIcon(QIcon(":/images/pointer.png"));
    connect(pointerToolButton, SIGNAL(clicked()),
            this, SLOT(pointerButtonClicked()));

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);
    colorToolBar->addWidget(lineWidthToolButton);
    colorToolBar->addWidget(pointerToolButton);


}

QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu;
    for (int i = 0; i < colors.count(); ++i)
    {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
        {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
QIcon MainWindow::creatTextColorIcon(QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(":/images/text");
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);
}
QIcon MainWindow::createLineIcon(const int lineWidth)
{
    QPixmap pixmap(320,80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(10,15,300,2+lineWidth),Qt::black);
    return QIcon(pixmap);
}
void MainWindow::fillColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                 ":/images/floodfill.png",
                 //qVariantValue<QColor>(fillAction->data())));
                 fillAction->data().value<QColor>()));
    fillColorButtonTriggered();
}
void MainWindow::outlineColorChanged()
{
    lineColorAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                 ":/images/linecolor.png",
                 //qVariantValue<QColor>(lineColorAction->data())));
                 //lineColorAction->data().value<QColor>()));
                 Qt::red));
    outlineColorButtonTriggered();
}

QMenu *MainWindow::createLineMenu(const char *slot )
{
    QList<int> lineWideList;
    lineWideList<< 0 << 1 << 3 << 5 << 9 << 13 << 17;
    QMenu *lineMenu = new QMenu;

    for(int i = 0; i != lineWideList.count(); ++i)
    {
        QAction *action = new QAction(this);
        action->setData(int(lineWideList.at(i)));
        action->setIcon(createLineIcon(lineWideList.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);//此处的slot是lineWidthChanged()
        lineMenu->addAction(action);
        if(lineWideList.at(i) == 1)
        {
             lineMenu->setDefaultAction(action);
        }
    }
    return lineMenu;
}
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile,
                        QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QIcon MainWindow::createLineToolButtonIcon()
{
    QPixmap pixmap(80,80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(10,15,60,2),Qt::black);
    painter.fillRect(QRect(10,35,60,6),Qt::black);
    painter.fillRect(QRect(10,55,60,12),Qt::black);
    return QIcon(pixmap);
}
void MainWindow::outlineWidthChanged()
{
    lineWidthAction = qobject_cast<QAction *>(sender());
    outlineWidthButtonTriggered();
}
void MainWindow::fillColorButtonTriggered()
{
    //myDiagramScene->setItemColor(qVariantValue<QColor>(fillAction->data()));
    myDiagramScene->setItemColor(fillAction->data().value<QColor>());
}
void MainWindow::outlineColorButtonTriggered()
{
    //myDiagramScene->setLineColor(qVariantValue<QColor>(lineColorAction->data()));
    myDiagramScene->setLineColor(lineColorAction->data().value<QColor>());
}
void MainWindow::outlineWidthButtonTriggered()
{
    //myDiagramScene->setLineWidth(qVariantValue<int>(lineWidthAction->data()));
    myDiagramScene->setLineWidth(lineWidthAction->data().value<int>());
}
void MainWindow::pointerButtonClicked()
{
    myDiagramScene->setMode(DiagramScene::MoveItem);
}
void MainWindow::bringToFront()
{
    if (myDiagramScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = myDiagramScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems)
    {
        if (item && (item->zValue() >= zValue))
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
void MainWindow::sendToBack()
{
    if (myDiagramScene->selectedItems().isEmpty())
        return;
    QGraphicsItem *selectedItem = myDiagramScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems)
    {
        if (item && (item->zValue() <= zValue))
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::newFile()
{
    if (okToContinue())
    {
        myDiagramScene->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Configure"), ".",
                                   tr("Configure files (*.cfg)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
void MainWindow::openRecentFile()
{
    if (okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}
bool MainWindow::okToContinue()
{
    if (isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("Configure"),
                        tr("The Configure has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}
bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Configure"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    if (!myDiagramScene->readFile(file))
    {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}
bool MainWindow::save()
{
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}
bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                    ".",tr("configue files(*.cfg)"));
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Configure"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    if(!myDiagramScene->writeFile(file))
    {
        statusBar()->showMessage(tr("Saving cancled"),2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"),2000);
    return true;

}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty())
    {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*]   %2").arg(shownName)
                                   .arg(tr("可重构“射频-数据”一体化平台  集同管理软件")));

}
void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);//QMutableListIterator<QString>
    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j)
    {
        if (j < recentFiles.count())
        {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
        {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::cut()
{
    QList<QGraphicsItem *> items = myDiagramScene->selectedItems();
    if(items.count() >= 1)
    {
        copy();
        myDiagramScene->clearSelection();
    }
    qDeleteAll(items);

}
void MainWindow::copy()
{
    QList<QGraphicsItem *> items = myDiagramScene->selectedItems();
    QString str;
    for(int i = 0; i != items.count(); i++)
    {
        if(i > 0)
            str += "\n";
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(items.at(i)))
        {
            str += QString("item %1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
                    .arg(QString::number(baseItem->type()))
                    .arg(QString::number(baseItem->rect().topLeft().x()))
                    .arg(QString::number(baseItem->rect().topLeft().y()))
                    .arg(QString::number(baseItem->rect().width()))
                    .arg(QString::number(baseItem->rect().height()))
                    .arg(baseItem->pen().color().name())
                    .arg(QString::number(baseItem->pen().width()))
                    .arg(baseItem->brush().color().name())
                    .arg(QString::number(baseItem->pos().x()))
                    .arg(QString::number(baseItem->pos().y()));
        }

    }
    QApplication::clipboard()->setText(str);

}
void MainWindow::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList strList = str.split('\n');
    for(int i = 0; i != strList.count(); i++)
    {
        QStringList itemStr = strList.at(i).split(' ');
        if (itemStr.count() >= 9 && itemStr.first() == "item")
        {
            switch(itemStr.at(1).toInt())
            {
            case QGraphicsItem::UserType + 3:
            {
                RectItem *rect = new RectItem;
                if(rect)
                {
                    rect->setRect(QRectF(QPointF(itemStr.at(2).toDouble(),itemStr.at(3).toDouble())
                                        ,QSize(itemStr.at(4).toDouble(), itemStr.at(5).toDouble())));
                    rect->setPen(QColor(itemStr.at(6)));
                    rect->setPen(int(itemStr.at(7).toInt()));
                    rect->setBrush(QBrush(QColor(itemStr.at(8))));
                    rect->setPos(itemStr.at(9).toDouble()+10.0,itemStr.at(10).toDouble()+10.0);
                    myDiagramScene->addItem(rect);
                }
                break;
            }
            case QGraphicsItem::UserType + 4:
            {
                EllipseItem *circle = new EllipseItem;
                if(circle)
                {
                    circle->setRect(QRectF(QPointF(itemStr.at(2).toDouble(),itemStr.at(3).toDouble())
                                        ,QSize(itemStr.at(4).toDouble(), itemStr.at(5).toDouble())));
                    circle->setPen(QColor(itemStr.at(6)));
                    circle->setPen(int(itemStr.at(7).toInt()));
                    circle->setBrush(QBrush(QColor(itemStr.at(8))));
                    circle->setPos(itemStr.at(9).toDouble()+10.0,itemStr.at(10).toDouble()+10.0);
                    myDiagramScene->addItem(circle);
                }
                break;
            }
            default:
                    ;
            }
        }
    }
}
void MainWindow::addCircle()
{
//设置当前的模式为circle，并且为item插入模式，这时场景中的mousePressEvent根据模式进行选择是否插入以及插入什么item
    myDiagramScene->setMode(DiagramScene::InsertCircleItem);
//    diagramScene->setItemType(DiagramItem::Circle);
//   QUndoCommand *addCommand = new AddCommand(DiagramItem::Circle, diagramScene);
//   undoStack->push(addCommand);
}
void MainWindow::addRect()
{
    myDiagramScene->setMode(DiagramScene::InsertRectItem);
}
void MainWindow::addLine()
{
    myDiagramScene->setMode(DiagramScene::InsertLineItem);
}
void MainWindow::addText()
{
    myDiagramScene->setMode(DiagramScene::InsertTextItem);
}
void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> items = myDiagramScene->selectedItems();
    foreach (QGraphicsItem *item, items)
    {
        if(item)
            myDiagramScene->removeItem(item);//并没有delete
    }
    qDeleteAll(items);
}
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();//不包含路径
}

void MainWindow::updateActions()
{
    bool hasSelection = !(myDiagramScene->selectedItems().isEmpty());
    cutAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);
    deleteAction->setEnabled(hasSelection);
    toFrontAction->setEnabled(hasSelection);
    sendBackAction->setEnabled(hasSelection);
    lineColorAction->setEnabled(hasSelection);
    lineWidthAction->setEnabled(hasSelection);
    fillAction->setEnabled(hasSelection);
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
    TextItem *textItem = qgraphicsitem_cast<TextItem *>(item);
    if(textItem)
    {
        QFont font = textItem->font();
        QColor color = textItem->defaultTextColor();//保留

        fontCombo->setCurrentFont(font);
        fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
        boldAction->setChecked(font.weight() == QFont::Bold);
        italicAction->setChecked(font.italic());
        underlineAction->setChecked(font.underline());
    }
}
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    myDiagramScene->setFont(font);
}
void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}
void MainWindow::properties()
{
    QList<QGraphicsItem *>items = myDiagramScene->selectedItems();
    if(items.count() > 1)
        return ;
    QGraphicsItem *item = items.first();
    if (item)
    {
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
        {
            PropertiesDialog dialog(baseItem, this);
            dialog.exec();
        }
        else if (TextItem *textItem = dynamic_cast<TextItem *>(item))
        {
            bool ok;
            QFont font = QFontDialog::getFont(&ok,textItem->font(),this);
            if(ok)
            {
                textItem->setFont(font);
            }
        }
    }
}
void MainWindow::textColor()
{
    QColor col = QColorDialog::getColor(Qt::black,this);
    myDiagramScene->setTextColor(col);
    textColorAction->setIcon(creatTextColorIcon(col));

}
void MainWindow::group()
{
    QList<QGraphicsItem *> oldItems = myDiagramScene->selectedItems();
    if(oldItems.size() < 2)
        return ;
    foreach (QGraphicsItem *item, oldItems)
    {
        if(item->type() == GroupItem::Type)
        {
           if(GroupItem *oldGroup = dynamic_cast<GroupItem *>(item))
           {
               myDiagramScene->myDestroyItemGroup(oldGroup);
           }
        }
    }
    QList<QGraphicsItem *> newItems = myDiagramScene->selectedItems();
    myDiagramScene->myCreatItemGroup(newItems);


}

void MainWindow::ungroup()
{
    QList<QGraphicsItem *> items = myDiagramScene->selectedItems();
    if(items.empty())
        return ;
    foreach (QGraphicsItem *item, items)
    {
        if(item->type() == GroupItem::Type)
        {
           if(GroupItem *group = dynamic_cast<GroupItem *>(item))
           {
               myDiagramScene->myDestroyItemGroup(group);
           }
        }
    }
}

void MainWindow::createToolBox()
{
/*    buttonGroup = new QButtonGroup;
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;*/
/*
    layout->addWidget(createCellWidget(tr("Conditional"),
                               DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"),
                      DiagramItem::Step),0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"),
                      DiagramItem::Io), 1, 0);
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")
                        .scaled(30, 30)));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);
*/
    QWidget *itemWidget = new QWidget;
//    itemWidget->setLayout(layout);

/*    backgroundButtonGroup = new QButtonGroup;
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton *)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton *)));

    QGridLayout *backgroundLayout = new QGridLayout;*/
/*    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                ":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                    ":/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                ":/images/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);
*/
    QWidget *backgroundWidget = new QWidget;
//    backgroundWidget->setLayout(backgroundLayout);


    QWidget *sqlWidget = new QWidget;
    QWidget *paramWidget = new QWidget;


//! [22]
    toolBox = new QToolBox;
//    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
//    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("设备监控"));
    toolBox->addItem(backgroundWidget, tr("知识管理"));
    toolBox->addItem(sqlWidget, tr("数据查询"));
    toolBox->addItem(paramWidget, tr("参数设置"));
}
