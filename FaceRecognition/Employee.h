#pragma once
#include <string>
#include "Picture.h"
#include "FaceImage.h"

struct Employee
{
	std::string name;
	std::string age;
	std::string sex;
	std::string birth;
	std::string address;
	std::string phone;
	std::string certificate;
};

class Identity
{
public:
	Identity();
	Identity(const Employee& e, Picture::Ptr picture);
	Identity(const Identity& id);
	~Identity();

	std::string name()const
	{
		return _employee.name;
	}

	std::string age()const
	{
		return _employee.age;
	}

	std::string sex()const
	{
		return _employee.sex;
	}

	std::string birth()const
	{
		return _employee.birth;
	}

	std::string address()const
	{
		return _employee.address;
	}

	std::string phone()const
	{
		return _employee.phone;
	}

	std::string certificate()const
	{
		return _employee.certificate;
	}

	std::string picutre_name()const
	{
		return pic_name_id;
	}

	FaceImage::Ptr GetFaceImage()const
	{
		poco_check_ptr(_face);
		return _face;
	}

protected:
	void Parse(const Employee& e);
private:

	Employee _employee;
	std::string pic_name_id;
	FaceImage::Ptr _face;
};

#include <list>

typedef std::vector<Identity*> IdentityList;

class IdentityDB
	: private IdentityList
{
	//typedef std::vector<Identity*> IdentityList;
public:
	IdentityDB();
	~IdentityDB();
	static IdentityDB& Instance();
	void Add(const Identity& id);
	void Get(size_t index, Identity& id);
	size_t Size();
private:
	//IdentityList elist;
};
