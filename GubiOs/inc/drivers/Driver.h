#ifndef __GUBIOS_DRIVERS_DRIVER_H
#define __GUBIOS_DRIVERS_DRIVER_H

namespace gubios
{
	namespace drivers
	{
		class Driver
		{

		public:
			Driver();
			~Driver();
			virtual void Activate();
			virtual void DeActivate();
			virtual int Reset();

			class DriverManager
			{
			public:
				Driver* drivers[255];
				int noOfDriver;
			public:
				DriverManager();
				~DriverManager();
				void AddDriver(Driver* driver);
				void ActivateAll();
			};

		};

	}
}
#endif
