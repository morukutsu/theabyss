// mkengine
// LittleSpace Studio 2013

#ifndef MK_ANCHOR
#define MK_ANCHOR

#include "Vector.h"

class Anchor
{
	public:
		Anchor();

		Anchor(NVector* anchor, NVector offset);

		void SetAnchor(NVector* a) { anchor = a; };
		void SetRotationOffset(NVector o) { staticOffset = o; };

		void SetOffset(NVector o) { offset = o; };
		void SetAngle(float a) { angle = a; };

		NVector GetTransformedPoint();

	private:
		NVector* anchor;		// Point d'origine à suivre
		NVector  staticOffset;  // offset par rapport au centre
		NVector  offset;		// Offset de l'ancre
		float    angle;			// Angle de rotation
};

#endif