#include "readFiles.h"


// removes any whitespace at the end of the string
void remove_spaces(string &s)
{
        int bufferSize = 33;
        if(s.empty())
        {
                cerr << "empty string" << endl;
        }
		else
		{
			for (int i = bufferSize - 1; i >= 0; i--)
			{
				if (s[i] == ' ')
				{
					s.pop_back();
				}
				else
				{
					break;
				}
			}
		}
}

// gets every item in appropriate bucket and verifies it matches with the user input
// prints its state abbrev along with full state name from the state hashtable
string get_state_abbrev(Hashtable<string, Place*>& pT, Hashtable<string, State*>& sT, string userP)
{
        vector<Place*> placeB = pT.getData(userP, hash_function_s);
        vector<State*> stateB, stateList;
        if(placeB.size() == 0)
        {
                cerr << "Place does not exist (case sensitive)\n";
        }
        cout << "Please enter corresponding state abbrev\n";
        cout << "-------------------------------\n";
        // find all same places and their different states
        for(int i=0;i<placeB.size();i++)
        {
                if(placeB[i]->name == userP)
                {
                        stateB = sT.getData(placeB[i]->stateAbbrev, hash_function_s);
                        if(stateB[0] == NULL)
                        {
                                cerr << "ERROR with state table returning null value\n";
                                return "NULL";
                        }
                        for(int j=0;j<stateB.size();j++) // add state to the list of states with same places
                        {
                                if(stateB[j]->abbrev == placeB[i]->stateAbbrev)
                                {
                                        cout << stateB[j]->abbrev << " " << stateB[j]->name << endl;
                                        stateList.push_back(stateB[j]);
                                }
                        }
                }
        }
        cout << "-------------------------------\n";

        string userS = "NULL";
        while(true)
        {
                cout << ": ";
                getline(cin,userS);
                for(int i=0;i<stateList.size();i++)
                {
                        if(stateList[i]->abbrev == userS)
                        {
                                return userS;
                        }
                }
                cout << "\"" << userS << "\" is not valid. Please select one of the above states.\n";
        }
        return userS;
}

bool is_state(Hashtable<string, State*>& sT, string userS)
{
        vector<State*> stateB = sT.getData(userS, hash_function_s);
        if(stateB.size() == 0)
        {
                return false;
        }

        for(int i=0;i<stateB.size();i++)
        {
                if(stateB[i]->abbrev == userS)
                {
                        return true;
                }
        }

        return false;
}

bool is_place(Hashtable<string, Place*>& pT, string userP)
{
        vector<Place*> placeB = pT.getData(userP, hash_function_s);
        if(placeB.size() == 0)
        {
                return false;
        }

        for(int i=0;i<placeB.size();i++)
        {
                if(placeB[i]->name == userP)
                {
                        return true;
                }
        }

        return false;
}

// collects data for place hashtable
void read_places(Hashtable<string, Place*>& pT)
{
        string path = "named-places.txt";
        ifstream file(path);
        if(file.is_open())
        {
                string line;
                while(getline(file,line))
                { // read in new place object
                        struct Place* p = new Place();
                        char* c;
                        p->code = stoi(line.substr(0,8));
                        p->stateAbbrev = line.substr(8,2);
                        p->name = line.substr(10,33);
                        remove_spaces(p->name);
                        p->population = strtol(line.substr(43,23).c_str(), &c, 10);
                        p->area = strtod(line.substr(66, 15).c_str(), &c);
                        p->latitude = strtod(line.substr(81, 11).c_str(), &c);
                        p->longitude = strtod(line.substr(91, 10).c_str(), &c);
                        p->roadCode = strtol(line.substr(101, 5).c_str(), &c, 10);
                        p->distanceFrom =strtod(line.substr(106, 7).c_str(), &c);

                        pT.add(p,hash_function_s);
                }
        }
        file.close();
}

void read_states(Hashtable<string, State*>&table)
{
        string path = "states.txt";
        ifstream file(path);
        if(file.is_open())
        {
                string line;
                while(getline(file,line))
                {
                        State* s = new State();
                        s->abbrev = line.substr(0,2);
                        if(!s->abbrev.empty() && s->abbrev.back() =='\r')
                        {
                                s->abbrev.pop_back();
                        }
                        s->name = line.substr(3, line.size() - 3); // get rest of line after abbreviation
                        table.add(s, hash_function_s);
                }
        }

        file.close();
}// collect innterrections into intersections table
 //
void read_intersections(string path, vector<Intersection*>& intersList)
{
        ifstream file(path);

        if(file.is_open())
        {
                string line;
                int lineN = 0; // start at 0
                while(getline(file, line))
                {
                        stringstream lineS(line);
                        string nPlaceholder;
                        Intersection* inters = new Intersection();
                        inters->num = lineN++;
                        lineS >> inters->longitude >> inters->latitude >> inters->distanceToPlace;
                        lineS >> inters->stateAbbrev;
                        while(lineS >> nPlaceholder)
                        {
                                nPlaceholder.append(" "); // space between words
                                inters->name.append(nPlaceholder);
                        }
                        inters->name.pop_back(); // delete extra space
                        intersList.push_back(inters);
                }
        }
        else
        {
                cerr << "Unable to open file\n";
        }
        file.close();
}

// read aall roads from connections file
//creates road graph from pointer to intersections and back
void read_connections(string path, vector<Intersection*>& intersList)
{
        ifstream file(path);

        if(file.is_open())
        {
                string line;

                while(getline(file,line))
                {
                        int aNum, bNum;
                        stringstream lineS(line);
                        string nPlaceholder;
                        Road* r = new Road();

                        lineS >> r->name >> r->type >> aNum >> bNum >> r->length;
                        // ceate edge connection from A side of road to A intersection
                        r->intA = intersList[aNum];
                        intersList[aNum]->exits.push_back(r);

                        // ceate edge connection from b side of road to B intersection
                        r->intB = intersList[bNum];
                        intersList[bNum]->exits.push_back(r);
                }
        }
        else
        {
                cerr << "Unable to open file\n";
        }
        file.close();
}


