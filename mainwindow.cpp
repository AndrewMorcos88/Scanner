#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include<qiterator.h>
#include <QString>
#include<QMap>
#include <QPalette>
#include <QApplication>

#include <QScrollArea>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

        ui->setupUi(this);
        this->removeToolBar(ui->mainToolBar);
        this->statusBar()->setVisible(false);
        mainPalette = new QPalette;
        this->setWindowTitle("Social Network Analysis");
        mainPalette->setColor(QPalette::Active,QPalette::Window,Qt::white);
        this->setPalette(*mainPalette);
        //this->setFixedSize(400,100);
        InitUI();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::InitUI()
{


       //init input layout
       inputLayout = new QHBoxLayout;
       InitFisrtPage();
       mainLayout = new QHBoxLayout;
       mainLayout->addItem(inputLayout);



       //adding main layout to central widget of window
       centralWidget = new QWidget(this);
       centralWidget->setLayout(mainLayout);
       setCentralWidget(centralWidget);
}

void MainWindow::InitFisrtPage(){

    QFont font("Volkhov",11,QFont::Light,true);


    browseButton = new QPushButton;
    browseButton->setText("Browse Input File");
    browseButton->setMinimumHeight(30);
    browseButton->setFont(font);

    RunButton = new QPushButton;
    RunButton->setText("Run Algorithm");
    RunButton->setMinimumHeight(30);
    RunButton->setFont(font);


    fileName = new QLineEdit;
    fileName->setDisabled(true);
    fileName->setFont(font);
    fileName->setMinimumHeight(30);

    firstInputPage = new QFormLayout;
    firstInputPage->addRow(fileName,browseButton);
    firstInputPage->addRow(RunButton);

    firstInputPage->setContentsMargins(10,10,10,10);
    firstInputPage->setSpacing(10);
    inputLayout->addItem(firstInputPage);
    QObject::connect(browseButton,SIGNAL(clicked()),this,SLOT(browseButtonPressed()));
    QObject::connect(RunButton,SIGNAL(clicked()),this,SLOT(runAlgorithmPressed()));


}




void MainWindow::browseButtonPressed()
{
    TEXT.clear();
    QString file_name = QFileDialog::getOpenFileName(this,"Choose input file","c://",tr("Text Files (*.txt)"));
    QFile file(file_name);
    fileName->setText(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"Input File Error","Please Select A Valid Input Text File");
        return;
    }
    else{
        QTextStream input(&file);
        while (!input.atEnd())
            {
         TEXT.push_back(input.readLine());
            }

        }

    file.close();

}





void MainWindow::runAlgorithmPressed()
{
    if(TEXT.size()==0){
        QMessageBox::warning(this,"Input File Error","Please Select A Valid Input Text File");
    }
    else{
        TOKENS_TYPE.clear();
        TOKENS_VALUE.clear();


        Reserved_Words["write"] = 1;
        Reserved_Words["read"] = 2;
        Reserved_Words["if"] = 3;
        Reserved_Words["else"] = 4;
        Reserved_Words["return"] = 5;
        Reserved_Words["begin"] = 6;
        Reserved_Words["end"] = 7;
        Reserved_Words["main"] = 8;
        Reserved_Words["string"] = 9;
        Reserved_Words["int"] = 10;
        Reserved_Words["real"] = 11;
        Reserved_Words["until"] = 12;
        Reserved_Words["repeat"] = 13;
        Reserved_Words["then"] = 14;


        Operators_Assigments[";"] = 1;
        Operators_Assigments[","] = 2;
        Operators_Assigments["("] = 3;
        Operators_Assigments[")"] = 4;
        Operators_Assigments["+"] = 5;
        Operators_Assigments["-"] = 6;
        Operators_Assigments["/"] = 7;
        Operators_Assigments["*"] = 8;
        Operators_Assigments["="] = 9;
        Operators_Assigments["!="] = 10;
        Operators_Assigments[":="] = 11;
        Operators_Assigments["["] = 12;
        Operators_Assigments["]"] = 13;
        Operators_Assigments["<"] = 14;
        Operators_Assigments[">"] = 15;
        Operators_Assigments[">="] = 16;
        Operators_Assigments["<="] = 17;





        int Size_string;
        bool This_is_comment = false;
        bool Flag_Error = false;
        QString One_Line;

        for (int j=0;j<TEXT.size();j++)
        {
            One_Line=TEXT[j];

    QString my_token = "";
    Size_string = One_Line.length();

    bool Wait_FOR_SemiCOLumn = false;
    bool Wait_FOR_IDENTIFIERS = false;
    bool There_is_Number = false;
    bool There_is_string = false;
     QString Comment = "";
    QString Number = "";
    QString My_string = "";


    for (int i = 0; i < Size_string; i++)
                {

                    if (!(One_Line[i]==';'|| One_Line[i] == ',' || One_Line[i] == '(' || One_Line[i] == ')' ||
                        One_Line[i] == '{' || One_Line[i] == '}' || One_Line[i] == '/' || One_Line[i] == '*' ||
                        One_Line[i] == '+' || One_Line[i] == '-' || One_Line[i] == ':' || One_Line[i] == '<' ||
                        One_Line[i] == '>' || One_Line[i] == '!' || One_Line[i] == '"' || One_Line[i] == ' ') || One_Line[i] =='=')
                        my_token += One_Line[i];  // take only the alphapit and digit charachters


                    if (This_is_comment == true) // comment opened and stil wait for closing it
                    {

                        if (One_Line[i] == '}')
                        {
                            This_is_comment = false;
                            my_token = "";
                            continue;

                        }
                            else
                            {
                                Comment += One_Line[i];  // still taking the comment
                                continue;
                            }



                    }





                    // check for "" string ""

                    if (There_is_string == true)
                    {
                        if (One_Line[i] == '"')
                        {
                            TOKENS_TYPE.push_back("String");
                            TOKENS_VALUE.push_back(My_string);
                            There_is_string = false;
                            my_token = "";
                        }
                        else
                            My_string += One_Line[i];

                        continue;

                    }


                    if (One_Line[i] == '"') {
                        There_is_string = true;

                    }





                    // you now have identifier to add it

                    if (Wait_FOR_IDENTIFIERS == true && (One_Line[i] == ';' || One_Line[i] == ' ' ||
                        One_Line[i]=='('  || One_Line[i]=='=' ) &&my_token!="" )
                    {
                        TOKENS_TYPE.push_back("IDENTIFIER");
                        TOKENS_VALUE.push_back(my_token);
                        IDENTIFIERS[my_token] = 1;  // add it to
                        Wait_FOR_IDENTIFIERS = false;
                        my_token = "";

                    }



















                    // check for identifiers
                    if (my_token != "")
                    {
                        it = IDENTIFIERS.find(my_token);
                        if (it != IDENTIFIERS.end()) // you found an identifier here
                        {
                            TOKENS_TYPE.push_back("IDENTIFIER");
                            TOKENS_VALUE.push_back(it.key());
                            my_token = "";
                        }

                    }

                    //check for reserved words

                    if (One_Line[i]==' ' || i==Size_string-1)
                    {
                        it = Reserved_Words.find(my_token);


                        if (it != Reserved_Words.end() )   // token found in Reserved words
                        {
                            Check_Reserved_Keywords(it.value(), TOKENS_TYPE, TOKENS_VALUE, Wait_FOR_IDENTIFIERS
                            ,Wait_FOR_SemiCOLumn);

                            my_token = "";
                            continue;
                        }

                    }





                        // check for numbers

                                          if ( One_Line[i].isDigit())
                                          {
                                              There_is_Number = true;
                                          }

                                          if (There_is_Number == true)
                                          {
                                              if (One_Line[i].isDigit() || One_Line[i] == '.')
                                                  Number += One_Line[i];
                                              else if (Number != "")
                                              {
                                                  TOKENS_TYPE.push_back("Number");
                                                  TOKENS_VALUE.push_back(Number);
                                                  Number="";
                                                  There_is_Number = false;
                                                  my_token = "";
                                              }

                                              if (i==Size_string-1 && There_is_Number==true)
                                              {
                                                  TOKENS_TYPE.push_back("Number");
                                                  TOKENS_VALUE.push_back(Number);
                                                  Number="";
                                                  There_is_Number = false;
                                                  my_token = "";

                                              }

                                          }





                        // check for comments

                                    if (One_Line[i] == '{')
                                    {
                                        This_is_comment = true;

                                    }




                                    // check for operators

                                    QString temp = "";
                                    temp += One_Line[i];
                                    it = Operators_Assigments.find(temp);  // search for operator of 1 char
                                    if (it != Operators_Assigments.end())
                                    {
                                        Check_Operators(it.value(), TOKENS_TYPE, TOKENS_VALUE,
                                                        Wait_FOR_SemiCOLumn, Wait_FOR_IDENTIFIERS ,my_token ,IDENTIFIERS);


                                    }
                                    else if (i + 1 < Size_string)   // check for 2 char operators
                                    {
                                        temp += One_Line[i + 1];
                                        it = Operators_Assigments.find(temp);
                                        if (it != Operators_Assigments.end())
                                        {
                                            Check_Operators(it.value(), TOKENS_TYPE, TOKENS_VALUE
                                                            , Wait_FOR_SemiCOLumn, Wait_FOR_IDENTIFIERS ,my_token ,IDENTIFIERS);
                                            i++;

                                        }

                                    }









                                    if  (  i==Size_string-1 && my_token != "")
                           Flag_Error = true;


                    }



    }

        if (Flag_Error==true)
               QMessageBox::warning(this,"Input File Error","The Example Is Not Correct!!");
        else {



            QLabel* tokenTypelabel = new QLabel("TOKEN TYPE");
            tokenTypelabel->setMinimumHeight(30);

            QLabel* tokenvaluelabel = new QLabel("TOKEN Value");
            tokenvaluelabel->setMinimumHeight(30);

            outputForm = new QFormLayout;
            outputForm->addRow(tokenTypelabel,tokenvaluelabel);

            outputForm->setContentsMargins(10,10,10,10);
            outputForm->setSpacing(10);

            QLineEdit* tokenType;
            QLineEdit* tokenvalue;

            for (int i=0;i<TOKENS_VALUE.size();i++)
            {
                 tokenType = new QLineEdit;
                 tokenType->setDisabled(true);
                 tokenType->setMinimumHeight(30);

                 tokenvalue= new QLineEdit;
                 tokenvalue->setDisabled(true);
                 tokenvalue->setMinimumHeight(30);


                 tokenType->setText(TOKENS_TYPE[i]);
                 tokenvalue->setText(TOKENS_VALUE[i]);

                 outputForm->addRow(tokenType,tokenvalue);

            }
            outputLayout =new QHBoxLayout();
            outputLayout->addItem(outputForm);
            mainLayout->addItem(outputLayout);

            QScrollArea *scrollarea = new QScrollArea(this);
            scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            scrollarea->setWidgetResizable(false);
            centralWidget = new QWidget(this);
            centralWidget->setLayout(mainLayout);
            scrollarea->setWidget(centralWidget);
            this->setCentralWidget(scrollarea);
            showMaximized();


        }

    }

    TEXT.clear();
    fileName->setText("");
}




void MainWindow::Check_Reserved_Keywords(int target, QVector<QString>& TOKENS_TYPE, QVector<QString> &TOKENS_VALUE,
    bool &Wait_FOR_IDENTIFIERS ,bool & Wait_FOR_SemiCOLumn)
{

    switch (target)
    {
    case 1: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("WRITE"); Wait_FOR_SemiCOLumn = true; break;
    case 2: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("READ"); Wait_FOR_SemiCOLumn = true;Wait_FOR_IDENTIFIERS=true;  break;
        case 3: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("IF"); break;
        case 4: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("ELSE"); break;
        case 5: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("RETURN"); Wait_FOR_SemiCOLumn = true; break;
        case 6: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("BEGIN"); break;
        case 7: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("END"); break;
        case 8: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("MAIN"); break;
        case 9: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("STRING"); Wait_FOR_SemiCOLumn = true; Wait_FOR_IDENTIFIERS = true;  break;
        case 10: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("INT"); Wait_FOR_SemiCOLumn = true; Wait_FOR_IDENTIFIERS = true; break;
        case 11: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("REAL"); Wait_FOR_SemiCOLumn = true; Wait_FOR_IDENTIFIERS = true;  break;
        case 12: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("UNTIL"); break;
        case 13: TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("REPEAT"); break;
        case 14:  TOKENS_TYPE.push_back("Fixed KEYWORD"); TOKENS_VALUE.push_back("THEN"); break;

        default: break;


    }
}


void  MainWindow:: Check_Operators(int target, QVector<QString>& TOKENS_TYPE, QVector<QString> &TOKENS_VALUE
                     , bool & Wait_FOR_SemiCOLumn, bool &Wait_FOR_IDENTIFIERS ,QString & My_Token , QMap<QString,int> &IDENTIFIERS){
    switch (target)
        {

         case 1:TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back(";");
            Wait_FOR_SemiCOLumn = false;
            break;

        case 2: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back(",");
            break;

        case 3: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("(");
            break;

        case 4: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back(")");
            break;

        case 5: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("+");
            break;

        case 6: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("-");
            break;

        case 7: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("/");
            break;

        case 8: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("*");
            break;
        case 9: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("=");
            break;

        case 10: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("!=");
            break;

        case 11:
         if(My_Token != "")
    {
             TOKENS_TYPE.push_back("IDENTIFIER");
             TOKENS_VALUE.push_back(My_Token);
             IDENTIFIERS[My_Token]=1;
            My_Token="";
            Wait_FOR_IDENTIFIERS=false;

    }
         TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back(":=");

            break;


        case 12: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("[");
            break;

        case 13: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("]");
            break;

        case 14: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("<");
            break;


        case 15: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back(">");
            break;


        case 16: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back(">=");
            break;


        case 17: TOKENS_TYPE.push_back("Assigment");
            TOKENS_VALUE.push_back("<=");
            break;

        default: break;

        }

}






















