#pragma once
#include<iostream>
#include<QDataStream>
#include"Node.h"
#include"Element.h"
#include<vtkTransform.h>
#include <vtkIdTypeArray.h>
#include <QOpenGLWidget>//UINT_PTR
#include <vtkPointData.h>//->AddArray(Ptr
enum Part_Type :int
{
	ET_PartLeg, ET_PartBody, ET_PartSeptum, ET_PartCrossArm, ET_PartCrossArmSep, ET_Tower, ET_Section,ET_PartInsulator,ET_TowerWireGroup,
	ET_Wire
};

class InterFace;

class Base
{
public:
	int m_id;
	int Get_id()const { return m_id; }

	static void set_InterFace(InterFace* pInterFace)
	{
		s_InterFace = pInterFace;
	}

	static InterFace* Get_InterFace()
	{
		if (s_InterFace != nullptr)
		{
			return s_InterFace;
		}
		else
		{
			std::cout << "InterFace指针未设置!\n";
			return nullptr;
		}
	}
	//保存数据文件
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
private:
	static InterFace* s_InterFace;
};

