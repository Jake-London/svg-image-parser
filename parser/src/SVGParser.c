/* 
    Jake London
    jlondon@uoguelph.ca
    0961071
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SVGParser.h"
#include "LinkedListAPI.h"

/* 
    TODO:
        - fix title and desc for longer titles
        - fix title and desc to expect empty
        - refactor numAttr to use a helper function
        - check if namespace is empty
        - title and description are not written to file
        - change type > 4 to between 1 and 3
 */

Path* allocatePaths(xmlAttr * temp, xmlNode * svgchild);
Circle* allocateCircles(xmlAttr* temp, xmlNode* svgchild);
Rectangle* allocateRectangles(xmlAttr* temp, xmlNode* svgchild);
Group* allocateGroups(xmlAttr* temp, xmlNode* svgchild);
void dummyDelete(void* data);
void recurseGroup(List* masterList, List* groups);
void addRectToList(List* masterList, List* rectangles);
void addPathToList(List* masterList, List* paths);
void addCircleToList(List* masterList, List* circles);
xmlDoc* createXMLFromSVG(SVGimage* img);
void addRectToDoc(List* rectangles, xmlNodePtr node, xmlNodePtr root);
void addCircleToDoc(List* circles, xmlNodePtr node, xmlNodePtr root);
void addPathToDoc(List* paths, xmlNodePtr node, xmlNodePtr root);
void addGroupToDoc(List* groups, xmlNodePtr node, xmlNodePtr root);
int checkCircles(List* circles);
int checkRects(List* rectangles);
int checkPaths(List* paths);
int checkAttributes(List* attributes);
int checkGroups(List* groups);
void freeAttribute(Attribute* attribute);
void replaceAttribute(Attribute* newAttribute, List* attributeList);
char* testFunction();
char* createValidSVGandJSON(char* filename, char* schemaFile);
char* createValidSVGandComponentJSON(char* filename, char* schemaFile);
char* updateTitleOfImage(char* filename, char* newTitle, char* newDescription, char* schemaFile);
int updateAttributeAndSaveFile(char* filename, char* schemaFile, char* newName, char* newDesc, int elemType, int elemIndex);
int addRectangleToSVGandSave(char* filename, char* schemaFile, float x, float y, float width, float height, char* units);
int addCircleToSVGandSave(char* filename, char* schemaFile, float cx, float cy, float r, char* units);


SVGimage* createSVGimage(char * fileName) {
    SVGimage* image = NULL;
    xmlDoc* document = NULL;
    xmlNode* node = NULL;
    xmlNode* svgchild = NULL;
    xmlAttr* temp = NULL;

    List* rectanglesTemp;
    List* circlesTemp;
    List* pathsTemp;
    List* groupsTemp;
    List* otherAttributesTemp;

    LIBXML_TEST_VERSION

    document = xmlReadFile(fileName, NULL, 0);

    if (document == NULL) {
        xmlFreeDoc(document);
        xmlCleanupParser();
        return NULL;
    }

    node = document->children;

    while (strcmp((char *)(node->name), "svg") != 0) {
        node = node->next;
    }

    if (node->next != NULL) {
        if (strcmp((char*)(node->next->name), "svg") == 0) {
            node = node->next;
        }
    }

    image = calloc(1, sizeof(SVGimage));

    strcpy(image->namespace, (char*)(node->ns->href));
    strcpy(image->title, "");
    strcpy(image->description, "");

    rectanglesTemp = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    circlesTemp = initializeList(&circleToString, &deleteCircle, &compareCircles);
    pathsTemp = initializeList(&pathToString, &deletePath, &comparePaths);
    groupsTemp = initializeList(&groupToString, &deleteGroup, &compareGroups);
    otherAttributesTemp = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    temp = node->properties;

    while (temp != NULL) {
        Attribute* tempAttr;
        int tempLen = 0;

        tempAttr = calloc(1, sizeof(Attribute));

        tempLen = strlen((char*)(temp->name)) + 1;
        tempAttr->name = calloc(tempLen, sizeof(char));

        tempLen = strlen((char*)(temp->children->content)) + 1;
        tempAttr->value = calloc(tempLen, sizeof(char));

        strcpy(tempAttr->name, (char*)(temp->name));
        strcpy(tempAttr->value, (char*)(temp->children->content));
        insertBack(otherAttributesTemp, tempAttr);

        temp = temp->next;
    }

    svgchild = node->children;

    temp = NULL;

    while (svgchild != NULL) {
        if (strcmp((char*)(svgchild->name), "path") == 0) {
            Path* newPath;
            newPath = allocatePaths(temp, svgchild);
            insertBack(pathsTemp, newPath);
        }

        else if (strcmp((char*)(svgchild->name), "circle") == 0) {
            Circle* newCircle;
            newCircle = allocateCircles(temp, svgchild);
            insertBack(circlesTemp, newCircle);
        }

        else if (strcmp((char*)(svgchild->name), "rect") == 0) {
            Rectangle* newRect;
            newRect = allocateRectangles(temp, svgchild);
            insertBack(rectanglesTemp, newRect);
        }

        else if (strcmp((char*)(svgchild->name), "g") == 0) {
            Group* newGroup;
            newGroup = allocateGroups(temp, svgchild);

            insertBack(groupsTemp, newGroup);
        }

        else if (strcmp((char*)(svgchild->name), "desc") == 0) {
            if (strlen((char*)(svgchild->children->content)) > 254) {
                strncpy(image->description, (char*)(svgchild->children->content), 254);
                image->description[255] = '\0';
            }
            else {
                strcpy(image->description, (char*)(svgchild->children->content));
            }
            
        }
        else if (strcmp((char*)(svgchild->name), "title") == 0) {
            if (strlen((char*)(svgchild->children->content)) > 254) {
                strncpy(image->title, (char*)(svgchild->children->content), 254);
                image->title[255] = '\0';
            }
            else {
                strcpy(image->title, (char*)(svgchild->children->content));
            }
        }

        svgchild = svgchild->next;
    }

    image->rectangles = rectanglesTemp;
    image->circles = circlesTemp;
    image->paths = pathsTemp;
    image->groups = groupsTemp;
    image->otherAttributes = otherAttributesTemp;

    xmlFreeDoc(document);
    xmlCleanupParser();    

    return image;
}

char* SVGimageToString(SVGimage* img) {
    return NULL;
}

void deleteSVGimage(SVGimage* img) {
    if (img != NULL) {
        freeList(img->rectangles);
        freeList(img->circles);
        freeList(img->paths);
        freeList(img->groups);
        freeList(img->otherAttributes);
        free(img);
    }
}

/* Helper functions */

void deleteAttribute(void* data) {
    Attribute* tempAttr;

    tempAttr = (Attribute*)(data);
    free(tempAttr->name);
    free(tempAttr->value);

    free(tempAttr);
}

char* attributeToString(void* data) {
    if (data != NULL) {
        char* s1;
        int len;
        Attribute* temp;

        temp = (Attribute*)(data);

        len = strlen(temp->name) + (strlen(temp->value) + 25);

        s1 = calloc(len, sizeof(char));

        sprintf(s1, "Name: %s, Value: %s.", temp->name, temp->value);

        return s1;
    }

    return NULL;
}

int compareAttributes(const void *first, const void *second) {
    return 0;
}

void deleteGroup(void* data) {
    Group* tempGroup;

    tempGroup = (Group*)(data);
    freeList(tempGroup->groups);
    freeList(tempGroup->rectangles);
    freeList(tempGroup->circles);
    freeList(tempGroup->paths);
    
    freeList(tempGroup->otherAttributes);

    free(tempGroup);

}

char* groupToString(void* data) {
    return NULL;
}

int compareGroups(const void *first, const void *second) {
    return 0;
}

void deleteRectangle(void* data) {
    Rectangle* tempRect;
    List* tempList;

    tempRect = (Rectangle*)(data);

    tempList = tempRect->otherAttributes;

    freeList(tempList);
    free(tempRect);
}

char* rectangleToString(void* data) {
    if (data != NULL) {
        char* s1;
        int len;
        Rectangle* temp;

        temp = (Rectangle*)(data);

        len = 60;

        s1 = calloc(len, sizeof(char));

        sprintf(s1, "x: %.2f, y: %.2f, width: %.2f, height: %.2f, units: %s.", temp->x, temp->y, temp->width, temp->height, temp->units);

        return s1;
    }

    return NULL;
}

int compareRectangles(const void *first, const void *second) {
    return 0;
}

void deleteCircle(void* data) {
    Circle* tempCircle;
    List* tempList;

    tempCircle = (Circle*)(data);

    tempList = tempCircle->otherAttributes;

    freeList(tempList);    
    free(tempCircle);
}

char* circleToString(void* data) {
    if (data != NULL) {
        char* s1;
        int len;
        Circle* temp;

        temp = (Circle*)(data);

        len = 60;

        s1 = calloc(len, sizeof(char));

        sprintf(s1, "cx: %.2f, cy: %.2f, r: %.2f, units: %s", temp->cx, temp->cy, temp->r, temp->units);

        return s1;
    }
    return NULL;
}

int compareCircles(const void *first, const void *second) {
    return 0;
}

void deletePath(void* data) {
    Path* tempPath;
    List* tempList;

    tempPath = (Path*)(data);
    tempList = tempPath->otherAttributes;

    freeList(tempList);
    free(tempPath->data);
    free(tempPath);
}

char* pathToString(void* data) {
    if (data != NULL) {
        char* s1;
        int len;
        Path* temp;

        temp = (Path*)(data);

        len = strlen(temp->data) + 10;

        s1 = calloc(len, sizeof(char));

        sprintf(s1, "d: %s", temp->data);

        return s1;
    }
    return NULL;
}

int comparePaths(const void *first, const void *second) {
    return 0;
}

List* getRects(SVGimage* img) {

    if (img != NULL) {
        List* masterList;
        List* groups;
        masterList = initializeList(&rectangleToString, &dummyDelete, &compareRectangles);
        groups = getGroups(img);
        Group* tempGroup;

        addRectToList(masterList, img->rectangles);

        ListIterator iter;
        iter = createIterator(groups);

        while ((tempGroup = nextElement(&iter))) {
            addRectToList(masterList, tempGroup->rectangles);
        }

        freeList(groups);
        return masterList;
    }
    return NULL;
}

void addRectToList(List* masterList, List* rectangles) {
    ListIterator iter;
    iter = createIterator(rectangles);
    Rectangle* tempRect;

    while ((tempRect = nextElement(&iter))) {
        insertBack(masterList, tempRect);
    }
}

List* getCircles(SVGimage* img) {
    if (img != NULL) {
        List* masterList;
        List* groups;
        masterList = initializeList(&circleToString, &dummyDelete, &compareCircles);
        groups = getGroups(img);
        Group* tempGroup;

        addCircleToList(masterList, img->circles);
        
        ListIterator iter;
        iter = createIterator(groups);

        while ((tempGroup = nextElement(&iter))) {
            addCircleToList(masterList, tempGroup->circles);
        }

        freeList(groups);
        return masterList;
    }
    return NULL;
}

void addCircleToList(List* masterList, List* circles) {
    ListIterator iter;
    iter = createIterator(circles);
    Circle* tempCircle;

    while ((tempCircle = nextElement(&iter))) {
        insertBack(masterList, tempCircle);
    }
}

List* getGroups(SVGimage* img) {
    if (img != NULL) {
        List* masterList;
        masterList = initializeList(&groupToString, &dummyDelete, &compareGroups);
        recurseGroup(masterList, img->groups);
        return masterList;
    }
    return NULL;
}

void recurseGroup(List* masterList, List* groups) {
    ListIterator iter;
    iter = createIterator(groups);
    Group* tempGroup;
    while ((tempGroup = nextElement(&iter))) {
        insertBack(masterList, tempGroup);
        recurseGroup(masterList, tempGroup->groups);
    }
}

List* getPaths(SVGimage* img) {
    if (img != NULL) {
        List* masterList;
        List* groups;
        masterList = initializeList(&rectangleToString, &dummyDelete, &compareRectangles);
        groups = getGroups(img);
        Group* tempGroup;

        addPathToList(masterList, img->paths);

        ListIterator iter;
        iter = createIterator(groups);

        while ((tempGroup = nextElement(&iter))) {
            addPathToList(masterList, tempGroup->paths);
        }

        freeList(groups);
        return masterList;
    }
    return NULL;
}

void addPathToList(List* masterList, List* paths) {
    ListIterator iter;
    iter = createIterator(paths);
    Path* tempPath;

    while ((tempPath = nextElement(&iter))) {
        insertBack(masterList, tempPath);
    }
}

int numRectsWithArea(SVGimage* img, float area) {
    if (img != NULL) {
        List* rectangles;
        Rectangle* temp;
        float tempArea;
        int i = 0;

        rectangles = getRects(img);

        ListIterator iter;
        iter = createIterator(rectangles);

        while (iter.current != NULL) {
            temp = (iter.current)->data;

            tempArea = temp->width * temp->height;

            if (ceil(tempArea) == ceil(area)) {
                i++;
            }

            iter.current = (iter.current)->next;
        }

        freeList(rectangles);

        return i;
    }
    return 0;
}

int numCirclesWithArea(SVGimage* img, float area) {
    if (img != NULL) {
        List* circles;
        Circle* temp;
        float tempArea;
        int i = 0;

        circles = getCircles(img);

        ListIterator iter;
        iter = createIterator(circles);

        while (iter.current != NULL) {
            temp = (iter.current)->data;

            tempArea = 3.14159 * (temp->r * temp->r);

            if (ceil(tempArea) == ceil(area)) {
                i++;
            }

            iter.current = (iter.current)->next;
        }

        freeList(circles);

        return i;
    }
    return 0;
}

int numPathsWithdata(SVGimage* img, char* data) {
    
    if (img != NULL && data != NULL) {
        List* paths;
        Path* temp;

        paths = getPaths(img);

        ListIterator iter;

        int i = 0;

        iter = createIterator(paths);

        while (iter.current != NULL) {
            temp = (iter.current)->data;
            if (strcmp(temp->data, data) == 0) {
                i++;
            }

            iter.current = (iter.current)->next;
        }

        freeList(paths);

        return i;
    }
    return 0;


}

int numGroupsWithLen(SVGimage* img, int len) {
    if (img != NULL) {
        List* groups;
        Group* temp;
        groups = getGroups(img);
        ListIterator iter;
        int j = 0;
        int count = 0;
        iter = createIterator(groups);

        while ((temp = nextElement(&iter))) {

            j += getLength(temp->rectangles);
            j += getLength(temp->circles);
            j += getLength(temp->paths);
            j += getLength(temp->groups);

            if (j == len) { 
                count++;
            }

            j = 0;
        }
        freeList(groups);
        return count;
    }
    return 0;
}

int numAttr(SVGimage* img) {
    if (img != NULL) {
        List* tempRect;
        List* tempCircles;
        List* tempPaths;
        List* tempGroups;
        List* tempAttr;

        int numAttr = 0;

        tempAttr = img->otherAttributes;
        tempRect = getRects(img);
        tempCircles = getCircles(img);
        tempGroups = getGroups(img);
        tempPaths = getPaths(img);

        ListIterator iter, iter2;
        iter = createIterator(tempAttr);

        while (iter.current != NULL) {
            numAttr++;
            iter.current = (iter.current)->next;
        }
        iter2 = createIterator(tempRect);
        
        while (iter2.current != NULL) {
            
            Rectangle* rectangle;
            ListIterator inner;
            rectangle = (Rectangle*)((iter2.current)->data);
            inner = createIterator(rectangle->otherAttributes);

            while (inner.current != NULL) {
                numAttr++;
                inner.current = (inner.current)->next;
            }
            
            iter2.current = (iter2.current)->next;
        }

        iter2 = createIterator(tempCircles);

        while (iter2.current != NULL) {
            Circle* circle;
            ListIterator inner;
            circle = (Circle*)((iter2.current)->data);

            inner = createIterator(circle->otherAttributes);

            while (inner.current != NULL) {
                numAttr++;
                inner.current = (inner.current)->next;
            }

            iter2.current = (iter2.current)->next;
        }

        iter2 = createIterator(tempPaths);

        while (iter2.current != NULL) {
            Path* path;
            ListIterator inner;
            path = (Path*)((iter2.current)->data);

            inner = createIterator(path->otherAttributes);

            while (inner.current != NULL) {
                numAttr++;
                inner.current = (inner.current)->next;
            }

            iter2.current = (iter2.current)->next;
        }

        iter2 = createIterator(tempGroups);

        while (iter2.current != NULL) {
            Group* group;
            ListIterator inner;
            group = (Group*)((iter2.current)->data);

            inner = createIterator(group->otherAttributes);

            while (inner.current != NULL) {
                numAttr++;
                inner.current = (inner.current)->next;
            }

            iter2.current = (iter2.current)->next;
        }

        freeList(tempRect);
        freeList(tempCircles);
        freeList(tempPaths);
        freeList(tempGroups);

        return numAttr;
    }
    return 0;
}

/* Additional Helpers */

Path* allocatePaths(xmlAttr* temp, xmlNode* svgchild) {
    Path* tempPath;
    List* tempPathList;
    Attribute* tempPathAttr;

    int tempLen = 0;

    tempPath = calloc(1, sizeof(Path));
    tempPathList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    temp = svgchild->properties;

    while (temp != NULL) {
        if (strcmp((char*)(temp->name), "d") == 0) {
            tempLen = strlen((char*)(temp->children->content)) + 1;
            tempPath->data = calloc(tempLen, sizeof(char));
            strcpy(tempPath->data, (char*)(temp->children->content));
        }
        else {
            tempPathAttr = calloc(1, sizeof(Attribute));
            tempLen = strlen((char*)(temp->name)) + 1;
            tempPathAttr->name = calloc(tempLen, sizeof(char));
            tempLen = strlen((char*)(temp->children->content)) + 1;
            tempPathAttr->value = calloc(tempLen, sizeof(char));
            strcpy(tempPathAttr->name, (char*)(temp->name));
            strcpy(tempPathAttr->value, (char*)(temp->children->content));
            insertBack(tempPathList, tempPathAttr);
        }
        temp = temp->next;
    }

    tempPath->otherAttributes = tempPathList;
    return tempPath;
}

Circle* allocateCircles(xmlAttr* temp, xmlNode* svgchild) {
    Circle* tempCircle;
    List* tempCircleList;
    Attribute* tempCircleAttr;
    int tempLen;
    char* tempChar = NULL;

    tempCircle = calloc(1, sizeof(Circle));
    tempCircleList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    temp = svgchild->properties;

    while (temp != NULL) {
        if (strcmp((char*)(temp->name), "cx") == 0) {
            tempCircle->cx = strtof((char*)(temp->children->content), &tempChar);
            if (tempChar[0] != '\0') {
                strcpy(tempCircle->units, tempChar);
            }
            else {
                strcpy(tempCircle->units, "");
            }
        }
        else if (strcmp((char*)(temp->name), "cy") == 0) {
            tempCircle->cy = strtof((char*)(temp->children->content), &tempChar);
        }
        else if (strcmp((char*)(temp->name), "r") == 0) {
            tempCircle->r = strtof((char*)(temp->children->content), &tempChar);
        }
        else {
            tempCircleAttr = calloc(1, sizeof(Attribute));
            tempLen = strlen((char*)(temp->name)) + 1;
            tempCircleAttr->name = calloc(tempLen, sizeof(char));
            tempLen = strlen((char*)(temp->children->content)) + 1;
            tempCircleAttr->value = calloc(tempLen, sizeof(char));
            strcpy(tempCircleAttr->name, (char*)(temp->name));
            strcpy(tempCircleAttr->value, (char*)(temp->children->content));
            insertBack(tempCircleList, tempCircleAttr);
        }
        temp = temp->next;
    }

    tempCircle->otherAttributes = tempCircleList;
    return tempCircle;
}

Rectangle* allocateRectangles(xmlAttr* temp, xmlNode* svgchild) {
    Rectangle* tempRect;
    List* tempRectList;
    Attribute* tempRectAttr;

    int tempLen;
    char* tempChar = NULL;
    
    tempRect = calloc(1, sizeof(Rectangle));
    tempRectList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    temp = svgchild->properties;

    while (temp != NULL) {
        if (strcmp((char*)(temp->name), "x") == 0) {
            tempRect->x = strtof((char*)(temp->children->content), &tempChar);
            if (tempChar[0] != '\0') {
                strcpy(tempRect->units, tempChar);
            }
            else {
                strcpy(tempRect->units, "");
            }
        }
        else if (strcmp((char*)(temp->name), "y") == 0) {
            tempRect->y = strtof((char*)(temp->children->content), &tempChar);
        }
        else if (strcmp((char*)(temp->name), "width") == 0) {
            tempRect->width = strtof((char*)(temp->children->content), &tempChar);
        }
        else if (strcmp((char*)(temp->name), "height") == 0) {
            tempRect->height = strtof((char*)(temp->children->content), &tempChar);
        }
        else {
            tempRectAttr = calloc(1, sizeof(Attribute));
            tempLen = strlen((char*)(temp->name)) + 1;
            tempRectAttr->name = calloc(tempLen, sizeof(char));
            tempLen = strlen((char*)(temp->children->content)) + 1;
            tempRectAttr->value = calloc(tempLen, sizeof(char));

            strcpy(tempRectAttr->name, (char*)(temp->name));
            strcpy(tempRectAttr->value, (char*)(temp->children->content));
            insertBack(tempRectList, tempRectAttr);
        }
        temp = temp->next;
    }
    tempRect->otherAttributes = tempRectList;
    return tempRect;
}

Group* allocateGroups(xmlAttr* temp, xmlNode* svgchild) {
    List* gRectList;
    List* gCircleList;
    List* gPathList;
    List* gGroupList;
    List* gAttrList;
    Group* newGroup = NULL;
    xmlNode* newRoot = NULL;
    xmlAttr* newTemp = NULL;

    newGroup = calloc(1, sizeof(Group));

    gRectList = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    gCircleList = initializeList(&circleToString, &deleteCircle, &compareCircles);
    gPathList = initializeList(&pathToString, &deletePath, &comparePaths);
    gGroupList = initializeList(&groupToString, &deleteGroup, &compareGroups);
    gAttrList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    newTemp = svgchild->properties;

    while (newTemp != NULL) {
        Attribute* gAttr;
        int tempLen = 0;

        gAttr = calloc(1, sizeof(Attribute));

        tempLen = strlen((char*)(newTemp->name)) + 1;
        gAttr->name = calloc(tempLen, sizeof(char));

        tempLen = strlen((char*)(newTemp->children->content)) + 1;
        gAttr->value = calloc(tempLen, sizeof(char));

        strcpy(gAttr->name, (char*)(newTemp->name));
        strcpy(gAttr->value, (char*)(newTemp->children->content));
        insertBack(gAttrList, gAttr);

        newTemp = newTemp->next;
    }
    
    newTemp = NULL;
    newRoot = svgchild->children;

    while (newRoot != NULL) {

        if (strcmp((char*)(newRoot->name), "path") == 0) {
            Path* newPath;
            newPath = allocatePaths(newTemp, newRoot);
            insertBack(gPathList, newPath);
        }

        else if (strcmp((char*)(newRoot->name), "circle") == 0) {
            Circle* newCircle;
            newCircle = allocateCircles(newTemp, newRoot);
            insertBack(gCircleList, newCircle);
        }

        else if (strcmp((char*)(newRoot->name), "rect") == 0) {
            Rectangle* newRect;
            newRect = allocateRectangles(newTemp, newRoot);
            insertBack(gRectList, newRect);
        }
        else if (strcmp((char*)(newRoot->name), "g") == 0) {
            Group* newGroup;
            newGroup = allocateGroups(newTemp, newRoot);

            insertBack(gGroupList, newGroup);
        }

        newRoot = newRoot->next;
    }

    newGroup->rectangles = gRectList;
    newGroup->circles = gCircleList;
    newGroup->paths = gPathList;
    newGroup->otherAttributes = gAttrList;
    newGroup->groups = gGroupList;

    return newGroup;
}

void dummyDelete(void* data) {
    return;
}

/* A2 M1 functions */

void addPathToDoc(List* paths, xmlNodePtr node, xmlNodePtr root) {

    ListIterator iter;
    iter = createIterator(paths);
    Path* tempPath = NULL;

    while ((tempPath = nextElement(&iter))) {
        
        node = xmlNewChild(root, NULL, BAD_CAST "path", NULL);

        xmlNewProp(node, BAD_CAST "d", BAD_CAST tempPath->data);

        ListIterator iter2;
        iter2 = createIterator(tempPath->otherAttributes);
        Attribute* tempAttr = NULL;

        while ((tempAttr = nextElement(&iter2))) {
            xmlNewProp(node, BAD_CAST tempAttr->name, BAD_CAST tempAttr->value);
        } 
    }
}

void addRectToDoc(List* rectangles, xmlNodePtr node, xmlNodePtr root) {
    ListIterator iter;
    iter = createIterator(rectangles);
    Rectangle* tempRect = NULL;

    while ((tempRect = nextElement(&iter))) {
        char buffer[1000];
        node = xmlNewChild(root, NULL, BAD_CAST "rect", NULL);
        
        sprintf(buffer, "%f%s", tempRect->x, tempRect->units);
        xmlNewProp(node, BAD_CAST "x", BAD_CAST buffer);

        sprintf(buffer, "%f%s", tempRect->y, tempRect->units);
        xmlNewProp(node, BAD_CAST "y", BAD_CAST buffer);

        sprintf(buffer, "%f%s", tempRect->width, tempRect->units);
        xmlNewProp(node, BAD_CAST "width", BAD_CAST buffer);

        sprintf(buffer, "%f%s", tempRect->height, tempRect->units);
        xmlNewProp(node, BAD_CAST "height", BAD_CAST buffer);

        ListIterator iter2;
        iter2 = createIterator(tempRect->otherAttributes);
        Attribute* tempAttr = NULL;

        while ((tempAttr = nextElement(&iter2))) {
            xmlNewProp(node, BAD_CAST tempAttr->name, BAD_CAST tempAttr->value);
        }  
    }
}

void addCircleToDoc(List* circles, xmlNodePtr node, xmlNodePtr root) {
    ListIterator iter;
    iter = createIterator(circles);
    Circle* tempCircle = NULL;

    while ((tempCircle = nextElement(&iter))) {
        char buffer[1000];
        node = xmlNewChild(root, NULL, BAD_CAST "circle", NULL);

        sprintf(buffer, "%f%s", tempCircle->cx, tempCircle->units);
        xmlNewProp(node, BAD_CAST "cx", BAD_CAST buffer);
        
        sprintf(buffer, "%f%s", tempCircle->cy, tempCircle->units);
        xmlNewProp(node, BAD_CAST "cy", BAD_CAST buffer);
        
        sprintf(buffer, "%f%s", tempCircle->r, tempCircle->units);
        xmlNewProp(node, BAD_CAST "r", BAD_CAST buffer);

        ListIterator iter2;
        iter2 = createIterator(tempCircle->otherAttributes);
        Attribute* tempAttr = NULL;

        while ((tempAttr = nextElement(&iter2))) {
            xmlNewProp(node, BAD_CAST tempAttr->name, BAD_CAST tempAttr->value);
        } 
    }
}

void addGroupToDoc(List* groups, xmlNodePtr node, xmlNodePtr root) {
    ListIterator iter;
    iter = createIterator(groups);
    Group* tempGroup = NULL;

    xmlNodePtr newNode = NULL;

    while ((tempGroup = nextElement(&iter))) {
        node = xmlNewChild(root, NULL, BAD_CAST "g", NULL);

        addRectToDoc(tempGroup->rectangles, newNode, node);
        addCircleToDoc(tempGroup->circles, newNode, node);
        addPathToDoc(tempGroup->paths, newNode, node);
        addGroupToDoc(tempGroup->groups, newNode, node);

        ListIterator iter2;
        iter2 = createIterator(tempGroup->otherAttributes);
        Attribute* tempAttr = NULL;

        while ((tempAttr = nextElement(&iter2))) {
            xmlNewProp(node, BAD_CAST tempAttr->name, BAD_CAST tempAttr->value);
        }
    }
}

xmlDocPtr createXMLFromSVG(SVGimage* img) {

    xmlDocPtr doc = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr node = NULL;
    xmlNsPtr ns = NULL;

    if (img == NULL) {
        return NULL;
    }

    LIBXML_TEST_VERSION

    doc = xmlNewDoc(BAD_CAST "1.0");
    root = xmlNewNode(NULL, BAD_CAST "svg");
    ns = xmlNewNs(root, BAD_CAST img->namespace, NULL);
    xmlSetNs(root, ns);
    xmlDocSetRootElement(doc, root);

    if (strcmp(img->title, "") != 0) {
        xmlNewChild(root, NULL, BAD_CAST "title", BAD_CAST img->title);
    }
    if (strcmp(img->description, "") != 0) {
        xmlNewChild(root, NULL, BAD_CAST "desc", BAD_CAST img->description);
    }

    ListIterator iter2;
    iter2 = createIterator(img->otherAttributes);
    Attribute* tempAttr = NULL;

    while ((tempAttr = nextElement(&iter2))) {
        xmlNewProp(root, BAD_CAST tempAttr->name, BAD_CAST tempAttr->value);
    }

    addRectToDoc(img->rectangles, node, root);
    addCircleToDoc(img->circles, node, root);
    addPathToDoc(img->paths, node, root);
    addGroupToDoc(img->groups, node, root);

    /* xmlSaveFormatFileEnc("test.svg", doc, "UTF-8", 1); */

    return doc;

    

    /* xmlCleanupParser();

    xmlMemoryDump(); */

    

}

int checkAttributes(List* attributes) {
    ListIterator iter;
    iter = createIterator(attributes);
    Attribute* tempAttr;
    while ((tempAttr = nextElement(&iter))) {
        if (tempAttr->name == NULL || tempAttr->value == NULL) {
            return 0;
        }
    }
    return 1;
}

int checkCircles(List* circles) {
    ListIterator iter;
    iter = createIterator(circles);
    Circle* tempCircle;
    while ((tempCircle = nextElement(&iter))) {
        if (tempCircle->r < 0 || tempCircle->otherAttributes == NULL) {
            return 0;
        }

        if (!checkAttributes(tempCircle->otherAttributes)) {
            return 0;
        }
    }
    return 1;
}

int checkRects(List* rectangles) {
    ListIterator iter;
    iter = createIterator(rectangles);
    Rectangle* tempRect;
    while ((tempRect = nextElement(&iter))) {
        if (tempRect->width < 0 || tempRect->height < 0 || tempRect->otherAttributes == NULL) {
            return 0;
        }
        if (!checkAttributes(tempRect->otherAttributes)) {
            return 0;
        }
    }
    return 1;
}

int checkPaths(List* paths) {
    ListIterator iter;
    iter = createIterator(paths);
    Path* tempPath;
    while ((tempPath = nextElement(&iter))) {
        if (tempPath->data == NULL || tempPath->otherAttributes == NULL) {
            return 0;
        }
        if (!checkAttributes(tempPath->otherAttributes)) {
            return 0;
        }
    }
    return 1;
}

int checkGroups(List* groups) {
    ListIterator iter;
    iter = createIterator(groups);
    Group* tempGroup;
    while ((tempGroup = nextElement(&iter))) {
        if (tempGroup->rectangles == NULL || tempGroup->circles == NULL|| tempGroup->paths == NULL || tempGroup->groups == NULL || tempGroup->otherAttributes == NULL) {
            return 0;
        }
        if (!checkAttributes(tempGroup->otherAttributes)) {
            return 0;
        }
        if (!checkRects(tempGroup->rectangles)) {
            return 0;
        }
        if (!checkCircles(tempGroup->circles)) {
            return 0;
        }
        if (!checkPaths(tempGroup->paths)) {
            return 0;
        }
        if (!checkGroups(tempGroup->groups)) {
            return 0;
        }
    }
    return 1;
}

bool validateSVGimage(SVGimage* image, char* schemaFile) {
    if (image != NULL) {

        if (image->title == NULL) {
            return false;
        }

        if (image->description == NULL) {
            return false;
        }

        if (image->namespace == NULL) {
            return false;
        }
        else {
            if (strcmp(image->namespace, "") == 0) {
                return false;
            }
        }

        if (!checkCircles(image->circles)) {
            return false;
        }
        if (!checkRects(image->rectangles)) {
            return false;
        }
        if (!checkPaths(image->paths)) {
            return false;
        }
        if (!checkAttributes(image->otherAttributes)) {
            return false;
        }
        if (!checkGroups(image->groups)) {
            return false;
        }

        if (schemaFile != NULL) {

            /* setup schema file */
            xmlSchemaPtr schema = NULL;
            xmlSchemaParserCtxtPtr ctxt = NULL;
            xmlDocPtr doc = NULL;
            int retNum = 1;

            ctxt = xmlSchemaNewParserCtxt(schemaFile);
            schema = xmlSchemaParse(ctxt);
            xmlSchemaFreeParserCtxt(ctxt);

            doc = createXMLFromSVG(image);

            xmlSchemaValidCtxtPtr vctxt;
            vctxt = xmlSchemaNewValidCtxt(schema);
            retNum = xmlSchemaValidateDoc(vctxt, doc);

            xmlSchemaFreeValidCtxt(vctxt);
            xmlFreeDoc(doc);

            if (schema != NULL) {
                xmlSchemaFree(schema);
                xmlSchemaCleanupTypes();
                xmlCleanupParser();
                xmlMemoryDump();
            }
            if (retNum == 0) {
                return true;
            }
            else {
                return false;
            }

        }
    }
    return false;
}

SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {

    if (fileName == NULL || schemaFile == NULL) {
        return NULL;
    }

    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt = NULL;
    xmlDocPtr doc = NULL;
    int retNum = 1;

    ctxt = xmlSchemaNewParserCtxt(schemaFile);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    doc = xmlReadFile(fileName, NULL, 0);

    xmlSchemaValidCtxtPtr vctxt;
    vctxt = xmlSchemaNewValidCtxt(schema);
    retNum = xmlSchemaValidateDoc(vctxt, doc);

    xmlSchemaFreeValidCtxt(vctxt);
    xmlFreeDoc(doc);

    if (schema != NULL) {
        xmlSchemaFree(schema);
        xmlSchemaCleanupTypes();
        xmlCleanupParser();
        xmlMemoryDump();
    }

    if (retNum != 0) {
        return NULL;
    }

    SVGimage* img;

    img = createSVGimage(fileName);

    bool result;

    result = validateSVGimage(img, schemaFile);

    if (result == false) {
        deleteSVGimage(img);
        return NULL;
    }

    return img;
}

bool writeSVGimage(SVGimage* image, char* fileName) {
    if (image == NULL || fileName == NULL) {
        return false;
    }

    int len;

    len = strlen(fileName);

    if (fileName[len - 1] == 'g' && fileName[len - 2] == 'v' && fileName[len - 3] == 's' && fileName[len - 4] == '.') {
        xmlDocPtr doc = NULL;

        doc = createXMLFromSVG(image);

        xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);

        xmlFreeDoc(doc);
        xmlCleanupParser();
        return true;
    }

    

    return false;
}

void freeAttribute(Attribute* attribute) {
    if (attribute != NULL) {
        if (attribute->name != NULL) {
            free(attribute->name);
        }
        if (attribute->value != NULL) {
            free(attribute->value);
        }
        free(attribute);
    }
}

void replaceAttribute(Attribute* newAttribute, List* attributeList) {
    ListIterator iter;
    iter = createIterator(attributeList);
    Attribute* tempAttr;

    while ((tempAttr = nextElement(&iter))) {
        if (strcmp(tempAttr->name, newAttribute->name) == 0) {
            int len = 0;
            free(tempAttr->value);
            len = strlen(newAttribute->value) + 1;
            tempAttr->value = calloc(len, sizeof(char));
            strcpy(tempAttr->value, newAttribute->value);
            freeAttribute(newAttribute);
            return;
        }
    }
    insertBack(attributeList, newAttribute);
    return;
}

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute) {
    if (image == NULL || (elemType < 0 || elemType > 4) || newAttribute == NULL || newAttribute->name == NULL || newAttribute->value == NULL) {
        return;
    }
    if (elemType == 0) {
        if (strcmp(newAttribute->name, "description") == 0) {
            strcpy(image->description, newAttribute->value);
            freeAttribute(newAttribute);
            return;
        }
        else if (strcmp(newAttribute->name, "title") == 0) {
            strcpy(image->title, newAttribute->value);
            freeAttribute(newAttribute);
            return;
        }
        else if (strcmp(newAttribute->name, "namespace") == 0) {
            strcpy(image->namespace, newAttribute->value);
            freeAttribute(newAttribute);
            return;
        }
        else {
            replaceAttribute(newAttribute, image->otherAttributes);
            return;
        }
    }
    if (elemType == 1) {
        int i = 0;
        int length = 0;
        length = getLength(image->circles);

        if (elemIndex < 0 || elemIndex > length - 1) {
            return;
        }

        Circle* tempCircle;
        ListIterator iter;
        iter = createIterator(image->circles);

        for (i = 0; i < elemIndex + 1; i++) {
            tempCircle = nextElement(&iter);
        }
        if (strcmp(newAttribute->name, "cx") == 0) {
            tempCircle->cx = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        if (strcmp(newAttribute->name, "cy") == 0) {
            tempCircle->cy = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        if (strcmp(newAttribute->name, "r") == 0) {
            tempCircle->r = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        else {
            replaceAttribute(newAttribute, tempCircle->otherAttributes);
            return;
        }
    }
    if (elemType == 2) {
        int i = 0;
        int length = 0;

        length = getLength(image->rectangles);

        if (elemIndex < 0 || elemIndex > length - 1) {
            return;
        }

        Rectangle* tempRect;
        ListIterator iter;
        iter = createIterator(image->rectangles);

        for (i = 0; i < elemIndex + 1; i++) {
            tempRect = nextElement(&iter);
        }

        if (strcmp(newAttribute->name, "x") == 0) {
            tempRect->x = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        if (strcmp(newAttribute->name, "y") == 0) {
            tempRect->y = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        if (strcmp(newAttribute->name, "width") == 0) {
            tempRect->width = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        if (strcmp(newAttribute->name, "height") == 0) {
            tempRect->height = strtof(newAttribute->value, NULL);
            freeAttribute(newAttribute);
            return;
        }
        else {
            replaceAttribute(newAttribute, tempRect->otherAttributes);
            return;
        }
    }
    if (elemType == 3) {
        int i = 0;
        int length = 0;
        length = getLength(image->paths);

        if (elemIndex < 0 || elemIndex > length - 1) {
            return;
        }

        Path* tempPath;
        ListIterator iter;
        iter = createIterator(image->paths);

        for (i = 0; i < elemIndex + 1; i++) {
            tempPath = nextElement(&iter);
        }

        if (strcmp(newAttribute->name, "d") == 0) {
            free(tempPath->data);
            int len = 0;
            len = strlen(newAttribute->value) + 1;
            tempPath->data = calloc(len, sizeof(char));
            strcpy(tempPath->data, newAttribute->value);
            freeAttribute(newAttribute);
            return;
        }
        else {
            replaceAttribute(newAttribute, tempPath->otherAttributes);
            return;
        }
    }
    if (elemType == 4) {
        int i = 0;
        int length = 0;
        length = getLength(image->groups);

        if (elemIndex < 0 || elemIndex > length - 1) {
            return;
        }

        Group* tempGroup;
        ListIterator iter;
        iter = createIterator(image->groups);

        for (i = 0; i < elemIndex + 1; i++) {
            tempGroup = nextElement(&iter);
        }

        replaceAttribute(newAttribute, tempGroup->otherAttributes);
        return;
    }
}

void addComponent(SVGimage* image, elementType type, void* newElement) {

    if (image == NULL || type > 4 || newElement == NULL) {
        return;
    }
    
    if (type == 1) {
        newElement = (Circle *) newElement;
        insertBack(image->circles, newElement);
    }
    if (type == 2) {
        newElement = (Rectangle *) newElement;
        insertBack(image->rectangles, newElement);
    }
    if (type == 3) {
        newElement = (Path *) newElement;
        insertBack(image->paths, newElement);
    }

    return;
}

char* attrToJSON(const Attribute *a) {
    if (a == NULL) {
        return NULL;
    }

    int nameLen = 0;
    int valueLen = 0;
    int defaultLen = 22;
    int totalLen = 0;

    nameLen = strlen(a->name);
    valueLen = strlen(a->value);

    totalLen = nameLen + valueLen + defaultLen + 1;

    char* newAttrStr = NULL;
    newAttrStr = calloc(totalLen, sizeof(char));

    sprintf(newAttrStr, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);

    return newAttrStr;
}

char* circleToJSON(const Circle *c) {
    if (c == NULL) {
        return NULL;
    }

    char buffer[1000];

    int cxLen = 0;
    int cyLen = 0;
    int rLen = 0;
    int attLen = 0;
    int unitLen = 0;
    int defaultLen = 40;
    int totalLen = 0;

    sprintf(buffer, "%.2f", c->cx);
    cxLen = strlen(buffer);
    sprintf(buffer, "%.2f", c->cy);
    cyLen = strlen(buffer);
    sprintf(buffer, "%.2f", c->r);
    rLen = strlen(buffer);
    sprintf(buffer, "%d", getLength(c->otherAttributes));
    attLen = strlen(buffer);
    unitLen = strlen(c->units);

    totalLen = cxLen + cyLen + rLen + defaultLen + unitLen + attLen + 1;

    char* newCircleStr = NULL;
    newCircleStr = calloc(totalLen, sizeof(char));

    sprintf(newCircleStr, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", c->cx, c->cy, c->r, getLength(c->otherAttributes), c->units);
    
    char* attrJSON = NULL;
    attrJSON = attrListToJSON(c->otherAttributes);
    int len;
    len = 0;
    len += strlen(newCircleStr);
    len += 13;
    len += 1;
    len += 1;
    len += strlen(attrJSON);

    char* newResult;
    newResult = calloc(len, sizeof(char));
    sprintf(newResult, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\",\"attributes\":", c->cx, c->cy, c->r, getLength(c->otherAttributes), c->units);
    strcat(newResult, attrJSON);
    strcat(newResult, "}");

    return newResult;
}

char* rectToJSON(const Rectangle *r) {
    if (r == NULL) {
        return NULL;
    }

    char buffer[1000];

    int xLen = 0;
    int yLen = 0;
    int wLen = 0;
    int hLen = 0;
    int attLen = 0;
    int unitLen = 0;
    int defaultLen = 43;
    int totalLen = 0;

    sprintf(buffer, "%.2f", r->x);
    xLen = strlen(buffer);
    sprintf(buffer, "%.2f", r->y);
    yLen = strlen(buffer);
    sprintf(buffer, "%.2f", r->width);
    wLen = strlen(buffer);
    sprintf(buffer, "%.2f", r->height);
    hLen = strlen(buffer);
    sprintf(buffer, "%d", getLength(r->otherAttributes));
    attLen = strlen(buffer);
    unitLen = strlen(r->units);

    totalLen = xLen + yLen + wLen + hLen + attLen + unitLen + defaultLen + 1;

    char* newRectStr = NULL;
    newRectStr = calloc(totalLen, sizeof(char));

    sprintf(newRectStr, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", r->x, r->y, r->width, r->height, getLength(r->otherAttributes), r->units);
    
    char* attrJSON = NULL;
    attrJSON = attrListToJSON(r->otherAttributes);
    int len;
    len = 0;
    len += strlen(newRectStr);
    len += 13;
    len += 1;
    len += 1;
    len += strlen(attrJSON);

    char* newResult;
    newResult = calloc(len, sizeof(char));
    sprintf(newResult, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\",\"attributes\":", r->x, r->y, r->width, r->height, getLength(r->otherAttributes), r->units);
    strcat(newResult, attrJSON);
    strcat(newResult, "}");

    return newResult;
}
char* pathToJSON(const Path *p) {
    if (p == NULL) {
        return NULL;
    }

    char buffer[1000];

    int dLen = 0;
    int attLen = 0;
    int defaultLen = 19;
    int totalLen = 0;

    dLen = strlen(p->data);
    if (dLen > 64) {
        dLen = 64;
    }

    char* tempStr;
    tempStr = calloc(dLen + 1, sizeof(char));

    snprintf(tempStr, dLen, "%s", p->data);

    sprintf(buffer, "%d", getLength(p->otherAttributes));
    attLen = strlen(buffer);

    totalLen = dLen + attLen + defaultLen + 1;

    char* newPathStr = NULL;
    newPathStr = calloc(totalLen, sizeof(char));

    sprintf(newPathStr, "{\"d\":\"%s\",\"numAttr\":%d}", tempStr, getLength(p->otherAttributes));
    
    char* attrJSON = NULL;
    attrJSON = attrListToJSON(p->otherAttributes);
    int len;
    len = 0;
    len += strlen(newPathStr);
    len += 13;
    len += 1;
    len += 1;
    len += strlen(attrJSON);

    char* newResult;
    newResult = calloc(len, sizeof(char));
    sprintf(newResult, "{\"d\":\"%s\",\"numAttr\":%d,\"attributes\":", tempStr, getLength(p->otherAttributes));
    strcat(newResult, attrJSON);
    strcat(newResult, "}");

    return newResult;
}

char* groupToJSON(const Group *g) {
    if (g == NULL) {
        return NULL;
    }

    int rLen = 0;
    int cLen = 0;
    int pLen = 0;
    int gLen = 0;

    int subTotalLen = 0;
    int subTotalCharLen = 0;

    int attLen = 0;
    int defaultLen = 24;
    int totalLen = 0;


    rLen = getLength(g->rectangles);
    cLen = getLength(g->circles);
    pLen = getLength(g->paths);
    gLen = getLength(g->groups);
    
    subTotalLen = rLen + cLen + pLen + gLen;

    char buffer[1000];

    sprintf(buffer, "%d", subTotalLen);
    subTotalCharLen = strlen(buffer);
    sprintf(buffer, "%d", getLength(g->otherAttributes));
    attLen = strlen(buffer);

    totalLen = subTotalCharLen + attLen + defaultLen + 1;

    char* newGroupStr = NULL;
    newGroupStr = calloc(totalLen, sizeof(char));

    sprintf(newGroupStr, "{\"children\":%d,\"numAttr\":%d}", subTotalLen, getLength(g->otherAttributes));
    char* attrJSON = NULL;
    attrJSON = attrListToJSON(g->otherAttributes);
    int len;
    len = 0;
    len += strlen(newGroupStr);
    len += 13;
    len += 1;
    len += 1;
    len += strlen(attrJSON);

    char* newResult;
    newResult = calloc(len, sizeof(char));
    sprintf(newResult, "{\"children\":%d,\"numAttr\":%d,\"attributes\":", subTotalLen, getLength(g->otherAttributes));

    strcat(newResult, attrJSON);
    strcat(newResult, "}");



    return newResult;
}

char* attrListToJSON(const List *list) {
    if (list == NULL) {
        return "[{}]";
    }

    List* tempList = (List*) list;

    int numElem = 0;
    int totalLen = 0;
    int i = 0;

    char** strArr;
    char* result;

    numElem = getLength(tempList);

    if (numElem == 0) {
        return "[{}]";
    }

    strArr = calloc(numElem, sizeof(char*));

    totalLen = 2 + (numElem - 1);

    ListIterator iter;
    iter = createIterator(tempList);
    Attribute* attr;

    for (i = 0; i < numElem; i++) {
        attr = nextElement(&iter);
        strArr[i] = attrToJSON(attr);
        totalLen += strlen(strArr[i]);
    }

    result = calloc(totalLen + 1, sizeof(char));

    strcat(result, "[");

    for (i = 0; i < numElem; i++) {
        strcat(result, strArr[i]);
        if (i != numElem - 1) {
            strcat(result, ",");
        }
    }

    strcat(result, "]");

    for (i = 0; i < numElem; i++) {
        free(strArr[i]);
    }
    free(strArr);

    return result;

}
char* circListToJSON(const List *list) {
    if (list == NULL) {
        return "[{}]";
    }

    List* tempList = (List*) list;

    int numElem = 0;
    int totalLen = 0;
    int i = 0;

    char** strArr;
    char* result;

    numElem = getLength(tempList);

    if (numElem == 0) {
        return "[{}]";
    }

    strArr = calloc(numElem, sizeof(char*));

    totalLen = 2 + (numElem - 1);

    ListIterator iter;
    iter = createIterator(tempList);
    Circle* circle;

    for (i = 0; i < numElem; i++) {
        circle = nextElement(&iter);
        strArr[i] = circleToJSON(circle);
        totalLen += strlen(strArr[i]);
    }

    result = calloc(totalLen + 1, sizeof(char));

    strcat(result, "[");

    for (i = 0; i < numElem; i++) {
        strcat(result, strArr[i]);
        if (i != numElem - 1) {
            strcat(result, ",");
        }
    }

    strcat(result, "]");

    for (i = 0; i < numElem; i++) {
        free(strArr[i]);
    }
    free(strArr);

    return result;
}

char* rectListToJSON(const List *list) {
    if (list == NULL) {
        return "[{}]";
    }

    List* tempList = (List*) list;

    int numElem = 0;
    int totalLen = 0;
    int i = 0;

    char** strArr;
    char* result;

    numElem = getLength(tempList);

    if (numElem == 0) {
        return "[{}]";
    }

    strArr = calloc(numElem, sizeof(char*));

    totalLen = 2 + (numElem - 1);

    ListIterator iter;
    iter = createIterator(tempList);
    Rectangle* rect;

    for (i = 0; i < numElem; i++) {
        rect = nextElement(&iter);
        strArr[i] = rectToJSON(rect);
        totalLen += strlen(strArr[i]);
    }

    result = calloc(totalLen + 1, sizeof(char));

    strcat(result, "[");

    for (i = 0; i < numElem; i++) {
        strcat(result, strArr[i]);
        if (i != numElem - 1) {
            strcat(result, ",");
        }
    }

    strcat(result, "]");

    for (i = 0; i < numElem; i++) {
        free(strArr[i]);
    }
    free(strArr);

    return result;
}

char* pathListToJSON(const List *list) {
    if (list == NULL) {
        return "[{}]";
    }

    List* tempList = (List*) list;

    int numElem = 0;
    int totalLen = 0;
    int i = 0;

    char** strArr;
    char* result;

    numElem = getLength(tempList);

    if (numElem == 0) {
        return "[{}]";
    }

    strArr = calloc(numElem, sizeof(char*));

    totalLen = 2 + (numElem - 1);

    ListIterator iter;
    iter = createIterator(tempList);
    Path* path;

    for (i = 0; i < numElem; i++) {
        path = nextElement(&iter);
        strArr[i] = pathToJSON(path);
        totalLen += strlen(strArr[i]);
    }

    result = calloc(totalLen + 1, sizeof(char));

    strcat(result, "[");

    for (i = 0; i < numElem; i++) {
        strcat(result, strArr[i]);
        if (i != numElem - 1) {
            strcat(result, ",");
        }
    }

    strcat(result, "]");

    for (i = 0; i < numElem; i++) {
        free(strArr[i]);
    }
    free(strArr);

    return result;
}

char* groupListToJSON(const List *list) {
    if (list == NULL) {
        return "[{}]";
    }

    List* tempList = (List*) list;

    int numElem = 0;
    int totalLen = 0;
    int i = 0;

    char** strArr;
    char* result;

    numElem = getLength(tempList);

    if (numElem == 0) {
        return "[{}]";
    }

    strArr = calloc(numElem, sizeof(char*));

    totalLen = 2 + (numElem - 1);

    ListIterator iter;
    iter = createIterator(tempList);
    Group* group;

    for (i = 0; i < numElem; i++) {
        group = nextElement(&iter);
        strArr[i] = groupToJSON(group);
        totalLen += strlen(strArr[i]);
    }

    result = calloc(totalLen + 1, sizeof(char));

    strcat(result, "[");

    for (i = 0; i < numElem; i++) {
        strcat(result, strArr[i]);
        if (i != numElem - 1) {
            strcat(result, ",");
        }
    }

    strcat(result, "]");

    for (i = 0; i < numElem; i++) {
        free(strArr[i]);
    }
    free(strArr);

    return result;
}

char* SVGtoJSON(const SVGimage* image) {
    if (image == NULL) {
        return NULL;
    }

    int numR = 0;
    int numC = 0;
    int numP = 0;
    int numG = 0;

    List* rectList = NULL;
    List* circList = NULL;
    List* pathList = NULL;
    List* groupList = NULL;

    rectList = getRects((SVGimage *) image);
    circList = getCircles((SVGimage *) image);
    pathList = getPaths((SVGimage *) image);
    groupList = getGroups((SVGimage *) image);

    numR = getLength(rectList);
    numC = getLength(circList);
    numP = getLength(pathList);
    numG = getLength(groupList);

    char buffer[1000];
    int totalLen = 0;
    totalLen = 48 + 1;

    sprintf(buffer, "%d", numR);
    totalLen += strlen(buffer);
    sprintf(buffer, "%d", numC);
    totalLen += strlen(buffer);
    sprintf(buffer, "%d", numP);
    totalLen += strlen(buffer);
    sprintf(buffer, "%d", numG);
    totalLen += strlen(buffer);

    char* result;

    result = calloc(totalLen, sizeof(char));

    sprintf(result, "{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}", numR, numC, numP, numG);

    freeList(rectList);
    freeList(circList);
    freeList(pathList);
    freeList(groupList);

    return result;
}

SVGimage* JSONtoSVG(const char* svgString) {
    return NULL;
}
Rectangle* JSONtoRect(const char* svgString) {
    return NULL;
}
Circle* JSONtoCircle(const char* svgString) {
    return NULL;
}

char* testFunction() {
    char a[] = "This is a test function!!!";
    char* temp;

    temp = malloc(sizeof(char) * (strlen(a) + 1));
    strcpy(temp, a);
    return temp;
}

char* createValidSVGandJSON(char* filename, char* schemaFile) {

    SVGimage* img;

    img = NULL;

    img = createValidSVGimage(filename, schemaFile);

    if (img == NULL) {
        return "{}";
    }
    else {
        return SVGtoJSON(img);
    }

}

char* createValidSVGandComponentJSON(char* filename, char* schemaFile) {
    SVGimage* img;

    img = NULL;

    img = createValidSVGimage(filename, schemaFile);

    char* rectData;
    char* circData;
    char* pathData;
    char* groupData;

    int totalLen;
    totalLen = 0;

    rectData = rectListToJSON(img->rectangles);
    circData = circListToJSON(img->circles);
    pathData = pathListToJSON(img->paths);
    groupData = groupListToJSON(img->groups);

    totalLen += strlen(rectData);
    totalLen += strlen(circData);
    totalLen += strlen(pathData);
    totalLen += strlen(groupData);

    totalLen += 3;
    totalLen += 2;
    totalLen += 5;
    totalLen += 7;
    totalLen += 5;
    totalLen += 6;
    totalLen += 1;
    totalLen += 8;
    totalLen += 15;
    totalLen += 9;
    totalLen += 4;

    totalLen += strlen(img->title);
    totalLen += strlen(img->description);

    char* retString;

    retString = calloc(totalLen, sizeof(char));

    int i;
    i = 0;

    for (i = 0; i < strlen(img->title); i++) {
        if (img->title[i] == '\"' || img->title[i] == '\n') {
            img->title[i] = '-';
        }
    }

    for (i = 0; i < strlen(img->description); i++) {
        if (img->description[i] == '\"' || img->description[i] == '\n') {
            img->description[i] = '-';
        }
    }

    strcpy(retString, "{\"rect\":");
    strcat(retString, rectData);
    strcat(retString, ",\"circle\":");
    strcat(retString, circData);
    strcat(retString, ",\"path\":");
    strcat(retString, pathData);
    strcat(retString, ",\"group\":");
    strcat(retString, groupData);
    strcat(retString, ",\"title\":\"");
    strcat(retString, img->title);
    strcat(retString, "\"");
    strcat(retString, ",\"description\":\"");
    strcat(retString, img->description);
    strcat(retString, "\"");
    strcat(retString, "}");

    return retString;
}

char* updateTitleOfImage(char* filename, char* newTitle, char* newDescription, char* schemaFile) {

    SVGimage* img;

    img = createValidSVGimage(filename, schemaFile);

    char* temp1;

    int len;
    len = strlen(newTitle) + 1;

    temp1 = calloc(len, sizeof(char));
    
    char* temp2;
    len = 0;
    len = strlen(newDescription) + 1;

    temp2 = calloc(len, sizeof(char));

    strcpy(temp1, newTitle);
    strcpy(temp2, newDescription);

    strcpy(img->title, newTitle);
    strcpy(img->description, newDescription);

    writeSVGimage(img, filename);

    return "newTitle";

}

int updateAttributeAndSaveFile(char* filename, char* schemaFile, char* newName, char* newVal, int elemType, int elemIndex) {

    SVGimage* img;
    img = NULL;
    img = createValidSVGimage(filename, schemaFile);

    Attribute* newAttr;
    int len1;
    int len2;
    len1 = 0;
    len2 = 0;

    len1 = strlen(newName) + 1;
    len2 = strlen(newVal) + 1;

    newAttr = calloc(1, sizeof(Attribute));

    newAttr->name = calloc(len1, sizeof(char));
    newAttr->value = calloc(len2, sizeof(char));

    strcpy(newAttr->name, newName);
    strcpy(newAttr->value, newVal);

    elementType newType;

    if (elemType == 0) {
        newType = SVG_IMAGE;
    }
    else if (elemType == 1) {
        newType = CIRC;
    }
    else if (elemType == 2) {
        newType = RECT;
    }
    else if (elemType == 3) {
        newType = PATH;
    }
    else if (elemType == 4) {
        newType = GROUP;
    }

    setAttribute(img, newType, elemIndex, newAttr);

    bool result;

    result = validateSVGimage(img, schemaFile);

    if (result == true) {
        writeSVGimage(img, filename);
        return 1;
    }
    else {
        return -1;
    }

}

int addRectangleToSVGandSave(char* filename, char* schemaFile, float x, float y, float width, float height, char* units) {
    SVGimage* img;

    img = NULL;
    img = createValidSVGimage(filename, schemaFile);

    Rectangle* rect;

    rect = calloc(1, sizeof(Rectangle));

    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    strcpy(rect->units, units);
    rect->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    insertBack(img->rectangles, rect);

    bool result;

    result = validateSVGimage(img, schemaFile);

    if (result == true) {
        writeSVGimage(img, filename);
        return 1;
    }
    else {
        return -1;
    }

}

int addCircleToSVGandSave(char* filename, char* schemaFile, float cx, float cy, float r, char* units) {

    SVGimage* img;

    img = NULL;
    img = createValidSVGimage(filename, schemaFile);

    Circle* circle;

    circle = calloc(1, sizeof(Circle));

    circle->cx = cx;
    circle->cy = cy;
    circle->r = r;
    strcpy(circle->units, units);
    circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    insertBack(img->circles, circle);

    bool result;

    result = validateSVGimage(img, schemaFile);

    if (result == true) {
        writeSVGimage(img, filename);
        return 1;
    }
    else {
        return -1;
    }

}