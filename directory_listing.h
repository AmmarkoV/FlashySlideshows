#ifndef DIRECTORY_LISTING_H_INCLUDED
#define DIRECTORY_LISTING_H_INCLUDED

void RemoveListItem(unsigned int item_num);

unsigned int LoadPicturesOfDirectory(char * thedirectory,unsigned int comp_func,unsigned int asc_desc,unsigned int recursive);
unsigned int GetDirectoryList(char * thedirectory,char *subdir,unsigned int space_to_allocate,unsigned int comp_func,unsigned int asc_desc,unsigned int recursive);
unsigned int GetTotalViewableFilesInDirectory();
unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename);
unsigned int CountPicturesInDirectory(char * thedirectory,int recursive);
unsigned int LaunchGUI_PickDir();
char * GetFilename(unsigned int file_id);
char * GetSubdir(unsigned int file_id);
unsigned int GetItemDate(unsigned int file_id,unsigned int data);

void PrintDirectoryListItem(unsigned int item);

int MoveOrRescaleOrCopyFileToDir(unsigned int file_id,unsigned int sort_id);


struct FilenameHolder
{
     unsigned int year;
     unsigned int month;
     unsigned int day;
     unsigned int day_of_year;
     unsigned int hour;
     unsigned int minute;
     unsigned int second;

     unsigned int filesize;

     char filename[256];
     char subdir[1024];
};


extern struct FilenameHolder * list;
extern unsigned long last_list_time_ms;
extern unsigned int list_size;
extern unsigned int pictures_count;
#endif // DIRECTORY_LISTING_H_INCLUDED
