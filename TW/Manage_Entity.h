#pragma once
/*
* ���ڶ����ţ��Լ������ݽ��й����Զ���š���ӡ�ɾ�������ҡ���ȡ�����棩�Ĺ�����
*/
#include <unordered_map>
#include <set>
#include<QVector>
#include<QDataStream>
#  define LLI long long
template<class ET>
class Manage_Entity : public std::unordered_map<LLI, ET*>
{//����ָ������ࣺ��ֵΪ��ţ�����Ϊ����ָ��
private:
	LLI m_idUsable;//��ǰ��С���ñ��
	LLI m_idMax;//��ǰ�����

	template <class T2>
	void Read_Stream(QDataStream& s,int size)
	{
		T2* pEntity = new T2;
		pEntity->Input(s);
		LLI idEntity = pEntity->Get_id() + size;//������

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
		qDebug() << "n = " << n;//����
		s << n;//����������
		for (const auto& i : *this)
		{
			const ET* pEntity = i.second;
			s << pEntity->My_PartType();
			pEntity->SaveTo(s);
		}
	}
	void Read(QDataStream& s,int size)
	{//��ȡ����
		LLI n = 0;
		s >> n;//�����������
		qDebug() << n;

		for (LLI i = 0; i < n; ++i)
		{
			enum Part_Type iType;

			s >> iType;//������������
			switch (iType)
			{
			case Part_Type::ET_PartLeg:
				Read_Stream<TowerPart_leg>(s, size); break;
			case Part_Type::ET_PartBody:
				Read_Stream<TowerPart_body>(s, size); break;
			case Part_Type::ET_PartCrossArm:
				Read_Stream<TowerPart_CrossArm>(s, size); break;
			//case Part_Type::ET_PartInsulator:
			//	Read_Stream<TowerPart_Insulator>(s); break;
			case Part_Type::ET_Tower:
				Read_Stream<Tower>(s, size); break;
			//case Part_Type::ET_TowerWireGroup:
			//	Read_Stream<TowerWireGroup>(s, size); break;
			//case Part_Type::ET_Section:
			//	Read_Stream<Section>(s, size); break;

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
			//cout << "�ҵ�����!" << "\n";
			return i->second;
		}
		else
		{//���󲻴���
			//cout << "û�ҵ�����!" << "\n";
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

