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
    QTableWidget* selectable;//查询界面下用于显示某一设备数据库中的可选参数
    QTableWidget* selected;
    QStringList codeList;//当前DeviceParam表中所有设备代号，不代表数据库中所有设备代号，之前在数据库中存在过的设备代号在查询界面ComboBox中无法显示，无法查询！！！
    QStringList paramList;//当前DeviceParam表中某一设备的所有参数
    QMultiMap<QString, QString> multiMap;//设备代号和监视参数键值对，同一个设备代号可对应多个参数，因此采用QMultiMap
    int rowCnt;//查询界面下已选参数个数
    QStringList selectedList;//查询界面下某一设备已选参数
    QStringList columnList;//数据库中某一设备表中所有列（包括时间列和参数列）

    QMap<QString, QString> paramValueMap;//具体某一设备的各参数和对应的值

    int genDataCnt;
    QList<double> genDataList;

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
    void addArrow();
    void addDoubleArrow();
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
    void sceneScaleChanged(const QString &scale);
    void sceneScaleChanged(double newScale);
    void lineWidthChanged();

    void properties();
    void textColor();
    void fillColor();
    void lineColor();

    void group();
    void ungroup();

    void insertValue();
    void startInsert();
    void stopInsert();

    void genData();

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createToolBox();

    void sleep(unsigned int msec);

    void writeLog(int iType, QString strLog);

    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QMenu *createLineMenu(const char *slot );
    QIcon createColorIcon(QColor color);
    QIcon creatTextColorIcon(QColor color);
    QIcon creatFillColorIcon(QColor color);
    QIcon creatLineColorIcon(QColor color);
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
    QToolBar *scaleToolBar;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QToolButton *lineWidthToolButton;

    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;
    QComboBox *sceneScaleCombo;
    QComboBox *lineWidthCombo;

    QListWidget* logWidget;
    int logRowCnt;

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
    QAction *addArrowAction;
    QAction *addDoubleArrowAction;
    QAction *deleteAction;
    QAction *fillAction;
    QAction *lineAction;
    QAction *lineWidthAction;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *propertiesAction;
    QAction *textColorAction;
    QAction *fillColorAction;
    QAction *lineColorAction;
    QAction *groupAction;
    QAction *ungroupAction;

    DiagramScene *myDiagramScene;
};

#endif // MAINWINDOW_H
