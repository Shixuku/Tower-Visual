#include "MultiSelectComboBox.h"
#include <QDebug>

MultiSelectComboBox::MultiSelectComboBox(QWidget* parent)
	: QComboBox(parent)
	, hidden_flag_(true)
	, show_flag_(false)
{
	list_widget_ = new QListWidget();
	line_edit_ = new QLineEdit();
	search_bar_ = new QLineEdit();

	/*����������*/
	QListWidgetItem* currentItem = new QListWidgetItem(list_widget_);
	//������������ʾ��Ϣ
	search_bar_->setPlaceholderText("Search.........");
	//��ʾ�����ť
	search_bar_->setClearButtonEnabled(true);
	list_widget_->addItem(currentItem);
	list_widget_->setItemWidget(currentItem, search_bar_);

	/*�����ı���*/
	//��Ϊֻ������Ϊ�������ֻ������ʾѡ�е�ѡ���Ϊ�ı��������Щ
	line_edit_->setReadOnly(true);
	//�ѵ�ǰ����װ(��ע��)Ϊ�¼�����������ǰҲ��Ϊ�����������¼�������ͨ���ڹ��캯���н���ע�ᡣ
	line_edit_->installEventFilter(this);
	//���ý�����ʽ����Ϊ������ʽ����ƣ���ʱ�������
	line_edit_->setStyleSheet("QLineEdit:disabled{background:rgb(233,233,233);}");

	this->setModel(list_widget_->model());
	this->setView(list_widget_);
	this->setLineEdit(line_edit_);
	connect(search_bar_, SIGNAL(textChanged(const QString&)), this, SLOT(onSearch(const QString&)));
	connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &MultiSelectComboBox::itemClicked);
}

MultiSelectComboBox::~MultiSelectComboBox()
{
}

void MultiSelectComboBox::hidePopup()
{
	show_flag_ = false;
	int width = this->width();
	int height = this->height();
	int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
	int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();
	if (x >= 0 && x <= width && y >= this->height() && y <= height + this->height())
	{
	}
	else
	{
		QComboBox::hidePopup();
	}
}

void MultiSelectComboBox::addItem(const QString& _text, const QVariant& _variant /*= QVariant()*/)
{
	Q_UNUSED(_variant);
	QListWidgetItem* item = new QListWidgetItem(list_widget_);
	QCheckBox* checkbox = new QCheckBox(this);
	checkbox->setText(_text);
	list_widget_->addItem(item);
	list_widget_->setItemWidget(item, checkbox);
	connect(checkbox, &QCheckBox::stateChanged, this, &MultiSelectComboBox::stateChange);
}

void MultiSelectComboBox::addItems(const QStringList& _text_list)
{
	for (const auto& text_one : _text_list)
	{
		addItem(text_one);
	}
}

QStringList MultiSelectComboBox::currentText()
{
	QStringList text_list;
	if (!line_edit_->text().isEmpty())
	{
		//��;Ϊ�ָ����ָ��ַ���
		text_list = line_edit_->text().split(':');
	}
	return text_list;
}

int MultiSelectComboBox::count() const
{
	int count = list_widget_->count() - 1;
	if (count < 0)
	{
		count = 0;
	}
	return count;
}

void MultiSelectComboBox::SetSearchBarPlaceHolderText(const QString _text)
{
	search_bar_->setPlaceholderText(_text);
}

void MultiSelectComboBox::SetPlaceHolderText(const QString& _text)
{
	line_edit_->setPlaceholderText(_text);
}

void MultiSelectComboBox::ResetSelection()
{
	int count = list_widget_->count();
	for (int i = 1; i < count; i++)
	{
		//��ȡ��Ӧλ�õ�QWidget����
		QWidget* widget = list_widget_->itemWidget(list_widget_->item(i));
		//��QWidget����ת���ɶ�Ӧ������
		QCheckBox* check_box = static_cast<QCheckBox*>(widget);
		check_box->setChecked(false);
	}
}

void MultiSelectComboBox::clear()
{
	line_edit_->clear();
	list_widget_->clear();
	QListWidgetItem* currentItem = new QListWidgetItem(list_widget_);
	search_bar_->setPlaceholderText("Search.........");
	search_bar_->setClearButtonEnabled(true);
	list_widget_->addItem(currentItem);
	list_widget_->setItemWidget(currentItem, search_bar_);
	SetSearchBarHidden(hidden_flag_);
	connect(search_bar_, SIGNAL(textChanged(const QString&)), this, SLOT(onSearch(const QString&)));
}

void MultiSelectComboBox::TextClear()
{
	line_edit_->clear();
	ResetSelection();
}

void MultiSelectComboBox::setCurrentText(const QString& _text)
{
	int count = list_widget_->count();
	for (int i = 1; i < count; i++)
	{
		//��ȡ��Ӧλ�õ�QWidget����
		QWidget* widget = list_widget_->itemWidget(list_widget_->item(i));
		//��QWidget����ת���ɶ�Ӧ������
		QCheckBox* check_box = static_cast<QCheckBox*>(widget);
		if (_text.compare(check_box->text()))
			check_box->setChecked(true);
	}
}

void MultiSelectComboBox::setCurrentText(const QStringList& _text_list)
{
	int count = list_widget_->count();
	for (int i = 1; i < count; i++)
	{
		//��ȡ��Ӧλ�õ�QWidget����
		QWidget* widget = list_widget_->itemWidget(list_widget_->item(i));
		//��QWidget����ת���ɶ�Ӧ������
		QCheckBox* check_box = static_cast<QCheckBox*>(widget);
		if (_text_list.contains(check_box->text()))
			check_box->setChecked(true);
	}
}

void MultiSelectComboBox::SetSearchBarHidden(bool _flag)
{
	hidden_flag_ = _flag;
	list_widget_->item(0)->setHidden(hidden_flag_);
}

QVector<int> MultiSelectComboBox::Get_idItems()
{
	QVector<int> idItems;
	int count = list_widget_->count();
	for (int i = 1; i < count; i++)
	{
		QWidget* widget = list_widget_->itemWidget(list_widget_->item(i));
		QCheckBox* check_box = static_cast<QCheckBox*>(widget);
		if (check_box->isChecked())
		{
			idItems.push_back(i);
		}
	}
	return idItems;
	
}

bool MultiSelectComboBox::eventFilter(QObject* watched, QEvent* event)
{
	//���õ�������Ҳ���Ե���������
	if (watched == line_edit_ && event->type() == QEvent::MouseButtonRelease && this->isEnabled())
	{
		showPopup();
		return true;
	}
	return false;
}

void MultiSelectComboBox::wheelEvent(QWheelEvent* event)
{
	//����QComboBoxĬ�ϵĹ����¼�
	Q_UNUSED(event);
}

void MultiSelectComboBox::keyPressEvent(QKeyEvent* event)
{
	QComboBox::keyPressEvent(event);
}

void MultiSelectComboBox::stateChange(int _row)
{
	Q_UNUSED(_row);
	QString selected_data("");
	int count = list_widget_->count();
	for (int i = 1; i < count; i++)
	{
		QWidget* widget = list_widget_->itemWidget(list_widget_->item(i));
		QCheckBox* check_box = static_cast<QCheckBox*>(widget);
		if (check_box->isChecked())
		{
			selected_data.append(check_box->text()).append(";");
		}
	}
	selected_data.chop(1);
	if (!selected_data.isEmpty())
	{
		line_edit_->setText(selected_data);
	}
	else
	{
		line_edit_->clear();
	}
	line_edit_->setToolTip(selected_data);
	emit selectionChange(selected_data);
}

void MultiSelectComboBox::onSearch(const QString& _text)
{
	for (int i = 1; i < list_widget_->count(); i++)
	{
		QCheckBox* check_box = static_cast<QCheckBox*>(list_widget_->itemWidget(list_widget_->item(i)));
		//�ı�ƥ������ʾ����֮����
		//Qt::CaseInsensitiveģ����ѯ
		if (check_box->text().contains(_text, Qt::CaseInsensitive))
			list_widget_->item(i)->setHidden(false);
		else
			list_widget_->item(i)->setHidden(true);
	}
}

void MultiSelectComboBox::itemClicked(int _index)
{
	if (_index != 0)
	{
		QCheckBox* check_box = static_cast<QCheckBox*>(list_widget_->itemWidget(list_widget_->item(_index)));
		check_box->setChecked(!check_box->isChecked());

	}
}

