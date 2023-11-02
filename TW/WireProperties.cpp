#include "WireProperties.h"
#include "InputWireInfor.h"
#include "InterFace.h"
#include "Material.h"
#include "Section_L.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
#pragma execution_character_set("utf-8")    
#endif

void WireProperties::ReadInfoSus(InputWireInfor* in, QTextStream& ssin, std::vector<WireProperties>&Propertys)
{
    QString strdata;

    //tension,i
    in->ReadLine(ssin, strdata);
    QStringList list_str = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
    Q_ASSERT(list_str.size() == 4);
    int idT = list_str[1].toInt();//

    //导线
    in->ReadLine(ssin, strdata);
    list_str = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
    Q_ASSERT(list_str.size() == 4);

    WireProperties property;

    double force = list_str[1].toDouble();//最低点轴力
    QString mat = list_str[2];//材料
    QString sec = list_str[3];//截面

    double Density = 0, A = 0; 
    InterFace* pInterFace = Get_InterFace();
    for (const auto& j : pInterFace->ME_Material)
    {
        if (j.second->m_Name == mat)
        {
            Density = j.second->Density;
        }
    }
    for (const auto& j : pInterFace->ME_Section_L)
    {
        if (j.second->m_Name == sec)
        {
            A = j.second->A;
        }
    }
    int Ma_id = GetSectionId(mat,sec, pInterFace);
    property.m_id = 0;
    property.MaId = Ma_id;
    property.idTension = idT;
    property.w_f = force;
    property.w_rou = Density * A;
    property.w_area = A;

    Propertys.push_back(property);

    //地线
    in->ReadLine(ssin, strdata);
    list_str = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
    Q_ASSERT(list_str.size() == 2);
    int num = list_str[1].toInt();//
    for (int i = 0; i < num; ++i)
    {
        WireProperties iproperty;

        //继续读一行有效数据
        if (!in->ReadLine(ssin, strdata))
        {//没有读取到有效数据，退出
            qDebug() << "Error: 地线数据不够";
        }

        QStringList strlist_node = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
        Q_ASSERT(strlist_node.size() == 4);

        int id = strlist_node[0].toInt();
        double force = strlist_node[1].toDouble();//最低点轴力
        QString imat = strlist_node[2];//材料
        QString isec = strlist_node[3];//截面

        double iDensity = 0, iA = 0;  
        InterFace* pInterFace = Get_InterFace();
        for (const auto& j : pInterFace->ME_Material)
        {
            if (j.second->m_Name == imat)
            {
                iDensity = j.second->Density;
            }
        }
        for (const auto& j : pInterFace->ME_Section_L)
        {
            if (j.second->m_Name == isec)
            {
                iA = j.second->A;
            }
        }
        int iMa_id = GetSectionId(imat, isec, pInterFace);
        iproperty.m_id = id;
        iproperty.MaId = iMa_id;
        iproperty.idTension = idT;
        iproperty.isGround = true;
        iproperty.w_f = force;
        iproperty.w_rou = iDensity * iA;
        iproperty.w_area = iA;

        Propertys.push_back(iproperty);
    }
}

int WireProperties::GetSectionId(QString MaterialName, QString SectionName, InterFace* m_pInterFace)
{
    int id = 0;
    int sectionSize = m_pInterFace->Ms.size();
    int MaterialId = 0;
    int SectionId = 0;
    double Iu = 0;
    double Iv = 0;
    double J = 0;
    double Area = 0;
    for (const auto& j : m_pInterFace->ME_Material)
    {
        if (j.second->m_Name == MaterialName)
        {

            MaterialId = j.second->m_id;
        }
    }
    for (const auto& j : m_pInterFace->ME_Section_L)
    {
        if (j.second->m_Name == SectionName)
        {
            SectionId = j.second->m_id;
            Iu = j.second->Iu;
            Iv = j.second->Iv;
            J = j.second->J;
            Area = j.second->A;
        }
    }
    if (sectionSize == 0)
    {
        Section* section = new Section(1, SectionName, Iu, Iv, J, MaterialId, Area);
        m_pInterFace->Ms.Add_Entity(section);
        id = 1;
    }
    else
    {
        for (const auto& j : m_pInterFace->Ms)
        {
            if (j.second->m_Name == SectionName)
            {
                if (j.second->ClassM = MaterialId)
                {
                    id = j.second->m_id;
                }
            }
        }
        if (id == 0)
        {
            id = m_pInterFace->Ms.size() + 1;
            Section* section = new Section(id, SectionName, Iu, Iv, J, MaterialId, Area);
            m_pInterFace->Ms.Add_Entity(section);
        }
    }
    return id;
}
