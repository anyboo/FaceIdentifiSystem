#include "stdafx.h"
#include "Employee.h"
#include <Poco/SingletonHolder.h>

Identity::Identity()
{

}

Identity::Identity(const Employee& e, Picture::Ptr picture)
{
	Parse(e);
	pic_name_id = picture->WriteToDisk();
	_face = new FaceImage(picture);
}

Identity::Identity(const Identity& id)
{
	Parse(id._employee);
	pic_name_id = id.pic_name_id;
	_face.assign(id.GetFaceImage());
}

Identity::~Identity()
{
}

void Identity::Parse(const Employee& e)
{
	_employee.name = e.name;
	_employee.age = e.age;
	_employee.sex = e.sex;
	_employee.birth = e.birth;
	_employee.address = e.address;
	_employee.phone = e.phone;
	_employee.certificate = e.certificate;
}

IdentityDB::IdentityDB()
{

}

IdentityDB::~IdentityDB()
{
	for (auto e : (*this))
	{
		if (e) delete e;
	}

	clear();
}

namespace 
{
	static Poco::SingletonHolder<IdentityDB> sh;
}

void IdentityDB::Add(const Identity& id)
{
	push_back(new Identity(id));
}

void IdentityDB::Get(size_t index, Identity& id)
{
	poco_assert(index < size());
	id = *((*this)[index]);
}

size_t IdentityDB::Size()
{
	return (*this).size();
}

IdentityDB& IdentityDB::Instance()
{
	return *sh.get();
}