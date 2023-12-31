#define _GNU_SOURCE
#include <ctype.h>

/*===========================================================================*/
bool ispotfilestring(int len, char *buffer)
{
uint8_t p;
for(p = 0; p < len; p++)
	{
	if((buffer[p] < 0x20) || (buffer[p] > 0x7e) || (buffer[p] == ':')) return false;
	}
return true;
}
/*===========================================================================*/
bool isasciistring(int len, uint8_t *buffer)
{
uint8_t p;
for(p = 0; p < len; p++)
	{
	if(buffer[p] == 0) return true;
	if((buffer[p] < 0x20) || (buffer[p] == 0x7f)) return false;
	}
return true;
}
/*===========================================================================*/
bool ishexvalue(const char *str, size_t len)
{
size_t c;

for(c = 0; c < len; c++)
	{
	if(str[c] < '0') return false;
	if(str[c] > 'f') return false;
	if((str[c] > '9') && (str[c] < 'A')) return false;
	if((str[c] > 'F') && (str[c] < 'a')) return false;
	}
return true;
}
/*===========================================================================*/
bool hex2bin(const char *str, uint8_t *bytes, size_t blen)
{
uint8_t pos;
uint8_t idx0;
uint8_t idx1;

uint8_t hashmap[] =
{
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVW
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ[\]^_
0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // `abcdefg
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // hijklmno
};

if(ishexvalue(str, blen) == false) return false;
memset(bytes, 0, blen);
for (pos = 0; ((pos < (blen*2)) && (pos < strlen(str))); pos += 2)
	{
	idx0 = ((uint8_t)str[pos+0] & 0x1F) ^ 0x10;
	idx1 = ((uint8_t)str[pos+1] & 0x1F) ^ 0x10;
	bytes[pos/2] = (uint8_t)(hashmap[idx0] << 4) | hashmap[idx1];
	};
return true;
}
/*===========================================================================*/
size_t ishexify(const char *string)
{
size_t len;

len = strlen(string);
if (len < 6) return 0;
if ((len &1)  == 1) return 0;
if (string[0]      != '$') return 0;
if (string[1]      != 'H') return 0;
if (string[2]      != 'E') return 0;
if (string[3]      != 'X') return 0;
if (string[4]      != '[') return 0;
if (string[len -1] != ']') return 0;
return (len -6)/2;
}
/*===========================================================================*/
char** create_upper_array(const char** from, size_t size)
{
size_t len;
size_t i;
uint8_t j;
char** to;

to = (char**)malloc(size * sizeof(char *));
if(to == NULL)
	{
	fprintf(stderr, "failed to allocate memory\n");
	exit(EXIT_FAILURE);
	}

for (i = 0; i < size; i++)
    {
    len = strlen(from[i]);
    to[i] = (char*)malloc(len * sizeof(char) + 1);
	if(to[i] == NULL)
		{
		fprintf(stderr, "failed to allocate memory\n");
		exit(EXIT_FAILURE);
		}
    for (j = 0; j <= len; j++) to[i][j] = toupper((unsigned char)from[i][j]);
    }
return to;
}
/*===========================================================================*/
void free_array(char** arr, size_t size)
{
size_t i;

for (i = 0; i < size; i++) free(arr[i]);
free(arr);
return;
}
/*===========================================================================*/
