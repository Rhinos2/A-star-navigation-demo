//
//
//

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "Vector2D.h"

class NavGraphNode
{

    int        m_iIndex;
    //the node's position
    Vector2D     m_vPosition;


public:

    NavGraphNode(int idx, Vector2D pos)
    {
        m_iIndex = idx;
        m_vPosition = pos;
    }


    ~NavGraphNode() {}

    Vector2D  Pos()const { return m_vPosition; }
    void       SetPos(Vector2D NewPosition) { m_vPosition = NewPosition; }

    int  Index()const { return m_iIndex; }
    void SetIndex(int NewIndex) { m_iIndex = NewIndex; }

};
#endif //GRAPHNODE_H
