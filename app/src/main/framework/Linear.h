
#ifndef _LINEAR_H
#define _LINEAR_H


#include <stdio.h>
#include <vector>

#include "../GLM/glm/vec3.hpp"
#include "../GLM/glm/gtx/norm.hpp"

class Linear
{
    bool bInited;
    glm::vec3                   mLastPos;
    std::vector<glm::vec3>      mPositions; // sample points
    std::vector<float>         mTimes;     // time to arrive at each point
    unsigned int    mCount;     // number of points and times
    float mAccumTime;
public:
    // ctr
    Linear();
	~Linear();
    // set up
    bool Initialize( std::vector<glm::vec3> NavPoints, std::vector<float> times, unsigned int count );

    // clean up
    void Clean();
    // evaluate position
    bool Evaluate( float t, glm::vec3& vTrn, float& orient );

};

#endif //_LINEAR_H
