#include <drivers/Driver.h>

extern void printf(const char*);
using namespace gubios::drivers;


	Driver::Driver()
	{

	}
	Driver::~Driver()
	{

	}
	 void Driver::Activate()
	{

	}
	 void Driver::DeActivate()
	{

	}
	 int Driver::Reset()
	{

	}

	 Driver::DriverManager::DriverManager()
	 {
		 noOfDriver = 0;
	 }

	 Driver::DriverManager::~DriverManager()
	 {

	 }

	void Driver::DriverManager::AddDriver(Driver* driver)
	{
		drivers[noOfDriver] = driver;
		noOfDriver++;

	}

	void Driver::DriverManager::ActivateAll()
	{
		for(int i=0;i<noOfDriver;i++)
		{
			drivers[i]->Activate();
		}
	}
