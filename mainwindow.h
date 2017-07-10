#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diagramscene.h"
#include "propertiesdialog.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QDatetime>
#include <QTimer>
#include <QtSql/QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "queryresult.h"
//#include "deviceinfo.h"

//#pragma execution_character_set("utf-8");

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QToolBar;
class QUndoStack;
class QFontComboBox;
class QFont;
class QToolBox;
QT_END_NAMESPACE
class DiagramScene;
class BaseItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsView *view;

    QTimer *timer;
    QDateTimeEdit* timeStart;
    QDateTimeEdit* timeEnd;
    QComboBox* comboBox_style;
    QComboBox* comboBox_code;
    QTableWidget* selectable;
    QTableWidget* selected;
    QStringList codeList;
    QStringList paramList;
    QMultiMap<QString, QString> multiMap;
    int rowCnt;
    QStringList selectedList;

//    DeviceInfo* deviceInfo;

protected:
//    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void bringToFront();
    void sendToBack();
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void cut();
    void copy();
    void paste();
    void addCircle();
    void addRect();
    void addLine();
    void addText();
    void addPix();
    void deleteItem();
    void updateActions();
    QString strippedName(const QString &fullFileName);
    void itemSelected(QGraphicsItem *item);
    void handleFontChange();

    void startQuery();
    void styleChanged(const QString& style);
    void codeChanged(const QString& code);
    void addParam(int i, int j);
    void deleteParam(int i, int j);
private slots:
    void fillColorChanged();
    void fillColorButtonTriggered();

    void outlineColorChanged();
    void outlineColorButtonTriggered();

    void outlineWidthChanged();
    void outlineWidthButtonTriggered();

    void pointerButtonClicked();

    void openRecentFile();
    void updateRecentFileActions();

    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);

    void properties();
    void textColor();

    void group();
    void ungroup();

    void insertLog();
    void startInsert();
    void stopInsert();

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createToolBox();

    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QMenu *createLineMenu(const char *slot );
    QIcon createColorIcon(QColor color);
    QIcon creatTextColorIcon(QColor color);
    QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);
    QIcon createLineToolButtonIcon();
    QIcon createLineIcon(const int width);

    bool okToContinue();
    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *arrangeMenu;
    QMenu *tableMenu;
    QMenu *toolMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *backgroundButtonGroup;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *ItemToolBar;
    QToolBar *colorToolBar;
    QToolBar *textToolBar;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QToolButton *lineWidthToolButton;

    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QStringList recentFiles;
    QString curFile;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;


    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *exitAction;
    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *addCircleAction;
    QAction *addRectangleAction;
    QAction *addTriangleAction;
    QAction *addLineAction;
    QAction *addTextAction;
    QAction *addPixAction;
    QAction *deleteAction;
    QAction *fillAction;
    QAction *lineColorAction;
    QAction *lineWidthAction;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *propertiesAction;
    QAction *textColorAction;
    QAction *groupAction;
    QAction *ungroupAction;

    DiagramScene *myDiagramScene;
};

#endif // MAINWINDOW_H
