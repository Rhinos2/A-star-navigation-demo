//

#include <cassert>
#include "Linear.h"
#include <string.h>
#include <errno.h>

Linear::Linear() :

    mCount( 0 ),
    mAccumTime(0.0f),
	mLastPos(0.0f, 0.2f, 0.0f)
{
	bInited = false;
}

Linear::~Linear()
{
    Clean();
}

bool Linear::Initialize(std::vector<glm::vec3> NavPoints, std::vector<float> times, unsigned int count )
{
	// make sure data is valid
	if (count < 2 || NavPoints.size() == 0 || times.size() == 0)
		return false;

    mPositions = NavPoints;
	mTimes = times;
    mCount = count;
    mAccumTime = times[0];
	mLastPos = mPositions[0];
	bInited = true;
    return true;

}

void Linear::Clean()
{
    mPositions.clear();
    mTimes.clear();
    mCount = 0;
	mAccumTime = 0.0f;
	mLastPos = glm::vec3(0.0f, 0.2f, 0.0f);
}

// Evaluate spline

bool Linear::Evaluate(float t, glm::vec3& vTrn, float& orient) {

	if (!bInited)
		return false;

	mAccumTime = mAccumTime + t;

	// make sure data is valid
	assert(mCount >= 2);
	if (mCount < 2) {
		vTrn = mPositions[0];
		bInited = false;
		return true;
}
	// handle boundary conditions
	if (mAccumTime <= mTimes[0]) {
		vTrn = mPositions[0];
		bInited = false;
		return true;
	}
	else if (mAccumTime >= mTimes[mCount - 1]){
		vTrn = mPositions[mCount - 1];
		vTrn.y = 0.4f;
		mLastPos = vTrn;
		bInited = false;
		return true;
	}
	// find segment and parameter
	unsigned int i;
	for (i = 0; i < mCount - 1; ++i)
	{
		if (mAccumTime < mTimes[i + 1])
		{
			break;
		}
	}
	float t0 = mTimes[i];
	float t1 = mTimes[i + 1];
	float u = (mAccumTime - t0) / (t1 - t0);

	// evaluate
	vTrn = (1 - u)*mPositions[i] + u * mPositions[i + 1];
	vTrn.y = 0.4f;//above XZ

    float DistSqr = glm::distance2(mLastPos, vTrn);
    if(DistSqr > 0) {
        glm::vec3 velocity  = vTrn - mLastPos;
        float angDegree = glm::degrees(glm::atan(velocity.x, velocity.z));
		float angEven = glm::roundEven(angDegree);

		orient = angEven;
		mLastPos = vTrn;
    }
	return true;
}  


