//
//
//

#ifndef NAVIGEDGE_H
#define NAVIGEDGE_H

class NavGraphEdge
{
    int     m_iFrom;
    int     m_iTo;
    float  m_dCost;

public:
    NavGraphEdge() {
        m_dCost = 0.0f;
        m_iFrom = 0;
        m_iTo = 0;

    }
    //ctor
    NavGraphEdge(int  from, int to, float cost)
    {
        m_dCost = cost;
        m_iFrom = from;
        m_iTo = to;

    }
    //copy ctor
    NavGraphEdge(const NavGraphEdge& e) {
        this->m_iFrom = e.From();
        this->m_iTo = e.To();
        this->m_dCost = e.Cost();

    }
    ~NavGraphEdge() {}

    int   From()const { return m_iFrom; }
    void  SetFrom(int NewIndex) { m_iFrom = NewIndex; }

    int   To()const { return m_iTo; }
    void  SetTo(int NewIndex) { m_iTo = NewIndex; }

    float Cost()const { return m_dCost; }
    void  SetCost(float NewCost) { m_dCost = NewCost; }

    //operators
    bool operator==(NavGraphEdge& rhs)
    {
        return rhs.m_iFrom == this->m_iFrom && rhs.m_iTo == this->m_iTo && rhs.m_dCost == this->m_dCost;
    }

    bool operator!=(NavGraphEdge& rhs)
    {
        return !(*this == rhs);
    }

};


#endif //NAVIGEDGE_H
