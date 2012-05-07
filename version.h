#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.05";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 482;
	static const long REVISION = 2671;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 613;
	#define RC_FILEVERSION 1,4,482,2671
	#define RC_FILEVERSION_STRING "1, 4, 482, 2671\0"
	static const char FULLVERSION_STRING[] = "1.4.482.2671";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 82;
	

}
#endif //VERSION_H
