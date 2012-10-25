
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <set>
#include <iterator>
#include "shaun.hpp"

using namespace std;

struct Nondeterministic_Finite_Automaton {
    vector< map<char, set<int> > > table;
    set<int> accepting_states;
    set<char> alphabet;
    
    void setAlphabet(string E) 
    /* Given a string, add those characters to the 
       alphabet set. */
    {
        alphabet = set<char>(E.begin(), E.end());
    }
    
    void setAccepting(vector<int> A) 
    {
        accepting_states = set<int>(A.begin(), A.end());
    }

    Nondeterministic_Finite_Automaton(int nstates) 
    {
        table.resize(nstates);
    }
    
    set<int> &delta(int src_q, char sigma) 
    {
        assert(alphabet.count(sigma) == 1);
        assert(src_q >= 0 && src_q < table.size());

        // If the sigma is not in there, add it.
        if (table[src_q].count(sigma) == 0)
            table[src_q][sigma] = set<int>();
        
        return table[src_q][sigma];
    }

    bool isAccepting(int q) 
    {
        return (accepting_states.count(q) == 1);
    }
    
    bool isAccepting(set<int> q) 
    {
        set<int> result;
        set_intersection( 
                         accepting_states.begin(), accepting_states.end(), 
                         q.begin(), q.end(),
                         insert_iterator< set<int> >( 
                                                     result, 
                                                     result.begin() ) );        
        
        return (result.size() > 0); // true if the A \cup Q has any elements.
        
    }
    
    set<int> lambda_closure(int q) 
    {
        set<int> L;
        L.insert(q);
        
        int size = -1;
        while (size != L.size())
            {
            size = L.size();
            
            set_union(L.begin(), L.end(), 
                      delta(q, '^').begin(), delta(q, '^').end(),
                      insert_iterator< set<int> >( L, L.begin()));
            
            }
        return L;
    }

    void eliminate_lambda_transitions() 
    {
        if (alphabet.count('^') == 0)
            return;
        
        set<int> s;
        
        // For each state, q...
        for (int q = 0; q < table.size(); q++) 
            {
            set<int> L = lambda_closure(q);

            // And every sigma...
            for (map<char,set<int> >::iterator it = table[q].begin();
                 it != table[q].end();
                 it++)
                {
                
                char sigma = it->first;
                
                if (sigma == '^')
                    continue;
                
                // Take the union of the lambda closure of q and add
                // it to the delta(q, sigma)
                set_union(L.begin(), L.end(), 
                          delta(q, sigma).begin(), delta(q, sigma).end(),
                          insert_iterator<set <int> >(delta(q, sigma), 
                                                      delta(q, sigma).begin()));

                // For all of the states, l, in the lambda closure,
                // add the transitions for delta(l, sigma) to delta(q,
                // sigma).
                for (set<int>::iterator l = L.begin(); 
                     l != L.end();
                     l++)
                    {
                    set_union(delta(q, sigma).begin(), delta(q, sigma).end(), 
                              delta(*l, sigma).begin(), delta(*l, sigma).end(),
                              insert_iterator<set <int> >(delta(q, sigma), 
                                                          delta(q, sigma).begin()));
                    }

                // Finally, remove q from delta(q, sigma) since it is redundant.
                delta(q, sigma).erase(delta(q, sigma).find(q));
                }            
            }
        
        // Remove ^-transitions.
        for (int q = 0; q < table.size(); q++) 
            {
            table[q].erase(table[q].find('^'));
            }

        alphabet.erase(alphabet.find('^'));
        
    }

    string toString() 
    {
        stringstream ss;
        
        for (int q = 0; q < table.size(); q++) 
            {
            for (map<char, set<int> >::iterator it = table[q].begin();
                 it != table[q].end();
                 it++)
                {
                char sigma = it->first;
                if (table[q][sigma].size() == 0)
                    continue;
                
                ss << q << " -" << sigma << "-> ";
                
                copy(delta(q, sigma).begin(), delta(q, sigma).end(),
                     ostream_iterator<int>(ss, " "));
                
                ss << endl;
                
                }
            
            }

        return ss.str();
        
        
    }
    
    
    
};


int main(int argc, char *argv[])
{
    Nondeterministic_Finite_Automaton nfa(3);
    nfa.setAlphabet("ab^");
    nfa.delta(0, 'a').insert(1);
    nfa.delta(0, 'b').insert(2);
    nfa.delta(0, '^');

    nfa.delta(1, 'a').insert(0);
    nfa.delta(1, 'b');
    nfa.delta(1, '^').insert(2);

    nfa.delta(2, 'a').insert(1);
    nfa.delta(2, 'b').insert(1);
    nfa.delta(2, '^');
    
    for (int q = 0; q < nfa.table.size(); q++) 
        {
        cout << "Lambda closure of " << q << ":" << endl;
        set<int> L = nfa.lambda_closure(q);
        copy(L.begin(), L.end(), ostream_iterator<int>(cout, " "));

        cout << endl;
        }
    
    cout << endl;
    
    cout << nfa.toString() << endl;
    
    nfa.eliminate_lambda_transitions();
    
    cout << nfa.toString() << endl;
    
    cout << "Done." << endl;
    return 0;
}
