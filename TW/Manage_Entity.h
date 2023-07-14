#pragma once
/*
* 基于对象编号，对集合数据进行管理（自动编号、添加、删除、查找、读取、保存）的管理类
*/
#include <unordered_map>
#include <set>
#include<QVector>
#include<QDataStream>
#  define LLI long long
template<class ET>
class Manage_Entity : public std::unordered_map<LLI, ET*>
{//对象指针管理类：键值为编号，内容为对象指针
private:
	LLI m_idUsable;//当前最小可用编号
	LLI m_idMax;//当前最大编号

	template <class T2>
	void Read_Stream(QDataStream& s,int size)
	{
		T2* pEntity = new T2;
		pEntity->Input(s);
		LLI idEntity = pEntity->Get_id() + size;//对象编号

		//清除原来编号的对象
		auto iFind = this->find(idEntity);
		if (iFind != this->end()) delete iFind->second;

		ET* pT = dynamic_cast<ET*>(pEntity);//类型转化
		Q_ASSERT(pT != nullptr);
		this->insert(std::pair<LLI, ET*>(idEntity, pT));
		Update_idUsable();
		if (idEntity > m_idMax) m_idMax = idEntity;//更新当前最大编号
	}

public:
	LLI Get_idMax() const { return m_idMax; }//返回最大编号

	void Save(QDataStream& s) const
	{//存储数据
		LLI n = this->size();
		qDebug() << "n = " << n;//个数
		s << n;//存入对象个数
		for (const auto& i : *this)
		{
			const ET* pEntity = i.second;
			s << pEntity->My_PartType();
			pEntity->SaveTo(s);
		}
	}
	void Read(QDataStream& s,int size)
	{//读取数据
		LLI n = 0;
		s >> n;//读出对象个数
		qDebug() << n;

		for (LLI i = 0; i < n; ++i)
		{
			enum Part_Type iType;

			s >> iType;//读出对象类型
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
				QString cs("不支持的对象类型");
				cs += QString::number(int(iType));
				qDebug() << cs;
			}
			break;
			}
		}
		Update_idUsable();//更新最小可用编号
	}
	void Get_Array(QVector<ET*>& VE, bool bSort = false) const
	{//得到向量形式的数据
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
	{//向Map中加入一个对象指针
		LLI idEntity = pEntity->Get_id();//加入对象的编号

		auto i = this->find(idEntity);
		if (i != this->end())
		{//找到原来对象
			ET* pFind = i->second;
			if (pFind != pEntity)
			{//原来的不等于新加入的
				(*this).erase(i);
				delete pFind;
			}
			else
			{//原来的等于新加入的
				return;
			}
		}
		this->insert(std::pair<LLI, ET*>(idEntity, pEntity));
		Update_idUsable();//更新最小可用编号
		if (idEntity > m_idMax) m_idMax = idEntity;//更新当前最大编号
	}

	LLI Get_New_id()
	{//得到新编号
		return m_idUsable;
	}

	bool Delete_Entity(LLI idEntity)
	{//根据编号，删除对应的对象
		auto i = this->find(idEntity);
		if (i != this->end())
		{//找到对象
			ET* pEntity = i->second;
			delete pEntity;
			this->erase(i);
			if (m_idUsable > idEntity) m_idUsable = idEntity;
			Update_idMax();//更新当前最大编号
			return true;
		}
		else
		{
			return false;
		}
	}
	void Delete_All()
	{//删除全部对象
		for (auto i : *this)
		{
			delete i.second;
		}
		this->clear();
		m_idMax = 0;
		m_idUsable = 1;
	}

	ET* Find_Entity(LLI idEntity) const
	{//根据编号，查找对象，返回对象的指针
		auto i = this->find(idEntity);
		if (i != this->end())
		{//找到对象
			//cout << "找到对象!" << "\n";
			return i->second;
		}
		else
		{//对象不存在
			//cout << "没找到对象!" << "\n";
			return nullptr;
		}
	}

	void Update_idUsable()
	{//更新最小可用编号
		while (Find_Entity(m_idUsable) != nullptr)
		{
			++m_idUsable;
		}
	}

	void Update_idMax()
	{//更新最大编号
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

