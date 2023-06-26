#pragma once
#include"Node.h"
#include <vtkRenderer.h>

//class InterFace;
class test_pic_focusforce
{
public:

	Node* m_node = nullptr;//选中的点
	
	//取点
	double x;
	double y;
	double z;
	int m_x;//集中力x的大小
	int m_y;//集中力y的大小
	int m_z;//集中力z的大小
	void creat(vtkRenderer* renderer);
	void test_x(int a, vtkRenderer* renderer);//a传1或者-1
	void test_y(int a, vtkRenderer* renderer);//a传1或者-1
	void test_z(int a, vtkRenderer* renderer);//a传1或者-1
};

