#include <iostream>
#include <string.h>
#include "Record.h"
#include "DBFile.h"
using namespace std;

void outerUsage()
{
	cout << "Welcome to database administration!\n";
	cout << "below are the tables that we have in our database:\n";
	cout << "_____________________________________________________________________________\n";
	cout << "|   region, nation, part, supplier, partsupp, customer, orders, lineitem.   |\n";
	cout << "=============================================================================\n";
	cout << "decide what you want to do:\n";
	cout << "=======================================================================\n";
	cout << "=                     enter \"quit\": exit this program.                =\n";
	cout << "=    enter one of the table's name: begin working on that table       =\n";
	cout << "=======================================================================\n";
	cout << "\n>>> ";
	
}

void innerUsage(char *textTablePath, char *binTablePath)
{
	cout << "Now, you can read text format record from \"" << textTablePath << "\" to binary representation table \"" << binTablePath << "\".\n";
	cout << "decide what you want to do:\n";
	cout << "========================================================================================================================\n";
	cout << "=            enter \"help\": display this command instruction                                                            =\n";
	cout << "=          enter \"return\": return to main menu                                                                         =\n";
	cout << "=          enter \"create\": you will create \""; cout << binTablePath << "\", if the file exist, it will be truncated   =\n"; 
	cout << "=            enter \"open\": you will open \""; cout << binTablePath << "\", if the file doesn't exist, no effect!       =\n";
	cout << "=           enter \"close\": you will close file\""; cout << binTablePath << "\"                                         =\n";
	cout << "=            enter \"load\": you will load all records from text file \"" << textTablePath << "\" to             =\n";
	cout << "=                            binary file \"" << binTablePath << "\"                                                 =\n"; 
	cout << "=           enter \"write\": get one text record and write(append) it into bin table                                     =\n";
	cout << "=            enter \"read\": read a record from bin table, first use will return the first record from bin               =\n";
	cout << "=                            after you perform read continutively, the \"pointer\" will increment to next bin record     =\n";
	cout << "=   enter \"reset pointer\": set the reading pointer back to the start of the bin table                                  =\n";
	cout << "========================================================================================================================\n";
	cout << "\n> ";
}

Schema getSchema(char* relationName)
{
	Schema tmp("catalog", relationName);
	return tmp;
}

void printSchema(char *relationName, Schema &print)
{
	cout << "The schema for the table \"" << relationName << "\" is\n";
	Attribute * attr = print.GetAtts();
	int numOfAttr = print.GetNumAtts();
	cout << "---------------------------------------------\n";
	for(int i = 0; i < numOfAttr; ++i)
	{
		cout << attr[i].name << ":\t\t\t";
		switch(attr[i].myType)
		{
			case Int: cout << "Int\n";break;
			case Double: cout << "Double\n";break;
			case String: cout << "String\n"; break;
		}
	}	
	cout << "---------------------------------------------\n";
}

void info(char *relationName)
{
	cout << "The physical location for the table \"" << relationName << "\" which store the records as text file is in\n"; 
	cout << "\t/cise/tmp/dbi_sp11/DATA/10M/" << relationName << ".tbl\n";
	cout << "Our binary representation for the table will be stored in\n";
	cout << "\t./" << relationName << ".bin\n";
}

int main()
{
	outerUsage();
	char outerCmd[20];
	char textTablePath[100]="/cise/tmp/dbi_sp11/DATA/10M/";
	char binTablePath[100]="";
	while(cin >> outerCmd)
	{
		if(!strcmp(outerCmd, "quit"))
			break;
	if(strcmp(outerCmd, "region") && strcmp(outerCmd, "nation") && strcmp(outerCmd, "part") && strcmp(outerCmd, "supplier")
&& strcmp(outerCmd, "partsupp") && strcmp(outerCmd, "customer") && strcmp(outerCmd, "orders") && strcmp(outerCmd, "lineitem"))
		{
			cout << "Unknown Command\n\n";
			outerUsage();
			continue;	
		}
		getchar();
		//I must write in this way, I can not split this into two sentence, I do not know why
		Schema mySchema = getSchema(outerCmd);
		printSchema(outerCmd, mySchema);
		info(outerCmd);
		char innerCmd[20];
		strcat(textTablePath, outerCmd);
		strcat(textTablePath, ".tbl");
		strcat(binTablePath, outerCmd);
		strcat(binTablePath, ".bin");
		innerUsage(textTablePath, binTablePath);
		//now define a DBFile instance that would be used in next operations
		DBFile mydbfile;
		fType filetype = heap;
		//open the text record file for later use
		FILE *fp = fopen(textTablePath, "r");
		bool fileHasBeenOpened = false;
		bool fileHasBeenLoaded = false;
		while(gets(innerCmd) != NULL)
		{
			if(!strcmp(innerCmd, "return"))
			{
				if(fileHasBeenOpened)
					cout << "Sorry, you should first execute \"close\" before \"return\", because the file is still opened\n";
				else
					break;
			}
			else if(!strcmp(innerCmd, "help"))
				innerUsage(textTablePath, binTablePath);
			else if(!strcmp(innerCmd, "create"))
			{
/*
				if(mydbfile.Open(binTablePath))
				{
					cout << "The file \"" << binTablePath << "\" has already exist, are you sure to create a new one?\n";
					cout << "enter 'y' means continue creating, enter 'n' means quit this command\n";
					cout << "> ";
					char decision;
					cin >> decision;
					if(decision == 'y')
					{
						mydbfile.Close();
						mydbfile.Create(binTablePath, filetype, NULL);
						cout << "You have create binary table \""; cout << binTablePath << "\" successfully! The old one has been deleted\n";	
						cout << "Note, this new file is closed after you create it, you need open it in order to perform other action on it.\n";
					}
					else
					{
						mydbfile.Close();
						cout << "You decided to use the existing file.\n";
					}	
				}
				else
				{
*/
					mydbfile.Create(binTablePath, filetype, NULL);
					cout << "You have create binary table \""; cout << binTablePath << "\" successfully!\n";	
					cout << "if this file existed here before, then the file will be truncated.\n";
					cout << "Note, this new file is closed after you create it, you need open it in order to perform other action on it.\n";
			//	}
			}
			else if(!strcmp(innerCmd, "open"))
			{
				if(fileHasBeenOpened)
				{
					cout << "Sorry, the file \"" << binTablePath << "\" has been opened, if you want to open another binary relation file, ";
					cout << "you should first close this file, then return to select another file!\n";
				}
				else
				{
					int indicate = mydbfile.Open(binTablePath);	
					if(indicate == 0)
						cout << "You haven't create this file, please input create command first!!!!\n";
					else
					{
						fileHasBeenOpened = true;
						cout << "You have open the file \""; cout << binTablePath << "\" successfully!\n";
						cout << "Please remember execute \"close\" command before you exit this program\n";
					}
				}
			}
			else if(!strcmp(innerCmd, "close"))
			{
				if(fileHasBeenOpened)
				{
					int indicate = mydbfile.Close();
					fileHasBeenOpened = false;
					if(indicate == 0)
						cout << "Something goes wrong when you try to close file \"" << binTablePath << "\"!\n";

					cout << "You have successfully close file \""; cout << binTablePath << "\"!\n"; 
				}
				else
					cout << "This file \"" << binTablePath << "\" has been closed, no need to close it again.\n";

			}
			else if(!strcmp(innerCmd, "load"))
			{
				if(fileHasBeenOpened && fileHasBeenLoaded == false)
				{
					mydbfile.Load(mySchema, textTablePath);
					fileHasBeenLoaded = true;
					cout << "you have succesfully load all records in the text table to the binary table\n";
				}
				else
					cout << "you should first open the file!!!!!!!!!!\n";
			}

			else if(!strcmp(innerCmd, "write"))
			{
				if(fileHasBeenOpened && fileHasBeenLoaded == false)
				{
					Record tmp;
					if(tmp.SuckNextRecord(&mySchema, fp))
					{
						cout << "This time you will append belowing record to the binary disk file in \"" << binTablePath << "\"\n";
						tmp.Print(&mySchema);
						mydbfile.Add(tmp);							
						cout << "You have append the record successfully!\n";
					}
					else
						cout << "There is no more text records in \"" << textTablePath << "\"\n";
				}
				else
				{
					cout << "There are two cases that makes you command abort:\n";
					cout << "either file \"" << binTablePath << "\" has not been opened, please open it first!\n";
					cout << "or file \"" << binTablePath << "\" has been loaded from \"" << textTablePath << "\"\n";
				}	
			}
			else if(!strcmp(innerCmd, "read"))
			{
				if(fileHasBeenOpened)
				{
					Record tmp;
					if(mydbfile.GetNext(tmp))
					{
						cout << "You get a record from binary representation database table \"" << binTablePath << "\" succesfully!\n";
						cout << "The record is as follows:\n";
						tmp.Print(&mySchema);
					}
				}
				else
					cout << "file \"" << binTablePath << "\" has not been opened yet, please open it first!\n";
			}
			else if(!strcmp(innerCmd, "reset pointer"))
			{
				if(fileHasBeenOpened)
				{
					mydbfile.MoveFirst();			
					cout << "you have successfully reset the reading pointer to the first record in the binary disk file \"" << binTablePath << "\"\n";
				}
				else
					cout << "file \"" << binTablePath << "\" has not been opened yet, please open it first!\n";
			}
			else
			{
				cout << "Unknow command\n";				
			}		
			cout << "\n> ";
		}					
		fclose(fp);
		outerUsage();
	}
	return 0;
}
