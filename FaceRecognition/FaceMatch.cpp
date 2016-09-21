#include "stdafx.h"
#include "FaceMatch.h"
#include "FaceImage.h"
#include "Employee.h"

FaceMatch::FaceMatch()
:activeMatch(this, &FaceMatch::activeMatchImp)
{
}


FaceMatch::~FaceMatch()
{
}

#include <Poco/Thread.h>
#include <sstream>

using Poco::Thread;

bool FaceMatch::activeMatchImp(const AddArgs& args)
{
	std::stringstream ss;
	ss << "currentTid : " << Thread::currentTid() << std::endl;
	OutputDebugStringA(ss.str().c_str());

	return false;

	try
	{
		FaceImage face1(args.a);
		OutputDebugStringA("consturct FaceImage");
		IdentityDB& db = IdentityDB::Instance();
		for (size_t i = 0; i < db.Size(); i++)
		{
			Identity id;
			db.Get(i, id);
			OutputDebugStringA("before face1 compare");
			FaceImage::Ptr pic = id.GetFaceImage();
			poco_check_ptr(pic);
			return face1.Compare(*pic);
		}
		return false;

	}
	catch (...)
	{
		return false;
	}
}