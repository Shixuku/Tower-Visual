#include "WindList.h"

WindList::WindList(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Initialize();
    connect(ui.read_btn, &QPushButton::clicked, this, &WindList::ReadData);
}

WindList::~WindList()
{}

void WindList::Initialize()
{
	ui.Speed_List->setColumnCount(2);
	QStringList headerLabels;
	headerLabels << "t(s)" << "V(m/s)";
	ui.Speed_List->setHorizontalHeaderLabels(headerLabels);
}

void WindList::ReadData()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "ѡ���ı��ļ�", QString(), "�ı��ļ� (*.txt)");
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "�޷����ļ�: " << file.errorString();
            return;
        }

        ui.Speed_List->clearContents();
        ui.Speed_List->setRowCount(0);

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(" ");

            if (fields.size() == 2)
            {
                int row = ui.Speed_List->rowCount();
                ui.Speed_List->insertRow(row);

                QTableWidgetItem* item1 = new QTableWidgetItem(fields[0]);
                QTableWidgetItem* item2 = new QTableWidgetItem(fields[1]);

                item1->setTextAlignment(Qt::AlignCenter);
                item2->setTextAlignment(Qt::AlignCenter);

                ui.Speed_List->setItem(row, 0, item1);
                ui.Speed_List->setItem(row, 1, item2);
            }
        }

        file.close();
    }
}
