#include "tinyxml2.cpp"

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <io.h>
#include <limits>

#include "UI.cpp"
#include "Point.h"
#include "Rectangle.h"
#include "Encapsulation.h"
#include "ClassAttributes.h"
#include "ClassMethod.h"
#include "ClassInfo.h"
#include "Package.h"
#include "Relation.h"
#include "Utility.h"

using namespace std;
using namespace tinyxml2;

vector<ClassInfo> classesInfo;

Package globalPackage;

Info info;

const pair<string, string> stdLibs[] = {
    {"std::string", "<string>"},
    {"std::vector", "<vector>"},
    {"std::map", "<map>"},
    {"std::mutex", "<mutex>"},
    {"_ptr", "<memory>"}
};

ClassInfo& getClassDataByName(const string& name)
{
    for(auto& classInfo : classesInfo)
        if(classInfo.name == name)
            return classInfo;

    throw runtime_error("Error! classData having "+name+" could not be found!");
}

const bool hasAttributeWithName(const vector<Attribute>& attributes, const string& name)
{
    for(auto& attribute : attributes)
        if(attribute.name == name)
            return true;

    return false;
}

string toUpper(const string& str)
{
    string upper;
    upper.reserve(str.length());

    if(isupper(str[0]))
        upper += "_";
    else 
        upper += "__";

    for(char c : str)
        if(isupper(c))
            (upper += '_') += c;
        else
            upper += toupper(c);

    upper += "_H__";

    return upper;
}

bool useStdObject(const ClassInfo& classInfo, const string& str)
{
    for(auto& attribute : classInfo.attributes)
        if(attribute.type.find(str) < attribute.type.length())
            return true;

    for(auto& method : classInfo.methods)
    {   
        if(method.returnType.find(str) < method.returnType.length())
            return true;

        for(auto& argument : method.argument)
            if(argument.type.find(str) < argument.type.length())
                return true;
    }

    return false;
}

void includeStdLibs(ostream& os, const ClassInfo& classInfo)
{
    bool needsEndl=false;

    for(auto& pair : stdLibs)
        if(useStdObject(classInfo, pair.first))
        {
            os << "#include " << pair.second << endl;
            needsEndl = true;
        }

    if(needsEndl)
        os << endl;
}

void includeDependantClasses(ostream& os, const ClassInfo& classInfo)
{
    for(auto& dependantClass : classInfo.dependantClasses)
        os << "#include \"" << globalPackage.getPath(classInfo, getClassDataByName(dependantClass)) << dependantClass << ".h\"" << endl;

    if(classInfo.dependantClasses.size()>0)
        os << endl;
}

void writeLockGuard(ostream& os, const Attribute& attribute)
{
    if(attribute.hasField("mutex"))
        os << "\tstd::lock_guard<std::mutex> lock("<<attribute.name<<"Mutex);" << endl;
}

Encapsulation writeAttributesInHeader(ostream& os, const ClassInfo& classInfo)
{
    Encapsulation prevEncap = PRIVATE;
    
    for(const Attribute& attribute: classInfo.attributes)
    {
        if(attribute.encapsulation!=prevEncap)
        {
            os<<attribute.encapsulation<<": "<<endl;
            prevEncap=attribute.encapsulation;
        }

        os<<"\t"<<attribute.type<<" "<<attribute.name<<";\n";
    }

    for(const Attribute& attribute: classInfo.attributes)
    {
        if(attribute.fields.size()>0)
            if(prevEncap!=PUBLIC)
            {
                os<<"\npublic: \n";
                prevEncap=PUBLIC;
            }

        if(attribute.hasField("get"))
        {
            if(attribute.type.find("std::unique_ptr")==0)
            {
                auto noUnique=attribute.type.substr(
                    strlen("std::unique_ptr<"),
                    attribute.type.length()-strlen("std::unique_ptr<")-1
                );
                os<<"\tconst "<<noUnique<<"& get"<<capitalize(attribute.name)<<"()";
            }

            else if(attribute.type.find("std::unique_ptr")<attribute.type.length()
            && attribute.type.find("std::unique_ptr")>2)
            {
                auto container = attribute.type.substr(0,attribute.type.find("<std::unique_ptr"));

                auto sub = attribute.type.substr(attribute.type.find("std::unique_ptr"));

                auto noUnique = sub.substr(
                    strlen("std::unique_ptr<"),
                    sub.length()-strlen("std::unique_ptr<")-2
                );

                os << "\t"<<container<<"<"<<noUnique<<" * const> get" << capitalize(attribute.name) << "()";
            }

            else
            {
                if(attribute.type.find("std::shared_ptr")==0
                || attribute.type.find("std::weak_ptr")==0)
                    os << "\tconst " << attribute.type << " get" << capitalize(attribute.name) << "()";
                else if(attribute.type.find('&')<attribute.type.length())
                    os << "\tconst " << attribute.type << " get" << capitalize(attribute.name) << "()";
                else if(attribute.isContainer())
                    os << "\tconst " << attribute.type << " get" << capitalize(attribute.name) << "()";
                else
                    os << "\tconst " << attribute.type << "& get" << capitalize(attribute.name) << "()";
            }

            if(!attribute.hasField("mutex"))
                os << " const";

            os << ";" << endl;
        }

        if(attribute.hasField("set"))
        {
            if(attribute.type.find("std::unique_ptr")==0)
                os << "\tvoid set"<<capitalize(attribute.name)<<"("+attribute.type<<" "+attribute.name+"_);" << endl;
            else if(attribute.type.find("std::shared_ptr")==0
            || attribute.type.find("std::weak_ptr")==0)
                os << "\tvoid set"<<capitalize(attribute.name)<<"(const "<<attribute.type<<" "+attribute.name+"_);" << endl;
            else if(attribute.type.find('&')<attribute.type.length())
                os << "\tvoid set"<<capitalize(attribute.name)<<"(const "<<attribute.type<<" "+attribute.name+"_);" << endl;
            else
                os << "\tvoid set"<<capitalize(attribute.name)<<"(const "<<attribute.type<<"& "+attribute.name+"_);" << endl;
        }

        if(attribute.hasField("has") || attribute.hasField("remove"))
        {
            if(!attribute.hasField("has"))
                os<<"private:\n";

            size_t begin=attribute.type.find('<');
            size_t end=attribute.type.find_last_of('>');

            string elemType=attribute.type.substr(begin+1, end-begin-1);

            os<<"\tbool has"<<capitalize(attribute.getSingularName())<<"(const ";

            if(elemType.find("std::unique_ptr")==0)
            {
                auto sub = attribute.type.substr(attribute.type.find("std::unique_ptr"));

                auto noUnique = sub.substr(
                    strlen("std::unique_ptr<"),
                    sub.length()-strlen("std::unique_ptr<")-2
                );

                os << noUnique << " * const";
            }

            else if(elemType.find("std::shared_ptr")!=0
            && elemType.find("std::weak_ptr")!=0)
                os << elemType << "&";
            else
                os << elemType;

            os << " " << attribute.getSingularName() <<")";
            
            if(!attribute.hasField("mutex"))
                os << " const";

            os << ";" << endl;

            if(!attribute.hasField("has"))
                os << "public:" << endl;
        }

        if(attribute.hasField("add"))
        {
            size_t begin = attribute.type.find('<');
            size_t end = attribute.type.find_last_of('>');

            string elemType = attribute.type.substr(begin+1, end-begin-1);

            os << '\t';

            if(attribute.hasField("has")
            || attribute.hasField("remove"))
                os << "bool ";
            else
                os << "void ";

            os << "add"<<capitalize(attribute.getSingularName())<<"(";

            if(elemType.find("std::unique_ptr")==0)
                os << elemType;
            else if(elemType.find("std::shared_ptr")==0
            || elemType.find("std::weak_ptr")==0)
                os << "const "<<elemType;
            else
                os << "const "<<elemType <<"&";

            os << " " << attribute.getSingularName() << ");" << endl;
        }

        if(attribute.hasField("remove"))
        {
            size_t begin = attribute.type.find('<');
            size_t end = attribute.type.find_last_of('>');

            string elemType = attribute.type.substr(begin+1, end-begin-1);

            os << "\tbool remove"<<capitalize(attribute.getSingularName())<<"(const ";

            if(elemType.find("std::unique_ptr")==0)
            {
                auto sub = attribute.type.substr(attribute.type.find("std::unique_ptr"));

                auto noUnique = sub.substr(
                    strlen("std::unique_ptr<"),
                    sub.length()-strlen("std::unique_ptr<")-2
                );

                os << noUnique << " * const";
            }
            else if(elemType.find("std::shared_ptr")!=0
            && elemType.find("std::weak_ptr")!=0)
                os << elemType << "&";
            else
                os << elemType;            

            os << " " << attribute.getSingularName() << ");"<< endl;
        }

        if(attribute.hasField("clear"))
            os << "\tvoid clear"<<capitalize(attribute.name)<<"();" << endl;

        if(attribute.fields.size()>0)
            os << endl;
    }

    return prevEncap;
}

ClassInfo& findClassContainingPoint(vector<ClassInfo>& classesInfo, const Point& point)
{
    for(auto& classInfo : classesInfo)
        if(classInfo.coords.contains(point))
            return classInfo;

    throw runtime_error("Error! classData containing point could not be found!");
}

void writeMethodsInHeader(ostream& os, const ClassInfo& classInfo, Encapsulation prevEncap)
{
    bool constructorWritten = false;

    for(const Method& method: classInfo.methods)
    {
        if(method.encapsulation!=prevEncap)
        {
            os<<method.encapsulation<<":"<<endl;
            prevEncap=method.encapsulation;
        }

        if(method.isDestructor())
            constructorWritten=true;
        
        if(!method.isConstructor() && !constructorWritten)
        {
            constructorWritten=true;
            
            if(classInfo.hasVirtualMethod())
                os<<"\tvirtual ~"+classInfo.name+"() {}\n\n";
            else
                os<<"\t~"+classInfo.name+"() {}\n\n";
        }

        if(method.name=="operator<=>")
        {
            string type=method.argument[0].type;
            type=type.substr(0, type.length()-1);
            type=normalizeName(type);

            if(type[0]==' ')
                type=type.substr(1);

            os<<endl;
            for(string ops: {"<",">","==","!="})
            {
                os<<"\tconst bool operator" <<ops<<
                "("<<method.argument[0].type<<" "<<method.argument[0].name<<") const";

                if(classInfo.isInterface)
                    os<<" = 0";

                os<<";"<<endl;            
            }
        }

        else if(method.isConstructor())
        {
            os<<"\t"<<classInfo.name<<"(";

            if(method.argument[0].type.empty())
            {
                const Attribute* argument = classInfo.getAttributeByName(method.argument[0].name);

                if(argument==nullptr)
                    throw runtime_error("Error! "+classInfo.name+"constructor argument "+
                    method.argument[0].name+" type could not be deduced!");

                string type=argument->type;

                if(type.find("std::unique_ptr")==0)
                    os << type << method.argument[0].name << "_";
                else if(type.find("std::shared_ptr")==0
                || type.find("std::weak_ptr")==0)
                    os << "const " << type << " " << method.argument[0].name << "_";
                else
                    os << "const " << type << "& " << method.argument[0].name << "_";
            }
            else
                os<<method.argument[0].type<<" "<<method.argument[0].name;

            for(size_t k=1 ; k<method.argument.size() ; k++)
            {
                os << ", ";

                if(method.argument[k].type.empty())
                {
                    const Attribute* argument = classInfo.getAttributeByName(method.argument[k].name);

                    if(argument == nullptr)
                        throw runtime_error("Error! "+classInfo.name+"constructor argument "+
                        method.argument[k].name+" type could not be deduced!");

                    string type = argument->type;

                    if(type.find("std::unique_ptr")==0)
                        os << type << method.argument[k].name << "_";
                    else
                    if(type.find("std::shared_ptr")==0
                    || type.find("std::weak_ptr")==0)
                        os << "const " << type << " " << method.argument[k].name << "_";
                    else
                        os << "const " << type << "& " << method.argument[k].name << "_";
                }

                else
                    os << method.argument[k].type << " " << method.argument[k].name;
            }

            os<<");\n";
        }

        else
        {
            os << "\t";
            if(method.isVirtual)
                os << "virtual ";

            os << method.returnType << " " << method.name << "(";

            if(method.argument.size()>0)
            {
                os << method.argument[0].type << " " << method.argument[0].name;

                for(size_t k=1; k<method.argument.size() ; k++)
                    os << ", " << method.argument[k].type << " " << method.argument[k].name;
            }

            os << ")";
            if(classInfo.isInterface)
                os << " = 0";
            os << ";" << endl;
        }
    }
}

void createRelation(const Relation& relation)
{
    auto& begin = findClassContainingPoint(classesInfo, relation.begin);
    auto& end   = findClassContainingPoint(classesInfo, relation.end);

    if(count(begin.dependantClasses.begin(), begin.dependantClasses.end(), end.name) == 0)
        begin.dependantClasses.push_back(end.name);

    string message = relation.message;
    string attribute;
    string attrType = end.name;

    switch(relation.type)
    {
        case COMPOSITION:

            if(message.find("shared") < message.length())
            {
                attrType = "shared_ptr<"+attrType+">";
                message = message.substr(0, message.find("shared")) + message.substr(message.find("shared")+strlen("shared"));
            }
            else
            if(getClassDataByName(end.name).isVirtual())
                attrType = "unique_ptr<"+attrType+">";

            if(message.find('*') < message.length())
            {
                attrType = "vector<"+attrType+">";
                message = message.substr(message.find('*')+1);
            }   

            if(message.find('[') < message.length())
            {
                auto s = split(message,"[");
                attribute = s[0]+":"+attrType+"["+s[1];
            }
            else
            {
                attribute = message+":"+attrType;
            }

            begin.addAttribute(parseAttribute(attribute));
            break;

        case AGGREGATION:
            if(message.find('*') < message.length())
            {
                if(message.find('&') < message.length())
                    throw runtime_error("Error! An attempt of creating a container of references was detected: "+message);

                attrType = "vector<weak_ptr<"+attrType+">>";
                message = message.substr(message.find('*')+1);
            }  
            else if(message.find('&') < message.length())
            {
                attrType = attrType+"&";
                message = message.substr(0, message.find('&')) + message.substr(message.find('&')+1);
            }
            else
                attrType = "weak_ptr<"+attrType+">";            

            if(message.find('[') < message.length())
            {
                auto s = split(message,"[");
                attribute = s[0]+":"+attrType+"["+s[1];
            }
            else
            {
                attribute = message+":"+attrType;
            }

            begin.addAttribute(parseAttribute(attribute));
            break;

        case INHERITANCE:
            begin.inheritances.push_back({
                end.isInterface,
                getClassDataByName(end.name)
            });
            break;
    }
}

void writeInHeader(ostream& os, const ClassInfo& classInfo)
{
    os<<"#ifndef "<<toUpper(classInfo.name)<<endl;
    os<<"#define "<<toUpper(classInfo.name)<<endl;

    includeStdLibs(os, classInfo);
    includeDependantClasses(os, classInfo);

    os<<"class "<<classInfo.name;

    for(size_t k=0; k<classInfo.inheritances.size(); k++)
    {
        os<<(k==0? ": " : ", ");

        if(classInfo.inheritances[k].isVirtual)
            os<<"virtual ";

        os<<"public "<<classInfo.inheritances[k].baseClass.name;
    }

    os<<endl<<"{"<<endl;

    auto encapsulation = writeAttributesInHeader(os, classInfo);
    os<<endl;

    writeMethodsInHeader(os, classInfo, encapsulation);
    os<<"};";
    os<<endl<<endl;

    os<<"#endif";
    os<<endl<<endl;
}

bool needsAlgorithmInclusion(const ClassInfo& classInfo)
{
    for(auto& attribute : classInfo.attributes)
        if(attribute.hasField("has")
        || attribute.hasField("remove"))
            return true;

    return false;
}

void writeAttributesInImpl(ostream& os, const vector<Attribute>& attributes, const string& className)
{
    for(const Attribute& attribute : attributes)
    {
        if(attribute.hasField("get"))
        {
            if(attribute.type.find("std::unique_ptr")==0)
            {
                auto noUnique = attribute.type.substr(
                    strlen("std::unique_ptr<"),
                    attribute.type.length()-strlen("std::unique_ptr<")-1
                );

                os << "const " << noUnique << "& "<< className <<"::get"<< capitalize(attribute.name) << "() ";
                
                if(!attribute.hasField("mutex"))
                    os << "const ";
                
                os << " {" << endl;
                writeLockGuard(os, attribute);
                os << "\treturn *" << attribute.name << ";" << endl
                << "}" << endl
                << endl;
            }
            else if(attribute.type.find("std::unique_ptr") < attribute.type.length()
            && attribute.type.find("std::unique_ptr") > 2)
            {
                auto container = attribute.type.substr(0,attribute.type.find("<std::unique_ptr"));

                auto sub = attribute.type.substr(attribute.type.find("std::unique_ptr"));

                auto noUnique = sub.substr(
                    strlen("std::unique_ptr<"),
                    sub.length()-strlen("std::unique_ptr<")-2
                );

                string type = container+"<"+noUnique+" * const>";

                os<<type<<" " << className <<"::get" << capitalize(attribute.name) << "()";
                
                if(!attribute.hasField("mutex"))
                    os << " const";

                os << " {" << endl;

                writeLockGuard(os, attribute);
                
                os
                << endl
                << "\t" << type << " v;" << endl
                << "\tv.reserve("<<attribute.name<<".size());" << endl
                << endl
                << "\tfor(auto& e : " << attribute.name << ") {" << endl
                << "\t\tv.push_back(e.get());" << endl
                << "\t}" << endl
                << endl
                << "\treturn v;" << endl
                << "}" << endl << endl;
            }
            else if(attribute.type.find("std::shared_ptr")==0
            || attribute.type.find("std::weak_ptr")==0)
            {   
                os << "const "<< attribute.type << " " << className << "::get"<<capitalize(attribute.name)<<"() ";
                
                if(!attribute.hasField("mutex"))
                    os << "const ";
                
                os << "{" << endl;
                writeLockGuard(os, attribute);
                os << "\treturn " << attribute.name << ";" << endl
                << "}" << endl
                << endl;
            }
            else if(attribute.type.find('&')<attribute.type.length())
            {   
                os << "const "<< attribute.type << " " << className << "::get"<<capitalize(attribute.name)<<"() ";

                if(!attribute.hasField("mutex"))
                    os << "const ";
                
                os << "{" << endl;
                writeLockGuard(os, attribute);
                os << "\treturn " << attribute.name << ";" << endl
                << "}" << endl
                << endl;
            }
            else if(attribute.isContainer())
            {
                os << "const "<< attribute.type << " " << className << "::get"<<capitalize(attribute.name)<<"() ";
                
                if(!attribute.hasField("mutex"))
                    os << "const ";

                os << "{" << endl;
                writeLockGuard(os, attribute);
                os << "\treturn " << attribute.name << ";" << endl
                << "}" << endl
                << endl;
            }
            else
            {
                os << "const "<< attribute.type << "& " << className << "::get"<<capitalize(attribute.name)<<"() ";
                
                if(!attribute.hasField("mutex"))
                    os << "const ";

                os << "{" << endl;
                writeLockGuard(os, attribute);
                os << "\treturn " << attribute.name << ";" << endl
                << "}" << endl
                << endl;
            }
        }

        if(attribute.hasField("set"))
        {
            if(attribute.type.find("std::unique_ptr")==0)
            {
                os << "void "<<className+"::set"<<capitalize(attribute.name)<<"("+attribute.type<<" "+attribute.name+"_) {" << endl;
                writeLockGuard(os, attribute);
                os << "\t" << attribute.name << " = std::move(" << attribute.name << "_);" << endl
                << "}" << endl
                << endl;
            }
            else if(attribute.type.find("std::shared_ptr")==0
            || attribute.type.find("std::weak_ptr")==0)
            {
                os << "void "<<className+"::set"<<capitalize(attribute.name)<<"(const "<<attribute.type<<" "+attribute.name+"_) {" << endl;
                writeLockGuard(os, attribute);
                os << "\t" << attribute.name << " = " << attribute.name << "_;" << endl
                << "}" << endl
                << endl;
            }
            else if(attribute.type.find('&') < attribute.type.length())
            {
                os << "void "<<className+"::set"<<capitalize(attribute.name)<<"(const "<<attribute.type<<" "+attribute.name+"_) {" << endl;
                writeLockGuard(os, attribute);
                os << "\t" << attribute.name << " = " << attribute.name << "_;" << endl
                << "}" << endl
                << endl;
            }
            else
            {
                os << "void "<<className+"::set"<<capitalize(attribute.name)<<"(const "<<attribute.type<<"& "+attribute.name+"_) {" << endl;
                writeLockGuard(os, attribute);
                os << "\t" << attribute.name << " = " << attribute.name << "_;" << endl
                << "}" << endl
                << endl;
            }
        }

        if(attribute.hasField("has")
        || attribute.hasField("remove"))
        {
            size_t begin = attribute.type.find('<');
            size_t end = attribute.type.find_last_of('>');

            string elemType = attribute.type.substr(begin+1, end-begin-1);

            os << "bool " << className << "::has"<<capitalize(attribute.getSingularName())<<"(const "<<elemType;

            if(elemType.find("std::shared_ptr")!=0
            && elemType.find("std::weak_ptr")!=0)
                os << "&";

            os << " "<<attribute.getSingularName()<<")";
            
            if(!attribute.hasField("mutex"))
                os << " const";

            os << " {" << endl;

            writeLockGuard(os, attribute);

            os << "\treturn std::find("<<attribute.name<<".begin(), "<<attribute.name<<".end(), "<<
            attribute.getSingularName() << ") != " << attribute.name << ".end();" << endl
            << "}" << endl << endl;

        }

        if(attribute.hasField("add"))
        {
            size_t begin = attribute.type.find('<');
            size_t end = attribute.type.find_last_of('>');

            string elemType = attribute.type.substr(begin+1, end-begin-1);

            if(attribute.hasField("has")
            || attribute.hasField("remove"))
                os << "bool ";
            else
                os << "void ";

            os << className<<"::add"<<capitalize(attribute.getSingularName())<<"(";

            if(elemType.find("std::unique_ptr")==0)
                os << elemType;
            else
            if(elemType.find("std::shared_ptr")==0
            || elemType.find("std::weak_ptr")==0)
                os << "const " << elemType;
            else
                os << "const " << elemType<< "&";

            os << " " << attribute.getSingularName() << ") {" << endl;

            writeLockGuard(os, attribute);

            if(attribute.hasField("has")
            || attribute.hasField("remove"))
                os << "\tauto it = std::find("<<attribute.name<<".begin(), "<<attribute.name<<".end(), "<<attribute.getSingularName() << ");" << endl
                << "\tif(it == " << attribute.name << ".end()) {" << endl
                << "\t\t" << attribute.name << ".push_back("<<attribute.getSingularName()<<");" << endl
                << "\t\treturn true;" << endl
                << "\t}" << endl
                << "\treturn false;"<<endl;
            else
                os << "\t" << attribute.name << ".push_back("<<attribute.getSingularName()<<");" << endl;

            os << "}" << endl << endl;
        }

        if(attribute.hasField("remove"))
        {
            size_t begin = attribute.type.find('<');
            size_t end = attribute.type.find_last_of('>');

            string elemType = attribute.type.substr(begin+1, end-begin-1);

            os << "bool "<<className<<"::remove"<<capitalize(attribute.getSingularName())<<"(const "<<elemType;

            if(elemType.find("std::shared_ptr")!=0
            && elemType.find("std::weak_ptr")!=0)
                os << "&";

            os << " "<<attribute.getSingularName()<<") {" << endl;

            writeLockGuard(os, attribute);

            os << "\tauto it = std::find("<<attribute.name<<".begin(), "<<attribute.name<<".end(), "<<attribute.getSingularName() << ");" << endl
            << "\tif(it != " << attribute.name << ".end()) {" << endl
            << "\t\t" << attribute.name << ".erase(it);" << endl
            << "\t\treturn true;" << endl
            << "\t}" << endl
            << "\treturn false;"<<endl
            << "}" << endl << endl;
        }

        if(attribute.hasField("clear"))
        {   
            os << "void "<<className<<"::clear"<<capitalize(attribute.name)<<"() {" << endl;
            writeLockGuard(os, attribute);
            os << "\t"<<attribute.name<<".clear();" << endl
            << "}" << endl << endl;
        }
    }
}

void writeMethodsInImpl(ostream& os, const ClassInfo& classInfo)
{
    for(const Method& method : classInfo.methods)
    {
        if(method.name=="operator<=>")
        {
            os << endl;
            for(string ops : {"<",">","=="})
                os << "const bool "<< classInfo.name <<"::operator" << ops <<
                "(" << method.argument[0].type << " " << method.argument[0].name << ") const {" << endl <<
                "\treturn false;" << endl <<
                "}" << endl <<
                endl;

            os << "const bool "<< classInfo.name <<"::operator!=" <<
            "(" << method.argument[0].type << " " << method.argument[0].name << ") const {" << endl <<
            "\treturn !( *this == " << method.argument[0].name << " );" << endl <<
            "}" << endl <<
            endl;
        }
        else
        if(method.isConstructor())
        {
            vector<string> baseClassesArguments;

            os << classInfo.name << "::" << classInfo.name << "(";

            if(method.argument.size()>0)
            {
                if(method.argument[0].type.empty())
                {
                    const Attribute* argument = classInfo.getAttributeByName(method.argument[0].name);

                    if(argument == nullptr)
                        throw runtime_error("Error! "+classInfo.name+"constructor argument "+
                        method.argument[0].name+" type could not be deduced!");

                    string type = argument->type;

                    if(type.find("std::unique_ptr")==0)
                        os << type << method.argument[0].name << "_";
                    else
                    if(type.find("std::shared_ptr")==0
                    || type.find("std::weak_ptr")==0)
                        os << "const " << type << " " << method.argument[0].name << "_";
                    else
                        os << "const " << type << "& " << method.argument[0].name << "_";

                    if(!hasAttributeWithName(classInfo.attributes, argument->type))
                        baseClassesArguments.push_back(argument->name);
                }
                else
                    os << method.argument[0].type <<
                    " " << method.argument[0].name;

                for(size_t k=1 ; k<method.argument.size() ; k++)
                {
                    os << ", ";

                    if(method.argument[k].type.empty())
                    {
                        const Attribute* argument = classInfo.getAttributeByName(method.argument[k].name);

                        if(argument == nullptr)
                            throw runtime_error("Error! "+classInfo.name+"constructor argument "+
                            method.argument[k].name+" type could not be deduced!");

                        string type = argument->type;

                        if(type.find("std::unique_ptr")==0)
                            os << type << method.argument[k].name << "_";
                        else
                        if(type.find("std::shared_ptr")==0
                        || type.find("std::weak_ptr")==0)
                            os << "const " << type << " " << method.argument[k].name << "_";
                        else
                            os << "const " << type << "& " << method.argument[k].name << "_";

                        if(!hasAttributeWithName(classInfo.attributes, argument->name))
                            baseClassesArguments.push_back(argument->name);
                    }
                    else
                        os << method.argument[k].type <<
                        " " << method.argument[k].name;
                }
            }

            os << ")" << endl;

            bool dots = false;

            if(classInfo.inheritances.size()>0)
            {
                for(auto& inheritance : classInfo.inheritances)
                {
                    const Method* constructor = inheritance.baseClass.getMatchingConstructor(
                        baseClassesArguments
                    );

                    if(constructor != nullptr)
                    {
                        if(!dots)
                        {
                            os << ":";
                            dots = true; 
                        }
                        else
                            os << ",";
                        
                        os << inheritance.baseClass.name << "(";

                        if( 
                            classInfo.getAttributeByName(
                                constructor->argument[0].name
                            )
                            ->type.find("std::unique_ptr")==0
                        )
                            os << "std::move(" << constructor->argument[0].name<<"_)";
                        else
                            os << "" << constructor->argument[0].name<<"_";

                        for(size_t k=1;k<constructor->argument.size();k++)
                            if(
                                classInfo.getAttributeByName(
                                    constructor->argument[k].name
                                )
                                ->type.find("std::unique_ptr")==0
                            )
                                os << ", std::move(" << constructor->argument[k].name<<"_)";
                            else
                                os << ", " << constructor->argument[k].name<<"_";

                        os << ")" << endl;
                    }
                    else
                    if(!inheritance.baseClass.hasDefaultConstructor())
                    {
                        if(!dots)
                        {
                            os << ":";
                            dots = true; 
                        }
                        else
                            os << ",";
                        
                        os << inheritance.baseClass.name <<  endl;
                    }
                }
            }

            for(size_t k=0 ; k<method.argument.size(); k++)
                if(method.argument[k].type.empty()
                && hasAttributeWithName(classInfo.attributes,method.argument[k].name))
                {
                    if(!dots)
                    {
                        os << ":";
                        dots = true; 
                    }
                    else
                        os << ",";
                    
                    string name = method.argument[k].name;

                    if(classInfo.getAttributeByName(method.argument[k].name)->type.find("std::unique_ptr")==0)
                        os << name << "(std::move(" << name << "_))" << endl;
                    else
                        os << name << "(" << name << "_)" << endl;
                }

            for(auto& attribute : classInfo.attributes)
                try
                {
                    if( !hasAttributeWithName(method.argument, attribute.name)
                    &&  !getClassDataByName(attribute.type).hasDefaultConstructor())
                    {
                        if(!dots)
                        {
                            os << ":";
                            dots = true; 
                        }
                        else
                            os << ",";

                        os << attribute.name << endl;
                    }
                }
                catch(const std::exception& e) {}

            os << "{}" << endl << endl;
        }
        else
        {
            if(!method.returnType.empty())
                os << method.returnType << " ";

            os << classInfo.name << "::" << method.name << "(";

            if(method.argument.size()>0)
            {
                os << method.argument[0].type << " " << method.argument[0].name;

                for(size_t k=1; k<method.argument.size() ; k++)
                    os << ", " << method.argument[k].type << " " << method.argument[k].name;
            }

            os << ")" << endl;

            if (method.returnType == "" || method.returnType == "void")
                os
                << "{" << endl
                << endl
                << "}" << endl
                << endl;
            else
                os
                << "{" << endl
                << "\t" << method.returnType << " autoGen;" << endl
                << endl
                << "\treturn autoGen;" << endl
                << "}" << endl
                << endl;
        }
    }
}

void writeInImpl(ostream& os, const ClassInfo& classInfo)
{
    os<<"#include \""+classInfo.name+".h\""<<endl;
    if(needsAlgorithmInclusion(classInfo))
        os<<"#include <algorithm>"<<endl;
    
    os<<endl;

    writeAttributesInImpl(os, classInfo.attributes, classInfo.name);
    os<<endl;
    writeMethodsInImpl(os, classInfo);
}

void writeJavaAttributes(ostream& os, const vector<Attribute>& attributes, const string& className)
{    
    for(const Attribute& attribute: attributes)
    {
        string type;
        if(attribute.type=="std::string")
            type="String";
        else if(attribute.type=="bool")
            type="boolean";
        else
            type=attribute.type;
        if(attribute.encapsulation!=PUBLIC)
            os<<"\t"<<attribute.encapsulation<<" "<<type<<" "<<attribute.name<<";\n";
        else
            os<<"\t"<<type<<" "<<attribute.name<<";\n";
    }
}

void writeJavaMethods(ostream& os, const vector<Attribute>& attributes, const ClassInfo& classInfo)
{
    for(const Attribute& attribute: attributes)
    {
        
        //getter and setter
        if(attribute.encapsulation==PRIVATE)
        {
            string type;
            if(attribute.type=="std::string")
                type="String";
            else if(attribute.type=="bool")
                type="boolean";
            else
                type=attribute.type;
            os<<"\tpublic "<<type
                <<" get"<<capitalize(attribute.name)
                <<"() {\n\t\treturn "<<attribute.name
                <<";\n\t}\n\n";
            os<<"\tpublic void"<<" set"<<capitalize(attribute.name)
                <<"("<<type<<" new"
                <<capitalize(attribute.name)
                <<") {\n\t\tthis."<<attribute.name
                <<" = new"<<capitalize(attribute.name)
                <<";\n\t}\n\n";
        }
    }

    for(const Method& method: classInfo.methods)
    {
        string type;
        if(method.returnType=="std::string")
            type="String";
        else if(method.returnType=="bool")
            type="boolean";
        else
            type=method.returnType;
        
        if(method.name==classInfo.name)
        {
            vector<string> baseClassesArguments;
            os<<"\t"<<classInfo.name<<" (";
            
            if(method.argument.size()>0)
            {
                if(method.argument[0].type.empty())
                {
                    const Attribute* argument=classInfo.getAttributeByName(method.argument[0].name);
                    string type=argument->type;
                    
                    os<<method.argument[0].name<<"_";

                    if(!hasAttributeWithName(classInfo.attributes, argument->type))
                        baseClassesArguments.push_back(argument->name);
                }
                else
                {
                    string type;
                    if(method.argument[0].type=="std::string")
                        type="String";
                    else if(method.argument[0].type=="bool")
                        type="boolean";
                    else
                        type=method.argument[0].type;
                    os<<type<<" "<<method.argument[0].name;
                }
                
                for(size_t k=1; k<method.argument.size(); k++)
                {
                    os<<", ";
                    if(method.argument[k].type.empty())
                    {
                        const Attribute* argument=classInfo.getAttributeByName(method.argument[k].name);
                        string type=argument->type;
                        os<<method.argument[k].name<<"_";

                        if(!hasAttributeWithName(classInfo.attributes, argument->type))
                            baseClassesArguments.push_back(argument->name);
                    }
                    else
                    {
                        string type;
                        if(method.argument[0].type=="std::string")
                            type="String";
                        else if(method.argument[0].type=="bool")
                            type="boolean";
                        else
                            type=method.argument[0].type;
                        os<<type<<" "<<method.argument[0].name;
                    }
                }

                os<<") {\n\t";
                for(size_t k=0; k<method.argument.size(); k++)
                    os<<classInfo.attributes[k].name<<" = "<<method.argument[k].name<<";\n";
                os<<"}\n\n";
            }

        }
        else
        {
            string type;
            os<<method.encapsulation<<" ";
            if(!method.returnType.empty())
            {
                if(method.returnType=="std::string")
                    type="String";
                else if(method.returnType=="bool")
                    type="boolean";
                else
                    type=method.returnType;
                os<<type<<" ";
            }
            else
                os<<"void ";
            
            os<<method.name<<"(";
            if(method.argument.size()>0)
            {
                string t;
                if(method.argument[0].type=="std::string")
                    t="String";
                else if(method.argument[0].type=="bool")
                    t="boolean";
                else
                    t=method.argument[0].type;
                os<<t<<" "<<method.argument[0].name;
                for(size_t k=1; k<method.argument.size(); k++)
                {
                    string t;
                    if(method.argument[k].type=="std::string")
                        t="String";
                    else if(method.argument[k].type=="bool")
                        t="boolean";
                    else
                        t=method.argument[k].type;
                    os<<", "<<method.argument[k].type<<" "<<method.argument[k].name;
                }
            }

            os<<") {\n\t";

            if(method.returnType==""||method.returnType=="void")
                os<<"}\n\n";
            else
                os<<"\t"<<type<<" autoGen;\n"<<"\t\treturn autoGen;\n\t}\n\n";
        }
    }
}

void writeInJavaClass(ostream& os, const ClassInfo& classInfo)
{
    os<<"class "<<classInfo.name;
    if(classInfo.inheritances.size()!=0)
    {
        if(classInfo.inheritances[0].baseClass.isInterface)
            os<<" implements ";
        else
            os<<" extends ";
        os<<classInfo.inheritances[0].baseClass.name;
    }
    os<<" {\n";
    writeJavaAttributes(os, classInfo.attributes, classInfo.name);
    os<<endl;
    writeJavaMethods(os, classInfo.attributes, classInfo);
    os<<"}";
}

bool hasClassContainingPoint(vector<ClassInfo>& classesInfo, const Point& point)
{
    for(auto& classInfo : classesInfo)
        if(classInfo.coords.contains(point))
            return true;

    return false;
}

void writeInJavaInterface(ostream& os, const ClassInfo& classInfo)
{
    os<<"interface "<<classInfo.name<<" {\n";
    
    for(const Method& method: classInfo.methods)
    {
        os<<method.returnType<<" "<<method.name<<"(";
        if(method.argument.size()>0)
        {
            os<<method.argument[0].type<<" "<<method.argument[0].name;
            for(size_t k=1; k<method.argument.size(); k++)
                os<<", "<<method.argument[k].type<<" "<<method.argument[k].name;
        }
        os<<");\n";
    }
    
}

void writeCpp(const ClassInfo& classInfo, const string& fileDestination)
{
    ofstream ofsh(fileDestination+classInfo.name+".h");
    writeInHeader(ofsh, classInfo);

    if(!classInfo.isInterface)
    {   
        ofstream ofsi(fileDestination+classInfo.name+".cpp");
        writeInImpl(ofsi, classInfo);
    }
}

void writeJava(const ClassInfo& classInfo, const string& fileDestination)
{
    ofstream ofjc(fileDestination+classInfo.name+".java");
    if(classInfo.isInterface)
    {
        ofstream ofji(fileDestination+classInfo.name+".java");
        writeInJavaInterface(ofji, classInfo);
    }
    else
        writeInJavaClass(ofjc, classInfo);
}

int main()
{    
    info = consoleUI();

    XMLDocument doc;
    doc.LoadFile(info.umlPath.c_str());
    auto elementList = doc.FirstChildElement("diagram");

    globalPackage.coords = Rectangle (
        Point (
            numeric_limits<int>::min() / 4,
            numeric_limits<int>::min() / 4
        ),
        numeric_limits<int>::max() / 2,
        numeric_limits<int>::max() / 2
    );

    globalPackage.name = info.srcCodePath;

    vector<Package> packages;

    for(
        XMLElement* elem;
        (elem=elementList->FirstChildElement("element"))!=nullptr;
        elementList->DeleteChild(elem)
    )
        if(string(elem->FirstChildElement("id")->GetText())=="UMLPackage")
            packages.push_back(parsePackage(elem));

    std::sort(packages.begin(), packages.end(), 
        [] (Package& a, Package& b) {
            return a.coords.width*a.coords.height > b.coords.width * b.coords.height;
        });
    
    for(auto& pack: packages)
        globalPackage.insert(pack);
    
    globalPackage.makeDirectories("");

    doc.LoadFile(info.umlPath.c_str());
    elementList=doc.FirstChildElement("diagram");

    for(
        XMLElement* elem;
        (elem=elementList->FirstChildElement("element"))!=nullptr;
        elementList->DeleteChild(elem)
    )
        if(string(elem->FirstChildElement("id")->GetText())=="UMLClass")
            classesInfo.push_back(parseClassInfo(elem));

    doc.LoadFile(info.umlPath.c_str());
    elementList=doc.FirstChildElement("diagram");

    for(
        XMLElement* elem;
        (elem=elementList->FirstChildElement("element"))!=nullptr;
        elementList->DeleteChild(elem)
    )
        if(string(elem->FirstChildElement("id")->GetText())=="Relation")
            createRelation(parseRelation(elem));
    
    std::cout<<"\n\t1. C++\n\t2. Java\n\nLanguage: ";
    int l;
    std::cin>>l;
    
    if(l==1)
    {
        for (auto& classInfo : classesInfo)
        writeCpp(classInfo, globalPackage.getDestination(classInfo));
    }
    else if(l==2)
    {
        for (auto& classInfo : classesInfo)
        writeJava(classInfo, globalPackage.getDestination(classInfo));
    }

    std::cout << "The files have been generated successfully" << std::endl;
    return 0;
}