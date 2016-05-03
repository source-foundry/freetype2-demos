// ftinspect.h

#ifndef FTINSPECT_H_
#define FTINSPECT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSizePolicy>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QTabWidget>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>


class MainGUI;


struct Font
{
  const char* filePathname;
  int faceIndex;
  int numIndices;
  size_t fileSize;
};


class Engine
{
public:
  Engine();

  void update(const MainGUI&);

private:
  FT_Library library;
  FTC_Manager cacheManager;
  FTC_ImageCache imageCache;
  FTC_SBitCache sbitsCache;

  double pointSize;
  double pixelSize;
  int dpi;
  int zoom;

  bool doHorizontalHinting;
  bool doVerticalHinting;
  bool doBlueZoneHinting;
  bool showSegments;
  bool doWarping;

  bool showBitmap;
  bool showPoints;
  bool showPointIndices;
  bool showOutlines;

  double gamma;
};


// we want to grey out items in a combo box;
// since Qt doesn't provide a function for this we derive a class
class QComboBoxx
: public QComboBox
{
  Q_OBJECT

public:
  void setItemEnabled(int, bool);
};


class MainGUI
: public QMainWindow
{
  Q_OBJECT

public:
  MainGUI();
  ~MainGUI();

  friend class Engine;

protected:
  void closeEvent(QCloseEvent*);

private slots:
  void about();
  void checkAntiAliasing();
  void checkCurrentFontFileIndex();
  void checkHintingMode();
  void checkShowPoints();
  void checkUnits();
  void closeFont();
  void loadFonts();
  void nextFont();
  void previousFont();

private:
  QStringList fontFileNames;
  int currentFontFileIndex;

  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *closeFontAct;
  QAction *exitAct;
  QAction *loadFontsAct;

  QCheckBox *blueZoneHintingCheckBox;
  QCheckBox *horizontalHintingCheckBox;
  QCheckBox *segmentDrawingCheckBox;
  QCheckBox *showBitmapCheckBox;
  QCheckBox *showOutlinesCheckBox;
  QCheckBox *showPointIndicesCheckBox;
  QCheckBox *showPointsCheckBox;
  QCheckBox *verticalHintingCheckBox;
  QCheckBox *warpingCheckBox;

  QComboBoxx *antiAliasingComboBoxx;
  QComboBoxx *hintingModeComboBoxx;
  QComboBox *lcdFilterComboBox;
  QComboBox *unitsComboBox;

  QDoubleSpinBox *sizeDoubleSpinBox;

  QGraphicsView *glyphView;

  QHBoxLayout *antiAliasingLayout;
  QHBoxLayout *fontSizeLayout;
  QHBoxLayout *gammaLayout;
  QHBoxLayout *hintingModeLayout;
  QHBoxLayout *ftinspectLayout;
  QHBoxLayout *lcdFilterLayout;
  QHBoxLayout *navigationLayout;
  QHBoxLayout *watchLayout;

  QLabel *antiAliasingLabel;
  QLabel *dpiLabel;
  QLabel *gammaLabel;
  QLabel *hintingModeLabel;
  QLabel *lcdFilterLabel;
  QLabel *sizeLabel;
  QLabel *zoomLabel;

  QLocale *locale;

  QMenu *menuFile;
  QMenu *menuHelp;

  QPushButton *nextFontButton;
  QPushButton *previousFontButton;
  QPushButton *toEndButton;
  QPushButton *toM1000Button;
  QPushButton *toM100Button;
  QPushButton *toM10Button;
  QPushButton *toM1Button;
  QPushButton *toP1000Button;
  QPushButton *toP100Button;
  QPushButton *toP10Button;
  QPushButton *toP1Button;
  QPushButton *toStartButton;
  QPushButton *watchButton;

  QSlider *gammaSlider;

  QSpinBox *dpiSpinBox;
  QSpinBox *zoomSpinBox;

  QTabWidget *tabWidget;

  QVBoxLayout *generalTabLayout;
  QVBoxLayout *leftLayout;
  QVBoxLayout *rightLayout;

  QWidget *ftinspectWidget;
  QWidget *generalTabWidget;
  QWidget *leftWidget;
  QWidget *rightWidget;
  QWidget *mmgxTabWidget;

  enum AntiAliasing
  {
    AntiAliasing_None,
    AntiAliasing_Normal,
    AntiAliasing_Slight,
    AntiAliasing_LCD,
    AntiAliasing_LCD_BGR,
    AntiAliasing_LCD_Vertical,
    AntiAliasing_LCD_Vertical_BGR
  };
  enum HintingModes
  {
    HintingMode_TrueType_v35,
    HintingMode_TrueType_v38,
    HintingMode_TrueType_v40,
    HintingMode_CFF_FreeType,
    HintingMode_CFF_Adobe,
    HintingMode_AutoHinting
  };
  enum LCDFilter
  {
    LCDFilter_Default,
    LCDFilter_Light,
    LCDFilter_None,
    LCDFilter_Legacy
  };
  enum Units
  {
    Units_px,
    Units_pt
  };

  void createActions();
  void createConnections();
  void createLayout();
  void createMenus();
  void clearStatusBar();
  void createStatusBar();
  void readSettings();
  void setDefaults();
  void writeSettings();
};


#endif // FTINSPECT_H_


// end of ftinspect.h
