/*****************************************************************************
* \file mainwindow.h
* \author Zirui Xu
* \date 2023/04/13
* \version v0.5
* \brief 实现了图的各类算法以及图的整理
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks
******************************************************************************/

#ifndef LOGIC_H
#define LOGIC_H


#include "element.h"


class Logic
{

public:


    Logic(QList <Element*> elementList);


    Logic();


    int findFromNode(ArrowElement* edge);


    int findToNode(ArrowElement* edge);


    void DFS(int u);


    void BFS(int u);


private:


    QVector <NodeElement*> nodes;
    QVector <ArrowElement*> edges;
    QVector <int> graph[1000];
    QVector <int> vis;


};

#endif // LOGIC_H
