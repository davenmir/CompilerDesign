/*
PROGRAMMER: Michael Davenport
Program #: Project 5
Due Date: 4/14/20
Instructor: Dr. Zhijiang Dong

Description: Constructing symbol table
*/

#include <sstream>
#include "TypeChecking.h"
#include "Absyn.h"

namespace semantics
{
	//insert a variable into the var/function symbol table
	void TypeChecking::insertVar(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a function into the var/function symbol table
	void TypeChecking::insertFunc(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "function " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a type into the type symbol table
	void TypeChecking::insertType(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getTypeEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getTypeEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getTypeEnv()->insert( name, entry );
	}

	const types::Type* TypeChecking::visit(const Absyn *v)
	{
		if ( dynamic_cast<const Exp *>(v) != NULL )
			return visit( dynamic_cast<const Exp *>(v) );
		else if ( dynamic_cast<const Var *>(v) != NULL )
			return visit( dynamic_cast<const Var *>(v) );
		else if ( dynamic_cast<const Dec *>(v) != NULL )
			return visit( dynamic_cast<const Dec *>(v) );
		else
			throw runtime_error("invalid node");
	}

	const types::Type* TypeChecking::visit(const Exp *e)
	{
		if (dynamic_cast<const OpExp*>(e) != NULL)			
			return visit((const OpExp*)e);
		else if (dynamic_cast<const VarExp*>(e) != NULL)	
			return visit((const VarExp*)e);
		else if (dynamic_cast<const NilExp*>(e) != NULL)	
			return visit((const NilExp*)e);
		else if (dynamic_cast<const IntExp*>(e) != NULL)	
			return visit((const IntExp*)e);
		else if (dynamic_cast<const StringExp*>(e) != NULL) 
			return visit((const StringExp*)e);
		else if (dynamic_cast<const CallExp*>(e) != NULL)	
			return visit((const CallExp*)e);
		else if (dynamic_cast<const SeqExp*>(e) != NULL)	
			return visit((const SeqExp*)e);
		else if (dynamic_cast<const AssignExp*>(e) != NULL) 
			return visit((const AssignExp*)e);
		else if (dynamic_cast<const IfExp*>(e) != NULL)		
			return visit((const IfExp*)e);
		else if (dynamic_cast<const WhileExp*>(e) != NULL)	
			return visit((const WhileExp*)e);
		else if (dynamic_cast<const ForExp*>(e) != NULL)	
			return visit((const ForExp*)e);
		else if (dynamic_cast<const BreakExp*>(e) != NULL)	
			return visit((const BreakExp*)e);
		else if (dynamic_cast<const LetExp*>(e) != NULL)	
			return visit((const LetExp*)e);
		else if (dynamic_cast<const ArrayExp*>(e) != NULL)	
			return visit((const ArrayExp*)e);
		else throw new runtime_error("TypeChecking.visit(Exp*)");
	}

	const types::Type* TypeChecking::visit(const Var *v)
	{
		if (dynamic_cast<const SimpleVar *>(v) != NULL)			
			return visit((const SimpleVar *) v);
		else if (dynamic_cast<const SubscriptVar *>(v) != NULL) 
			return visit((const SubscriptVar *)v);
		else throw new runtime_error("TypeChecking.visit(Var*)");
	}

	const types::Type* TypeChecking::visit(const Dec *d)
	{
		if (dynamic_cast<const TypeDec *>(d) != NULL)			
			return visit((const TypeDec *) d);
		else if (dynamic_cast<const VarDec *>(d) != NULL)		
			return visit((const VarDec *) d);
		else throw new runtime_error("TypeChecking.visit(Dec*)");
	}

	const types::Type* TypeChecking::visit(const Ty *t)
	{
		if (dynamic_cast<const NameTy *>(t) != NULL)			
			return visit((const NameTy *) t);
		else if (dynamic_cast<const ArrayTy *>(t) != NULL)		
			return visit((const ArrayTy *) t);
		else throw new runtime_error("TypeChecking.visit(Ty*)");
	}

	//functions checking semantic error of different type of Var nodes
	const types::Type* TypeChecking::visit(const SimpleVar *v)
	{
		/* check if the variable is defined by looking up the symbol table*/
		string name = v->getName();
		string msg;
		stringstream ss(msg);

		if (!(env.getVarEnv()->contains(name)))
		{
			ss << "undefined variable";
			error(v, ss.str());
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const SubscriptVar *v)
	{
		/* check both the variable and index */
		return NULL;
	}


	//functions checking semantic error of different type of Exp nodes
	const types::Type* TypeChecking::visit(const OpExp *e)
	{
		/* check both operands */
		const Exp* left = e->getLeft();
		if (left != NULL) {
			const types::Type* leftType = visit(left);
		}
		const Exp* right = e->getRight();
		if (right != NULL) {
			const types::Type* rightType = visit(right);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const VarExp *e)
	{
		/* check the variable */
		const Var* var = e->getVar();
		if (var != NULL) {
			visit(var);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const NilExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const IntExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const StringExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const CallExp *e)
	{
		/*
			step 1: check the function name
			step 2: check every argument expression
		*/
		string funcName = e->getFunc();

		string msg;
		stringstream ss(msg);
		if (!(env.getVarEnv()->contains(funcName)))
		{
			ss << "undefined function";
			error(e, ss.str());
		}

		// Check argument expression
		const ExpList* args = e->getArgs();
		while (args != NULL)
		{
			const Exp* head = args->getHead();
			if (head != NULL) {
				visit(head);
			}
			args = args->getRest();
		}


		return NULL;
	}

	const types::Type* TypeChecking::visit(const SeqExp *e)
	{
		/*	check every expression in the sequence */
		const ExpList* expList = e->getList();
		while (expList != NULL)
		{
			const Exp* head = expList->getHead();
			if (head != NULL) {
				visit(head);
			}
			expList = expList->getRest();
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const AssignExp *e)
	{
		/* check both variable and expression */
		const Var* var = e->getVar();
		if (var != NULL){
			visit(var);
		}
		const Exp* exp = e->getExp();
		if (exp != NULL) {
			visit(exp);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const IfExp *e)
	{
		/* check test condition, then-clause, and else-clause (if exists) */
		const Exp* test = e->getTest();
		if (test != NULL) {
			visit(test);
		}
		const Exp* thenClause = e->getThenClause();
		if (thenClause != NULL) {
			visit(thenClause);
		}
		const Exp* elseClause = e->getElseClause();
		if (elseClause != NULL) {
			visit(elseClause);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const WhileExp *e)
	{
		/* check both the test condition and loop-body expression */
		const Exp* test = e->getTest();
		if (test != NULL) {
			visit(test);
		}
		const Exp* body = e->getBody();
		if (body != NULL) {
			visit(body);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const ForExp *e)
	{
		/*
			step 1: begin a new scope for variable
			step 2: check var declaration, upper bound expression, and loop-body
			step 3: end the scope
		*/
		env.getVarEnv()->beginScope();
		const VarDec* var = e->getVar();
		if (var != NULL) {
			visit(var);
		}
		const Exp* hiExp = e->getHi();
		if (hiExp != NULL) {
			visit(hiExp);
		}
		const Exp* body = e->getBody();
		if (body != NULL) {
			visit(body);
		}
		env.getVarEnv()->endScope();
		return NULL;
	}

	const types::Type* TypeChecking::visit(const BreakExp *e)
	{
		//don't need to do anything
		return NULL;
	}

	const types::Type* TypeChecking::visit(const LetExp *e)
	{
		/*
			step 1: begin a new scope for type and variable/function symbol tables
			step 2: check every declaraion in the decl list
			step 3: check the body expression
			step 4: end the scope for both symbol tables
		*/
		env.getTypeEnv()->beginScope();
		env.getVarEnv()->beginScope();
		const DecList* decList = e->getDecs();
		while (decList != NULL)
		{
			const Dec* head = decList->getHead();
			if (head != NULL) {
				visit(head);
			}
			decList = decList->getRest();
		}
		const Exp* body = e->getBody();
		if (body != NULL) {
			visit(body);
		}
		env.getVarEnv()->endScope();
		env.getTypeEnv()->endScope();
		return NULL;
	}

	const types::Type* TypeChecking::visit(const ArrayExp *e)
	{
		/*
			step 1: check the type of the array by looking up the type symbol table
			step 2: check the size expression
			step 3: check the initial expression
		*/
		string msg;
		stringstream ss(msg);
		string typeName = e->getType();
		if (!env.getTypeEnv()->contains(typeName)) {
			ss << "undefined type";
			error(e, ss.str());
		}
		const Exp* size = e->getSize();
		if (size < 0) {
			visit(size);
		}
		const Exp* init = e->getInit();
		if (init != NULL) {
			visit(init);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const VarDec *d)
	{
		/*
			step 1: insert the variable to the var/function symbol table
			step 2: if the type information is provided, check the type
			step 3: check the initial expression
		*/
		symbol::SymTabEntry *entry = new symbol::SymTabEntry;
		entry->node = d;
		entry->level = env.getVarEnv()->getLevel();
		entry->info = NULL;
		insertVar(d->getName(), *entry);
		const NameTy* type = d->getType();
		if (type != NULL) {
			visit(type);
		}
		const Exp* init = d->getInit();
		if (init != NULL) {
			visit(init);
		}
		return NULL;
	}

	const types::Type* TypeChecking::visit(const TypeDec *d)
	{
		/*
			step 1: insert the name of the new type to the type symbol table
			step 2: check the type information of the new type
		*/
		const TypeDec *curTypeDec = d;
		while (curTypeDec != NULL)
		{
			symbol::SymTabEntry *entry = new symbol::SymTabEntry;
			entry->node = d;
			entry->level = env.getTypeEnv()->getLevel();
			entry->info = NULL;
			insertType(d->getName(), *entry);
			const Ty* ty = d->getTy();
			if (ty != NULL) {
				visit(ty);
			}
			curTypeDec = curTypeDec->getNext();
		}
		return NULL;
	}

	//functions checking semantic error of different type of Ty nodes
	const types::Type* TypeChecking::visit(const NameTy *t)
	{
		/*
			step 1: check if the type name is defined by looking up the type symbol table
		*/
		string msg;
		stringstream ss(msg);
		string typeName = t->getName();
		if (!env.getTypeEnv()->contains(typeName)) {
			ss << "undefined type";
			error(t, ss.str());
		}
		return NULL;
	}


	const types::Type* TypeChecking::visit(const ArrayTy *t)
	{
		/*
			step 1: Check the name of the array type by looking up the type symbol table
		*/
		string msg;
		stringstream ss(msg);
		string typeName = t->getName();
		if (!env.getTypeEnv()->contains(typeName)) {
			ss << "undefined type";
			error(t, ss.str());
		}
		return NULL;
	}


} // end of namespace semantics
