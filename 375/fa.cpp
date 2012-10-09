#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <sstream>
#include <cstdlib>
#include "shaun.hpp"

using namespace std;

struct Transition_Table {
    vector< map<char,int> > table;
    
    void resize(int new_size) {
        table.resize(new_size);
    }

    void insert(int q, char sigma, int q2)
    {
        table[q].insert(std::pair<char,int>(sigma, q2));
    }

    int delta(int q, char sigma) 
    {
        if (q != -1 && 
            q < table.size() && 
            table[q].find(sigma) != table[q].end())
            return table[q][sigma];
        
        return -1; // not found or unfindable.
    }
    
    int delta(int q, string sigma)
    {
        int transition = 0;

        for (int i = 0; i < sigma.size(); i++)
            transition = delta(transition, sigma[i]);

        return transition;
    }
    string toString() 
    {
        stringstream ss;
        for (int q = 0; q < table.size(); q++)
            {
            for (map<char,int>::iterator iter = table[q].begin(); 
                 iter != table[q].end(); 
                 iter++)
                {
                ss << q << " --" << iter->first 
                   << "--> " << iter->second << endl;
                }
            }
        return ss.str();
    }

    void minimize(vector<bool> accepting_states)
    {
        int nstates = table.size();
        vector<bool> triangle_array(nstates, false);

        // cout << "First pass." << endl;

        // Mark all states where p or q is in A (but not both)
        for (int p = 0; p < nstates-1; p++)
            {
            for (int q = p+1; q < nstates; q++)
                {
                if (accepting_states[p] ^ accepting_states[q]) 
                    {
                    triangle_array[(q-1)*q/2 + p] = true;
                    // cout << "Marking " << p << ", " << q << endl;
                    }
                }
            }
        
        // Mark all states where the ordered-pair (d(p, s), d(q, s)) is marked.
        int new_marks;
        int pass = 1;
        do 
            {
            new_marks = 0;
            ++pass;
            
            for (int p = 0; p < nstates - 1; p++)
                {
                for (int q = p+1; q < nstates; q++)
                    {
                    if (triangle_array[(q-1)*q/2 + p]) // skip already marked.
                        continue;
                    
                    // check each symbol in the assumed alphabet
                    for (map<char,int>::iterator symbol = table[q].begin(); 
                         symbol != table[q].end(); 
                         symbol++)
                        {

                        int p1 = delta(p, symbol->first);
                        int q1 = delta(q, symbol->first);

                        /* cout << "  checking (d(" 
                             << p << ", " << symbol->first << "), d("
                             << q << ", " << symbol->first << "))" 
                             << " = (" << p1 << ", " << q1 << ")" << endl; */

                        if (p1 == q1)
                            continue;
                        /* Re-order (p,q) because
                           the index formula requires p<q */
                        if ((p1 < q1
                            && triangle_array[(q1-1) * q1 / 2 + p1])
                            || ((p1 > q1)
                                && triangle_array[(p1-1) * p1 /2 + q1]))
                            {
                            triangle_array[(q-1)*q / 2 + p] = true;
                            new_marks++;
                            // cout << "Marking " << p << ", " << q << endl;
                            
                            break;
                            }
                        }
                    }
                }
            } while (new_marks > 0);

        // Everything below here doesn't really work.

        vector< vector<int> > groups(table.size());
        vector<int> group_membership(table.size());

        for (int p = 0; p < nstates - 1; p++)
            {
            for (int q = p+1; q < nstates; q++)
                {
                if (triangle_array[(q-1)*q/2 + p] == false)
                    { 
                    cout << "Combine " << p << " and " << q << endl;
                    if (p < q)
                        groups[q].push_back(p);
                    else 
                        groups[p].push_back(q);
                    }                
                }
            }

        // Remove empty groups.
        for (int i = 0; i < groups.size(); i++)
            {
            if (groups[i].size() == 0) {
                groups.erase(groups.begin()+i);
                i--;
            }
            }

        // update group_membership
        for (int g = 0; g < groups.size(); g++)
            {
            for (vector<int>::iterator s = groups[g].begin();
                 s != groups[g].end();
                 s++)
                {
                group_membership[*s] = g;
                }
            }

        Transition_Table new_table;
        new_table.resize(groups.size());
    
        for (int g = 0, i = 0; g < groups.size(); g++)
            {
            if (groups[g].size() == 0)
                continue;

            for (map<char,int>::iterator symbol = table[groups[g][0]].begin(); 
                 symbol != table[groups[g][0]].end(); 
                 symbol++)
                {            // for all symbols in alphabet
                char o = symbol->first;
                int dest_state = delta(groups[g][0], o);
                int mapped_dest_state = group_membership[dest_state];
                new_table.insert(i, o, mapped_dest_state);
                }
            i++;
            }

        table = new_table.table;
    }
};

struct Alphabet {
    bool alphabet[255] = {false};
    void fromString(string input_alphabet) 
    {
        for (int i = 0; i < input_alphabet.length(); i++)
            alphabet[input_alphabet[i]] = true;
    }

    bool contains(char o) 
    {
        return alphabet[o];
    }

    bool contains(string s) 
    {
        for (int i = 0; i < s.length(); i++)
            if (!alphabet[s[i]])
                return false;
        return true;
    }
    string toString() 
    {
        stringstream ss;
        for (int i = 0; i < 255; i++)
            {
            if (alphabet[i])
                {
                if (i > 0)
                    ss << " ";
                ss << (char)i;
                }
            }
        return ss.str();
    }
};

int main(int argc, char *argv[])
{

    if (argc < 4) 
        {
        cout << "SYNTAX: " 
             << argv[0] 
             << " <fa definition> <strings> <output file>"
             << " [minimized FA output file]" << endl;
        return 0;
        }

    argv++; 

    char *fa_definition_file_path = argv[0];
    char *strings_file_path = argv[1];
    char *output_file_path = argv[2];
    
    ifstream fa_definition(fa_definition_file_path);
    ifstream strings_file(strings_file_path);
    ofstream output_file(output_file_path);
    
    int nstates = 0;
    int naccepting = 0;
    Transition_Table transition_table;
    vector<bool> accepting_states;
    Alphabet alphabet;
    vector<string> strings;

    string line;
    enum {INIT, ALPHABET, NUM_STATES, NUM_ACCEPTING, 
          ACCEPTING_STATES, TRANSITIONS};
    unsigned int read_type = INIT;
    while (getline(fa_definition, line)) 
        {
        read_type++;
        if (read_type == ALPHABET) 
            alphabet.fromString(line);
        
        else if (read_type == NUM_STATES) {
            nstates = atoi(line.c_str());
            transition_table.resize(nstates);
            accepting_states.resize(nstates);
        }
        else if (read_type == NUM_ACCEPTING) {
            naccepting = atoi(line.c_str());
        }
        else if (read_type == ACCEPTING_STATES)
            {
            vector<string> str_accepted_states = split(line, " ");
            assert(str_accepted_states.size() == naccepting);
            for (int i = 0; i < str_accepted_states.size(); i++)
                accepting_states[atoi(str_accepted_states[i].c_str())] = true;
            }
        else // reading a transition
            {
            vector<string> transition = split(line, " ");
            if (transition.size() != 3)
                {
                cout << "Skipping (line " << read_type << "):" << line << endl
                     << " Reason: invalid transition format."
                     << endl;
                continue;
                }

            if (transition[1].length() > 1) 
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: symbol is wider than one character."
                     << endl;
                continue;
                }

            int source_q = atoi(transition[0].c_str());
            char symbol = transition[1][0];
            int dest_q = atoi(transition[2].c_str());

            if (!alphabet.contains(symbol))
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: transition contains character that is "
                     << "not in the alphabet."
                     << endl;
                continue;
                }

            if (source_q >= nstates || dest_q >= nstates) 
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: refers to a state that does not exist."
                     << endl;
                continue;
                }

            if (transition_table.delta(source_q, symbol) != -1) 
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: Entry already in transition table "
                     << "for state " << source_q << " on symbol " << symbol 
                     << "." << endl;
                continue;
                }

            transition_table.insert(source_q, 
                                    symbol,
                                    dest_q);
            }

        }
  
    assert(read_type >= TRANSITIONS);

    // Time to read in the strings file.

    while (getline(strings_file, line) && line != "~")
        {
        if (!alphabet.contains(line)) 
            {
            cout << "Skipping: string '" << line << "' contains symbol(s) "
                 << "not found in the alphabet." << endl;
            continue;
            }
        strings.push_back(line);
        }


    // Print status
    cout << "\nalphabet: " << alphabet.toString() << endl
         << "nstates : " << nstates << endl
         << "astates : " << naccepting  
         << " (" << vector_to_string(accepting_states) << ')' << endl
         << "Transitions: " << endl
         << transition_table.toString() << endl
         << "Strings read: " << endl
         << vector_to_string(strings) << endl << endl
         << "String results:" << endl;
    
    // Test all strings
    for (int i = 0; i < strings.size(); i++)
        {
        if (accepting_states[transition_table.delta(0, strings[i])])
            cout << strings[i] << "\t" << "PASS" << endl;
        else
            cout << strings[i] << "\t" << "FAIL" << endl;
        }

    cout << "\nMinimizing: " << endl;
    transition_table.minimize(accepting_states);

    cout << "Resulting FA:" << endl
         << transition_table.toString() << endl;
    cout << "Done." << endl;
    return 0;
}
