#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    void сonnectionToDatabase();

private:

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
