#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 323;
	static const long REVISION = 1827;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 385;
	#define RC_FILEVERSION 1,3,323,1827
	#define RC_FILEVERSION_STRING "1, 3, 323, 1827\0"
	static const char FULLVERSION_STRING[] = "1.3.323.1827";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 23;
	

}
#endif //VERSION_H
