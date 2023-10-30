#include "InputWireInfor.h"
#include <QFileDialog>
#include"WireData.h"
#include"InterFace.h"
#include"TowerWireGroup.h"
#include"CreateStrainWire.h"
void InputWireInfor::ReadWireInfor(InterFace* InterFace)
{
    m_pInterFace = InterFace;
	QString str = QFileDialog::getOpenFileName( nullptr,"打开", "/", "textfile(*.txt);;All files(*.*)");
    if (str == nullptr)
    {
        std::cout << "open failed\n";
        return;
    }
    qDebug() << str;
    Input_FemData(str);
    //Show();
}

bool InputWireInfor::Input_FemData(const QString& FileName)
{
    if (!QFile::exists(FileName))
    {//文件不存在
        qDebug() << "Error: 文件 " << FileName << " 不存在";
        exit(1);
        return false;
    }
    wd = new WireData();
    QFile aFile(FileName);//定义文件
    aFile.open(QIODevice::ReadOnly);//打开文件进行读入
    QTextStream ssin(&aFile);//绑定文件流

    QString strdata;
    while (ReadLine(ssin, strdata))
    {//读取到有效数据行，存放在strdata中
        QStringList list_str = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
        if (list_str[0].compare("*Node", Qt::CaseInsensitive) == 0)
        {//数据行为节点关键字行
            Q_ASSERT(list_str.size() == 2);
            int nNode = list_str[1].toInt();//得到节点个数，可控制后续循环次数
            qDebug() << "\n节点数: " << nNode;
            for (int i = 0; i < nNode; ++i)
            {
                //继续读一行有效数据
                if (!ReadLine(ssin, strdata))
                {//没有读取到有效数据，退出
                    qDebug() << "Error: 节点数据不够";
                    exit(1);
                }

                QStringList strlist_node = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//利用空格,分解字符串
                Q_ASSERT(strlist_node.size() == 4);
                int idNode = strlist_node[0].toInt();//取得节点号
                double xi = strlist_node[1].toDouble();
                double yi = strlist_node[2].toDouble();
                double zi = strlist_node[3].toDouble();
                qDebug() << idNode << "  " << xi << " " << yi << " " << zi;//输出，以便检查
                //...其他操作，如保存到模型数据库
            }
        }
        else if (list_str[0].compare("*Section", Qt::CaseInsensitive) == 0)//导线固有属性
        {//读取到挂点
            Q_ASSERT(list_str.size() == 2);

            int nPro = list_str[1].toInt();//
            qDebug() << "\n耐张段数: " << nPro;
            for (int i = 0; i < nPro; ++i)
            {
                WireProperties::ReadInfoSus(this, ssin, wd->Property);
            }

        }
        else if (list_str[0].compare("*Sus", Qt::CaseInsensitive) == 0)//悬挂点信息
        {//读取到挂点
            Q_ASSERT(list_str.size() == 2);

            std::vector<Insulator_Base> Insulators;

            int nSus = list_str[1].toInt();//
            qDebug() << "\n挂点数: " << nSus;
            for (int i = 0; i < nSus; ++i)
            {
                //继续读一行有效数据
                if (!ReadLine(ssin, strdata))
                {//没有读取到有效数据，退出
                    qDebug() << "Error: 挂点数据不够";
                    exit(1);
                }
                //...其他操作，如保存到模型数据库
                Insulator_Base Insulator;
                Insulator.ReadInfoSus(strdata);

                Insulators.push_back(Insulator);
            }

            //根据耐张段分
            for (auto& Insulator : Insulators)
            {
                int id = Insulator.m_idSegment;
                auto iFind = wd->Test_a.find(id);
                if (iFind == wd->Test_a.end())//没找到
                {
                    wd->Test_a.insert(std::make_pair(id, std::vector<Insulator_Base>{Insulator}));
                }
                else
                {
                    wd->Test_a[id].push_back(Insulator);
                }
            }

            //根据线路id排序
            for (auto& segment : wd->Test_a)//遍历已有的段
            {
                vector<Insulator_Base>& temp = segment.second;
                sort_line(temp);
                //std::sort(temp.begin(), temp.end(), cmp);
            }

        }
    }
    aFile.close();//关闭文件
    return true;
}


void InputWireInfor::sort_line(vector<Insulator_Base>& object)
{
    int num = object.size();

    for (int i = 0; i < num - 1; i++) 
    {
        for (int j = 0; j < num - 1 - i; j++)
        {
            if (object[j].m_line > object[j + 1].m_line)
            {
                Insulator_Base temp = object[j];
                object[j] = object[j + 1];
                object[j + 1] = temp;
            }
        }
    }
}




void InputWireInfor::Show()
{
    towerWire = new TowerWireGroup();
    towerWire->Suspensioncombined();
    towerWire->VectorToMap();
    CreateStrainWire* c = new CreateStrainWire();
    c->Test_a = wd->Test_a;
    c->Property = wd->Property;
    c->Create_Mesh();
    c->StrainAllRestraintNode = wd->StrainAllRestraintNode;
    c->StrainJointRestraintNode = wd->StrainJointRestraintNode;
    towerWire->AddStrainWireToGroup(c);
    towerWire->Show_VTKnode(m_pInterFace->m_Renderer);
    towerWire->Show_VTKbeam(m_pInterFace->m_Renderer);
    towerWire->Show_VTKtruss(m_pInterFace->m_Renderer);
    m_pInterFace->m_Renderer->ResetCamera();
}
