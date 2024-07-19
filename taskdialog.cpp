#include "taskdialog.h"
#include "ui_taskdialog.h"
#include <QMessageBox>


TaskDialog::TaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

TaskDialog::TaskDialog(QString &title, QString &description, QDateTime &dateTime, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    ui->lineEdit_title->setText(title);
    ui->textEdit->setText(description);
    ui->dateTimeEdit->setDateTime(dateTime);
}

TaskDialog::~TaskDialog()
{
    delete ui;
}

QString TaskDialog::getTitle() const {
    return ui->lineEdit_title->text();
}

QString TaskDialog::getDescription() const {
    return ui->textEdit->toPlainText();
}

QDateTime TaskDialog::getDateTime() const {
    return ui->dateTimeEdit->dateTime();
}

void TaskDialog::on_buttonBox_accepted() {  // Нажатие на ОК
    if(!getTitle().isEmpty() && !getDescription().isEmpty()){ // Если название и описание на заполнены
        accept();
    }else{
        QMessageBox::warning(this, "Input Error", "Наименование или описание на заполнены.");
    }
}

void TaskDialog::on_buttonBox_rejected(){ // Нажатие на Отмена
    reject();
}

