#include "stdafx.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b)     (((a) > (b)) ? (a) : (b))
#define MIN_BUF_SIZE  1024

// |<-   size       ->|
// foo<-  gap    ->bar
//    ^           ^
//    |           | 
//    cursor      gap_end
typedef struct gap_buf
{
  size_t  size;
  size_t  cursor;
  size_t  gap_end;
  char*   buffer;
} GapBuf;

GapBuf*
new_buffer(size_t init_size);


void
free_buffer(GapBuf* buf);

#define gb_front(buf) ((buf)->cursor)                  /* size of text before cursor      */
#define gb_back(buf)  ((buf)->size - (buf)->gap_end)   /* size of text after cursor       */
#define gb_used(buf)  (gb_front(buf) + gb_back(buf))   /* total number of used characters */

// move back of buf to back of new_buf
void
move_back(GapBuf* buf, char* new_buf, size_t new_size);


// shrink buf to new_size
// no return value; no bool as in grow_buffer() -> does not matter if we cannot shrink buffer
// in other words: shrink_buffer() cannot fail!
void
shrink_buffer(GapBuf* buf, size_t new_size);


// grow buf to new_size
bool
grow_buffer(GapBuf* buf, size_t new_size);


#define capped_dbl_size(s) ((s) < SIZE_MAX / 2) ? (2 * (s)) : SIZE_MAX

// insert 'b'
// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// foob              bar
//     ^            ^
//     |            |
//     cursor       gap_end
bool
insert_character(GapBuf* buf, char c);


// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// fo               obar
//   ^             ^
//   |             |
//   cursor        gap_end
void
cursor_left(GapBuf* buf);

// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// foob               ar
//     ^             ^
//     |             |
//     cursor        gap_end
void
cursor_right(GapBuf* buf);


// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// fo                bar
//   ^              ^
//   |              |
//   cursor         gap_end
void
backspace(GapBuf* buf);


// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// foo                ar
//    ^              ^
//    |              |
//    cursor         gap_end
void deletex(GapBuf* buf);

char*
extract_text(GapBuf* buf);

char* extractFront(GapBuf* buf);
char* extractBack(GapBuf* buf);

void
print_buffer(GapBuf* buf);
