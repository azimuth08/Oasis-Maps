#ifndef READ_FILES_H
#define READ_FILE_H

#include <fstream>
#include "HashTable.h"
#include "MapStructs.h"
#include <sstream>
#include <string>
// removes any whitespace at the end of the string
void remove_spaces(string &s);
// gets every item in appropriate bucket and verifies it matches with the user input
// prints its state abbrev along with full state name from the state hashtable
std::string get_state_abbrev(Hashtable<string, Place*>& pT, Hashtable<string, State*>& sT, string userP);
      
bool is_state(Hashtable<std::string, State*>& sT, std::string userS);

bool is_place(Hashtable<string, Place*>& pT, string userP);

// collects data for place hashtable
void read_places(Hashtable<string, Place*>& pT);

void read_states(Hashtable<string, State*>&table);
 //
void read_intersections(string path, vector<Intersection*>& intersList);

// read aall roads from connections file
//creates road graph from pointer to intersections and back
void read_connections(string path, vector<Intersection*>& intersList);

// MAP FILES
//void 


#endif
