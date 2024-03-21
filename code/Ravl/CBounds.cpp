/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

////////////////////////////////////////////////////////////////////////////////////////
// RAVEN STANDARD TEMPLATE LIBRARY
//  (c) 2002 Activision
//
//
// Vector Library
// --------------
//
//
//
//
// NOTES:
// 05/31/02 - CREATED
//
//
////////////////////////////////////////////////////////////////////////////////////////

#if !defined(ASSERT_H_INC)
#include <cassert>
#define ASSERT_H_INC
#endif

#include <cmath>
#include <cstdio>
#include <cfloat>
#include "CBounds.h"

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
/*void CBBox::ThroughMatrix(const CBBox &from, const CMatrix4 &mat)
{
	Clear();
	CVec3 bb,t;
	int i;
	const CVec3 &xmn=from.GetMin();
	const CVec3 &xmx=from.GetMax();
	for ( i = 0; i < 8; i++ )
	{
		if ( i & 1 )
			bb[0] = xmn[0];
		else
			bb[0] = xmx[0];
		if ( i & 2 )
			bb[1] = xmn[1];
		else
			bb[1] = xmx[1];
		if ( i & 4 )
			bb[2] = xmn[2];
		else
			bb[2] = xmx[2];
		mat.XFormPoint(t,bb);
		AddPoint(t);
	}
}*/

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
float CBBox::LargestAxisSize() const
{
	CVec3 work(mMax);
	work -= mMin;
	return work.MaxElement();
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
float CBBox::DistanceEstimate(const CVec3& p) const
{
	float ret = 0.0f;

	// X Axis
	//--------
	if (p[0] > mMax[0])
	{
		ret = p[0] - mMax[0];
	}
	else if (p[0] < mMin[0])
	{
		ret = mMax[0] - p[0];
	}

	// Y Axis
	//--------
	if (p[1] > mMax[1])
	{
		ret += p[1] - mMax[1];
	}
	else if (p[1] < mMin[1])
	{
		ret += mMax[1] - p[1];
	}

	// Z Axis
	//--------
	if (p[2] > mMax[2])
	{
		ret += p[2] - mMax[2];
	}
	else if (p[2] < mMin[2])
	{
		ret += mMax[2] - p[2];
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
float CBBox::AreaEstimate(const CVec3& p) const
{
	const float distance = DistanceEstimate(p);
	if (distance)
	{
		return LargestAxisSize() / distance;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CBBox::Intersect(const CBBox& b2)
{
	mMin.Max(b2.mMin);
	mMax.Min(b2.mMax);
	Validate();
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CBBox::Union(const CBBox& b2)
{
	mMin.Min(b2.mMin);
	mMax.Max(b2.mMax);
	Validate();
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
ESide CBBox::InOutTest(const CVec3& v) const
{
	if (v > mMin && v < mMax)
	{
		return Side_In;
	}
	return Side_Out;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
ESide CBBox::InOutTest(const CVec3& v, const float tolout, const float tolin) const
{
	if (v[0] < mMin[0] - tolout || v[0] > mMax[0] + tolout ||
		v[1] < mMin[1] - tolout || v[1] > mMax[1] + tolout ||
		v[2] < mMin[2] - tolout || v[2] > mMax[2] + tolout)
	{
		return Side_Out;
	}
	if (v[0] > mMin[0] + tolin && v[0] < mMax[0] - tolin &&
		v[1] > mMin[1] + tolin && v[1] < mMax[1] - tolin &&
		v[2] > mMin[2] + tolin && v[2] < mMax[2] - tolin)
	{
		return Side_In;
	}
	return Side_None;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
bool CBBox::BoxTouchTest(const CBBox& b2, const float tolout) const
{
	if (mMin[0] - tolout > b2.mMax[0] ||
		mMin[1] - tolout > b2.mMax[1] ||
		mMin[2] - tolout > b2.mMax[2] ||
		b2.mMin[0] - tolout > mMax[0] ||
		b2.mMin[1] - tolout > mMax[1] ||
		b2.mMin[2] - tolout > mMax[2])
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
bool CBBox::SphereTouchTest(const CVec3& v, const float rad) const
{
	if (v[0] < mMin[0] - rad || v[0] > mMax[0] + rad ||
		v[1] < mMin[1] - rad || v[1] > mMax[1] + rad ||
		v[2] < mMin[2] - rad || v[2] > mMax[2] + rad)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
TPlanes CBBox::PlaneFlags(const CVec3& p)
{
	TPlanes ret = 0;
	if (p[0] < mMin[0])
	{
		ret |= 1;
	}
	else if (p[0] > mMax[0])
	{
		ret |= 2;
	}
	if (p[1] < mMin[1])
	{
		ret |= 4;
	}
	else if (p[1] > mMax[1])
	{
		ret |= 8;
	}
	if (p[2] < mMin[2])
	{
		ret |= 16;
	}
	else if (p[2] > mMax[2])
	{
		ret |= 32;
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// return true if the segment intersect the box, in that case, return the first contact.
////////////////////////////////////////////////////////////////////////////////////////
bool CBBox::HitTest(CBTrace& tr) const
{
	// Quick Box Cull
	//----------------
	CBBox tmp;
	tmp.AddPoint(tr.mStart);
	tmp.AddPoint(tr.mStop);
	if (!BoxTouchTest(tmp))
	{
		return false;
	}

	// Initialize Our Ranges
	//-----------------------
	tr.mRange = -1E30f;
	tr.mRangeMax = 1E30f;

	for (int axis = 0; axis < 3; axis++)
	{
		if (fabs(tr.mAim[axis]) > 1E-6f)
		{
			// Find Mins And Maxs From The Start Along The Axis Of Aim
			//---------------------------------------------------------
			float tmax = (mMax[axis] - tr.mStart[axis]) / tr.mAim[axis];
			float tmin = (mMin[axis] - tr.mStart[axis]) / tr.mAim[axis];
			if (tmax < tmin)
			{
				const float temp = tmax;
				tmax = tmin;
				tmin = temp;
			}

			// Adjust Range Max
			//------------------
			if (tmax < tr.mRangeMax)
			{
				tr.mRangeMax = tmax;
			}

			// Adjust Range Min
			//------------------
			if (tmin > tr.mRange)
			{
				tr.mRange = tmin;
				tr.mNormal.Clear();
				tr.mNormal[axis] = -1.0f;
			}
		}
	}

	// Missed?
	//---------
	if (tr.mRangeMax < tr.mRange || tr.mRangeMax < 0.0f || tr.mRange > tr.mLength)
	{
		return false;
	}

	// Start Solid Conditions
	//------------------------
	if (tr.mRange < 0.0f)
	{
		tr.mRange = 0.0f;
		tr.mPoint = tr.mStart;
		return true;
	}

	// Calculate The End Point
	//-------------------------
	tr.mPoint = tr.mAim;
	tr.mPoint *= tr.mRange;
	tr.mPoint += tr.mStart;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CBBox::FromStr(const char* s)
{
	assert(s && s[0]);

	char min_s[256];
	char max_s[266];
	sscanf(s, "(%s|%s)", min_s, max_s);

	mMin.FromStr(min_s);
	mMax.FromStr(max_s);
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CBBox::ToStr(char* s) const
{
	assert(s && s[0]);

	char min_s[256];
	char max_s[266];

	mMin.ToStr(min_s);
	mMax.ToStr(max_s);
	sprintf(s, "(%s|%s)", min_s, max_s);
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CBBox::Validate() const
{
	assert(mMax >= mMin);
}