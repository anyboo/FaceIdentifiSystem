#pragma once
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveResult.h"
#include "Picture.h"

class FaceMatch
{
public:
	FaceMatch();
	~FaceMatch();

	struct AddArgs
	{
		Picture::Ptr a;
	};

	Poco::ActiveMethod<bool, AddArgs, FaceMatch> activeMatch;
private:
	bool activeMatchImp(const AddArgs& args);
};

