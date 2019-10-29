#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include<qiterator.h>
#include <QString>
#include<QMap>
#include <QPalette>
#include <QGraphicsScene>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QList>
#include <QPair>
#include<QMessageBox>
#include <QChar>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<QString>TOKENS_TYPE;
    QVector<QString>TOKENS_VALUE;
    QMap<QString, int>Reserved_Words;
    QMap<QString, int>::iterator it;
    QMap<QString, int>IDENTIFIERS;
    QMap<QString, int>Operators_Assigments;
    void InitUI();
      void InitFisrtPage();
      QVector<QString>TEXT;


      void Check_Reserved_Keywords(int target, QVector<QString>& TOKENS_TYPE, QVector<QString> &TOKENS_VALUE,
          bool &Wait_FOR_IDENTIFIERS , bool & Wait_FOR_SemiCOLumn);
      void Check_Operators(int target, QVector<QString>& TOKENS_TYPE, QVector<QString> &TOKENS_VALUE
          , bool & Wait_FOR_SemiCOLumn, bool &Wait_FOR_IDENTIFIERS);







public slots:
    void browseButtonPressed();
    void runAlgorithmPressed();



private:
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    //colors and pallete
    QPalette *mainPalette;
    //layout
    QFormLayout *firstInputPage;
    QFormLayout *outputForm;
    QHBoxLayout *mainLayout;
    QHBoxLayout *inputLayout;
    QHBoxLayout *outputLayout;

    // buttons
    QPushButton *browseButton;
    QPushButton *RunButton;

    //text
    QLineEdit *fileName;
    QLineEdit * First_Line;
    QLineEdit * Second_Line;


};

#endif // MAINWINDOW_H
