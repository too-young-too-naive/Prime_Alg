#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include<cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include<algorithm>
using namespace std;

#define int_max 1000000
#define maxnum 100000

float sum;

int nodeNum, edgeNum;
FILE *fp;
FILE *newfp;
string fileName, fileName2, newFileName, newFileName2 ;


int getNumberOfNodes()
{
	int verticeNum;
	fp=fopen(fileName.c_str(),"rt+");
	if(fp==NULL) cout<<"no file "<<endl;
    fscanf(fp,"%d\n",&verticeNum);
    fclose(fp);
    cout<<" the node number is "<<verticeNum<<endl;
    return verticeNum;

}


int getNumberOfEdges()
{
	char flag;
	int edgeNum,count;
	fp=fopen(fileName.c_str(),"rt+");
	while(!feof(fp)){

		flag=fgetc(fp);
		if(flag=='\n') count++ ;
	}

	edgeNum=count;
	fclose(fp);
	cout<<" the edge number is "<<edgeNum<<endl;
	return edgeNum;

}


typedef struct linkedList{
	float weg;
	int nodenum;
	struct linkedList *next;
}linkedList;

typedef struct Node{
	float weg;
	int nodenum;
}Node;

typedef struct Heap{
	Node allnode[maxnum+1];
	int size;
}Heap;
linkedList path;
Node nodeinHeap;
Heap heap;
int visited[maxnum+1];


void enterheap(int nodenum,float weg){

    int i, index;
    for(i = 1;i <= heap.size;i ++){
        if(heap.allnode[i].nodenum == nodenum){
            if(heap.allnode[i].weg > weg){
            	heap.allnode[i].weg = weg;
                for(index = i;heap.allnode[index / 2].weg > weg;index /= 2){
                	heap.allnode[index].nodenum = heap.allnode[index / 2].nodenum;
                	heap.allnode[index].weg = heap.allnode[index / 2].weg;
                }
                heap.allnode[index].nodenum = nodenum;
                heap.allnode[index].weg = weg;
            }
            return ;
        }
    }
    heap.size++;
    for(i = heap.size;heap.allnode[i / 2].weg > weg;i /= 2){
    	heap.allnode[i].nodenum = heap.allnode[i / 2].nodenum;
        heap.allnode[i].weg = heap.allnode[i / 2].weg;
    }
    heap.allnode[i].nodenum = nodenum;
    heap.allnode[i].weg = weg;

}

int extractmin(){

	return heap.allnode[1].nodenum;//0 or 1?
}

int sortHeap()
{


	int i, child;
	heap.allnode[1].nodenum = heap.allnode[heap.size].nodenum;
	heap.allnode[1].weg = heap.allnode[heap.size].weg;//size-1?
	heap.size--;
	for(int i =1;i*2<=heap.size;i=child)
	{
		child = 2 * i;
		if(child != heap.size && heap.allnode[child + 1].weg < heap.allnode[child].weg){
			child++;
		}
		if(heap.allnode[child].weg < heap.allnode[i].weg){
			float wegstore = heap.allnode[i].weg;
			int nodestore = heap.allnode[i].nodenum;
			heap.allnode[i].nodenum = heap.allnode[child].nodenum;
		    heap.allnode[i].weg = heap.allnode[child].weg;
		    heap.allnode[child].nodenum = nodestore;
		    heap.allnode[child].weg = wegstore;
		}
		else{
			break;
		}
	}

	return 0;
}



void buildLinkedList(linkedList *edgeRelation,int beginNode, int endNode,float weg){
	linkedList *former = &edgeRelation[beginNode];
	linkedList *nextNode = (linkedList*)malloc(sizeof(linkedList));
	nextNode->nodenum = endNode;
	nextNode->weg = weg;
	nextNode->next = NULL;
	while(former->next&&former->nodenum!=endNode)
	{
		former = former->next;
	}
	if(former->nodenum ==endNode)
	{
		if(former->weg>weg)
		{
			former->weg = weg;
		}
	}
	else
	{
		former->next = nextNode;
	}
}

int primalg(linkedList *edgeRelation,int wholenode){
	heap.size = 0;
	heap.allnode[0].weg = -1;
	edgeRelation[1].weg = 0;
	int nodenum;
	linkedList *conedge;
	enterheap(1,0);
	int updateNode = 0;
	float updateWeg;
//	int count=0;
	while(heap.size!=0)
	{
		nodenum = extractmin();
		sortHeap();
		visited[nodenum] = 1;
		conedge = edgeRelation[nodenum].next;
		while(conedge)
		{
			if(visited[conedge->nodenum]==0)
			{//cout<<"enter no visit"<<endl;
				if(conedge->weg<edgeRelation[conedge->nodenum].weg)
				{

					edgeRelation[conedge->nodenum].weg = conedge->weg;
					edgeRelation[conedge->nodenum].nodenum = nodenum;
					updateNode = conedge->nodenum;
					updateWeg = edgeRelation[conedge->nodenum].weg;
				}
				enterheap(conedge->nodenum, edgeRelation[conedge->nodenum].weg);

			}
			conedge = conedge->next;
		}

	}
	return 0;
}


int main() {
	cout<<"====================================="<<endl;
	for (int j = 0; j <= 1; j++) {

		if (j == 0) {
			fileName = "sparse_100000.txt";
			newFileName = "sparseMSF.txt";
			cout<<"This test is for sparse graph."<<endl;

		}

		if (j == 1) {
			fileName = "dense_100000.txt";
			newFileName = "denseMSF.txt";
			cout<<"This test is for dense graph."<<endl;

		}

		nodeNum = getNumberOfNodes();
		edgeNum = getNumberOfEdges();
		fp = fopen(fileName.c_str(), "rt+");

		//create the output file
		FILE *newfp = fopen(newFileName.c_str(), "wt+");

		fscanf(fp, "%d\n", &nodeNum);
		linkedList *linklist = (linkedList*) malloc(
				sizeof(linkedList) * (nodeNum + 1));
		for (int i = 1; i <= nodeNum; i++) {
			linklist[i].nodenum = i;
			linklist[i].weg = int_max;
			linklist[i].next = NULL;
			visited[i] = 0;
		}


		float weight;
		int a, b;
		for (int i = 1; i <= nodeNum; i++) {
			fscanf(fp, "%d%d%f\n", &a, &b, &weight);
			buildLinkedList(linklist, a, b, weight);
			buildLinkedList(linklist, b, a, weight);
		}
		fclose(fp);

		primalg(linklist, nodeNum);
		double sum2 = 0;
		for (int i = 1; i <= nodeNum; i++) {
			if (linklist[i].weg == int_max) {
				continue;
			} else {
				sum2 += linklist[i].weg;

//output the result to file

				fprintf(newfp, "%d", linklist[i].nodenum);
				fputs(" ", newfp);
				fprintf(newfp, "%d", linklist[i].next->nodenum);
				fputs(" ", newfp);
				fprintf(newfp, "%f\n", linklist[i].next->weg);

			}
		}
		cout << "The sum weight is " << sum2 << endl;
		cout<<"====================================="<<endl;
	}

	return 0;

}




