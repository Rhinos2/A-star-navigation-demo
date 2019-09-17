//
//
//

#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include <cstddef>
#include <list>
#include <vector>
#include "GraphEdge.h"
#include "IndxPriorityQ.h"
#include "Vector2D.h"


//template <class GRAPH>
class Graph_SearchAStar {


    SparseGraph m_Graph;

    std::vector<NavGraphEdge *> m_ShortestPathTree;
    std::vector<NavGraphEdge *> m_SearchFrontier;
    std::vector<NavGraphEdge *> m_Temp;
    //Contains the 'real' accumulative cost to that node
    std::vector<float> m_GCosts;
    std::vector<float> m_CostToThisNode;

    //Contains the cost from adding m_GCosts[n] to
    //the heuristic cost from n to the target node. This is the vector the
    //iPQ indexes into.
    std::vector<float> m_FCosts;

    int m_iSource;
    int m_iTarget;


    float Calculate(int nd1, int nd2) {
        Vector2D v1 = m_Graph.GetNode(nd1).Pos();
        Vector2D v2 = m_Graph.GetNode(nd2).Pos();
        float ySeparation = v2.y - v1.y;
        float xSeparation = v2.x - v1.x;

        return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
    }

public:

    void Reload(){
        m_iSource = -1;
        m_iTarget = -1;
        m_FCosts.clear();
        m_FCosts = std::vector<float>(m_Graph.NumNodes(), 0.0);
        m_GCosts.clear();
        m_GCosts = std::vector<float>(m_Graph.NumNodes(), 0.0);
        m_ShortestPathTree.clear();
        m_ShortestPathTree = std::vector<NavGraphEdge *>(m_Graph.NumNodes());
        m_SearchFrontier.clear();
        m_SearchFrontier = std::vector<NavGraphEdge *>(m_Graph.NumNodes());

        for (unsigned int i = 0; i < m_Temp.size(); i++)
            delete m_Temp[i];
        m_Temp.clear();
    }

    Graph_SearchAStar(SparseGraph graph) :

            m_Graph(graph),
            m_ShortestPathTree(graph.NumNodes()),
            m_SearchFrontier(graph.NumNodes()),
            m_GCosts(graph.NumNodes(), 0.0),
            m_FCosts(graph.NumNodes(), 0.0)
    {
        m_iSource = -1;
        m_iTarget = -1;

    }

    ~Graph_SearchAStar() {
        for (unsigned int i = 0; i < m_Temp.size(); i++)
            delete m_Temp[i];
    }

    std::list<int> GetPathToTarget() {

        std::list<int> path;
        //just return an empty path if no target or no path found
        if (m_iTarget < 0) return path;

        int nd = m_iTarget;
        path.push_front(nd);

        while ((nd != m_iSource) && (m_ShortestPathTree[nd] != 0)) {
            nd = m_ShortestPathTree[nd]->From();
            path.push_front(nd);
        }

        return path;
    }
void PrintPthNodes(){

    std::list<int> pathTo =  GetPathToTarget();
    std::list<int>::iterator pos;

    for(pos = pathTo.begin(); pos != pathTo.end(); ++pos) {
        NavGraphNode node = m_Graph.GetNode(*pos);
        Vector2D vec = node.Pos();
    }

}
    //returns the total cost to the target
    float GetCostToTarget() { return m_GCosts[m_iTarget]; }
    SparseGraph& GetGraph() { return m_Graph; }

    void Search(int source, int   target) {
        m_iSource = source;
        m_iTarget = target;
        //create an indexed priority queue of nodes. The nodes with the
        //lowest overall F cost (G+H) are positioned at the front.
        IndexedPriorityQLow<float> pq(m_FCosts, m_Graph.NumNodes());

        //put the source node on the queue
        pq.insert(m_iSource);

        //while the queue is not empty
        while (!pq.empty()) {
            //get lowest cost node from the queue
            int NextClosestNode = pq.Pop();

            //move this node from the frontier to the spanning tree
            m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode];

            //if the target has been found exit
            if (NextClosestNode == m_iTarget) return;

            //now to test all the edges attached to this node
            std::vector<std::list<NavGraphEdge> > ADJ = m_Graph.GetADJ();

            typename std::list<NavGraphEdge>::iterator pE;

            for (pE = ADJ[NextClosestNode].begin(); pE != ADJ[NextClosestNode].end(); pE++) {
                //calculate the heuristic cost from this node to the target (H)
                float HCost = Calculate(m_iTarget, pE->To());

                //calculate the 'real' cost to this node from the source (G)
                float first = m_GCosts[NextClosestNode];
                float second = pE->Cost();
                //float GCost = m_GCosts[NextClosestNode] + pE->Cost();
                float GCost = first + second;
                //if the node has not been added to the frontier, add it and update
                //the G and F costs
                if (m_SearchFrontier[pE->To()] == NULL) {
                    m_FCosts[pE->To()] = GCost + HCost;
                    m_GCosts[pE->To()] = GCost;

                    pq.insert(pE->To());
                    NavGraphEdge *ee = new NavGraphEdge(*pE);
                    m_Temp.push_back(ee);
                    m_SearchFrontier[pE->To()] = ee;// pE;
                }

                    //if this node is already on the frontier but the cost to get here
                    //is cheaper than has been found previously, update the node
                    //costs and frontier accordingly.
                else if ((GCost < m_GCosts[pE->To()]) && (m_ShortestPathTree[pE->To()] == NULL)) {
                    m_FCosts[pE->To()] = GCost + HCost;
                    m_GCosts[pE->To()] = GCost;

                    pq.ChangePriority(pE->To());
                    NavGraphEdge *e = new NavGraphEdge(*pE);
                    m_Temp.push_back(e);
                    m_SearchFrontier[pE->To()] = e;
                }

            }
        }
    }

};
#endif //ASTARSEARCH_H
