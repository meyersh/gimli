#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include "shaun.hpp"

using namespace std;

struct Transition_Table {
    vector< map<char,int> > table;
    
    int size() 
    {
        return table.size();
    }
        
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

    string toFAString()
    {
        stringstream ss;

        // Dump the transition table.
        for (int q = 0; q < table.size(); q++)
            {
            for (map<char,int>::iterator iter = table[q].begin(); 
                 iter != table[q].end(); 
                 iter++)
                {
                ss << q << " " 
                   << iter->first << " " 
                   << iter->second << endl;
                }
            }
        return ss.str();
        
    }
    
    vector<bool> minimize(vector<bool> accepting_states)
    {
        int nstates = table.size();
        vector<bool> triangle_array(nstates, false);
        vector< vector<int> > groups(table.size());
        vector<int> group_membership(table.size());

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

                        /* Re-order (p1,q1) because
                           the index formula requires p1 < q1 */
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

        // Initialize our groups && group_membership data structures.
        for (int i = 0; i < table.size(); ++i) {
            group_membership[i] = i;
            groups[i].push_back(i);
        }

        for (int p = 0; p < nstates - 1; p++)
            {
            for (int q = p+1; q < nstates; q++)
                {
                if (triangle_array[(q-1)*q/2 + p] == false)
                    { 
                    cout << "Combine " << p << " and " << q << endl;
                    if (p < q) 
                        {
                        int dest_group = p;
                        // find our 'final' group.
                        while (group_membership[dest_group] != dest_group)
                            dest_group = group_membership[dest_group];
                            
                        groups[dest_group].push_back(q);
                        group_membership[q] = dest_group;
                        }
                    }                
                }
            }

        // Remove nodes that have been combined with others.
        for (int i = 0; i < group_membership.size(); i++)
            if (group_membership[i] != i)
                groups[i].erase(groups[i].begin());

        // Remove empty groups.
        for (int i = 0; i < groups.size(); i++)
            {
            if (groups[i].size() == 0) 
                {
                groups.erase(groups.begin()+i);
                i--;
                }
            }

        // update group_membership values to reflect groups 
        // that have been removed.
        for (int g = 0; g < groups.size(); g++)
            {
            for (vector<int>::iterator s = groups[g].begin();
                 s != groups[g].end();
                 s++)
                {
                group_membership[*s] = g;
                // cout << "g[" << g << "]: state " << *s << " -> group " << g << endl;
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
                { // for all symbols in alphabet
                char o = symbol->first;
                int dest_state = delta(groups[g][0], o);
                int mapped_dest_state = group_membership[dest_state];
                new_table.insert(i, o, mapped_dest_state);
                }
            i++;
            }

        table = new_table.table;

        // Compute and return the new accepting states.
        vector<bool> new_accepting_states(table.size(), false);
        for (int i = 0; i < accepting_states.size(); i++)
            if (accepting_states[i])
                new_accepting_states[group_membership[i]] = true;

        return new_accepting_states;
    }
};

struct Alphabet {
    bool alphabet[255] = {false};
    int alphabet_size = 0;
    
    int size() 
    {
        return alphabet_size;
    }
    
    void fromString(string input_alphabet) 
    {
        for (int i = 0; i < input_alphabet.length(); i++)
            if (isalpha(input_alphabet[i])) 
                {
                alphabet[input_alphabet[i]] = true;
                alphabet_size++;
                }
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
        for (int i = 0, written=0; i < 255; i++)
            {
            if (alphabet[i])
                {
                if (written)
                    ss << " ";
                ss << (char)i;
                written++;
                }
            }
        return ss.str();
    }
};

struct Deterministic_Finite_Automaton
{
    Transition_Table transition_table;
    Alphabet alphabet;
    vector<bool> accepting_states;
    int n_accepting;
    int n_states;
    
    int resize(int new_size) 
    {
        n_states = new_size;
        transition_table.resize(new_size);
        accepting_states.resize(new_size);
        return n_states;
    }
    

    bool accepts(string s) 
    {
        int delta = transition_table.delta(0, s);
        if (delta == -1)
            return false;
        
        return accepting_states[delta];
    }
    
    int nstates() 
    {
        return transition_table.size();
    }
    
    string accepting() 
    {
        stringstream ss;
        
        for (int i = 0, written=0; i < accepting_states.size(); i++) 
            {
            if (accepting_states[i]) 
                {
                if (written) 
                    {
                    ss << " ";
                    }
                
                ss << i;
                written++;
                
                }
            }
        return ss.str();
        
    }
    
    
    void minimize() 
    {
        accepting_states = transition_table.minimize(accepting_states);
        
        // update n_accepting
        n_accepting = 0;
        for (int i = 0; i < accepting_states.size(); i++)
            {
            if (accepting_states[i]) 
                n_accepting++;
            }
    }
    
    
    string toString() 
    {
        stringstream ss;
        
        ss << "alphabet: " << alphabet.toString() << endl
           << "nstates : " << nstates() << endl
           << "astates : " << n_accepting
           << " (" << accepting() << ')' << endl
           << "Transitions: " << endl
           << transition_table.toString() << endl;

        return ss.str();
        
    }
    
    string toFile() 
    {
        stringstream ss;
        ss << alphabet.toString() << endl
           << nstates() << endl
           << n_accepting << endl
           << accepting() << endl
           << transition_table.toFAString();
        
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
    char *minimized_fa_file_path = (argc == 5) ? argv[3] : NULL;
        
    ifstream fa_definition(fa_definition_file_path);
    ifstream strings_file(strings_file_path);
    ofstream output_file(output_file_path);
    
    Deterministic_Finite_Automaton fa;
    vector<string> strings;

    /*
     * Read in the FA definition file.
     * This uses a basic FA to read in the file where
     * the read_type variable contains one of our 
     * "reading"-states.
     */

    string line;
    enum {INIT, ALPHABET, NUM_STATES, NUM_ACCEPTING, 
          ACCEPTING_STATES, TRANSITIONS};
    unsigned int read_type = INIT;
    int expected_transitions = 0;
    int transitions_read = 0;
    while (getline(fa_definition, line)) 
        {
        read_type++;
        if (read_type == ALPHABET) 
            fa.alphabet.fromString(line);
        
        else if (read_type == NUM_STATES) {
        fa.resize(atoi(line.c_str()));
        }
        else if (read_type == NUM_ACCEPTING) {
            fa.n_accepting = atoi(line.c_str());
        }
        else if (read_type == ACCEPTING_STATES)
            {
            vector<string> str_accepted_states = split(line, " ");
            assert(str_accepted_states.size() == fa.n_accepting);
            for (int i = 0; i < str_accepted_states.size(); i++)
                fa.accepting_states[atoi(str_accepted_states[i].c_str())] = true;
            expected_transitions = fa.nstates() * fa.alphabet.size();
            }
        else // reading a transition
            {
            vector<string> transition = split(line, " ");
            if (transition.size() != 3)
                {
                cout << "Skipping (line " << read_type << "):" << line << endl
                     << " Reason: invalid transition format or empty line."
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

            if (!fa.alphabet.contains(symbol))
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: transition contains character that is "
                     << "not in the alphabet."
                     << endl;
                continue;
                }

            if (source_q >= fa.nstates() || dest_q >= fa.nstates()) 
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: refers to a state that does not exist."
                     << endl;
                continue;
                }

            if (fa.transition_table.delta(source_q, symbol) != -1) 
                {
                cout << "Skipping (line " << read_type << "): " << line << endl
                     << " Reason: Entry already in transition table "
                     << "for state " << source_q << " on symbol " << symbol 
                     << "." << endl;
                continue;
                }

            fa.transition_table.insert(source_q, symbol, dest_q);
            transitions_read++;
            }

        }
  
    if (read_type < TRANSITIONS) 
        {
        cout << "ERROR! Incomplete or missing FA definition file at '" 
             << fa_definition_file_path << "'!" << endl
             << "Nothing to do, bailing out." << endl << endl;
        return 1;
        }

    if (expected_transitions != transitions_read) 
        {
        cout << "WARNING! Read " << transitions_read << " transitions (expected: " 
             << expected_transitions << ".)" << endl
             << "  Unexpected behaviour may result." << endl;
        }
        
    // Print the FA we've just loaded.
    cout << "FA config:\n" << fa.toString() << endl;

    // Time to read in the strings file.
    cout << "Reading strings file..." << endl;
    int linum = 0;
    while (getline(strings_file, line) && line != "~")
        {
        linum++;
        if (!fa.alphabet.contains(line)) 
            {
            cout << "Strings file: (line: " << linum <<") '" << line << "' contains symbol(s) "
                 << "not found in the alphabet." << endl;
            continue;
            }
        strings.push_back(line);
        }
    cout << strings.size() << " strings read." << endl;
            
    // Test all strings - writing to cout and output_file as we go
    if (strings.size() == 0) 
        {
        cout << "ERROR! No strings have been read in from '" 
             << strings_file_path << "'!" << endl;
        }
    else 
        {
                     
        cout << "Testing strings:" << endl;
        for (int i = 0; i < strings.size(); i++)
            {
            if (fa.accepts(strings[i])) 
                {
                output_file << strings[i] << " " << "PASS" << endl;
                cout << strings[i] << "\t" << "PASS" << endl;
                }
            else
                {
                cout << strings[i] << "\t" << "FAIL" << endl;
                output_file << strings[i] << " " << "FAIL" << endl;
                }
            }
        }
    
    output_file.close();
    
    // minimize if there is a fourth parameter
    if (minimized_fa_file_path)
        {
        cout << "\nMinimizing: " << endl;
    
        fa.minimize();
        
        cout << endl << "Minimized FA:" << endl
             << fa.toString();
        
        ofstream minimized_fa(minimized_fa_file_path);
        
        minimized_fa << fa.toFile();
        
        minimized_fa.close();
                        
        // Test all strings
        for (int i = 0; i < strings.size(); i++)
            {
            if (fa.accepts(strings[i]))
                cout << strings[i] << "\t" << "PASS" << endl;
            else
                cout << strings[i] << "\t" << "FAIL" << endl;
            }
        
        }

    cout << "Done." << endl;
    return 0;
}
