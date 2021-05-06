#pragma once
#include "stdafx.h"
#include <iostream>
#include "TempestClassParser.h"

static std::map<std::string, tempestClassDetails*>* s_FoundClasses;
static std::string baseName;

using namespace clang;
class classMatcher
{
public:
	classMatcher(std::string p_className) : m_className(p_className) {}
	std::string m_className;

	bool callback(const CXXRecordDecl* p_decl)
	{
		return p_decl->getQualifiedNameAsString() == m_className;
	}
	CXXRecordDecl::ForallBasesCallback getCallback()
	{

	}
};

bool callback(const CXXRecordDecl* p_decl)
{
	std::string showname = p_decl->getQualifiedNameAsString();
	return p_decl->getQualifiedNameAsString() != baseName;

}

bool SearchForBaseClass(std::string p_className, CXXRecordDecl* p_decl)
{
	std::string currentclassname = p_decl->getQualifiedNameAsString();
	baseName = p_className;

	int vbase = p_decl->getNumVBases();
	int norm = p_decl->getNumBases();;

	if (norm + vbase == 0)
	{
		return false;
	}
	return !p_decl->forallBases(callback, false);
}


class FindNamedClassVisitor : public RecursiveASTVisitor<FindNamedClassVisitor> {

public:

	//This is an overloaded version of the TraverseDecl function.
	//The purpose of this overload is to skip files that we don't want to parse, such as files in the STL.

	//!!!!CURRENTLY CRASHES WHEN CALLING D->getLocation() !!!!

	/*bool TraverseDecl(Decl *D)
	{
		if(!D)
		{
			return true;
		}

		if(mContext->getSourceManager().isInSystemHeader(D->getLocation()) || mContext->getSourceManager().isInExternCSystemHeader(D->getLocation()))
		{
			return false;
		}
		return RecursiveASTVisitor<FindNamedClassVisitor>::TraverseDecl(D);
	}*/

	explicit FindNamedClassVisitor(ASTContext *Context)
		: mContext(Context)
	{
	}

	bool VisitFieldDecl(FieldDecl *declaration) {

		//This function will visit the class members we have traversed.
		//The goal is to store data about these members in tempestMembers which are attached to tempestClassDetails.
		//The definitions for these classes can be found at the top of TempestClassParser.h and are contained in the s_FoundClasses global.

		//!!!THIS CODE IS UNTESTED AND CRASHES SOMETIMES!!!
		//!!!BE CAREFUL WHEN ACCESSING SPECIFIC TYPES! A BAD ACCESS WILL CRASH!!!
		/*if (declaration->getType()->isBuiltinType())
		{
			std::cout << declaration->getName().str() << " is a " << declaration->getType()->getAs<BuiltinType>()->getNameAsCString(PrintingPolicy(LangOptions())) << std::endl;
		}

		if (declaration->getType()->isClassType())
		{
			std::cout << declaration->getName().str() << " is a " << declaration->getType()->getAs<RecordType>()->getDecl()->getQualifiedNameAsString() << std::endl;
		}
		if (declaration->getType()->isPointerType())
		{
			std::cout << declaration->getName().str() << " is a pointer to a " << declaration->getType()->getAs<PointerType>()->getPointeeType()->getAs<RecordType>()->getDecl()->getQualifiedNameAsString() << std::endl;
		}*/

		return true;
	}

	bool VisitCXXRecordDecl(CXXRecordDecl *declaration) {

		if(mContext->getSourceManager().isInSystemHeader(declaration->getLocation()) || mContext->getSourceManager().isInExternCSystemHeader(declaration->getLocation()))
		{
			return true;
		}
		if (!declaration->isClass()|| declaration->getQualifiedNameAsString()._Starts_with("std::") || !declaration->isThisDeclarationADefinition())
		{
			return true;
		}
		//std::cout << "This class is named: " << declaration->getQualifiedNameAsString() << "  " << "I found it in the file: " << mContext->getFullLoc(declaration->getBeginLoc()).getFileEntry()->tryGetRealPathName().str() << std::endl;

		if (s_FoundClasses->find(declaration->getNameAsString()) == s_FoundClasses->end())
		{
			std::string name = declaration->getQualifiedNameAsString();
			std::filesystem::path file_location(mContext->getFullLoc(declaration->getBeginLoc()).getFileEntry()->tryGetRealPathName().str());
			(*s_FoundClasses)[declaration->getQualifiedNameAsString()] = new tempestClassDetails(
				name,
				SearchForBaseClass("component", declaration),
				SearchForBaseClass("scriptCPP", declaration),
				SearchForBaseClass("systemBase", declaration),
				declaration->isAbstract(),
				file_location
			);
		}
		return true;
	}

private:
	ASTContext *mContext;
	CXXRecordDecl *mLatestClass;
	FieldDecl *mLatestField;
};

// Glue from documentation
class FindNamedClassConsumer : public clang::ASTConsumer {
public:
	explicit FindNamedClassConsumer(ASTContext *Context)
		: Visitor(Context) {}

	virtual void HandleTranslationUnit(clang::ASTContext &Context) {
		Visitor.TraverseDecl(Context.getTranslationUnitDecl());
	}
private:
	FindNamedClassVisitor Visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
public:
	FindNamedClassAction(std::map<std::string, tempestClassDetails*>* p_foundClasses) { s_FoundClasses = p_foundClasses; }
	virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
		clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
		return std::unique_ptr<clang::ASTConsumer>(
			new FindNamedClassConsumer(&Compiler.getASTContext()));
	}
};