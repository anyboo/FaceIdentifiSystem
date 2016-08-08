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
	face = new FaceImage(picture);
}

Identity::Identity(const Identity& id)
{
	Parse(id._employee);
	pic_name_id = id.pic_name_id;
	face.assign(id.face);
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
	for (auto e : elist)
	{
		if (e) delete e;
	}

	elist.clear();
}

namespace 
{
	static Poco::SingletonHolder<IdentityDB> sh;
}

void IdentityDB::Add(const Identity& id)
{
	elist.push_back(new Identity(id));
}

void IdentityDB::Get(size_t index, Identity& id)
{
	poco_assert(index > elist.size());
	id = *(elist[index]);
}

IdentityDB& IdentityDB::Instance()
{
	return *sh.get();
}