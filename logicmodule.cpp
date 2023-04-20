#include "logicmodule.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QQueue>
#include <QtMath>

LogicModule::LogicModule(QObject *parent) : QObject(parent){
    highlightColor = QColor(255,184,131);
}


void LogicModule::sleepMsec(int msec)
{
    QTime curTimer = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < curTimer)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}


void LogicModule::clearVis(){
    vis.clear();
    for(int i = 0;i < nodes.size();i++){
        vis.push_back(0);
    }
}


int LogicModule::findFromNode(ArrowElement *edge){
    NodeElement* target = (NodeElement*)(edge -> getFromElement());
    for(int i = 0;i < nodes.size();i++){
        if(nodes[i] == target) return i;
    }
    return -1;
}


int LogicModule::findToNode(ArrowElement *edge){
    NodeElement* target = (NodeElement*)(edge -> getToElement());
    for(int i = 0;i < nodes.size();i++){
        if(nodes[i] == target) return i;
    }
    return -1;
}


int LogicModule::findNodeIndex(NodeElement *node){
    for(int i = 0;i < nodes.size();i++){
        if(node == nodes[i]) return i;
    }
    return -1;
}


void LogicModule::highlightEdge(int u, int v){
    for(int i = 0;i < edges.size();i++){
        ArrowElement* &e = edges[i];
        if((e -> getFromElement() == nodes[u] && e -> getToElement() == nodes[v])
           ||
           (!(e -> getIsDirected()) && e -> getFromElement() == nodes[v] && e -> getToElement() == nodes[u])){
            originEdgeColor[i] = e -> getEdgeColor();
            e -> setEdgeColor(highlightColor);
            e -> setLineWidth(e -> getLineWidth() + 3);
            e -> setFontSize(e -> getFontSize() + 3);
            break;
        }
    }
    emit updatePainter();
}


void LogicModule::deHighlightEdge(int u, int v){
    for(int i = 0;i < edges.size();i++){
        ArrowElement* &e = edges[i];
        if((e -> getFromElement() == nodes[u] && e -> getToElement() == nodes[v])
           ||
           (!(e -> getIsDirected()) && e -> getFromElement() == nodes[v] && e -> getToElement() == nodes[u])){
            e -> setEdgeColor(originEdgeColor[i]);
            e -> setLineWidth(e -> getLineWidth() - 3);
            e -> setFontSize(e -> getFontSize() - 3);
            break;
        }
    }
    emit updatePainter();
}


void LogicModule::highlightNode(int u){
    NodeElement* &node = nodes[u];
    originNodeColor[u] = node -> getEdgeColor();
    node -> setEdgeColor(highlightColor);
    node -> setLineWidth(node -> getLineWidth() + 3);
    emit updatePainter();
}


void LogicModule::deHighlightNode(int u){
    NodeElement* &node = nodes[u];
    node -> setEdgeColor(originNodeColor[u]);
    node -> setLineWidth(node -> getLineWidth() - 3);
    emit updatePainter();
}


void LogicModule::DFS(int u){
    vis[u] = 1;
    highlightNode(u);
    sleepMsec(500);
    for(auto idx : graph[u]){
        edge &e = path[idx];
        if(e.isAdditional) continue;
        if(!vis[e.v]){
            highlightEdge(u, e.v);
            DFS(e.v);
            deHighlightEdge(u, e.v);
        }
    }
    deHighlightNode(u);
    vis[u] = 0;
}


void LogicModule::BFS(int s){
    clearVis();
    QQueue <int> q, visNodes, visPaths;
    q.push_back(s);
    visNodes.push_back(s);
    highlightNode(s);
    while(!q.empty()){
        int u = q.front();
        q.pop_front();
        vis[u] = 1;
        for(auto idx : graph[u]){
            edge &e = path[idx];
            if(e.isAdditional) continue;
            if(!vis[e.v]){
                vis[e.v] = 1;
                q.push_back(e.v);
                visNodes.push_back(e.v);
                visPaths.push_back(idx);
                highlightNode(e.v);
                highlightEdge(u, e.v);
                sleepMsec(500);
            }
        }
    }
    sleepMsec(1000);
    for(auto u : visNodes){
        deHighlightNode(u);
    }
    for(auto p : visPaths){
        edge &e = path[p];
        deHighlightEdge(e.u, e.v);
    }
}


void LogicModule::polyLayout(int s){
    clearVis();
    QQueue <int> q, visNodes;
    q.push_back(s);
    visNodes.push_back(s);
    vis[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop_front();
        for(auto idx : graph[u]){
            edge &e = path[idx];
            if(!vis[e.v]){
                vis[e.v] = 1;
                q.push_back(e.v);
                visNodes.push_back(e.v);
            }
        }
    }
    double n = visNodes.size();
    QPointF ori(nodes[s]->getXPos(), nodes[s]->getYPos()), toCenter(50.0, 50.0 / qTan(M_PI / n));
    QPointF toTarget = -toCenter;
    for(int i = 1;i < n;i++){
        toTarget = rotateN(toTarget, n);
        nodes[visNodes[i]] -> setPos(ori + toCenter + toTarget);
    }
    emit updatePainter();
}


bool cmp(QPair<int, int> a, QPair<int, int> b){
    if(a.second == b.second) return a.first < b.first;
    else return a.second < b.second;
}


void LogicModule::treeLayout(int s){
    clearVis();
    int inf = 0x3f3f3f3f;
    double vSpacing = 100, hSpacing = 100;
    QQueue <int> q;
    QVector <int> level(nodes.size() + 5, inf), pre(nodes.size() + 5, -1);
    int minlevel = 1, maxlevel = -inf;
    q.push_back(s);
    level[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop_front();
        for(auto idx : graph[u]){
            edge &e = path[idx];
            if(!vis[e.v]){
                level[e.v] = level[u] + (e.isAdditional ? -1 : 1);
                if(e.isAdditional){
                    pre[u] = e.v;
                }

                else{
                    pre[e.v] = u;
                }
                minlevel = qMin(minlevel, level[e.v]);
                maxlevel = qMax(maxlevel, level[e.v]);
                vis[e.v] = 1;
                q.push_back(e.v);
            }
        }
    }
    QVector <QPair<int, int> > tree[maxlevel - minlevel + 2];
//    for(int i = 0;i < nodes.size();i++){
//        qDebug() << level[i] << " " << nodes[i] -> getContext();
//    }
    for(int i = 0;i < nodes.size();i++){
        if(level[i] != inf){
            tree[level[i] - minlevel].push_back(qMakePair(i, pre[i]));
        }
    }
    int maxNodeCnt = 0;
    for(int i = 0;i <= maxlevel - minlevel;i++){
        std::sort(tree[i].begin(), tree[i].end(), cmp);
        maxNodeCnt = qMax(maxNodeCnt, (int)tree[i].size());
    }
    QPointF ori(nodes[s]->getXPos(), nodes[s]->getYPos());
    double width = maxNodeCnt * hSpacing;
    for(int i = 0;i <= maxlevel - minlevel;i++){
        for(int j = 0;j < tree[i].size();j++){
            int nodeidx = tree[i][j].first;
            int x = ori.x() + (j + 0.5) * width / tree[i].size(),
                y = ori.y() + i * vSpacing;
            nodes[nodeidx] -> setPos(x, y);
        }
    }
    emit updatePainter();
}


void LogicModule::topoLayout(int s){
    clearVis();
    int inf = 0x3f3f3f3f;
    double vSpacing = 100, hSpacing = 100;
    QQueue <int> q;
    QVector <int> level(nodes.size() + 5, inf), pre(nodes.size() + 5, -1);
    int minlevel = 1, maxlevel = -inf;
    q.push_back(s);
    level[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop_front();
        for(auto idx : graph[u]){
            edge &e = path[idx];
            if(!vis[e.v]){
                level[e.v] = level[u] + (e.isAdditional ? -1 : 1);
                if(e.isAdditional){
                    pre[u] = e.v;
                }

                else{
                    pre[e.v] = u;
                }
                minlevel = qMin(minlevel, level[e.v]);
                maxlevel = qMax(maxlevel, level[e.v]);
                vis[e.v] = 1;
                q.push_back(e.v);
            }
        }
    }
    QVector <QPair<int, int> > tree[maxlevel - minlevel + 2];
    for(int i = 0;i < nodes.size();i++){
        if(level[i] != inf){
            tree[level[i] - minlevel].push_back(qMakePair(i, pre[i]));
        }
    }
    int maxNodeCnt = 0;
    for(int i = 0;i <= maxlevel - minlevel;i++){
        std::sort(tree[i].begin(), tree[i].end(), cmp);
        maxNodeCnt = qMax(maxNodeCnt, (int)tree[i].size());
    }
    QPointF ori(nodes[s]->getXPos(), nodes[s]->getYPos());
    double width = maxNodeCnt * vSpacing;
    for(int i = 0;i <= maxlevel - minlevel;i++){
        for(int j = 0;j < tree[i].size();j++){
            int nodeidx = tree[i][j].first;
            int y = ori.y() + (j + 0.5) * width / tree[i].size(),
                x = ori.x() + i * hSpacing;
            nodes[nodeidx] -> setPos(x, y);
        }
    }
    emit updatePainter();
}


QPointF LogicModule::rotateN(QPointF ori, double n){
    double c = qCos(2.0 * M_PI / n), s = qSin(2.0 * M_PI / n);
    double x = c * ori.x() - s * ori.y(), y = s * ori.x() + c * ori.y();
    return QPointF(x, y);
}


void LogicModule::update(QList<Element *> elementList){
    nodes.clear();
    edges.clear();
    vis.clear();
    for(auto eptr : elementList){
        switch (eptr -> getType()) {
        case NODE:
            nodes.push_back((NodeElement*)eptr);
            break;
        case ARROW:
            edges.push_back((ArrowElement*)eptr);
            break;
        case STACK:
        case QUEUE:
        case BLOCK:
        case CURSOR:
            continue;
        }
    }
    for(int i = 0;i < nodes.size();i++) graph[i].clear();
    for(int i = 0;i < edges.size();i++){
        ArrowElement* &e = edges[i];
        int u = findFromNode(e), v = findToNode(e);
        if(u != -1 && v != -1){
            path.push_back((edge){u, v, 0});
            path.push_back((edge){v, u, e -> getIsDirected()});
            graph[u].push_back(path.size() - 2);
            graph[v].push_back(path.size() - 1);
        }
    }
    for(int i = 0;i < nodes.size();i++){
        vis.push_back(0);
    }
}
