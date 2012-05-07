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
	static const long BUILD = 473;
	static const long REVISION = 2627;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 603;
	#define RC_FILEVERSION 1,4,473,2627
	#define RC_FILEVERSION_STRING "1, 4, 473, 2627\0"
	static const char FULLVERSION_STRING[] = "1.4.473.2627";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 73;
	

}
#endif //VERSION_H
