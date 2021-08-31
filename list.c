
#include<stdio.h>
#include "list.h"


static List listHeads[LIST_MAX_NUM_HEADS];
static Node listNodes[LIST_MAX_NUM_NODES];


static int freePositions[LIST_MAX_NUM_NODES+LIST_MAX_NUM_HEADS];

static int headCount = 0;
static int nodeCount = 0;
static int freeHeadCount = 0;
static int freeNodeCount = 0;

void Initializer(){
    int headCtr,nodeCtr, freePositionsCtr;
    
    for( headCtr = 0; headCtr<LIST_MAX_NUM_HEADS;headCtr++){
    
        listHeads[headCtr].head = NULL;
        listHeads[headCtr].tail = NULL;
        listHeads[headCtr].curr = NULL;
        listHeads[headCtr].usedHeadIndex = -1;

    }

    for(nodeCtr = 0 ; nodeCtr<LIST_MAX_NUM_NODES; nodeCtr++){
        
        listNodes[nodeCtr].data = NULL;
        listNodes[nodeCtr].nextNode = NULL;
        listNodes[nodeCtr].prevNode = NULL;
        listNodes[nodeCtr].usedNodeIndex = -1;

        
    }
    for(freePositionsCtr =0; freePositionsCtr<(LIST_MAX_NUM_HEADS+LIST_MAX_NUM_NODES); freePositionsCtr++){
        freePositions[freePositionsCtr] = -1;
    }

  

}

// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or 
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){

    if( headCount == 0){
        Initializer();
    }

    if(headCount >= LIST_MAX_NUM_HEADS && freeHeadCount == 0){    //both exhausted
            printf("Out of List Heads");
            return NULL;
    }


    if(headCount >= LIST_MAX_NUM_HEADS){  //original exhausted
    
        int position = freePositions[LIST_MAX_NUM_NODES-1 + freeHeadCount];
        List * listI = &listHeads[position];  

        // tracking head used.
        listI -> usedHeadIndex = position;
        

        listI -> listSize = 0;
        listI->beforeHead = false;
        listI->afterTail = false;
        listI->curr = NULL;
        freeHeadCount--;
        
        return listI;
    }
    else{
        List * listI = &listHeads[headCount];  

        // tracking head used.
        listI -> usedHeadIndex = headCount;
       

        listI -> listSize = 0;
        listI->beforeHead = false;
        listI->afterTail = false;
        listI->curr = NULL;
        headCount++;
        
        return listI;
    }
}

// Returns the number of items in pList.
int List_count(List* pList){
    
    int elmCount = 1;

    if(pList->head == NULL){                          
        elmCount = 0;                                
    }
   
    else{
        Node * temp = pList->head;
        while(temp->nextNode != NULL){
            elmCount++;
            temp = temp->nextNode;
        }
  
    }  
    return elmCount;

}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){

    if(pList -> listSize == 0){ // empty list
        pList -> curr = NULL;
        return NULL;
    }
   
    
    pList -> curr = pList -> head;
    return pList->curr->data;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){


    if(pList -> listSize == 0){ // empty list
        pList -> curr = NULL;
        return NULL;
    }

    
    pList -> curr = pList -> tail;
    
    
    return pList-> curr->data;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
     
        if(pList->beforeHead){
            pList->curr = pList->head;
            pList->beforeHead = false;
            return pList->curr->data;
        }

        if(pList->curr == pList->tail){
            pList->afterTail = true;
            pList->curr = NULL;
            return NULL;
        }
        pList->curr=pList->curr->nextNode;
        return pList->curr->data;
       

}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){

        
        if(pList -> afterTail){ 
            pList -> curr = pList->tail;
            pList->afterTail = false;
            return pList->curr->data;
        }
        if(pList->curr == pList->head){
            pList->beforeHead = true;
            pList->curr = NULL;
            return NULL;
        }
        pList->curr = pList->curr->prevNode;
        return pList->curr->data;
       
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    return pList->curr->data;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem){

    
    if(nodeCount >= LIST_MAX_NUM_NODES && freeNodeCount == 0){          //both exhausted
        return -1;                                              //no more nodes, FAILED OPN
    }
    
    Node * addNode;
    if(nodeCount >= LIST_MAX_NUM_NODES){                        //if first exhausted use second

        int position = freePositions[freeNodeCount];
       //new node to be added made
        addNode = &listNodes[position];
        
        // // tracking the nodes.
        addNode -> usedNodeIndex = position;

        addNode -> data = pItem;
        addNode -> nextNode = NULL;
        addNode -> prevNode = NULL;
        
        freeNodeCount--; 
    }

    else{
        //new node to be added made
        addNode = &listNodes[nodeCount];
        // tracking the nodes.
        addNode -> usedNodeIndex = nodeCount;

        addNode -> data = pItem;
        addNode -> nextNode = NULL;
        addNode -> prevNode = NULL;
        
        nodeCount++;
    }
    
    
    if(pList -> listSize == 0){  //list empty, curr is NULL
    //printf("Ran");
        pList -> head = addNode;
        pList -> tail = addNode;
        pList -> curr = addNode;
        pList -> beforeHead = false;
        pList -> afterTail = false; //as they point curr is at addNode.
        pList -> listSize++;
        
        return 0;
    }

    if(pList -> beforeHead){
        //printf("Ran");
        pList -> head = addNode;
        pList -> tail = addNode;
        pList -> curr = addNode;
        pList -> beforeHead = false;
        pList -> afterTail = false; //as they point curr is at addNode.
        pList -> listSize++;
        return 0;

    }


    if(pList -> afterTail){  //curr beyond end, list full, curr is NULL
        pList->tail -> nextNode = addNode;
        addNode -> prevNode = pList -> tail;
        pList -> tail = addNode;
        pList -> curr = addNode;
        pList -> beforeHead = false;
        pList -> afterTail = false; //as they point curr is at addNode.
        pList -> listSize++;
        return 0;
    }


    if(pList->curr == pList-> head){
        if(pList->listSize == 1){
      
            pList->head->nextNode = addNode;
            addNode -> prevNode = pList->head;
            pList->curr = addNode;
            pList -> tail = addNode;
            pList -> beforeHead = false; 
            pList -> afterTail = false; 
            pList -> listSize++;
            return 0;
        }
    }
    if(pList->curr==pList->tail){
   
        pList->tail->nextNode = addNode;
        addNode->prevNode = pList->tail;
        pList->curr = addNode;
        pList->tail = addNode;
        pList -> beforeHead = false;
        pList -> afterTail = false; 
        pList -> listSize++;
        return 0;

    }

    
    pList->curr->nextNode->prevNode= addNode;
    addNode -> nextNode = pList->curr->nextNode;
    addNode->prevNode = pList->curr;
    pList->curr->nextNode = addNode;
    pList->curr = addNode;
    pList->beforeHead = false;
    pList -> afterTail = false;
    pList->listSize++;
    return 0;

}


// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem){

    if(nodeCount >= LIST_MAX_NUM_NODES && freeNodeCount == 0){  //both exhausted 
        return -1;                                              //no more nodes, FAILED OPN
    }


    Node * insertNode;    
    if(nodeCount >= LIST_MAX_NUM_NODES){
       
        int position = freePositions[freeNodeCount];
        insertNode = &listNodes[position];

        // // tracking the nodes.
        insertNode -> usedNodeIndex = position;

        insertNode -> data = pItem;
        insertNode -> nextNode = NULL;
        insertNode -> prevNode = NULL;
        freeNodeCount--; // new node to insert 
    }

    else{
        insertNode = &listNodes[nodeCount];

        // tracking the nodes.
        insertNode -> usedNodeIndex = nodeCount;
    

        insertNode -> data = pItem;
        insertNode -> nextNode = NULL;
        insertNode -> prevNode = NULL;
        nodeCount++; // new node to insert
    }

    if(pList -> listSize == 0){ // list empty
  
        pList -> head = insertNode;
        pList -> tail = insertNode;
        pList -> curr = insertNode;
        pList -> listSize++;
        pList -> beforeHead = false;
        pList -> afterTail = false;
        return 0;
    }


    if(pList -> beforeHead){ // list empty
 
        pList -> head = insertNode;
        pList -> tail = insertNode;
        pList -> curr = insertNode;
        pList -> listSize++;
        pList -> beforeHead = false;
        pList -> afterTail = false;
        return 0;
    }

    if(pList -> afterTail){
 
        pList -> tail -> nextNode = insertNode;
        insertNode -> prevNode = pList -> tail;
        pList -> tail = insertNode;
        pList -> curr = insertNode;
        pList -> afterTail = false;
        pList -> beforeHead = false;
        pList -> listSize++;
        return 0;
    }


   if(pList->curr == pList->head){
    
       pList->head->prevNode = insertNode;
       insertNode->nextNode = pList->head;
       pList->curr = insertNode;
       pList->head = insertNode;
       pList -> afterTail = false;
       pList -> beforeHead = false;
       pList -> listSize++;
        return 0;
      }


    pList->curr->prevNode->nextNode = insertNode;
    insertNode->prevNode = pList->curr->prevNode;
    insertNode->nextNode = pList->curr;
    pList->curr->prevNode = insertNode;  
    pList -> curr = insertNode;
    pList -> afterTail = false;
    pList -> beforeHead = false;
    pList -> listSize++;
    return 0;

}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){

    if(nodeCount >= LIST_MAX_NUM_NODES && freeNodeCount == 0){    // no more nodes, FAILED OPN, both exhausted
        return -1;
    }

    Node * appendNode;
    if(nodeCount >= LIST_MAX_NUM_NODES){
        int position = freePositions[freeNodeCount];
        appendNode = &listNodes[position];

        // // tracking the nodes.
        appendNode->usedNodeIndex = position;

        appendNode -> data = pItem;
        appendNode -> nextNode = NULL;
        appendNode -> prevNode = NULL;
        freeNodeCount--;
    }
    else{

        appendNode = &listNodes[nodeCount];

        // tracking the nodes.
        appendNode->usedNodeIndex = nodeCount;

        appendNode -> data = pItem;
        appendNode -> nextNode = NULL;
        appendNode -> prevNode = NULL;
        nodeCount++;
    }
    


    if(pList -> listSize == 0){ // empty list
        pList -> head = appendNode;
        pList -> tail = appendNode;
        pList -> curr = appendNode;
        pList -> listSize++;
        pList -> beforeHead = false;
        pList -> afterTail = false;
        return 0;
    }

    pList -> tail -> nextNode = appendNode;
    appendNode -> prevNode = pList->tail;
    pList -> tail = appendNode;
    pList -> curr = appendNode;
    pList -> beforeHead = false;
    pList -> afterTail = false;
    pList -> listSize++;
    return 0;
    
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){

    if( nodeCount >= LIST_MAX_NUM_NODES && freeNodeCount== 0){  //no more nodes, both exhausted FAILED OPN 
        return -1;
    }

    Node * prependNode;
    if(nodeCount >=LIST_MAX_NUM_NODES){

        int position = freePositions[freeNodeCount];
        prependNode = &listNodes[position];

        // // tracking the nodes.
        prependNode -> usedNodeIndex = position;

        prependNode -> data = pItem;
        prependNode -> nextNode = NULL;
        prependNode -> prevNode = NULL;
        freeNodeCount--;
    }
    else{
        prependNode = &listNodes[nodeCount];

        // tracking the nodes.
        prependNode -> usedNodeIndex = nodeCount;

        prependNode -> data = pItem;
        prependNode -> nextNode = NULL;
        prependNode -> prevNode = NULL;
        nodeCount++;
    }
    
    

    if(pList -> listSize == 0){ //basically list empty scenario
        pList -> head = prependNode;
        pList -> tail = prependNode;
        pList -> curr = prependNode;
        pList -> listSize++;
        pList ->beforeHead = false;
        pList -> afterTail = false;
        prependNode -> nextNode = NULL;
        prependNode -> prevNode = NULL;
        return 0;
    }

    pList -> head -> prevNode = prependNode; //normal working
    prependNode -> nextNode = pList->head;
    pList -> head = prependNode;
    pList -> listSize++;
    pList -> curr = prependNode;
    pList -> beforeHead = false;
    pList -> afterTail = false;
    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){

        if(pList-> beforeHead || pList-> afterTail || (pList->listSize == 0)){
            return NULL;
        }

        Node * removeCurr = pList->curr;

        if(pList-> listSize == 1){
            pList-> head = NULL;
            pList->tail = NULL;
            pList -> curr = NULL;
            pList->listSize--;

            freeNodeCount++;    //basically adding that node to the freeNodes so that it can be used later
            freePositions[freeNodeCount] = removeCurr->usedNodeIndex;
            return removeCurr->data;
        }

        if(pList->curr == pList->head){
            
            pList->head = pList->head->nextNode;
            pList->head->prevNode = NULL;

            pList->curr -> nextNode =NULL;
            pList->curr -> prevNode = NULL;
            
            pList->curr = pList->head;
            pList->listSize--;
            freeNodeCount++;
            freePositions[freeNodeCount] = removeCurr->usedNodeIndex;
            return removeCurr->data;
        }
        if(pList->curr == pList-> tail){
            pList->tail = pList->tail->prevNode;
            pList->tail-> nextNode = NULL;
            
            pList->curr-> prevNode = NULL;
            pList->curr->nextNode = NULL;

            pList->curr = pList->tail;
            pList->listSize--;
            freeNodeCount++;
            freePositions[freeNodeCount] = removeCurr->usedNodeIndex;
            return removeCurr->data;

        }
        
        pList->curr->prevNode->nextNode = pList->curr->nextNode;
        pList->curr->nextNode->prevNode = pList->curr->prevNode;
        pList->curr = pList->curr->nextNode;
        pList->listSize--;
        freeNodeCount++;
        freePositions[freeNodeCount] = removeCurr->usedNodeIndex;

        return removeCurr->data;

}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){

    if(pList1-> listSize == 0){
    
        pList1 -> head = pList2->head;
        pList1 -> tail = pList2-> tail;
        pList1 -> curr = pList2 -> curr;
        pList2 -> curr = NULL;
        pList1->listSize = pList2->listSize;

        freeHeadCount++;
        freePositions[LIST_MAX_NUM_NODES-1 + freeHeadCount] = pList2->usedHeadIndex;
     
    }
    //list2 empty
    else if(pList2-> listSize == 0){
        pList1->tail -> nextNode = NULL;
        freeHeadCount++;
        freePositions[LIST_MAX_NUM_NODES-1 + freeHeadCount] = pList2 -> usedHeadIndex;
    }

    else{
        pList1 -> tail -> nextNode = pList2 -> head;
        pList2 -> head -> prevNode = pList1 -> tail;
        pList2 -> curr = NULL;
        pList1 -> tail = pList2->tail;
        pList2 -> head = NULL;
        pList1->listSize = pList1->listSize + pList2->listSize;
        freeHeadCount++;
        freePositions[LIST_MAX_NUM_NODES-1 + freeHeadCount] = pList2->usedHeadIndex;
    }
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);

// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    
    int i,size;
    if(pList->beforeHead || pList->afterTail){
        pList->curr = pList->head;
        pList->beforeHead = false;
        pList->afterTail = false;
        printf("\ncurr is %d\n",*((int*)pList->curr->data));
        i = 0,size = pList->listSize;
        while(i<size){
            
            (*pItemFreeFn)(pList->curr->data);
            List_remove(pList);
            i++;
            if(i==size){
                int j;
                printf("\nNodes from index 0-99, heads from 100-109");
                for(j=0;j<LIST_MAX_NUM_NODES+LIST_MAX_NUM_HEADS;j++){
                    if(j==100){
                        printf("\nHeads from here");
                    }
                    printf("\n Array position: %d, usable index: %d ",j,freePositions[j]);
                    
                    }
            }

        }
        //printf("nodes done, head now");
        freeHeadCount++;
        freePositions[LIST_MAX_NUM_NODES-1 + freeHeadCount] = pList->usedHeadIndex;
    }

    else{
        pList->curr = pList->head;
        printf("\ncurr is %d\n",*((int*)pList->curr->data));
        i = 0,size = pList->listSize;
        while(i<size){
            
            (*pItemFreeFn)(pList->curr->data);
            List_remove(pList);
            i++;
            if(i==size){
                int j;
                printf("\nNodes from index 0-99, heads from 100-109");
                for(j=0;j<LIST_MAX_NUM_NODES+LIST_MAX_NUM_HEADS;j++){
                    if(j==100){
                        printf("\nHeads from here");
                    }
                    printf("\n Array position: %d, usable index: %d ",j,freePositions[j]);
                    
                    }
            }

        }

        //printf("nodes done, head now");
        freeHeadCount++;
        freePositions[LIST_MAX_NUM_NODES-1 + freeHeadCount] = pList->usedHeadIndex;
    }

    
    //printing the available indexes

   
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    
    if(pList-> listSize == 0){
        return NULL;
    }

    Node * tailKeeper = pList->tail;

    if(pList-> listSize == 1){
        pList-> head = NULL;
        pList->tail = NULL;
        pList -> curr = NULL;
        pList->listSize--;
        freeNodeCount++;
        freePositions[freeNodeCount] = tailKeeper->usedNodeIndex;

        return tailKeeper->data;
    }
    
    pList->curr = pList->tail;
    pList->tail = pList->tail->prevNode;
    pList->tail->nextNode = NULL;
    
    pList->curr->prevNode = NULL;
    pList->curr->nextNode = NULL;


    pList->curr = pList->tail;
    pList->listSize--;
    freeNodeCount++;
    freePositions[freeNodeCount] = tailKeeper->usedNodeIndex;
    return tailKeeper->data;

}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){

    Node * checker;
    int size = pList->listSize;

    if(pList->listSize == 0){ // list empty nothing to be done
        return NULL;
    }

    if(pList->beforeHead){
        checker = pList->head;
        while(size != 0){
            if((*pComparator)(checker->data,pComparisonArg)==1){
                pList->curr = checker;
                pList->beforeHead = false;
               
                return checker->data;
            }

            size--;
            checker = checker->nextNode;

        }
        //if here then no match found and end reached
        pList->curr = NULL;
        pList-> afterTail = true;
        return NULL;
    }
    if(pList->curr == pList->tail){
        if((*pComparator)(pList->curr->data,pComparisonArg)==1){
            return pList->curr->data;
        }
    }
    //some issue here
    checker = pList-> curr;

    
    while(checker -> nextNode != NULL){
        if((*pComparator)(checker->data,pComparisonArg)==1){
            pList->curr = checker;
            return checker->data;
        }
        checker = checker -> nextNode;
    }
   
    pList -> curr = NULL;
    pList -> afterTail = true;
    return NULL;

}


