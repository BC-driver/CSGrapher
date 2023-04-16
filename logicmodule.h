/*****************************************************************************
* \file element.h
* \author Zirui Xu
* \date 2023/04/16
* \version v0.5
* \brief 实现图的抽象化,遍历,排布及其可视化
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 添加Element类的抽象化使其更易于处理
*          实现BFS,DFS两种搜索方式
*          实现多边形排布
******************************************************************************/
#ifndef LOGICMODULE_H
#define LOGICMODULE_H

#include <QObject>
#include "element.h"
#include <QMap>
#include <QColor>

class LogicModule : public QObject
{
    Q_OBJECT
public:


    explicit LogicModule(QObject *parent = nullptr);


    void sleepMsec(int msec);


    void clearVis();


    int findFromNode(ArrowElement* edge);


    int findToNode(ArrowElement* edge);


    int findNodeIndex(NodeElement* node);


    void highlightEdge(int u, int v);


    void highlightNode(int u);


    void deHighlightEdge(int u, int v);


    void deHighlightNode(int u);


    void DFS(int u);


    void BFS(int s);


    void polyLayout(int s);


    QPointF rotateN(QPointF ori, double n);


    void update(QList <Element*> elementList);


private:

    struct edge{
        int u, v;
        bool isAdditional;
    };
    QVector <NodeElement*> nodes;
    QVector <ArrowElement*> edges;
    QVector <int> graph[1000];
    QVector <edge> path;
    QVector <int> vis;
    QColor highlightColor;
    QMap <int, QColor> originNodeColor, originEdgeColor;


signals:


    void updatePainter();

};

#endif // LOGICMODULE_H
