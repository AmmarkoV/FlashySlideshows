#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.08";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 5;
	static const long BUILD = 544;
	static const long REVISION = 2985;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 700;
	#define RC_FILEVERSION 1,5,544,2985
	#define RC_FILEVERSION_STRING "1, 5, 544, 2985\0"
	static const char FULLVERSION_STRING[] = "1.5.544.2985";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 44;
	

}
#endif //VERSION_H
