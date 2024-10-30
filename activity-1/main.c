#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 10

typedef struct{
  int priority;
  char direction[20];
  char lane[20];
  int time;
}Traffic;

typedef struct{
  Traffic Elem[MAX];
  int last;
}Heap;

/*
  Priority Levels:
  1 - Vehicles Straight Main
  2 - Vehicles Left Main
  3 - Vehicles Right Main
  4 - Vehicles Straight Diversion
  5 - Vehicles Left Diversion
  6 - Vehicles Right Diversion
  7 - Pedestrian Main
  8 - Pedestrian Diversion
*/

// Helper Functions
Traffic createTraffic(int p, char *direction, char *lane, int time);
void displayTraffic(Traffic t);

// Problem Functions
void initTrafficData(Traffic list[], int count);
void insertTrafficDataMinHeap(Heap *H);
int calculatePedestrianTime(Heap *H);

// POT Operations
void initHeap(Heap *H);
void insert(Heap *H, Traffic t);
void preorder(Heap H, int node);
Traffic deleteMin(Heap *H);
void heapifyMin(Heap *H, int parent, int last);

int main(void){
  Traffic list[6];
  Heap H;
  initHeap(&H);

  list[0] = createTraffic(3, "right", "main", 50);
  list[1] = createTraffic(5, "left", "diversion", 40);
  list[2] = createTraffic(1, "straight", "main", 60);
  list[3] = createTraffic(6, "right", "diversion", 30);
  list[4] = createTraffic(2, "left", "main", 20);
  list[5] = createTraffic(7, "pedestrian", "main", 10);

  initTrafficData(list, 6);
  insertTrafficDataMinHeap(&H);
 
  printf("It takes %d seconds for the pedestrian on the main road to cross", calculatePedestrianTime(&H));
  return 0;
}

// delete root and shift elements
Traffic deleteMin(Heap *H){
  if(H->last >= 0){
    int last = H->last;
    Traffic temp = H->Elem[0];
    H->Elem[0] = H->Elem[H->last];
    H->Elem[H->last--] = temp;
    heapifyMin(H, 0, --last);
  }
}

void heapifyMin(Heap *H, int parent, int last){
  int LC = (parent*2)+1;
  int RC = (parent*2)+2;
  int smallest = parent;

  if(LC <= last && H->Elem[LC].priority < H->Elem[smallest].priority){
    smallest = LC;
  }
  if(RC <= last && H->Elem[RC].priority < H->Elem[smallest].priority){
    smallest = RC;
  }

  if(smallest != parent){
    Traffic temp = H->Elem[parent];
    H->Elem[parent] = H->Elem[smallest];
    H->Elem[smallest] = temp;
    heapifyMin(H, smallest, last);
  } 
}

int calculatePedestrianTime(Heap *H){
  int time = 0;
  while(H->last > 0){
    int j;
    for(j=0;j<=H->last; j++){
      displayTraffic(H->Elem[j]);
    }
    printf("\n");
    time += H->Elem[0].time;
    deleteMin(H);
  }
  return time;
}

void preorder(Heap H, int node){
  if(node <= H.last){
    displayTraffic(H.Elem[node]);
    preorder(H, (node*2)+1);
    preorder(H, (node*2)+2);
  }
}

// insert by priority, min is highest. Parent should always be lesser value than child
void insert(Heap *H, Traffic t){
  if(H->last < MAX){
    // insert
    H->Elem[++H->last] = t;

    // shift parent and child if POT is not observed
    Traffic temp;
    int child = H->last;
    int parent = (child-1)/2;
    while(H->Elem[child].priority < H->Elem[parent].priority){
      temp = H->Elem[child];
      H->Elem[child] = H->Elem[parent];
      H->Elem[parent] = temp;
    }
  }
}

void insertTrafficDataMinHeap(Heap *H){
  FILE *fp;
  fp = fopen("traffic.dat", "rb");  //read
  if(fp != NULL){
    Traffic t;
    while(fread(&t, sizeof(Traffic), 1, fp)){
      insert(H, t);
    }
	}
  fclose(fp);
}

void initTrafficData(Traffic list[], int count){
  FILE *fp;
  fp = fopen("traffic.dat", "wb");  //write

  if(fp != NULL){
  	fwrite(list, sizeof(Traffic), 6, fp);
	}
  fclose(fp);
}

void initHeap(Heap *H){
  int i;
  for(i-0;i<MAX;i++){
      H->Elem[i].priority = -1;
  }
  H->last = -1;
}

void displayTraffic(Traffic p){
  if(p.priority != -1){
    printf("prio: %-3d - %s %s - %d seconds\n", p.priority, p.direction, p.lane, p.time);
  }else{
    printf("Empty\n");
  }
	
}

Traffic createTraffic(int pri, char *direction, char *lane, int t){
  Traffic p;
  strcpy(p.direction, direction);
  strcpy(p.lane, lane);
  p.priority = pri;
  p.time = t;
  return p;
}