#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "31";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2014";
	static const char UBUNTU_VERSION_STYLE[] =  "14.03";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 6;
	static const long BUILD  = 614;
	static const long REVISION  = 3368;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 831;
	#define RC_FILEVERSION 1,6,614,3368
	#define RC_FILEVERSION_STRING "1, 6, 614, 3368\0"
	static const char FULLVERSION_STRING [] = "1.6.614.3368";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
