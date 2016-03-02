#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "02";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 6;
	static const long BUILD  = 614;
	static const long REVISION  = 3365;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 830;
	#define RC_FILEVERSION 1,6,614,3365
	#define RC_FILEVERSION_STRING "1, 6, 614, 3365\0"
	static const char FULLVERSION_STRING [] = "1.6.614.3365";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
