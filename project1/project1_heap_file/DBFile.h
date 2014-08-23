#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
private:
	//I only define one buffer for both both reading and writing as the assignment request
	Page *buffer;
	//tool is for actually read page and write page
	//in fact, tool will work on buffer
	File tool;
	//I need two variables to keep record of where the user has his reading pointer on
	//in fact, these two variables act as pointer to the disk file.
	//currentReadingPage begins from 0, currentReadingRecord begins from 0, too.
	int currentReadingPage;
	//currentReadingPage is a little weird, this indicate the index of a record that hasn't
	//accessed, it is a record that after the "factual current working record"
	int currentReadingRecord;
	//writing doesnot need a record pointer since we always append record to the file
	int currentWritingPage;
	//this variable tell us whether user has add record to the disk file or not.
	bool fileChanged;

public:
	DBFile(); 

	int Create (char *fpath, fType file_type, void *startup);
	int Open (char *fpath);
	int Close ();

	void Load (Schema &myschema, char *loadpath);

	void MoveFirst ();
	void Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

};
#endif
