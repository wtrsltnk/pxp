#include "clara.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <system.io/system.io.file.h>
#include <system.xml/system.xml.h>

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

int descendantsDepths(System::Xml::XmlNode *node)
{
    if (node == nullptr)
    {
        return 1;
    }

    int depth = 1;

    for (auto child : node->ChildNodes())
    {
        auto d = descendantsDepths(child) + 1;
        if (d > depth)
        {
            d = depth;
        }
    }

    return depth;
}

bool isOneliner(System::Xml::XmlNode *node)
{
    if (descendantsDepths(node) > 1) return false;

    for (auto child : node->ChildNodes())
    {
        if (child->NodeType() != System::Xml::XmlNodeType::Text)
        {
            return false;
        }
    }

    return true;
}

void printNode(System::Xml::XmlNode *node, int depth, std::ostream &s)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->LocalName()[0] != '#')
    {
        bool oneliner = isOneliner(node);

        for (int i = 0; i < depth; i++)
        {
            s << "  ";
        }

        s << "<" << node->LocalName() << ">";

        if (!oneliner)
        {
            s << "\n";
        }

        for (auto child : node->ChildNodes())
        {
            printNode(child, depth + 1, s);
        }

        if (!oneliner)
        {
            for (int i = 0; i < depth; i++)
            {
                s << "  ";
            }
        }
        s << "</" << node->LocalName() << ">\n";
    }
    else if (node->LocalName() == "#cdata")
    {
        s << "<![CDATA[" << node->InnerText() << "]]>";
    }
    else
    {
        s << ltrim_copy(rtrim_copy(node->InnerText()));
    }
}

int main(int argc, char *argv[])
{
    std::string input;
    bool showHelp = false;

    auto cli = clara::Arg(input, "input")("input file name") |
               clara::Help(showHelp);

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;

        cli.writeToStream(std::cout);

        return 1;
    }

    if (showHelp)
    {
        cli.writeToStream(std::cout);

        return 0;
    }

    System::Xml::XmlDocument doc;

    if (System::IO::File::Exists(input))
    {
        doc.Load(input);
    }
    else
    {
        doc.LoadXml(input);
    }

    std::cout << "\n";
    printNode(doc.DocumentElement(), 0, std::cout);
    std::cout << "\n";

    return 0;
}
