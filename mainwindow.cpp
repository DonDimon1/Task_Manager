#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectionToDatabase(); // Подключаемся к БД
    setupModel(); // Создаём модель
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::connectionToDatabase(){ // Подключаемся к БД
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("taskList.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database.";
        return;
    }

    QSqlQuery query;
    //query.exec("DROP TABLE tasks");
    //query.clear();
    query.exec("CREATE TABLE IF NOT EXISTS tasks ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "title TEXT,"
               "status INTEGER,"
               "dateTime TEXT,"
               "description MEDIUMTEXT)");

    if(!query.exec("SELECT * FROM tasks"))
        qDebug() << "Не удалось подключиться к таблице: tasks";
    query.clear();
}

void MainWindow::setupModel() { // Создаём модель
    model = new TaskSqlTableModel(this, db);
    model->setTable("tasks");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Наименование"));
    model->setHeaderData(model->fieldIndex("dateTime"), Qt::Horizontal, tr("Дата и время"));
    model->setHeaderData(model->fieldIndex("status"), Qt::Horizontal, tr("Статус"));
    model->setHeaderData(model->fieldIndex("description"), Qt::Horizontal, tr("Описание"));
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    // Строка таблицы на всё доступное место
    QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Interactive);

    // Установка режима растягивания последнего столбца
    horizontalHeader->setStretchLastSection(true);
    //ui->tableView->setItemDelegate(new TaskDelegate(this));
}

void MainWindow::on_tableView_clicked(const QModelIndex &index) { // Обработка активной строки
    activeRow = index.row(); // При нажатии на строку происходит обновление активной строки.
}

void MainWindow::on_pushButton_Create_clicked() { // Создать задачу
    TaskDialog dialog(this);
    QString title;
    QString description;
    QDateTime dateTime;

    if (dialog.exec() == QDialog::Accepted) { // Если все данные введены правильно и пользователь нажал ок
        title = dialog.getTitle();
        description = dialog.getDescription();
        dateTime = dialog.getDateTime();
    } else{
        return;
    }

    int newRowIndex = model->rowCount();
    model->insertRow(newRowIndex);
    model->setData(model->index(newRowIndex, model->fieldIndex("title")), title);
    model->setData(model->index(newRowIndex, model->fieldIndex("description")), description);
    model->setData(model->index(newRowIndex, model->fieldIndex("dateTime")), dateTime.toString("yyyy-MM-dd HH:mm"));
    model->setData(model->index(newRowIndex, model->fieldIndex("status")), "Выполняется"); // По умолчанию задача имеет статус "Выполняется"
    model->submitAll();
}

void MainWindow::on_pushButton_Delete_clicked() { // Удалить задачу
    model->removeRow(activeRow);
    model->submitAll();
}

void MainWindow::on_pushButton_State_clicked() { // Изменение состояния задачи
    auto index = model->index(activeRow, model->fieldIndex("status"));
    QVariant task = model->dataRaw(index, Qt::DisplayRole);
    if(task.toInt() == InTheProcess)
        model->setDataRaw(index, Completed);
    else
        model->setDataRaw(index, InTheProcess);
    model->submitAll();
}


void MainWindow::on_pushButton_Refresh_clicked() { // Обновление задачи
    QModelIndex indexTitle = model->index(activeRow, model->fieldIndex("title"));
    QModelIndex indexDescription = model->index(activeRow, model->fieldIndex("description"));
    QModelIndex indexDateTime = model->index(activeRow, model->fieldIndex("dateTime"));

    QString title = model->data(indexTitle).toString();
    QString description = model->data(indexDescription).toString();
    QDateTime dateTime = model->data(indexDateTime).toDateTime();

    TaskDialog dialog(title, description, dateTime, this);

    if (dialog.exec() == QDialog::Accepted) { // Если все данные введены правильно и пользователь нажал ок
        title = dialog.getTitle();
        description = dialog.getDescription();
        dateTime = dialog.getDateTime();
    } else{
        return;
    }

    model->setData(indexTitle, title);
    model->setData(indexDescription, description);
    model->setData(indexDateTime, dateTime.toString("yyyy-MM-dd HH:mm"));
    model->submitAll();
}

void MainWindow::on_checkBox_filter_apply_stateChanged(int arg1)
{
    if(arg1 == 2) {
        QString title = ui->lineEdit_filter_title->text();
        QString description = ui->textEdit_filter_description->toPlainText();
        QDateTime dateTimeBegin = ui->dateTimeEdit_filter_date_begin->dateTime();
        QDateTime dateTimeEnd = ui->dateTimeEdit_filter_date_end->dateTime();
        int status = ui->comboBox_filter_state->currentIndex();

        model->setTitleFilter(title);
        model->setDescriptionFilter(description);
        model->setStatusFilter(status);
        model->setDateTimeFilter_Start(dateTimeBegin);
        model->setDateTimeFilter_End(dateTimeEnd);

    }
    else { // ВЫКЛ фильтр
        model->setFilter("");
        model->select();
    }
    // if(arg1 == 2) {
    //     QString title = ui->lineEdit_filter_title->text();
    //     QString description = ui->textEdit_filter_description->toPlainText();
    //     QDateTime dateTimeBegin = ui->dateTimeEdit_filter_date_begin->dateTime();
    //     QDateTime dateTimeEnd = ui->dateTimeEdit_filter_date_end->dateTime();
    //     int status = ui->comboBox_filter_state->currentIndex();

    //     model->setFilterConditions(title, description, dateTimeBegin, dateTimeEnd, status);

    // }
    // else { // ВЫКЛ фильтр
    //     model->setFilter("");
    //     model->select();
    // }
}


void MainWindow::on_pushButton_filter_reset_clicked() { // Сбросить настройки фильтра
    ui->lineEdit_filter_title->clear();
    ui->textEdit_filter_description->clear();
    ui->comboBox_filter_state->setCurrentIndex(0);
    ui->dateTimeEdit_filter_date_begin->setDateTime(QDateTime(QDate(1970, 1, 1), QTime(0, 0)));
    //ui->dateTimeEdit_filter_date_end->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_filter_date_end->setDateTime(QDateTime(QDate(2100, 1, 1), QTime(0, 0)));
    model->setFilter("");
    model->select();
}


void MainWindow::on_lineEdit_filter_title_textChanged(const QString &arg1) {// Если фильтр названия изменяется
    if(ui->checkBox_filter_apply->isChecked()){ // Если чекбокс фильтра активен
        model->setTitleFilter(arg1);
    }
}


void MainWindow::on_textEdit_filter_description_textChanged() { //Если фильтр описания измениться
    if(ui->checkBox_filter_apply->isChecked()){ // Если чекбокс фильтра активен
        QString descriptionText = ui->textEdit_filter_description->toPlainText();
        model->setDescriptionFilter(descriptionText);
    }
}

void MainWindow::on_comboBox_filter_state_currentIndexChanged(int index) { // Если фильтр статуса изменился
    if(ui->checkBox_filter_apply->isChecked()){ // Если чекбокс фильтра активен
        model->setStatusFilter(index);
    }
}


void MainWindow::on_dateTimeEdit_filter_date_begin_dateTimeChanged(const QDateTime &dateTimeBegin) { // Если дата начала изменилась
    if(ui->checkBox_filter_apply->isChecked()){ // Если чекбокс фильтра активен
        model->setDateTimeFilter_Start(dateTimeBegin);
    }
}


void MainWindow::on_dateTimeEdit_filter_date_end_dateTimeChanged(const QDateTime &dateTimeEnd) { // Если дата конца изменилась
    if(ui->checkBox_filter_apply->isChecked()){ // Если чекбокс фильтра активен
        model->setDateTimeFilter_End(dateTimeEnd);
    }
}

