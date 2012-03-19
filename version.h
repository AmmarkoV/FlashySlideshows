#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 283;
	static const long REVISION = 1619;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 329;
	#define RC_FILEVERSION 1,2,283,1619
	#define RC_FILEVERSION_STRING "1, 2, 283, 1619\0"
	static const char FULLVERSION_STRING[] = "1.2.283.1619";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 83;
	

}
#endif //VERSION_H
