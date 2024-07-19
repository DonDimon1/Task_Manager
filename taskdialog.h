#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>

namespace Ui {
class TaskDialog;
}

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget *parent = nullptr);
    explicit TaskDialog(QString &title, QString &description, QDateTime &dateTime, QWidget *parent = nullptr);
    ~TaskDialog();

    QString getTitle() const;
    QString getDescription() const;
    QDateTime getDateTime() const;

private:
    Ui::TaskDialog *ui;
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // TASKDIALOG_H
