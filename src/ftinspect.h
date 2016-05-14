// ftinspect.h

#ifndef FTINSPECT_H_
#define FTINSPECT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_CFF_DRIVER_H
#include FT_LCD_FILTER_H
#include FT_MODULE_H
#include FT_OUTLINE_H
#include FT_TRUETYPE_DRIVER_H

// internal FreeType header files; only available in the source code bundle
#include FT_INTERNAL_DRIVER_H
#include FT_INTERNAL_OBJECTS_H

#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColor>
#include <QComboBox>
#include <QDateTime>
#include <QDesktopWidget>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHash>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainterPath>
#include <QPen>
#include <QPushButton>
#include <QSettings>
#include <QSignalMapper>
#include <QSizePolicy>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QTabWidget>
#include <QTransform>
#include <QVariant>
#include <QVector2D>
#include <QVBoxLayout>
#include <QWidget>


class MainGUI;


// A structure to hold a physical font.
//
// A valid font contains one or more multiple faces.
// A valid face contains one or more instances.
// A valid instance gets assigned an entry in MainGUI's `faceIDHash'.
//
// An invalid font is marked as having one face but zero instances.
// An invalid face is marked as having -1 instances.

struct Font
{
  QString filePathname;

  // the number of instances per face;
  // the size of the list gives the number of faces
  QList<int> numberOfNamedInstancesList;
};


// This structure is used to map the (font,face,instance) index triplet to
// abstract IDs (generated by a running number stored in MainGUI's
// `faceCounter' member).
//
// Qt's `QHash' class needs an implementation of `==' and a global,
// overloaded `qHash' function.

struct FaceID
{
  int fontIndex;
  int faceIndex;
  int namedInstanceIndex;

  FaceID();
  FaceID(int, int, int);
  bool operator==(const FaceID& other) const;
};

uint qHash(FaceID key);


// FreeType specific data.

class Engine
{
public:
  Engine(MainGUI*);
  ~Engine();

  const QString& currentFamilyName();
  const QString& currentStyleName();
  QString glyphName(int);
  int numberOfFaces(int);
  int numberOfNamedInstances(int, int);
  int loadFont(int, int, int); // returns number of glyphs
  FT_Outline* loadOutline(int);
  void removeFont(int, int, int);
  void setCFFHintingMode(int);
  void setTTInterpreterVersion(int);
  void update();

  friend class MainGUI;

  // XXX cover all available modules
  enum FontType
  {
    FontType_CFF,
    FontType_TrueType,
    FontType_Other
  };

private:
  MainGUI* gui;

  QString curFamilyName;
  QString curStyleName;

  FT_Library library;
  FTC_Manager cacheManager;
  FTC_ImageCache imageCache;
  FTC_SBitCache sbitsCache;

  FTC_ScalerRec scaler;
  FT_Size ftSize;

  int cffHintingEngineDefault;
  int cffHintingEngineOther;

  int ttInterpreterVersionDefault;
  int ttInterpreterVersionOther;
  int ttInterpreterVersionOther1;

  int fontType;

  int haveWarping;

  double pointSize;
  double pixelSize;
  int dpi;

  bool doHinting;
  bool doAutoHinting;
  bool doHorizontalHinting;
  bool doVerticalHinting;
  bool doBlueZoneHinting;
  bool showSegments;
  bool doWarping;

  double gamma;

  int loadFlags;
};


class Grid
: public QGraphicsItem
{
public:
  Grid(const QPen&,
       const QPen&);
  QRectF boundingRect() const;
  void paint(QPainter*,
             const QStyleOptionGraphicsItem*,
             QWidget*);

private:
  QPen gridPen;
  QPen axisPen;
};


class GlyphOutline
: public QGraphicsItem
{
public:
  GlyphOutline(const QPen&,
               FT_Outline*);
  QRectF boundingRect() const;
  void paint(QPainter*,
             const QStyleOptionGraphicsItem*,
             QWidget*);

private:
  QPen outlinePen;
  FT_Outline* outline;
  QRectF bRect;
};


class GlyphPoints
: public QGraphicsItem
{
public:
  GlyphPoints(const QPen&,
              const QPen&,
              FT_Outline*);
  QRectF boundingRect() const;
  void paint(QPainter*,
             const QStyleOptionGraphicsItem*,
             QWidget*);

private:
  QPen onPen;
  QPen offPen;
  FT_Outline* outline;
  QRectF bRect;
};


class GlyphPointNumbers
: public QGraphicsItem
{
public:
  GlyphPointNumbers(const QPen&,
                    const QPen&,
                    FT_Outline*);
  QRectF boundingRect() const;
  void paint(QPainter*,
             const QStyleOptionGraphicsItem*,
             QWidget*);

private:
  QPen onPen;
  QPen offPen;
  FT_Outline* outline;
  QRectF bRect;
};


class GlyphBitmap
: public QGraphicsItem
{
public:
  GlyphBitmap(FT_Outline*,
              FT_Library,
              int,
              const QVector<QRgb>&,
              const QVector<QRgb>&);
  ~GlyphBitmap();
  QRectF boundingRect() const;
  void paint(QPainter*,
             const QStyleOptionGraphicsItem*,
             QWidget*);

private:
  FT_Outline transformed;
  FT_Library library;
  int pixelMode;
  const QVector<QRgb>& monoColorTable;
  const QVector<QRgb>& grayColorTable;
  QRectF bRect;
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


// we want buttons that are horizontally as small as possible
class QPushButtonx
: public QPushButton
{
  Q_OBJECT

public:
  QPushButtonx(const QString&, QWidget* = 0);
  virtual ~QPushButtonx(){}
};


class MainGUI
: public QMainWindow
{
  Q_OBJECT

public:
  MainGUI();
  ~MainGUI();

  void setDefaults();
  void update(Engine*);

  friend class Engine;
  friend FT_Error faceRequester(FTC_FaceID,
                                FT_Library,
                                FT_Pointer,
                                FT_Face*);

protected:
  void closeEvent(QCloseEvent*);

private slots:
  void about();
  void aboutQt();
  void adjustGlyphIndex(int);
  void checkAntiAliasing();
  void checkAutoHinting();
  void checkCurrentFaceIndex();
  void checkCurrentFontIndex();
  void checkCurrentInstanceIndex();
  void checkHinting();
  void checkHintingMode();
  void checkLcdFilter();
  void checkShowPoints();
  void checkUnits();
  void closeFont();
  void drawGlyph();
  void loadFonts();
  void nextFace();
  void nextFont();
  void nextInstance();
  void previousFace();
  void previousFont();
  void previousInstance();
  void zoom();

private:
  Engine* engine;

  QList<Font> fontList;
  int currentFontIndex;
  int currentFaceIndex;
  int currentNamedInstanceIndex;

  int currentNumberOfGlyphs;
  int currentGlyphIndex;

  int faceCounter; // a running number used to initialize `faceIDHash'
  QHash<FaceID, int> faceIDHash;

  int currentCFFHintingMode;
  int currentTTInterpreterVersion;

  // layout related stuff
  GlyphOutline *currentGlyphOutlineItem;
  GlyphPoints *currentGlyphPointsItem;
  GlyphPointNumbers *currentGlyphPointNumbersItem;
  GlyphBitmap *currentGlyphBitmapItem;

  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *closeFontAct;
  QAction *exitAct;
  QAction *loadFontsAct;

  QCheckBox *autoHintingCheckBox;
  QCheckBox *blueZoneHintingCheckBox;
  QCheckBox *hintingCheckBox;
  QCheckBox *horizontalHintingCheckBox;
  QCheckBox *segmentDrawingCheckBox;
  QCheckBox *showBitmapCheckBox;
  QCheckBox *showOutlinesCheckBox;
  QCheckBox *showPointNumbersCheckBox;
  QCheckBox *showPointsCheckBox;
  QCheckBox *verticalHintingCheckBox;
  QCheckBox *warpingCheckBox;

  QComboBoxx *antiAliasingComboBoxx;
  QComboBoxx *hintingModeComboBoxx;
  QComboBox *lcdFilterComboBox;
  QComboBox *unitsComboBox;

  QDoubleSpinBox *sizeDoubleSpinBox;

  QGraphicsScene *glyphScene;
  QGraphicsView *glyphView;

  QGridLayout *fontLayout;
  QGridLayout *infoRightLayout;

  QHash<int, int> hintingModesTrueTypeHash;
  QHash<int, int> hintingModesCFFHash;
  QHash<FT_LcdFilter, int> lcdFilterHash;

  QHBoxLayout *antiAliasingLayout;
  QHBoxLayout *blueZoneHintingLayout;
  QHBoxLayout *ftinspectLayout;
  QHBoxLayout *gammaLayout;
  QHBoxLayout *hintingModeLayout;
  QHBoxLayout *horizontalHintingLayout;
  QHBoxLayout *infoLeftLayout;
  QHBoxLayout *lcdFilterLayout;
  QHBoxLayout *navigationLayout;
  QHBoxLayout *pointNumbersLayout;
  QHBoxLayout *segmentDrawingLayout;
  QHBoxLayout *sizeLayout;
  QHBoxLayout *verticalHintingLayout;
  QHBoxLayout *warpingLayout;

  QLabel *antiAliasingLabel;
  QLabel *dpiLabel;
  QLabel *fontFilenameLabel;
  QLabel *fontNameLabel;
  QLabel *gammaLabel;
  QLabel *glyphIndexLabel;
  QLabel *glyphNameLabel;
  QLabel *hintingModeLabel;
  QLabel *lcdFilterLabel;
  QLabel *sizeLabel;
  QLabel *zoomLabel;

  QList<int> hintingModesAlwaysDisabled;

  QLocale *locale;

  QMenu *menuFile;
  QMenu *menuHelp;

  QPen axisPen;
  QPen blueZonePen;
  QPen gridPen;
  QPen offPen;
  QPen onPen;
  QPen outlinePen;
  QPen segmentPen;

  QPushButton *nextFaceButton;
  QPushButton *nextFontButton;
  QPushButton *nextInstanceButton;
  QPushButton *previousFaceButton;
  QPushButton *previousFontButton;
  QPushButton *previousInstanceButton;

  QPushButtonx *toEndButtonx;
  QPushButtonx *toM1000Buttonx;
  QPushButtonx *toM100Buttonx;
  QPushButtonx *toM10Buttonx;
  QPushButtonx *toM1Buttonx;
  QPushButtonx *toP1000Buttonx;
  QPushButtonx *toP100Buttonx;
  QPushButtonx *toP10Buttonx;
  QPushButtonx *toP1Buttonx;
  QPushButtonx *toStartButtonx;

  QSignalMapper *glyphNavigationMapper;

  QSlider *gammaSlider;

  QSpinBox *dpiSpinBox;
  QSpinBox *zoomSpinBox;

  QTabWidget *tabWidget;

  QVBoxLayout *generalTabLayout;
  QVBoxLayout *leftLayout;
  QVBoxLayout *rightLayout;

  QVector<QRgb> grayColorTable;
  QVector<QRgb> monoColorTable;

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
  enum HintingMode
  {
    HintingMode_TrueType_v35,
    HintingMode_TrueType_v38,
    HintingMode_TrueType_v40,
    HintingMode_CFF_FreeType,
    HintingMode_CFF_Adobe,
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
  void setGraphicsDefaults();
  void showFont(bool = false);
  void writeSettings();
};


#endif // FTINSPECT_H_


// end of ftinspect.h
