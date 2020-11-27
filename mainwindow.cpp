#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QString>

FILE* pFile = NULL;
static const char* filename = "C:\\Users\\dell\\Desktop\\ALL\\qt\\english_search_tools\\dict.txt";
static int iLine = 0;
struct Dict{
    char* word;
    char* translates;
};

typedef struct Dict dict;
dict* pDict;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    pFile = fopen(filename, "r");
    if (pFile == NULL)
    {
        return;
    }

    fseek(pFile, 0, SEEK_SET);
    while (!feof(pFile))
    {
        if (fgetc(pFile) == '\n')
            iLine++;
    }

    fseek(pFile, 0, SEEK_SET);

    iLine = iLine / 2;
    pDict = (dict*)malloc(sizeof(dict) * iLine);
    memset(pDict, 0, sizeof(dict) * iLine);
    if (pDict == NULL){
        return;
    }

    // 加载字典进Dict数组中
    char buf[1024];
    unsigned int dwIndex = 0;
    while (!feof(pFile)){
        memset(buf, 0, 1024);
        fgets(buf, 1024, pFile);
        buf[strlen(buf)+1] = '\0';
        pDict[dwIndex].word = (char*)malloc(strlen(buf)+1); // 防止引用，导致数据相同，所以这里开辟空间来进行使用
        memset(pDict[dwIndex].word, 0, strlen(buf) + 1);
        strcpy(pDict[dwIndex].word, &buf[1]);

        memset(buf, 0, 1024);
        fgets(buf, 1024, pFile);
        buf[strlen(buf) + 1] = '\0';
        pDict[dwIndex].translates = (char*)malloc(strlen(buf)+1);
        memset(pDict[dwIndex].translates, 0, strlen(buf) + 1);
        strcpy(pDict[dwIndex].translates, buf);

        dwIndex++;
    }
    fclose(pFile);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < iLine; i++){
        free(pDict[i].word);
        free(pDict[i].translates);
    }
    free(pDict);
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //开始进行输入匹配
    QString txt = ui->textEdit->toPlainText();
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    if(txt.isEmpty())
    {
        ui->label_2->setText("请输入你要查询的内容!");
        return;
    }

    char* buf = codec->fromUnicode(txt).data();
    for(int i=0;i<iLine;i++)
    {
        if(strncmp(buf, pDict[i].word, strlen(buf))==0){
            ui->label_2->setText(codec->toUnicode(pDict[i].translates));
            return;
        }
    }
    ui->label_2->setText("未查询到指定的单词翻译!");
    return;
}
