//
//
//

#ifndef NAVGRAPH_H
#define NAVGRAPH_H

#include <iostream>
#include <vector>
#include <list>
#include <cassert>


class SparseGraph
{
    //the nodes that comprise this graph
    std::vector<NavGraphNode>      m_Nodes;

    //a vector of adjacency edge lists. (each node index keys into the
    //list of edges associated with that node)
    std::vector<std::list<NavGraphEdge> >  ADJ;

    //is this a directed graph?
    bool            m_bDigraph;

    //the index of the next node to be added
    int             m_iNextNodeIndex;


    //returns true if an edge is not already present in the graph. Used
    //when adding edges to make sure no duplicates are created.
    bool  UniqueEdge(int from, int to)const {
        typename std::list<NavGraphEdge>::const_iterator curEdge;
        for (curEdge = ADJ[from].begin(); curEdge != ADJ[from].end(); curEdge++)
        {
            if (curEdge->To() == to)
            {
                return false;
            }
        }
        return true;
    }
public:
    //ctor
    SparseGraph(bool digraph) {
        m_iNextNodeIndex = 0;
        m_bDigraph = digraph;
    }

    std::vector<std::list<NavGraphEdge> >& GetADJ() { return ADJ;  }

    NavGraphNode&  GetNode(int idx) {
        assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<SparseGraph::GetNode>: invalid index");
        return m_Nodes[idx];
    }

    //adds a node to the graph and returns its index
    int   AddNode(NavGraphNode node) {
        if (node.Index() < (int)m_Nodes.size())
        {
            //make sure the client is not trying to add a node with the same ID as
            //a currently active node
            assert(m_Nodes[node.Index()].Index() == -1);
            m_Nodes[node.Index()] = node;
            return m_iNextNodeIndex;
        }

        else
        {
            m_Nodes.push_back(node);
            ADJ.push_back(std::list<NavGraphEdge>());
            return m_iNextNodeIndex++;
        }
    }

    void  AddEdge(NavGraphEdge edge) {

            if (UniqueEdge(edge.From(), edge.To()))
            {
                //int n = edge.From();
                ADJ[edge.From()].push_back(edge);
            }
            //if the graph is undirected add another connection in the opposite direction
            if (!m_bDigraph)
            {
                //check to make sure the edge is unique before adding
                if (UniqueEdge(edge.To(), edge.From()))
                {
                    NavGraphEdge NewEdge = edge;
                    NewEdge.SetTo(edge.From());
                    NewEdge.SetFrom(edge.To());
                    ADJ[edge.To()].push_back(NewEdge);
                }
            }
    }

    //returns the number of active + inactive nodes present in the graph
    unsigned int   NumNodes() { return m_Nodes.size(); }

    //returns true if the graph is directed
    bool  isDigraph()const { return m_bDigraph; }
    //returns true if the graph contains no nodes
    bool	isEmpty()const { return m_Nodes.empty(); }
    //clears the graph ready for new node insertions
    void Clear() { m_iNextNodeIndex = 0; m_Nodes.clear(); ADJ.clear(); }

    void RemoveEdges()
    {
        for (int i = 0; i < ADJ.size(); i++) {
            ADJ[i].clear();
        }
        ADJ.clear();

    }
    //returns the total number of edges present in the graph
    int   NumEdges()const
    {
        int tot = 0;
        for (int i = 0; i < ADJ.size(); i++) {
            tot = tot + ADJ[i].size();
        }
        return tot;
    }
};

#endif //NAVGRAPH_H
