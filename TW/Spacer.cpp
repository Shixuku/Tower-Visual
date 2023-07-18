#include "Spacer.h"
#include"Senior.h"

Spacer::Spacer(Senior* sen, QWidget* parent)
	: QDialog(parent)
{
	m_sen = sen;
	ui.setupUi(this);
	connect(ui.Btn_ensure, &QPushButton::clicked, this, [=]() {GetData(); this->close(); });
	connect(ui.Btn_cancel, &QPushButton::clicked, this, [=]() { this->close(); });
}

Spacer::~Spacer()
{}

void Spacer::Set_Lists(int N)
{
	QStringList headertext;
	headertext << "档位" << "间隔棒个数";
	ui.Section_Lists->setColumnCount(headertext.count());
	ui.Section_Lists->setRowCount(N-1);//首先默认0行
	ui.Section_Lists->setHorizontalHeaderLabels(headertext);
	ui.Section_Lists->verticalHeader()->setVisible(false);
	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < N-1; i++)
	{
		ui.Section_Lists->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
		//表格文字设置居中
		ui.Section_Lists->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		for (int j = 1; j < headertext.count(); j++)
		{
			ui.Section_Lists->setItem(i, j, new QTableWidgetItem(QString::number(0)));
			//表格文字设置居中
			ui.Section_Lists->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
	}

	ui.Section_Lists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Spacer::GetData()
{
	m_sen->ChooseWay = ui.Chose_Box->currentIndex();
	for (int i = 0; i < ui.Section_Lists->rowCount(); i++)
	{
		int n = ui.Section_Lists->item(i, 1)->text().toInt();
		m_sen->SpacerNum.push_back(n); //存入每档间隔棒个数
	}
}
