#include "node.hpp"

void DestroyNode(Node & node)
{
    free(node.id);
    free(node.trainId);
}

void DestroyMainNode(MainNode & mainNode)
{
    for(unsigned int i = 0; i < mainNode.iNodeCount; i++)
    {
        DestroyNode(mainNode.iNodes[i]);
    }
    for(unsigned int i = 0; i < mainNode.oNodeCount; i++)
    {
        DestroyNode(mainNode.oNodes[i]);
    }
    free(mainNode.iNodes);
    free(mainNode.oNodes);
    free(mainNode.id);
}

void PrintNodeString(const MainNode & mainNode)
{
    Serial.printf("Id: %s\n", mainNode.id);
    Serial.printf("%d Lines In.\n", mainNode.iNodeCount);
    for(unsigned int i = 0; i < mainNode.iNodeCount; i++)
    {
        Serial.printf("\tDestination: %s, ", mainNode.iNodes[i].id);
        Serial.printf("Train: %s\n", mainNode.iNodes[i].trainId);
    }
    Serial.printf("%d Lines Out.\n", mainNode.oNodeCount);
    for(unsigned int i = 0; i < mainNode.oNodeCount; i++)
    {
        Serial.printf("\tDestination: %s, Train: %s\n", mainNode.oNodes[i].id, mainNode.oNodes[i].trainId);
    }
}

void WriteNodeString(char * & nodeStr,  String & toWrite)
{
   // Serial.printf("\t\t-->Writing %s to node\n", toWrite.c_str());
    toWrite.trim();
    RemoveCharacterFromString(toWrite, '\"');
    //Serial.printf("NodeStr: %u\n", nodeStr);
    if(nodeStr != NULL && strcmp(nodeStr, toWrite.c_str()) == 0) { return; }
    //Serial.printf("String different\n");
    if(nodeStr != NULL && strlen(nodeStr) != toWrite.length());
    {
        //Serial.printf("\t\t-->Freeing and reallocating memory\n");
        free(nodeStr);
        nodeStr = (char*) malloc((toWrite.length() + 1) * sizeof(char));
    }
    if(nodeStr == NULL)
    {
        //Serial.printf("\t\t-->Allocating memory\n");
        nodeStr = (char*) malloc((toWrite.length() + 1) * sizeof(char));
    }
    if(nodeStr == NULL)
    {
        Serial.printf("Failed to allocate memory for trainId %s\n", toWrite.c_str());
        return;
    }
    sprintf(nodeStr, "%s", toWrite.c_str());
    //Serial.printf("\t\t-->Success\n");
}

void SetId(char * & id, String data)
{
    //Serial.printf("Setting Id: %s\n", data.c_str());
    RemoveCharacterFromString(data, '\"');
    //Serial.printf("Comparing strings\n");
    if(id != NULL && strcmp(id, data.c_str()) == 0) { return; }
    //Serial.printf("String different\n");
    if(id == NULL)
    {
        //Serial.printf("Allocating memory for id\n");
        id = (char*)malloc(sizeof(char) * (data.length() + 1));
    }
    else if(strlen(id) != data.length())
    {
        //Serial.printf("Freeing and reallocating memory for id\n");
        free(id);
        id = (char*)malloc(sizeof(char) * (data.length() + 1));
    }
    if(id == NULL)
    {
        Serial.printf("Failed to allocate memory for MainNodeId %s\n", data.c_str());
        return;
    }
    //Serial.printf("Setting id\n");
    sprintf(id, "%s", data.c_str());
}

void CreateNodes(Node * & nodes, int & prevCount, String data)
{
    //Serial.printf("Creating node from config: %s\n", data.c_str());
    RemoveCharacterFromString(data, '{');
    RemoveCharacterFromString(data, '}');
    //Serial.printf("CreateNodes:\n\t\t-->input: %s\n", data.c_str());
    int nodeCount = 0;
    for(int i = 0; i < data.length(); i++)
    {
        if(data[i] == ':') { nodeCount++; }
    }
    //Serial.printf("\t\t-->Found %d nodes\n", nodeCount);
    if(nodeCount != prevCount)
    {
        //Serial.printf("\t\t-->Deleting old nodes\n");
        for(unsigned int i = 0; i < prevCount; i++)
        {
            DestroyNode(nodes[i]);
        }
        free(nodes);
        nodes = NULL;
        prevCount = nodeCount;
    }
    if(nodes == NULL)
    {
        //Serial.printf("\t\t-->Allocating new nodes\n");
        nodes = (Node*)calloc(nodeCount, sizeof(Node));
    }
    int nodeIdx = 0;
    int sIdx = 0;
    for(int i = 0; i < data.length(); i++)
    {
        if(data[i] == ',') 
        { 
            String substr = data.substring(sIdx, i);
            WriteNodeString(nodes[nodeIdx].trainId, substr);
            nodeIdx++; 
            sIdx = i+1;
        }
        else if(data[i] == ':') 
        { 
            String substr = data.substring(sIdx, i);
            WriteNodeString(nodes[nodeIdx].id, substr);
            sIdx = i+1;
        }
    }
    // Last write 
    String substr = data.substring(sIdx, data.length()-1);
    WriteNodeString(nodes[nodeIdx].trainId, substr);
}

void GetMainNodeFromConfig(MainNode & mainNode, String config)
{
    int sIdx = 1;
    int eIdx = -1;
    bool inDict = false;
    int section = 0;
    int i = 0;
    while(config[i] != '{') { i++; }
    for(i = i + 1; i < config.length(); i++)
    {
        if(config[i] == '{') { inDict = true; }
        else if(inDict && config[i] == '}') { inDict = false; }
        else if(!inDict && config[i] == ':') { sIdx = i + 1; }
        else if((!inDict && config[i] == ',') || (!inDict && config[i] == '}'))
        {
            eIdx = i - 1;
            String substr = config.substring(sIdx, eIdx);
            substr.trim();
            switch(section)
            {
                case 0:
                {
                    SetId(mainNode.id, substr);
                    //Serial.printf("Id: %s\n", mainNode.id);
                    break;
                }
                case 1:
                {
                    CreateNodes(mainNode.iNodes, mainNode.iNodeCount, substr);
                    break;
                }
                case 2:
                {
                    CreateNodes(mainNode.oNodes, mainNode.oNodeCount, substr);
                    break;
                }
            }
            section++;
        }
    }
    PrintNodeString(mainNode);
}