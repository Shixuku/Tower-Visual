#pragma once
/*
* ���ڶ����ţ��Լ������ݽ��й����Զ���š���ӡ�ɾ�������ҡ���ȡ�����棩�Ĺ�����
*/
#include <unordered_map>
#include <set>
#include<QVector>

#include <QDataStream>

using namespace std;

#define LLI long long

template<class ET>
class Manage_Entity : public std::unordered_map<LLI, ET*>
{//����ָ������ࣺ��ֵΪ��ţ�����Ϊ����ָ��
private:
	LLI m_idUsable;//��ǰ��С���ñ��
	LLI m_idMax;//��ǰ�����

	template <class T2>
	void Read_Stream(QDataStream& s)
	{
		T2* pEntity = new T2;
		pEntity->Input(s);
		LLI idEntity = pEntity->Get_id();//������
		qDebug() << typeid(*pEntity).name() << "  " << idEntity;

		//���ԭ����ŵĶ���
		auto iFind = this->find(idEntity);
		if (iFind != this->end()) delete iFind->second;

		ET* pT = dynamic_cast<ET*>(pEntity);//����ת��
		Q_ASSERT(pT != nullptr);
		this->insert(std::pair<LLI, ET*>(idEntity, pT));
		Update_idUsable();
		if (idEntity > m_idMax) m_idMax = idEntity;//���µ�ǰ�����
	}

public:
	LLI Get_idMax() const { return m_idMax; }//���������

	void Save(QDataStream& s) const
	{//�洢����
		LLI n = this->size();
		qDebug() << "n = " << n;
		s << n;//����������
		for (const auto& i : *this)
		{
			const ET* pEntity = i.second;
			qDebug() << pEntity->m_id;
			s << pEntity->My_EntityType();
			pEntity->SaveTo(s);
		}
	}


	void Read(QDataStream& s)
	{//��ȡ����
		LLI n = 0;
		s >> n;//�����������
		qDebug() << n;

		for (LLI i = 0; i < n; ++i)
		{
			enum Entity_Type iType;
			s >> iType;//������������
			switch (iType)
			{
			case  Entity_Type::ET_Node:
				Read_Stream<Node_Base>(s); break;
			case Entity_Type::ET_LineElement:
				Read_Stream<Element_Base>(s); break;

			default:
			{
				QString cs("��֧�ֵĶ�������");
				cs += QString::number(int(iType));
				qDebug() << cs;
			}
			break;
			}
		}
		Update_idUsable();//������С���ñ��
	}


	void Get_Array(QVector<ET*>& VE, bool bSort = false) const
	{//�õ�������ʽ������
		const LLI n = this->size();
		VE.resize(n);
		LLI k = n - 1;
		for (const auto& i : *this)
		{
			VE[k--] = i.second;
		}

		if (bSort)
		{
			for (LLI i = 0; i < n; ++i)
			{
				LLI imin = i;
				for (LLI j = i + 1; j < n; ++j)
				{
					if (VE[j]->Get_id() < VE[imin]->Get_id())
					{
						imin = j;
					}
				}
				if (i != imin) std::swap(VE[i], VE[imin]);
			}
		}
	}

	//template <class T2>
	//void Get_set(std::set<T2*, PtrCmp>& Entity_set) const
	//{//ȡ�ð���������set
	//	for (auto i : *this)
	//	{
	//		T2* pEntity = dynamic_cast<T2*>(i.second);
	//		if (pEntity != nullptr)
	//		{
	//			Entity_set.insert(pEntity);
	//		}
	//	}
	//}

	void Add_Entity(ET* pEntity)
	{//��Map�м���һ������ָ��
		LLI idEntity = pEntity->Get_id();//�������ı��

		auto i = this->find(idEntity);
		if (i != this->end())
		{//�ҵ�ԭ������
			ET* pFind = i->second;
			if (pFind != pEntity)
			{//ԭ���Ĳ������¼����
				(*this).erase(i);
				delete pFind;
			}
			else
			{//ԭ���ĵ����¼����
				return;
			}
		}
		this->insert(std::pair<LLI, ET*>(idEntity, pEntity));
		Update_idUsable();//������С���ñ��
		if (idEntity > m_idMax) m_idMax = idEntity;//���µ�ǰ�����
	}

	LLI Get_New_id()
	{//�õ��±��
		return m_idUsable;
	}

	bool Delete_Entity(LLI idEntity)
	{//���ݱ�ţ�ɾ����Ӧ�Ķ���
		auto i = this->find(idEntity);
		if (i != this->end())
		{//�ҵ�����
			ET* pEntity = i->second;
			delete pEntity;
			this->erase(i);
			if (m_idUsable > idEntity) m_idUsable = idEntity;
			Update_idMax();//���µ�ǰ�����
			return true;
		}
		else
		{
			return false;
		}
	}
	void Delete_All()
	{//ɾ��ȫ������
		for (auto i : *this)
		{
			delete i.second;
		}
		this->clear();
		m_idMax = 0;
		m_idUsable = 1;
	}

	ET* Find_Entity(LLI idEntity) const
	{//���ݱ�ţ����Ҷ��󣬷��ض����ָ��
		auto i = this->find(idEntity);
		if (i != this->end())
		{//�ҵ�����
			return i->second;
		}
		else
		{//���󲻴���
			return nullptr;
		}
	}

	void Update_idUsable()
	{//������С���ñ��
		while (Find_Entity(m_idUsable) != nullptr)
		{
			++m_idUsable;
		}
	}

	void Update_idMax()
	{//���������
		while (m_idMax > 0 && Find_Entity(m_idMax) == nullptr)
		{
			--m_idMax;
		}
	}

	Manage_Entity()
	{
		m_idMax = 0;
		m_idUsable = 1;
	}

	virtual ~Manage_Entity(void)
	{
		Delete_All();
	}
};

