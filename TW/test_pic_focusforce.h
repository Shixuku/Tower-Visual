#pragma once
#include"Node.h"
#include <vtkRenderer.h>

//class InterFace;
class test_pic_focusforce
{
public:

	Node* m_node = nullptr;//ѡ�еĵ�
	
	//ȡ��
	double x;
	double y;
	double z;
	int m_x;//������x�Ĵ�С
	int m_y;//������y�Ĵ�С
	int m_z;//������z�Ĵ�С
	void creat(vtkRenderer* renderer);
	void test_x(int a, vtkRenderer* renderer);//a��1����-1
	void test_y(int a, vtkRenderer* renderer);//a��1����-1
	void test_z(int a, vtkRenderer* renderer);//a��1����-1
};

