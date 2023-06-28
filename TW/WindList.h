#pragma once

#include <QDialog>
#include "ui_WindList.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QWidget>
#pragma execution_character_set("utf-8")
class WindList : public QDialog
{
	Q_OBJECT

public:
	WindList(QWidget *parent = nullptr);
	~WindList();
	void Initialize();
	void ReadData();
private:
	Ui::WindListClass ui;
};
