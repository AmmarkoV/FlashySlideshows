#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 454;
	static const long REVISION = 2505;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 583;
	#define RC_FILEVERSION 1,4,454,2505
	#define RC_FILEVERSION_STRING "1, 4, 454, 2505\0"
	static const char FULLVERSION_STRING[] = "1.4.454.2505";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 54;
	

}
#endif //VERSION_H
