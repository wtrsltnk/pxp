#include <system.xml/system.xml.h>
#include <system.io/system.io.file.h>
#include <iostream>

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

void printNode(System::Xml::XmlNode *node, int depth, std::ostream &s)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->LocalName()[0] != '#')
    {
        for (int i = 0; i < depth; i++)
        {
            s << "  ";
        }
        s << "<" << node->LocalName() << ">";
        if (descendantsDepths(node) > 1)
        {
            s << "\n";
        }

        for (auto child : node->ChildNodes())
        {
            printNode(child, depth + 1, s);
        }
        if (descendantsDepths(node) != 0)
        {
            for (int i = 0; i < depth; i++)
            {
                s << "  ";
            }
        }
        s << "</" << node->LocalName() << ">";
    }
    else if (node->LocalName() == "#cdata")
    {
        s << "<![CDATA[" << node->InnerText() << "]]>";
    }
    else
    {
        s << node->InnerText();
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cerr << "No parameters given\n";
        return 0;
    }

    std::string input = argv[1];
    System::Xml::XmlDocument doc;

    std::cout << input << "\n\n";
    if (System::IO::File::Exists(input))
    {
        std::cout << "loading from file";
        doc.Load(input);
    }
    else
    {
        std::cout << "loading from xml data";
        doc.LoadXml(input);
    }

    std::cout << "\n";
    printNode(doc.DocumentElement(), 0, std::cout);
    std::cout << "\n";

    return 0;
}
