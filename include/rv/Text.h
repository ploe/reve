#ifndef _REVE_TEXT_H
#define _REVE_TEXT_H

typedef char *rv_Text;

rv_Text rv_TextNew(const char *format, ...);
rv_Text rv_TextFromFile(const char *path);
rv_Text rv_TextFree(rv_Text s);


#endif
