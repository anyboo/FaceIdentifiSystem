#include "stdafx.h"
#include "FaceMatch.h"
#include "FaceImage.h"

FaceMatch::FaceMatch()
:activeMatch(this, &FaceMatch::activeMatchImp)
{
}


FaceMatch::~FaceMatch()
{
}

bool FaceMatch::activeMatchImp(const AddArgs& args)
{
	try
	{
		FaceImage face1(args.a);
		FaceImage face2(args.b);
		return face1.Compare(face2);
	}
	catch (...)
	{
		return false;
	}
}