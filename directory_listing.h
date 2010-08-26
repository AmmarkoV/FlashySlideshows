#ifndef DIRECTORY_LISTING_H_INCLUDED
#define DIRECTORY_LISTING_H_INCLUDED

int GetDirectoryList(char * thedirectory);
int GetTotalFilesInDirectory();
int GetTotalViewableFilesInDirectory();
int GetViewableFilenameforFile(int file_id,char *filename);

#endif // DIRECTORY_LISTING_H_INCLUDED
