#ifndef DIRECTORY_LISTING_H_INCLUDED
#define DIRECTORY_LISTING_H_INCLUDED

unsigned int GetDirectoryList(char * thedirectory,unsigned int store_results_in_space,unsigned int comp_func,unsigned int asc_desc);
unsigned int GetTotalFilesInDirectory();
unsigned int GetTotalViewableFilesInDirectory();
unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename);
unsigned int CountPicturesInDirectory(char * thedirectory);
unsigned int LaunchGUI_PickDir();
char * GetFilename(unsigned int file_id);
unsigned int GetItemDate(unsigned int file_id,unsigned int data);
#endif // DIRECTORY_LISTING_H_INCLUDED
