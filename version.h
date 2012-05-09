#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.05";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 489;
	static const long REVISION = 2713;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 623;
	#define RC_FILEVERSION 1,4,489,2713
	#define RC_FILEVERSION_STRING "1, 4, 489, 2713\0"
	static const char FULLVERSION_STRING[] = "1.4.489.2713";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 89;
	

}
#endif //VERSION_H
