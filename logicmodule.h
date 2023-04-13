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


    void update(QList <Element*> elementList);


private:


    QVector <NodeElement*> nodes;
    QVector <ArrowElement*> edges;
    QVector <int> graph[1000];
    QVector <int> vis;
    QColor highlightColor;
    QMap <int, QColor> originNodeColor, originEdgeColor;


signals:


    void updatePainter();

};

#endif // LOGICMODULE_H
