#ifndef DIRECTORY_LISTING_H_INCLUDED
#define DIRECTORY_LISTING_H_INCLUDED

void RemoveListItem(unsigned int item_num);

unsigned int GetDirectoryList(char * thedirectory,unsigned int store_results_in_space,unsigned int comp_func,unsigned int asc_desc);
unsigned int GetTotalFilesInDirectory();
unsigned int GetTotalViewableFilesInDirectory();
unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename);
unsigned int CountPicturesInDirectory(char * thedirectory);
unsigned int LaunchGUI_PickDir();
char * GetFilename(unsigned int file_id);
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

     char filename[512];
};


extern struct FilenameHolder * list;
extern unsigned int list_size;
extern unsigned int last_list_total_countNOTUSEDANYWHERE; // We dont actually care about the total count of files , only about the pictures thus NOTUSEDANYWHERE :P
extern unsigned int last_list_total_pictures_count;
#endif // DIRECTORY_LISTING_H_INCLUDED
