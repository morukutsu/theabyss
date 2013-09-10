// mkengine
// LittleSpace Studio 2013

#include "Anchor.h"

Anchor::Anchor()
{
	angle = 0.0f;
	staticOffset = NVector(0, 0);
}

Anchor::Anchor(NVector* anc, NVector off)
{
	anchor = anc;
	offset = off;
	angle = 0.0f;
}

NVector Anchor::GetTransformedPoint()
{
	NVector point = offset + *anchor;
	point.Rotate((*anchor + staticOffset), angle);
	return point;
}