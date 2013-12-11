#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.12";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 612;
	static const long REVISION = 3355;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 830;
	#define RC_FILEVERSION 1,6,612,3355
	#define RC_FILEVERSION_STRING "1, 6, 612, 3355\0"
	static const char FULLVERSION_STRING[] = "1.6.612.3355";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 12;
	

}
#endif //VERSION_H
