/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "directory_sorting.h"
#include "directory_listing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

unsigned int FirstItemSmallerOrEqualToSecond(unsigned int item_a,unsigned int item_b,unsigned int comp_func,unsigned int asc_desc)
{
    unsigned int true_value=1;
    unsigned int false_value=0;

    if (!asc_desc) { /*IF descending , we swap the result!*/ true_value=0; false_value=1;}

    if (comp_func == 0)
     { //Compare by name
       unsigned int size_of_item_a_filename = strlen(list[item_a].filename);
       unsigned int size_of_item_b_filename = strlen(list[item_b].filename);
       unsigned int length=size_of_item_b_filename;
       if (size_of_item_a_filename<size_of_item_b_filename) { length=size_of_item_a_filename; }

       unsigned int i=0;
       for (i=0; i<length; i++)
       {
          if ( list[item_a].filename[i]<list[item_b].filename[i] ) { return true_value;  }
          if ( list[item_a].filename[i]>list[item_b].filename[i] ) { return false_value;  }
       }

       if (size_of_item_a_filename<size_of_item_b_filename) { return true_value; }
     } else
    if (comp_func == 1)
     { // Compare by date
       if (list[item_a].year < list[item_b].year) { return true_value; }
       if (list[item_a].year > list[item_b].year) { return false_value; }

       if (list[item_a].month < list[item_b].month) { return true_value; }
       if (list[item_a].month > list[item_b].month) { return false_value; }

       if (list[item_a].day < list[item_b].day) { return true_value; }
       if (list[item_a].day > list[item_b].day) { return false_value; }

       if (list[item_a].hour < list[item_b].hour) { return true_value; }
       if (list[item_a].hour > list[item_b].hour) { return false_value; }

       if (list[item_a].minute < list[item_b].minute) { return true_value; }
       if (list[item_a].minute > list[item_b].minute) { return false_value; }

       if (list[item_a].second < list[item_b].second) { return true_value; }
       if (list[item_a].second > list[item_b].second) { return false_value; }
     } else
    if (comp_func == 2 )
     { // Compare by size
       if (list[item_a].filesize <= list[item_b].filesize) { return true_value; } else
                                                           { return false_value; }
     }

    return false_value;
}

void SwapListItems(unsigned int item_a,unsigned int item_b)
{
  struct FilenameHolder temp;
  temp=list[item_a];
  list[item_a]=list[item_b];
  list[item_b]=temp;
}


void RemoveListItem(unsigned int item_num)
{
  //Since the list is always sorted , we will have to move all items one step back
  // TODO , this could be a linked list to make things easier , typically remove list items
  // is not called in sorting so it will not have a performance impact
  unsigned int item_iterator=item_num+1;
  while (item_iterator<pictures_count)
   {
      list[item_iterator-1]=list[item_iterator];
   }
   --pictures_count;
}



void SortDirectoryList(unsigned int beg,unsigned int end,unsigned int comp_func,unsigned int asc_desc)
{
  if (end > beg + 1)
  {
    unsigned int piv=beg, l = beg + 1, r = end;
    while (l < r)
    {
      if ( FirstItemSmallerOrEqualToSecond(l,piv,comp_func,asc_desc) )
        {
          l++;
        }
      else
        {
          SwapListItems(l,--r);
        }
    }
    SwapListItems(--l,beg);

    SortDirectoryList(beg,l,comp_func,asc_desc);
    SortDirectoryList(r,end,comp_func,asc_desc);
  }
}


void RandomizeDirectoryListSorting(unsigned int beg,unsigned int end)
{
  if (end > beg + 1)
  {
    srand(time(0));
    unsigned int total=end-beg , piv=0;
    //Crude randomizer :P
    while (piv<end) {  SwapListItems(piv++,rand()%total);  }
  }
}


