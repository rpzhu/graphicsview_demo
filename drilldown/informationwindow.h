﻿#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QtWidgets>
#include <QtSql>

class InformationWindow : public QDialog
{
    Q_OBJECT

public:
    InformationWindow(int id, QSqlRelationalTableModel *items,
                      QWidget *parent = 0);

    int id();

signals:
    void imageChanged(int id, const QString &fileName);

private slots:
    void revert();
    void submit();
    void enableButtons(bool enable = true);

private:
    void createButtons();

    int itemId;
    QString displayedImage;

    QComboBox *imageFileEditor;
    QLabel *itemText;
    QTextEdit *descriptionEditor;

    QPushButton *closeButton;
    QPushButton *submitButton;
    QPushButton *revertButton;
    QDialogButtonBox *buttonBox;

    QDataWidgetMapper *mapper;
};

#endif
