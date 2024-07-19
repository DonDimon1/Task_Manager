#ifndef TASKSQLTABLEMODEL_H
#define TASKSQLTABLEMODEL_H

#include <QSqlTableModel>

enum State{Unknown = 0, InTheProcess = 1, Completed = 2};

class TaskSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    TaskSqlTableModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override; // Получение значения ячейки из БД
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override; // Установка значения в БД

    QVariant dataRaw(const QModelIndex &index, int role = Qt::EditRole) const; // Информация из БД без преобразования в QString
    bool setDataRaw(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); // Установка значения в БД без строки

    void setFilterConditions(const QString &title,      // Фильтр
                             const QString &description,
                             const QDateTime &dateBegin,
                             const QDateTime &dateEnd,
                             const int &status);

    // Методы для установки фильтров
    void setTitleFilter(const QString &titleText);
    void setDescriptionFilter(const QString &descriptionText);
    void setStatusFilter(const int &statusText);
    void setDateTimeFilter_Start(QDateTime startDate);
    void setDateTimeFilter_End(QDateTime endDate);

private:
    QString stateToSting(int state) const;  // Преобразовения из int в QString и обратно
    int stringToState(const QString &state) const;
    void updateFilter(); // Метод для обновления общей строки фильтрации

    QString stateStr[2] = {"Выполняется", "Выполнено"};
    QString titleFilter; // Строки для фильтроа наименования
    QString descriptionFilter; // Строки для фильтра описания
    QString statusFilter; // Строка для фильтра статуса
    QString dateTimeFilter_Start; // Строки для фильтра дат
    QString dateTimeFilter_End;
};

#endif // TASKSQLTABLEMODEL_H

