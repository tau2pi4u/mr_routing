#pragma once

#include "utils.hpp"
#include <String>

struct Node
{
    char * id;
    char * trainId;
};

struct MainNode
{
    char * id;
    Node * iNodes;
    int iNodeCount;
    Node * oNodes;
    int oNodeCount;
};

void DestroyMainNode(MainNode & mainNode);
void PrintNodeString(const MainNode & mainNode);
void WriteNodeString(char * & nodeStr,  String & toWrite);
void SetId(char * & id, String data);
void CreateNodes(Node * & nodes, int & prevCount, String data);
void GetMainNodeFromConfig(MainNode & mainNode, String config);



