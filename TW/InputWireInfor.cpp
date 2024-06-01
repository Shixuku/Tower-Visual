#include "InputWireInfor.h"
#include <QFileDialog>
#include <iostream>
#include<filesystem>
#include"WireData.h"
#include"InterFace.h"
#include"TowerWireGroup.h"
#include"CreateStrainWire.h"
#pragma execution_character_set("utf-8")

//void InputWireInfor::ReadWireInfor(InterFace* InterFace)
//{
//    m_pInterFace = InterFace;
//	QString str = QFileDialog::getOpenFileName( nullptr,"��", "/", "textfile(*.txt);;All files(*.*)");
//    if (str == nullptr)
//    {
//        std::cout << "open failed\n";
//        return;
//    }
//    qDebug() << str;
//    Input_FemData(str);
//    //Show();
//}

void InputWireInfor::ReadWireInfor(InterFace* InterFace)
{
    m_pInterFace = InterFace;
    vector<string> fileNames;
    string  path("../HangPointTXT"); 	//�Լ�ѡ��Ŀ¼����
     GetAllHangPointTXT(path, fileNames);
    for (int i = 0; i < fileNames.size(); i++)
    {
        towerWire = nullptr;
       // QString str = QString::fromStdString(fileNames[i]);
        QString str = QString(QString::fromLocal8Bit(fileNames[i].data()));//��ֹת������
        Input_FemData(str);
       
    }

}

void InputWireInfor::GetAllHangPointTXT(string path, vector<string>& files)
{
    intptr_t hFile = 0;
    //�ļ���Ϣ
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //�����Ŀ¼,�ݹ����
            //�������,���ļ�����·������vector��
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (string(fileinfo.name).substr(string(fileinfo.name).find_last_of(".") + 1) == "txt")
                {
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                }
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

bool InputWireInfor::Input_FemData(const QString& FileName)
{
    if (!QFile::exists(FileName))
    {//�ļ�������
        qDebug() << "Error: �ļ� " << FileName << " ������";
        exit(1);
        return false;
    }

    QFileInfo fileInfo(FileName);

    QString baseName = fileInfo.baseName(); // ��ȡ�ļ�����������·������չ��

    for (auto& i: m_pInterFace->TWG)
    {
        if (i.second->m_Name == baseName)
        {
            towerWire = i.second;
            break;
        }
    }
    wd = new WireData();
    QFile aFile(FileName);//�����ļ�
    aFile.open(QIODevice::ReadOnly);//���ļ����ж���
    QTextStream ssin(&aFile);//���ļ���

    QString strdata;
    while (ReadLine(ssin, strdata))
    {//��ȡ����Ч�����У������strdata��
        QStringList list_str = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//���ÿո�,�ֽ��ַ���
        if (list_str[0].compare("*Node", Qt::CaseInsensitive) == 0)
        {//������Ϊ�ڵ�ؼ�����
            Q_ASSERT(list_str.size() == 2);
            int nNode = list_str[1].toInt();//�õ��ڵ�������ɿ��ƺ���ѭ������
            qDebug() << "\n�ڵ���: " << nNode;
            for (int i = 0; i < nNode; ++i)
            {
                //������һ����Ч����
                if (!ReadLine(ssin, strdata))
                {//û�ж�ȡ����Ч���ݣ��˳�
                    qDebug() << "Error: �ڵ����ݲ���";
                    exit(1);
                }

                QStringList strlist_node = strdata.split(QRegularExpression("[\t, ]"), Qt::SkipEmptyParts);//���ÿո�,�ֽ��ַ���
                Q_ASSERT(strlist_node.size() == 4);
                int idNode = strlist_node[0].toInt();//ȡ�ýڵ��
                double xi = strlist_node[1].toDouble();
                double yi = strlist_node[2].toDouble();
                double zi = strlist_node[3].toDouble();
                qDebug() << idNode << "  " << xi << " " << yi << " " << zi;//������Ա���
                //...�����������籣�浽ģ�����ݿ�
            }
        }
        else if (list_str[0].compare("*Section", Qt::CaseInsensitive) == 0)//���߹�������
        {//��ȡ���ҵ�
            Q_ASSERT(list_str.size() == 2);

            int nPro = list_str[1].toInt();//
            qDebug() << "\n���Ŷ���: " << nPro;
            for (int i = 0; i < nPro; ++i)
            {
                WireProperties::ReadInfoSus(this, ssin, wd->Property);
            }

        }
        else if (list_str[0].compare("*Sus", Qt::CaseInsensitive) == 0)//���ҵ���Ϣ
        {//��ȡ���ҵ�
            Q_ASSERT(list_str.size() == 3);

            std::vector<Insulator_Base> Insulators;

            int nSus = list_str[1].toInt();//
            wd->fenlie = list_str[2].toInt();
            qDebug() << "\n�ҵ���: " << nSus;
            for (int i = 0; i < nSus; ++i)
            {
                //������һ����Ч����
                if (!ReadLine(ssin, strdata))
                {//û�ж�ȡ����Ч���ݣ��˳�
                    qDebug() << "Error: �ҵ����ݲ���";
                    exit(1);
                }
                //...�����������籣�浽ģ�����ݿ�
                Insulator_Base Insulator;
                Insulator.ReadInfoSus(strdata);

                Insulators.push_back(Insulator);
            }

            //�������Ŷη�
            for (auto& Insulator : Insulators)
            {
                int id = Insulator.m_idSegment;
                auto iFind = wd->Test_a.find(id);
                if (iFind == wd->Test_a.end())//û�ҵ�
                {
                    wd->Test_a.insert(std::make_pair(id, std::vector<Insulator_Base>{Insulator}));
                }
                else
                {
                    wd->Test_a[id].push_back(Insulator);
                }
            }

            //������·id����
            for (auto& segment : wd->Test_a)//�������еĶ�
            {
                vector<Insulator_Base>& temp = segment.second;
                sort_line(temp);
                //std::sort(temp.begin(), temp.end(), cmp);
            }

        }
        CreateStrainWire* c = new CreateStrainWire();
        c->fenlie = wd->fenlie;
        c->Test_a = wd->Test_a;
        c->Property = wd->Property;
        c->Create_Mesh();
        towerWire->AddStrainWireToGroup(c);
    }
    aFile.close();//�ر��ļ�
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


