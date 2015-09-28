/* 
 * File:   util.h
 * Author: noah
 *
 * Created on November 9, 2014, 3:24 PM
 */

#ifndef UTIL_H
#define	UTIL_H

char *tohex(unsigned int n, int length);
char *todecimal(int n);
void *memset(void *s, int c, unsigned int n);
int strcmpn(char *stra, char *strb, unsigned int len);

#endif	/* UTIL_H */

