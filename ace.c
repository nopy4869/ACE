/*	ace.c
 *	
 *		Advanced copying executable
 *			accepts a list of files as an argument, or
 *			accepts -d to read the list of files from
 *			a file.
 *	
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv);
int parselist(char* filename);
int read(char* filename);
int write(char* filename);

char* errorcodes[] = {
	"\n",
	"File opening failed.\n",
	"Failed reading file.\n",
	"Failed writing file.\n",
	"Buffer overrun parsing filename.\n"
};

enum
{
	E_OPEN = 1,
	E_READ = 2,
	E_WRITE = 3,
	E_BUFF = 4
};

typedef struct{
	unsigned char* content;
	long long int size;
} document;

document file;

int main(int argc, char** argv)
{
	int x;
	int ret;
	printf("Advance Copying Executable!\nUsage: ace.exe [file to copy] [-d [name of output filename list]] [output filename(s)]\n");
	ret = read(argv[1]);
	if(ret != 0)
	{
		printf("Failed intial load\n");
		printf("%s", errorcodes[ret]);
		return ret;
	}
	else
	{
		for(x = 2; x < argc; x++)
		{
			if(strncmp(argv[x], "-d", 2) == 0)
			{
				x++;
				ret = parselist(argv[x]);
				if(ret != 0)
					printf("%s", errorcodes[ret]);
			}
			else
			{
				ret = write(argv[x]);
				if(ret != 0)
				{
					printf("Error writing %s to %s: %s",argv[1], argv[x], errorcodes[ret]);
				}
			}
		}
	}
	return 0;
};

int parselist(char* filename)
{
	printf("%s\n", filename);
	FILE* list;
	int x, ret;
	char in, e, nump;
	char* buff = malloc(2048);
	list = fopen(filename, "rb");
	if(list == NULL)
		return E_OPEN;
	for(x = 0; x < 2048; x++)
	{
		e = fread(&in, 1, 1, list);
		if(e == 0)
		{
			in = 0;
			buff[x] = 0;
			x = -1;
			ret = write(buff);
			if(ret == 0)
				nump++;
			else
				printf("%s", errorcodes[ret]);
			printf("End of list file\nEntries processed: %d\n", nump);
			return 0;
		}
		if(in == '\n')
		{
			in = 0;
			buff[x] = 0;
			x = -1;
			ret = write(buff);
			if(ret == 0)
				nump++;
			else
				printf("%s", errorcodes[ret]);
		}
		else if(in == '\r')
		{
			e = fread(&in, 1, 1, list);
			if(in == '\n')
			{
				in = 0;
				buff[x] = 0;
				x = -1;
				ret = write(buff);
				if(ret == 0)
					nump++;
				else
					printf("%s", errorcodes[ret]);
			}
			else
			{
				fseek(list, -1, SEEK_CUR);
				in = 0;
				buff[x] = 0;
				x = -1;
				ret = write(buff);
				if(ret == 0)
					nump++;
				else
					printf("%s", errorcodes[ret]);
			}
		}
		else if(in == '\0')
		{
			e = fread(&in, 1, 1, list);
			if(in == '\r')
			{
				e = fread(&in, 1, 1, list);
				if(in == '\n')
				{
					in = 0;
					buff[x] = -1;
					x = 0;
					ret = write(buff);
					if(ret == 0)
						nump++;
					else
						printf("%s", errorcodes[ret]);
				}
				else
				{
					fseek(list, -1, SEEK_CUR);
					in = 0;
					buff[x] = 0;
					x = -1;
					ret = write(buff);
					if(ret == 0)
						nump++;
					else
						printf("%s", errorcodes[ret]);
				}
			}
			else if(in == '\n')
			{
				in = 0;
				buff[x] = 0;
				x = -1;
				ret = write(buff);
				if(ret == 0)
					nump++;
				else
					printf("%s", errorcodes[ret]);
			}
			else
			{
				fseek(list, -1, SEEK_CUR);
				in = 0;
				buff[x] = 0;
				x = -1;
				ret = write(buff);
				if(ret == 0)
					nump++;
				else
					printf("%s", errorcodes[ret]);
			}
		}
		else
			buff[x] = in;
	}
	return E_BUFF;
};

int read(char* filename)
{
	FILE* in;
	long long int read;
	in = fopen(filename, "rb");
	if(in == NULL)
		return E_OPEN;
	fseek(in, 0, SEEK_END);
	file.size = ftell(in);
	fseek(in, 0, SEEK_SET);
	file.content = malloc(file.size);
	read = fread(file.content, 1, file.size, in);
	fclose(in);
	if(read != file.size)
		return E_READ;
	return 0;
};

int write(char* filename)
{
	FILE* out;
	long long int written;
	out = fopen(filename, "wb");
	if(out == NULL)
		return E_OPEN;
	written = fwrite(file.content, 1, file.size, out);
	fclose(out);
	if(written != file.size)
		return E_WRITE;
	return 0;
};
