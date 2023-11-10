#pragma once

#include <QComboBox>
#include <QListWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QEvent>

class MultiSelectComboBox : public QComboBox
{
	Q_OBJECT

public:
	MultiSelectComboBox(QWidget* parent = Q_NULLPTR);
	~MultiSelectComboBox();

	//����������
	virtual void hidePopup();

	//���һ��ѡ��
	void addItem(const QString& _text, const QVariant& _variant = QVariant());

	//��Ӷ���ѡ��
	void addItems(const QStringList& _text_list);

	//���ص�ǰѡ��ѡ��
	QStringList currentText();

	//���ص�ǰѡ������
	int count()const;

	//����������Ĭ������
	void SetSearchBarPlaceHolderText(const QString _text);

	//�����ı���Ĭ������
	void SetPlaceHolderText(const QString& _text);

	//������״̬�ָ�Ĭ��
	void ResetSelection();

	//�����������
	void clear();

	//�ı����������
	void TextClear();

	//����ѡ���ı�--��
	void setCurrentText(const QString& _text);

	//����ѡ���ı�--��
	void setCurrentText(const QStringList& _text_list);

	//�����������Ƿ����
	void SetSearchBarHidden(bool _flag);

	//ѡ�е�item�ı��
	QVector<int> Get_idItems();
	QVector<QString> Get_idItemsText();

protected:

	//�¼�������
	virtual bool eventFilter(QObject* watched, QEvent* event);

	//�����¼�
	virtual void wheelEvent(QWheelEvent* event);

	//�����¼�
	virtual void keyPressEvent(QKeyEvent* event);

private slots:

	//�ۺ������ı����ı��仯
	void stateChange(int _row);

	//�ۺ������������ı��仯
	void onSearch(const QString& _text);

	//�ۺ��������������ѡ��
	void itemClicked(int _index);

signals:

	//�źţ����͵�ǰѡ��ѡ��
	void selectionChange(const QString _data);

private:
	//������
	QListWidget* list_widget_;
	//�ı���������
	QLineEdit* line_edit_, * search_bar_;
	//��������ʾ��־
	bool hidden_flag_;
	//��������ʾ��־
	bool show_flag_;
};

