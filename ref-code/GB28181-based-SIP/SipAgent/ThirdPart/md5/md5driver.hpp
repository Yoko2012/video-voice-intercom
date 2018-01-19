/* 
  md5driver.h
  md5sum APIͷ�ļ�,�ɶ��ļ����ַ�����������ǩ��
  �ڵ���ʱֻ��Ҫinclude����ļ�,���ɽ���md5ǩ��;
  
  Sample:
  MDString (char *);  // ��һ���ַ�������ǩ��
  MDFile (char *);  // ��һ���ļ�����md5ǩ��,����ͬmd5sumִ��һ��
*/



#ifndef _MD5DRIVER_H
#define _MD5DRIVER_H

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;


typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

// void MD5Init PROTO_LIST ((MD5_CTX *));
// void MD5Update PROTO_LIST
//   ((MD5_CTX *, unsigned char *, unsigned int));
// void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));

void MD5Init (MD5_CTX *);
void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
void MD5Final(unsigned char [16], MD5_CTX *);


void MDString (const char *str, char *md5sign);
void MDTimeTrial(void);
// void MDTestSuite (void);
void MDFile (const char *filename, char *md5sign);
void MDFilter (void);
void MDPrint(unsigned char *digest, size_t len );

#endif



