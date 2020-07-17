/*
PROGRAMMER: Michael Davenport
Program #: Project 6
Due Date: 4/25/20
Instructor: Dr. Zhijiang Dong

Description: Performing type checking operations
*/
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

namespace symbol
{

	//get the level of current scope
	template<class Entry>
	int SymbolTable<Entry>::getLevel() const
	{
		return level;
	}

	//Save the information of current scope to the string dumpinfo
	//It should be called at the beginning of the endScope function.
	template<class Entry>
	void SymbolTable<Entry>::dump()
	{
		int			level = tables.size();
		string		indent;
		HashTable	current = *(tables.begin());
		HashTable::iterator	it = current.begin();

		for(int i=0; i<level; i++)
			indent += "    ";
		string			info;
		stringstream	ss(info);

		for(; it != current.end(); ++it)
			ss << indent << it->first << endl;
		ss << indent << "-------------------------------" << endl;
		dumpinfo += ss.str();
	}

	//to remove all scopes from the hashtable list so that
	//all remaining scopes will be dumped.
	template<class Entry>
	SymbolTable<Entry>::~SymbolTable()
	{
		//delete all remaining scopes 
		for(unsigned int i=0; i<tables.size(); i++)
			endScope();

		cout << "*******************Symbol Table************************" << endl;
		cout << dumpinfo << endl;
		cout << "*******************Symbol Table************************" << endl;
	}


	//Retrieve the value associated with the given lexeme in the hashtable list
	//search from the head to tail
	//an exception is thrown if the lexeme doesn't exist
	template<class Entry>
	Entry& SymbolTable<Entry>::lookup(string lexeme)
	{
		for(Iterator it=tables.begin(); it!=tables.end(); it++)
		{
			HashTable		&current = *it;
			HashTable::iterator	vit = current.find(lexeme);

			if ( vit != current.end() )
			{
				return current[lexeme];	//found the lexeme
			}
		}
		throw runtime_error("The given Lexeme " + lexeme + " doesn't exist!") ;
	}

	//Retrieve the value associated with the given lexeme in the global level only
	//an exception is thrown if the lexeme doesn't exist
	template<class Entry>
	Entry& SymbolTable<Entry>::globalLookup(string lexeme)
	{
		Iterator	prev;
		Iterator	cur = tables.begin();

		while( cur != tables.end() )
		{
			prev = cur;
			cur ++;
		}

		HashTable		&current = *prev;

		if ( current.find(lexeme) != current.end() )
		{
			return current[lexeme];	//found the lexeme
		}
		throw runtime_error("The given Lexeme " + lexeme + " doesn't exist!") ;
	}

	//create a new scope as the current scope
	template<class Entry>
	void SymbolTable<Entry>::beginScope()
	{
		level ++;
		tables.push_front( HashTable() );
	}

	//destroy the current scope, and its parent becomes the current scope
	template<class Entry>
	void SymbolTable<Entry>::endScope()
	{
		dump();

		//remove current scope level from the symbol table
		if ( level >= 0 )
		{
			tables.pop_front();
			level --;
		}
	};

	/****************************
	Provide implementation of all other member functions here
	****************************/
	
	//check if a lexeme is contained in the symbol table list
	//search from the head to tail
	template<class Entry>
	bool SymbolTable<Entry>::contains(string lexeme)
	{
		for (Iterator it = tables.begin(); it != tables.end(); it++)
		{
			HashTable		current = *it;

			if (current.find(lexeme) != current.end())
				return true;	//found the lexeme
		}
		return false;
	}


	template<class Entry>
	bool SymbolTable<Entry>::localContains(string lexeme)
	{
		Iterator	it = tables.begin();

		if ( it->find(lexeme) != it->end() )
			return true;	//found the lexeme
		else
			return false;
	}

	//check if a lexeme is contained in the global level
	template<class Entry>
	bool SymbolTable<Entry>::globalContains(string lexeme)
	{
		Iterator	it = tables.begin();

		if (it->find(lexeme) != it->end())
			return true;	//found the lexeme
		else
			return false;
	}

	//insert a lexeme and binder to the current scope, i.e. the first hashtable in the list
	//if it exists, an exception will be thrown
	template<class Entry>
	void SymbolTable<Entry>::insert(string lexeme, const Entry value)
	{
		string			info;
		stringstream	ss(info);

		ss << "The ID " << lexeme << " already exist in the current scope!" << endl;

		Iterator		current = tables.begin();

		if (current->find(lexeme) != current->end())
			throw ss.str();

		current->insert({lexeme, value});
	}

} //end of namespace Environment



