#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 137;
	static const long REVISION = 799;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 150;
	#define RC_FILEVERSION 1,1,137,799
	#define RC_FILEVERSION_STRING "1, 1, 137, 799\0"
	static const char FULLVERSION_STRING[] = "1.1.137.799";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 37;
	

}
#endif //VERSION_H
