#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "taskdialog.h"
#include "tasksqltablemodel.h"

#include <QDebug>
#include <QSqlQuery> // Подключаемся к БД
//#include <QSqlTableModel> // Модель для БД

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connectionToDatabase(); // Подключаемся к БД
    void setupModel(); // Создаём модель



private slots:
    void on_pushButton_Create_clicked();

    void on_pushButton_Delete_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_State_clicked();

    void on_pushButton_Refresh_clicked();

    void on_checkBox_filter_apply_stateChanged(int arg1);

    void on_pushButton_filter_reset_clicked();

    void on_lineEdit_filter_title_textChanged(const QString &arg1);

    void on_textEdit_filter_description_textChanged();

    void on_comboBox_filter_state_currentIndexChanged(int index);

    void on_dateTimeEdit_filter_date_begin_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_filter_date_end_dateTimeChanged(const QDateTime &dateTime);

private:
    Ui::MainWindow *ui;
    //QSqlTableModel *model;
    TaskSqlTableModel *model;
    int activeRow; //Номер активной строки
protected:
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
