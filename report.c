/*
 Data analytics program to search for exam collaboration.
 ***************************************************************
 * Author         Dept.             Date          Notes
 ***************************************************************
 * Joseph D       Comp. Science. Nov 1 2020    Initial version.
 * Joseph D       Comp. Science. Nov 3 2020    Fixed output format.
*/

#include <stdio.h>
#include <string.h>

typedef int bool;
const bool FALSE=0, TRUE=1;
// Ideally, should also use #define to set up other constants like maximum length of a name, etc., here
//  instead of hard-coding constant values all through the code. 

struct logrecord
{
	char name[100];
	char IPAddress[50];
};

struct logrecord readLog(char* logline)
{
	struct logrecord lr;
	char *name = NULL, *ip = NULL;
	name = strtok(logline, ",");
	strcpy(lr.name, name);
	for(int i=0; i<6; i++) // Get to the last field.
		ip = strtok(NULL, ","); //parse down to the last field of the line.
	strcpy(lr.IPAddress, ip);	// which is the IP address.
	return lr;
}

bool checkNameExists(FILE* csvfile, char* name, char* ip)
{
	char logline[200];
	struct logrecord lr;

	while(!feof(csvfile))
	{
		fgets(logline, 200, csvfile);
		if(feof(csvfile)) break; //No more data;

		lr = readLog(logline);
		if(strcmp(lr.name, name) == 0) //found a match.
		{
			strcpy(ip, lr.IPAddress); //Save the IP Address.
			return TRUE;
		}
	}
	return FALSE;
}

bool findCollaborators(char* sname, char *sip, FILE* csvfile, FILE* rptfile)
{
	char logline[200];
	struct logrecord lr;
	bool found = FALSE;
	char prevname[100] = {'\0'}; //store the previously reported collaborator's name.

	while(!feof(csvfile))
	{
		fgets(logline, 200, csvfile);
		if(feof(csvfile)) break; //No more data;

		lr = readLog(logline);
		// Look for same IP address, but different user name
		if(strcmp(lr.IPAddress, sip) == 0 && strcmp(lr.name, sname) != 0 && strcmp(lr.name, prevname) !=0 ) //found a new match.
		{
			found = TRUE;
			strcpy(prevname, lr.name);
			fprintf(rptfile, "%s\n", lr.name); //Record the collaborator's name.
		}
	}
	return found;
}

int main(int argc, char* argv[])
{
	char logline[200], sname[100], sIPAddress[50];
	FILE *csvfile, *rptfile;
	bool found;

	if(argc != 4)
	{
		fprintf(stderr, "Usage ./report <csvfile> \"<student name>\" <reportfile>\n");
		return 1;
	}

	csvfile = fopen(argv[1],"rt");
	if(csvfile == NULL) //Where we able to open the csv file?
	{
		fprintf(stderr, "Error, unable to open csv file %s\n", argv[1]);
		return 1;
	}

	fgets(logline, 200, csvfile);//Move past the header.
	if(checkNameExists(csvfile, argv[2], sIPAddress) == FALSE) // check if the student name can be located, gather IP
	{
		fprintf(stderr, "Error, unable to locate %s\n", argv[2]);
		return 1;
	}

	rptfile = fopen(argv[3], "wt"); //Time to create the output file.
	if(rptfile == NULL) //Where we able to open the output file?
	{
		fprintf(stderr, "Error, unable to open the output file %s\n", argv[3]);
		return 1;
	}

	//Important !! Need to start searching from the top for collaborators.
	fseek(csvfile, 0, SEEK_SET); // equivalent to rewind(csvfile)
	fgets(logline, 200, csvfile);//Move past the header.

	found = findCollaborators(argv[2], sIPAddress, csvfile, rptfile);
	if(!found)
		fprintf(rptfile, "No collaborators found for %s\n", argv[2]);

	//Close the file descriptors.
	fclose(csvfile); fclose(rptfile);

	return 0;
}
