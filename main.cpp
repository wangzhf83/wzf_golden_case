
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "C:\work\ethereum\cpp-ethereum-develop\libethash\ethash.h"
#include "C:\work\ethereum\cpp-ethereum-develop\libethash\sha3.h"
#include "C:\work\ethereum\cpp-ethereum-develop\libethash\sha3.c"

using namespace std;

#define ETHASH_MIX_BYTES 128
#define MIX_WORDS (ETHASH_MIX_BYTES/4)
#define NODE_WORDS (64/4)
#define MIX_NODES (MIX_WORDS / NODE_WORDS)
#define FNV_PRIME 0x01000193

typedef union node {
	uint8_t bytes[200];
	uint32_t words[200/4];
	uint64_t double_words[200/8];
} node;

typedef struct hash_256 {
   uint8_t *src;
   uint32_t len;
   uint8_t *result;
} hash_256;

typedef struct hash_512 {
   uint8_t *src;
   uint32_t len;
   uint8_t *result;
} hash_512;


int main(int argc, char** argv) {
//int main() {
   for (int i=0; i < argc; ++i)
      printf("i %d is %s.\n", i, argv[i]);

   std::cout << "golden case generator ..." << std::endl;
   hash_256 hash_2;
   hash_512 hash_5;
   uint8_t uint_temp;
   
   //char temp[] = {"1234567890123456789012345678901234567890"};
   //char temp[] = {"123456789012345678901234567890123456789012345678901234567890123456789012"};
   //char temp[] = {"00000000000000000000000000000000000000000000000000000000000000000000000000000000"};
   char temp[] = {"1"};
   
//   for (int i=0; i < strlen(argv[1]); i++) {
//      printf("i %d is x%x c%c .\n", i, argv[1][i], argv[1][i]);
//   }

   if (argc > 1) strcpy(temp, argv[1]);
   
   hash_2.len = strlen(temp)/2 + strlen(temp)%2; //strlen(argv[1]);
   hash_5.len = strlen(temp)/2 + strlen(temp)%2; //strlen(argv[1]);       
   
   hash_2.src = new uint8_t[hash_2.len]; //(uint8_t *)malloc(hash_2.len);   
   hash_5.src = new uint8_t[hash_5.len]; //(uint8_t *)malloc(hash_5.len);   
   hash_2.result = new uint8_t[32]; //(uint8_t *)malloc(32);   
   hash_5.result = new uint8_t[64]; //(uint8_t *)malloc(64);  
   
   for (int i=0; i < hash_2.len*2; i=i+2) {
      //c2uint[0] = temp[i*2 + 0]; c2uint[1] = temp[i*2 + 1];
      //uint_temp = atoi(c2uint);
      //printf("i %d is x%x .\n", i, uint_temp);
      uint_temp = (temp[i] >= 'a') ? temp[i] - 'a' + 10 : temp[i] - '0'; //(uint8_t)strtoul(temp+i,temp+i,10); //temp[i] - '0';
      if (strlen(temp)%2 == 0) uint_temp = uint_temp * 16 + ((temp[i+1] >= 'a') ? temp[i+1] - 'a' + 10 : temp[i+1] - '0');
      hash_2.src[i/2] = uint_temp;
      //hash_5.src[i] = (temp[i] >= 'a') ? temp[i] - 'a' + 10 : temp[i] - '0'; //(uint8_t)strtoul(temp[i],temp[i],10); //temp[i] - '0';
      hash_5.src[i/2] = uint_temp;
      //printf("i %d is x%x c%c .\n", i, hash_2.src[i], temp[i]);
   }

   std::cout << "hash source text length " << hash_2.len << std::endl;
   std::cout << "hash source text: hash_2.src  " << std::endl;
   for (int i=0; i < hash_2.len; ++i) {
      printf("%02x",hash_2.src[i]);
   } 
   printf("\n");
   
   //hash 256
   sha3_256(hash_2.result, 32, hash_2.src, hash_2.len);
   
   std::cout << "sha3 hash 256 result: " << std::endl;
   for (int i=0; i < 32; ++i) {
      printf("%02x",hash_2.result[i]);
   } 
   printf("\n");

   //hash 512
   sha3_512(hash_5.result, 64, hash_5.src, hash_5.len);
   
   std::cout << "sha3 hash 512 result: " << std::endl;
   for (int i=0; i < 64; ++i) {
      printf("%02x",hash_5.result[i]);
   } 
   printf("\n");

   delete [] hash_2.src; //free(hash_2.src);
   delete [] hash_5.src; //free(hash_5.src);

   
   
   //----------------------------------------------------------------------------------------------
   //generate random testcase   
   
   srand(123); //srand( time(NULL) );  //srand( time(0) );  
   //for( int i = 0; i < 10; i++ )  printf( "Random number #%d: %d\n", i, rand() ); 
   
   ofstream file_256;
   ofstream file_512;
   file_256.open("gc_256.dat", ios::out | ios::trunc);// | ios::binary);
   file_512.open("gc_512.dat", ios::out | ios::trunc);// | ios::binary);   
   
   uint32_t src_len;
   uint32_t max_src_len = 100;
   uint32_t loop_num = 1;
   
   for (uint32_t i=0; i < loop_num; ++i) {
      src_len = rand() % max_src_len + 1;
      hash_2.src = new uint8_t[src_len];
      hash_5.src = new uint8_t[src_len];
      hash_2.len = src_len;
      hash_5.len = src_len;
      cout << "rand len = " << src_len << endl;
      if (i == 0) {
         for (uint32_t i=0; i < hash_2.len; ++i) hash_2.src[i] = 0;//rand() % 255;
      } else if (i == 1) {
         for (uint32_t i=0; i < hash_2.len; ++i) hash_2.src[i] = 255;
      } else if (i == 2) {
         for (uint32_t i=0; i < hash_2.len; ++i) hash_2.src[i] = 0xaa;
      } else if (i == 3) {
         for (uint32_t i=0; i < hash_2.len; ++i) hash_2.src[i] = 0x55;
      } else {
         for (uint32_t i=0; i < hash_2.len; ++i) hash_2.src[i] = rand() % 255;
         //for (uint32_t i=0; i < hash_5.len; ++i) hash_5.src[i] = rand() % 255;
      }
      for (uint32_t i=0; i < hash_5.len; ++i) hash_5.src[i] = hash_2.src[i];

      
      sha3_256(hash_2.result, 32, hash_2.src, hash_2.len);
      sha3_512(hash_5.result, 64, hash_5.src, hash_5.len);
      
      char c2uint[2];
      char *buf;
      buf = new char[hash_2.len*2 + 1 + 32*2];
      for (uint32_t i=0; i < hash_2.len; ++i) {
         c2uint[0] = hash_2.src[i] / 16 >= 10 ? 'a' + hash_2.src[i] / 16 - 10 : '0' + hash_2.src[i] / 16;
         c2uint[1] = hash_2.src[i] % 16 >= 10 ? 'a' + hash_2.src[i] % 16 - 10 : '0' + hash_2.src[i] % 16;
         buf[i*2] = c2uint[0];
         buf[i*2 + 1] = c2uint[1];
      };
      buf[hash_2.len*2] = ' ';
      for (uint32_t i=0; i < 32; ++i) {
         c2uint[0] = hash_2.result[i] / 16 >= 10 ? 'a' + hash_2.result[i] / 16 - 10 : '0' + hash_2.result[i] / 16;
         c2uint[1] = hash_2.result[i] % 16 >= 10 ? 'a' + hash_2.result[i] % 16 - 10 : '0' + hash_2.result[i] % 16;
         buf[hash_2.len*2 + 1 + i*2] = c2uint[0];
         buf[hash_2.len*2 + 1 + i*2 + 1] = c2uint[1];
      };
      
      cout << buf << endl;
      file_256 << buf << endl;
      delete [] buf; 
      
      
      buf = new char[hash_5.len*2 + 1 + 64*2];
      for (uint32_t i=0; i < hash_5.len; ++i) {
         c2uint[0] = hash_5.src[i] / 16 >= 10 ? 'a' + hash_5.src[i] / 16 - 10 : '0' + hash_5.src[i] / 16;
         c2uint[1] = hash_5.src[i] % 16 >= 10 ? 'a' + hash_5.src[i] % 16 - 10 : '0' + hash_5.src[i] % 16;
         buf[i*2] = c2uint[0];
         buf[i*2 + 1] = c2uint[1];
      };
      buf[hash_5.len*2] = ' ';
      for (uint32_t i=0; i < 64; ++i) {
         c2uint[0] = hash_5.result[i] / 16 >= 10 ? 'a' + hash_5.result[i] / 16 - 10 : '0' + hash_5.result[i] / 16;
         c2uint[1] = hash_5.result[i] % 16 >= 10 ? 'a' + hash_5.result[i] % 16 - 10 : '0' + hash_5.result[i] % 16;
         buf[hash_5.len*2 + 1 + i*2] = c2uint[0];
         buf[hash_5.len*2 + 1 + i*2 + 1] = c2uint[1];
      };
      
      cout << buf << endl;
      file_512 << buf << endl;
      delete [] buf;
      
      //for (int i=0; i < hash_2.len; ++i) printf("%x",hash_2.src[i]);
      //printf("\n");
   
      //for (int i=0; i < 32; ++i) printf("%0x",hash_2.result[i]);
      //printf("\n");
      
   }


   file_256.close();
   file_512.close();  
   delete [] hash_2.result; //free(hash_2.result);
   delete [] hash_5.result; //free(hash_5.result);
      
   return 0;

}


