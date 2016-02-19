#include "widget.h"
#include "ui_widget.h"
#include "qtjsonpath.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->bt_ok, SIGNAL(clicked(bool)), SLOT(checkPath()));
    connect( ui->le_path, SIGNAL(returnPressed()), SLOT(checkPath()));

    QByteArray json ("{ \"store\": {"
                     "    \"book\": [ "
                     "      { \"category\": \"reference\","
                     "        \"author\": \"Nigel Rees\","
                     "        \"title\": \"Sayings of the Century\","
                     "        \"price\": 8.95"
                     "      },"
                     "      { \"category\": \"fiction\","
                     "        \"author\": \"Evelyn Waugh\","
                     "        \"title\": \"Sword of Honour\","
                     "        \"price\": 12.99"
                     "      },"
                     "      { \"category\": \"fiction\","
                     "        \"author\": \"Herman Melville\","
                     "        \"title\": \"Moby Dick\","
                     "        \"isbn\": \"0-553-21311-3\","
                     "        \"price\": 8.99"
                     "      },"
                     "      { \"category\": \"fiction\","
                     "        \"author\": \"J. R. R. Tolkien\","
                     "        \"title\": \"The Lord of the Rings\","
                     "        \"isbn\": \"0-395-19395-8\","
                     "        \"price\": 22.99"
                     "      }"
                     "    ],"
                     "    \"bicycle\": {"
                     "      \"color\": \"red\","
                     "      \"price\": 19.95"
                     "    }"
                     "  }"
                     "}");

    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);

    QtJsonPath jsonPath (jsonDoc);

    qDebug () << "/store/book/1"        << jsonPath.getValue ("/store/book/1");
    qDebug () << "/store/book/2/author" << jsonPath.getValue ("/store/book/2/author");
    qDebug () << "/store/bicycle"       << jsonPath.getValue ("/store/bicycle");
    qDebug () << "/store/toto"          << jsonPath.getValue ("/store/toto", "N/A");

    this->setWindowTitle(QStringLiteral("QJsonpath Tester"));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::checkPath()
{
    QByteArray txt(ui->input->toPlainText().toUtf8());

    qDebug() << "input:: " << txt << endl;

    QJsonDocument doc = QJsonDocument::fromJson(txt);
    QtJsonPath path1(doc);

    QString tarPath = ui->le_path->text();
    QVariant var1 =  path1.getValue(tarPath);

    QString targetText = var1.toString();
    ui->viewer->setText( targetText );
    qDebug() << "1::  " << tarPath  << " = " << var1 << endl;

    QJsonValue jsonVal1 = QJsonValue::fromVariant(var1);
    if(jsonVal1.isArray()) {
        qDebug("\x1b[32m aHa it is array!!! \x1b[0m");
        targetText.clear();
        QJsonArray jsonArr = jsonVal1.toArray();
        QJsonArray::Iterator it;
        for(it = jsonArr.begin(); it != jsonArr.end();  ++it){
//            qDebug() << "it"  << *it << endl;
            QJsonValue tmp(*it);
            targetText.append(" | ");
            targetText.append( tmp.toString() );
        }
        ui->viewer->setText( targetText );
        qDebug()<< "target text:: " << targetText << endl;
    }else{
        qDebug("\x1b[33m aHa it is not array!!! \x1b[0m");
    }
}

void Widget::on_btExit_clicked()
{
    this->close();
}
