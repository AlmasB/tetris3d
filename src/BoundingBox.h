/**
* Please note some bits and pieces of code have been taken from Aidan Delaney
* Original source code: https://github.com/AidanDelaney/CI224-game
*
* Might rewrite those parts in due time
*/

/*
Copyright (C) 2006-2010 Sony Computer Entertainment Inc.
All rights reserved.

Redistribution and use in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the Sony Computer Entertainment Inc nor the names
of its contributors may be used to endorse or promote products derived
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/



#ifndef __BOX_H__
#define __BOX_H__

#include <utility>

#include <SDL_opengl.h>

#include "Common.h"

static Vector3 UNIT_X_AXIS(1.0, 0.0, 0.0);
static Vector3 UNIT_Y_AXIS(0.0, 1.0, 0.0);
static Vector3 UNIT_Z_AXIS(0.0, 0.0, 1.0);

enum AXIS { X, Y, Z };

class Box {
	private:
	public:
		Point3 center;

		Box(const Point3 &);
		Box(float, float, float);
		void draw();

		bool collidesWith(const Box &);
		std::pair<float, float> projectOntoAxis(const Box &, enum AXIS);
};

#endif