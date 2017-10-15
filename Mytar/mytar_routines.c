#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes)
{
	int totalBytes = 0;
	int byteOrigin; //char size equal to byte size

	if (origin == NULL || destination == NULL)
		return -1;

	while((totalBytes < nBytes) && ((byteOrigin = getc(origin)) != EOF)){
		putc(byteOrigin, destination);
		totalBytes++;
	}

	return totalBytes;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	char* stringFromFile;
	char charFromFile;
	int length = 0;
	int i;

	if (file == NULL)
		return NULL;

	while ((charFromFile = getc(file)) != '\0'){
		length++;
	}

	stringFromFile = malloc(sizeof(char)*(length+1));

	fseek(file,-(length+1),SEEK_CUR);

	for (i = 0; i < length+1; i++){
		stringFromFile[i] = getc(file);
	}

	return stringFromFile;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE *tarFile, int *nFiles)
{
	stHeaderEntry* array = NULL;
	int nrFiles = 0;
	int i;

	if (tarFile == NULL)
		return NULL;

	fread(&nrFiles,sizeof(int),1,tarFile);

	array = malloc(sizeof(stHeaderEntry)*nrFiles);

	for(i=0; i < nrFiles; i++){
		array[i].name = loadstr(tarFile);
		fread(&array[i].size, sizeof(int), 1, tarFile);
	}

	(*nFiles) = nrFiles;

	return array;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{

	FILE *outputFile;
	stHeaderEntry *array;
	int i;
	int offsetFile = 0;
	int bytesCopied;

	array = malloc(sizeof(stHeaderEntry)*nFiles);

	//calculation of offset size for output file
	offsetFile += sizeof(int); //for numfiles into header
	offsetFile += sizeof(int)* nFiles;

	for(i = 0; i < nFiles; i++){
		offsetFile += strlen(fileNames[i])+1;
	}

	if ((outputFile = fopen(tarName,"w")) == NULL)
		return EXIT_FAILURE;

	fseek(outputFile,offsetFile,SEEK_SET);

	for (i = 0; i < nFiles; i++){
		FILE *readFile = fopen(fileNames[i], "r");

		if (readFile == NULL)
			return EXIT_FAILURE;

		bytesCopied = copynFile(readFile, outputFile, INT_MAX);
		fclose(readFile);

		//save in the header information about name and size of file
		array[i].name = fileNames[i];
		array[i].size = bytesCopied;

	}

	rewind(outputFile);

	if ((fwrite(&nFiles,sizeof(int),1,outputFile)) != 1)
		return EXIT_FAILURE;


	for (i=0; i<nFiles;i++){
		fwrite(array[i].name, strlen(array[i].name)+1, 1, outputFile);
		fwrite(&array[i].size, sizeof(int), 1, outputFile);
	}

	free(array);

	fclose(outputFile);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{
	stHeaderEntry *array;
	FILE *inputFile;
	int numFiles;
	int i;

	if ((inputFile = fopen(tarName, "r")) == NULL)
		return EXIT_FAILURE;

	array = readHeader(inputFile, &numFiles);

	for (i=0; i<numFiles; i++){
		FILE *outputFile = fopen(array[i].name,"w");
		copynFile(inputFile,outputFile,array[i].size);
		fclose(outputFile);
	}

	//free memory
	for (i=0; i<numFiles; i++){
		free(array[i].name);
	}

	free(array);

	fclose(inputFile);

	return EXIT_FAILURE;
}
