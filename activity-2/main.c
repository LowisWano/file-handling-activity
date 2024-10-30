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
void savePedestrianCalculationData(Traffic list[], int count, char *scenarioName);
void insertTrafficDataMinHeap(Heap *H, char *scenarioName);
int calculatePedestrianTime(Heap *H);

// POT Operations
void initHeap(Heap *H);
void insert(Heap *H, Traffic t);
void preorder(Heap H, int node);
Traffic deleteMin(Heap *H);
void heapifyMin(Heap *H, int parent, int last);

int main(void){
  // clear any previous data
  remove("traffic_result.dat");

  Traffic scenario1[6];
  Traffic scenario2[6];
  Traffic scenario3[6];
  Traffic scenario4[6];

  scenario1[0] = createTraffic(3, "right", "main", 50);
  scenario1[1] = createTraffic(5, "left", "diversion", 40);
  scenario1[2] = createTraffic(1, "straight", "main", 60);
  scenario1[3] = createTraffic(6, "right", "diversion", 30);
  scenario1[4] = createTraffic(2, "left", "main", 20);
  scenario1[5] = createTraffic(7, "pedestrian", "main", 10);
  
  scenario2[0] = createTraffic(4, "straight", "diversion", 45);
  scenario2[1] = createTraffic(8, "pedestrian", "diversion", 15);
  scenario2[2] = createTraffic(2, "left", "main", 35);
  scenario2[3] = createTraffic(6, "right", "diversion", 25);
  scenario2[4] = createTraffic(1, "straight", "main", 55);
  scenario2[5] = createTraffic(7, "pedestrian", "main", 20);

  scenario3[0] = createTraffic(5, "left", "diversion", 50);
  scenario3[1] = createTraffic(3, "right", "main", 30);
  scenario3[2] = createTraffic(1, "straight", "main", 40);
  scenario3[3] = createTraffic(8, "pedestrian", "diversion", 10);
  scenario3[4] = createTraffic(4, "straight", "diversion", 20);
  scenario3[5] = createTraffic(7, "pedestrian", "main", 25);

  scenario4[0] = createTraffic(2, "left", "main", 60);
  scenario4[1] = createTraffic(6, "right", "diversion", 35);
  scenario4[2] = createTraffic(4, "straight", "diversion", 45);
  scenario4[3] = createTraffic(1, "straight", "main", 50);
  scenario4[4] = createTraffic(5, "left", "diversion", 30);
  scenario4[5] = createTraffic(7, "pedestrian", "main", 15);

  savePedestrianCalculationData(scenario1, 6,"scenario1");
  savePedestrianCalculationData(scenario2, 6,"scenario2");
  savePedestrianCalculationData(scenario3, 6,"scenario3");
  savePedestrianCalculationData(scenario4, 6,"scenario4");

  Heap Heap1;
  initHeap(&Heap1);
  insertTrafficDataMinHeap(&Heap1, "scenario1");
  printf("Scenario 1: It takes %d seconds for the pedestrian on the main road to cross\n \n", calculatePedestrianTime(&Heap1));
  
  Heap Heap2;
  initHeap(&Heap2);
  insertTrafficDataMinHeap(&Heap2, "scenario2");
  printf("Scenario 2: It takes %d seconds for the pedestrian on the main road to cross\n \n", calculatePedestrianTime(&Heap2));
  
  Heap Heap3;
  initHeap(&Heap3);
  insertTrafficDataMinHeap(&Heap3, "scenario3");
  printf("Scenario 3: It takes %d seconds for the pedestrian on the main road to cross\n \n", calculatePedestrianTime(&Heap3));
  
  Heap Heap4;
  initHeap(&Heap4);
  insertTrafficDataMinHeap(&Heap4, "scenario4");
  printf("Scenario 4: It takes %d seconds for the pedestrian on the main road to cross\n \n", calculatePedestrianTime(&Heap4));

  return 0;
}

void insertTrafficDataMinHeap(Heap *H, char *scenarioName){
  FILE *fp;
  fp = fopen("traffic_result.dat", "r");  // read text
  if(fp != NULL){
    char buffer[100];
    while(fgets(buffer, sizeof(buffer), fp)){
      Traffic t;
      char scenario[20];
      sscanf(buffer, "%[^,],%d,%[^,],%[^,],%d", scenario, &t.priority, t.direction, t.lane, &t.time);
      if(strcmp(scenario, scenarioName) == 0){
        insert(H, t);
      }
    }
    fclose(fp);
  } else {
    printf("Error opening file for reading.\n");
  }
}

void savePedestrianCalculationData(Traffic list[], int count, char *scenarioName){
  FILE *fp = fopen("traffic_result.dat", "a");
  if (fp != NULL) {
    for (int i = 0; i < count; i++) {
      fprintf(fp, "%s,%d,%s,%s,%d\n", scenarioName, list[i].priority, list[i].direction, list[i].lane, list[i].time);
    }
    fclose(fp);
  } else {
    printf("Error opening file for writing.\n");
  }
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
  while(H->Elem[0].priority != 7){
    printf("Pedestrian Time: %d\n", H->Elem[0].time);
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