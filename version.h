#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 0;
	static const long BUILD = 10;
	static const long REVISION = 52;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 14;
	#define RC_FILEVERSION 1,0,10,52
	#define RC_FILEVERSION_STRING "1, 0, 10, 52\0"
	static const char FULLVERSION_STRING[] = "1.0.10.52";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 10;
	

}
#endif //VERSION_H
