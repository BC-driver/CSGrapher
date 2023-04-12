#include "logic.h"
#include "element.h"
#include <QDebug>

Logic::Logic(QList <Element*> elementList){
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
    for(int i = 0;i < edges.size();i++){
        ArrowElement* &e = edges[i];
        int u = findFromNode(e), v = findToNode(e);
        if(u != -1 && v != -1){
            graph[u].push_back(v);
            if(e -> getIsDirected() == 0){
                graph[v].push_back(u);
            }
        }
    }
    for(int i = 0;i < nodes.size();i++){
        vis.push_back(0);
    }
//    for(int i = 0;i < nodes.size();i++){
//        for(auto v : graph[i]){
//            qDebug() << i <<" -> "<< v;
//        }
//    }
}


Logic::Logic(){}


int Logic::findFromNode(ArrowElement *edge){
    NodeElement* target = (NodeElement*)(edge -> getFromElement());
    for(int i = 0;i < nodes.size();i++){
        if(nodes[i] == target) return i;
    }
    return -1;
}


int Logic::findToNode(ArrowElement *edge){
    NodeElement* target = (NodeElement*)(edge -> getToElement());
    for(int i = 0;i < nodes.size();i++){
        if(nodes[i] == target) return i;
    }
    return -1;
}


void Logic::DFS(int u){
    if(vis[u]) return;
    vis[u] = 1;

}
