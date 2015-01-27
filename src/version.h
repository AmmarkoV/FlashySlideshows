#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2015";
	static const char UBUNTU_VERSION_STYLE[] =  "15.01";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 6;
	static const long BUILD  = 615;
	static const long REVISION  = 3370;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 831;
	#define RC_FILEVERSION 1,6,615,3370
	#define RC_FILEVERSION_STRING "1, 6, 615, 3370\0"
	static const char FULLVERSION_STRING [] = "1.6.615.3370";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 15;
	

}
#endif //VERSION_H
