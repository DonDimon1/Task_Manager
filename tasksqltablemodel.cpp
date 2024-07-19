#include "tasksqltablemodel.h"
#include "qdatetime.h"

// Поскольку состояний задач может быть больше, для хранения состояния задачи в БД мы используем значения типа int.
// Но для удобства пользователя в таблице выводится строковое представление этих значений.
TaskSqlTableModel::TaskSqlTableModel(QObject* parent, QSqlDatabase db) : QSqlTableModel(parent, db) {}

QVariant TaskSqlTableModel::data(const QModelIndex &index, int role) const { // Получение значения ячейки из БД
    if(role == Qt::DisplayRole && index.column() == fieldIndex("status")){
        int state = QSqlTableModel::data(index, Qt::EditRole).toInt();
        return stateToSting(state);
    }
    return QSqlTableModel::data(index, role);
}

bool TaskSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role){ // Установка значения в БД
    if(role == Qt::EditRole && index.column() == fieldIndex("status")){
        int state = stringToState(value.toString());
        return QSqlTableModel::setData(index, state, role);
    }
    return QSqlTableModel::setData(index, value, role);
}

QString TaskSqlTableModel::stateToSting(int state) const{ // Преобразовения из int в QString
    switch(state){
        case InTheProcess: return stateStr[0];
        case Completed: return stateStr[1];
        default: return "Неизвестно";
    }
}

int TaskSqlTableModel::stringToState(const QString &state) const{ // Преобразовения из QStrin в int
    if(state == stateStr[0]) return InTheProcess;
    else if(state == stateStr[1]) return Completed;
    else return -1;
}

QVariant TaskSqlTableModel::dataRaw(const QModelIndex &index, int role) const { // Информация из БД без преобразования в QString
    return QSqlTableModel::data(index, role);
}

bool TaskSqlTableModel::setDataRaw(const QModelIndex &index, const QVariant &value, int role){ // Сохраняем информацию в БД без преобразования в QString
    return QSqlTableModel::setData(index, value, role);
}

void TaskSqlTableModel::setTitleFilter(const QString &titleText) { // Фильтр Наименования
    if(!titleText.isEmpty()){
        titleFilter = QString("title LIKE '%%1%'").arg(titleText);
    } else
        titleFilter = QString("");
    updateFilter();
}

void TaskSqlTableModel::setDescriptionFilter(const QString &descriptionText) { // Фильтр Описания
    if(!descriptionText.isEmpty()){
        descriptionFilter = QString("description LIKE '%%1%'").arg(descriptionText);
    } else
        descriptionFilter = QString("");
    updateFilter();
}

void TaskSqlTableModel::setStatusFilter(const int &status) { // Фильтр состояния
    if(status != 0)
        statusFilter = QString("status LIKE '%%1%'").arg(status);
    else
        statusFilter = QString("");
    updateFilter();
}

void TaskSqlTableModel::setDateTimeFilter_Start(QDateTime startDate) {
    dateTimeFilter_Start = QString("dateTime >= '%1%'").arg(startDate.toString("yyyy-MM-dd HH:mm"));
    updateFilter();
}
void TaskSqlTableModel::setDateTimeFilter_End(QDateTime endDate) {
    dateTimeFilter_End = QString("dateTime <= '%1%'").arg(endDate.toString("yyyy-MM-dd HH:mm"));
    updateFilter();
}

void TaskSqlTableModel::updateFilter() {
    setFilter("");
    QStringList conditions;
    if(!titleFilter.isEmpty()) conditions << titleFilter;
    if(!descriptionFilter.isEmpty()) conditions << descriptionFilter;
    if(!statusFilter.isEmpty()) conditions << statusFilter;
    if(!dateTimeFilter_Start.isEmpty()) conditions << dateTimeFilter_Start;
    if(!dateTimeFilter_End.isEmpty()) conditions << dateTimeFilter_End;

    setFilter(conditions.join(" AND "));
    select();
}

// void TaskSqlTableModel::setFilterConditions(const QString &title, const QString &description,
//                                             const QDateTime &dateBegin, const QDateTime &dateEnd, const int &status){ // Фильтр
//     QStringList conditions;

//     if (!title.isEmpty()) {
//         QString likeFilter = QString("title LIKE '%%1%'").arg(title);
//         conditions << likeFilter;
//     }

//     if (!description.isEmpty()) {
//         QString likeFilter = QString("description LIKE '%%1%'").arg(description);
//         conditions << likeFilter;
//     }

// //    if (dateBegin.isValid() && dateEnd.isValid()) {
// //        QString dateFilter = QString("date BETWEEN '%1' AND '%2'").arg(dateBegin.toString(Qt::ISODate)).arg(dateEnd.toString(Qt::ISODate));
// //        conditions << dateFilter;
// //    } else if (dateBegin.isValid()) {
// //        QString startDateFilter = QString("date >= '%1'").arg(dateBegin.toString(Qt::ISODate));
// //        conditions << startDateFilter;
// //    } else if (dateEnd.isValid()) {
// //        QString endDateFilter = QString("date <= '%1'").arg(toString.toString(Qt::ISODate));
// //        conditions << endDateFilter;
// //    }
//     if(status != 0) {
//          QString likeFilter = QString("status LIKE '%%1%'").arg(status);
//          conditions << likeFilter;
//     }


//     QSqlTableModel::setFilter(conditions.join(" AND "));
//     QSqlTableModel::select();
//     //return conditions;
// }

//QVariant TaskSqlTableModel::displayData(const QModelIndex &index, int role = Qt::DisplayRole) const{
//    if(role == Qt::DisplayRole && index.column() == fieldIndex("status")){
//        int state = QSqlTableModel::data(index, Qt::EditRole).toInt();
//        return stateToSting(state);
//    }
//    return QSqlTableModel::data(index, role);
//}

