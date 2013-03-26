#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 5;
	static const long BUILD = 586;
	static const long REVISION = 3222;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 785;
	#define RC_FILEVERSION 1,5,586,3222
	#define RC_FILEVERSION_STRING "1, 5, 586, 3222\0"
	static const char FULLVERSION_STRING[] = "1.5.586.3222";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 86;
	

}
#endif //VERSION_H
