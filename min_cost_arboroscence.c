/*#######################################

Name        : Ish Rajesh Shelley
Entry No.   : 2018EEB1153
Course      : CS201
Assignment  : Lab04 - Arboresence

I have done this assignment on my own .
I have marked the time stamp using gettimeofday()

The sample test cases given by sir work well .
Where as some of the queries do not work well in cases where 
the node in unreachable or double cycle is detected in one iteration . 


Tested test cases are given at the end .


###########################################*/




#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>


typedef struct Node{
    int data;
    int weight;
    int visible;
    int visited;
    struct Node* next;
}node;

int input;
int* stack;
int rear ;
int front;
int * hash;
int count_hash;
int cycle;
int stack_front[200];
int stack_back[200];
int front_vertex =-1;
int back_vertex =-1;
//node** smaller;
node** arborescence;
int squeze[200];
int squeze_index=-1;
int squeze_data;

int min(int a,int b){
    return a<b ? a : b ;
}

int insertend2(node* matrix[],int j,int k ,int weight , int visible){
    node* detect;
    detect = matrix[j];

    while(detect){
        if(detect->data == k){
            detect->weight= min(detect->weight , weight);
            return 0;
        }
        detect=detect->next;
    }


    node* temp= (node*)malloc(sizeof(node));
    temp->data=k;
    temp->next==NULL;
    temp->weight =weight;
    temp->visible=visible;
    node* traverse;
    if(matrix[j]==NULL){
        matrix[j]=temp;
    }
    else if(matrix[j]->next==NULL){
        matrix[j]->next = temp;
    }
    else{
        traverse = matrix[j];
        while(traverse->next){
            traverse=traverse->next;
        }
        traverse->next=temp;
    }
    return 0;
}

int printadj2(node* matrix[] ,int num){
    node* temp;
    for(int i=0;i<num ;i++){
        temp=matrix[i];
        while(temp){
            printf("(- %d -) %d ",temp->weight,temp->data + 1);
            if(temp->visible== 1){
                printf("Z");
            }
            temp=temp->next;
        }
        printf("\n");
    }
    return 0;
}

int reduce_graph(node** matrix ,int num){
    node* temp;
    int min;
    for(int i=0;i< num ;i++){
        temp = matrix[i];
        if(temp != NULL){ min =temp->weight;}
        while(temp!=NULL){
            if(temp->weight < min){
                min = temp->weight;
            }
            temp=temp->next;
        }
        temp = matrix[i];
        while(temp!=NULL){
            temp->weight = temp->weight - min;
            temp=temp->next;
        }
    }
    return 0;
}

int copy(node** matrix1 , node** matrix2 ,int num){
    node* temp;
    for(int i=0;i<num;i++){
        matrix2[i]=NULL;
    }
    for(int i=0;i<num;i++){
        temp= matrix1[i];
        while(temp != NULL){
            insertend2( matrix2 , i , temp->data , temp->weight , temp->visible);
            temp=temp->next;
        }
    }
    return 0;
}

int reverse(node** matrix1 ,node** matrix2 ,int num){
    for(int i=0;i<num;i++){
        matrix2[i]=NULL;
    }
    node* temp;
    for(int i=0;i<num;i++){
        temp=matrix1[i];
        while(temp){
            insertend2(matrix2 ,temp->data,i,temp->weight , temp->visible);
            temp=temp->next;
        }
    }
}

int dfs_visit(node* matrix[] ,int data ,char color[]){
    hash[data]=1;
    color[data]='g';
    //time++;
    node* temp = matrix[data];
    while(temp){
        if(temp->visible == 1 && color[temp->data]=='w'){
        dfs_visit(matrix , temp->data ,color);   
        }
        temp=temp->next;
    }
    color[data]='b';
    if(input==0){
        front++;   
        stack[front]=data;
    }
    return 0;
}

int dfs_visit2(node* matrix[] ,int data ,char color[]){
    hash[data]=1;
    color[data]='g';
    //time++;
    node* temp = matrix[data];
    while(temp){
        if(color[temp->data]=='w'){
        dfs_visit(matrix , temp->data ,color);   
        }
        temp=temp->next;
    }
    color[data]='b';
    if(input==0){
        front++;   
        stack[front]=data;
    }
    return 0;
}

int update(node* matrix[], node* arborescence[],int num){
    node* temp;
    node* temp1;
    for(int i=0 ;i<num ;i++){
        temp=arborescence[i];
        while(temp){
            if( temp->visible==1){
                temp1=matrix[i];
                while(temp1)
                {
                    if(temp->data==temp1->data){
                        temp1->visible=1;
                    }
                    else{
                        temp1->visible=0;
                    }
                    temp1=temp1->next;
                }   
            }
            temp=temp->next;
        }
    }
    return 0;
}

int induce_visible(node* matrix[] ,int num){
    node* temp;
    int min_valid = -1;
    for(int i=0;i<num ;i++){
        temp=matrix[i];
        min_valid= num+1;
        while(temp){
            if(temp->weight==0 && temp->data < min_valid ){
                min_valid = temp->data;                
            }
            temp=temp->next;
        }
        temp=matrix[i];
        while(temp){
            if(temp->data == min_valid){
                temp->visible=1;
            }
            temp=temp->next;
        }
    }
}

int* parentlist;
int* reachlist;

int min_cost(node* matrix[], node* arborescence[],int num){
    node*temp ;
    node* temp1;
    int cost=0;
    for(int i=0 ;i<num ;i++){
        temp=arborescence[i];
        temp1=matrix[i];
        while(temp){
            if( temp->visible==1 && reachlist[temp->data]>0){
                cost+= temp1->weight;
            }
            temp1=temp1->next;
            temp=temp->next;
        }
    }
    return cost;
}



int contract(node* matrix[] , node* smaller[] , int num ,int* how){
   // printf("****Enter CONTRACT*****\n");
    for(int i=0; i<num;i++){
        smaller[i]=NULL;
    }
    //node** rev_smaller =(node**)malloc(sizeof(node*)*num);
    node* temp;
    int min_index=-1;
    for(int z=0;z<num;z++){
        if(hash[z] == 1) {
            min_index=z;
            break;
        }
    }
   // printf("source node index = %d\n",min_index+1);
    for(int i=0;i<num;i++){
        temp=matrix[i];
        while(temp){
            if(hash[i]==0){
                if(hash[temp->data]==0){ ///=======#### Here check do we have to put temp->visible 1 or 0 or leave as it is
                    insertend2(smaller ,i , temp->data ,temp->weight , 0);
                }
                else if(hash[temp->data]==1){ ///=======#### Here check do we have to put temp->visible 1 or 0 or leave as it is
                    insertend2(smaller, i , min_index , temp->weight , 0);
                }
            }
            else if(hash[i]==1){
                if(hash[temp->data]==0){ ///=======#### Here check do we have to put temp->visible 1 or 0 or leave as it is
                    insertend2(smaller , min_index ,temp->data ,temp->weight , 0);
                }
                else if(hash[temp->data]==1){
                    1;//do nothing
                }
            }
            temp=temp->next;
        }
    }
    //reverse( smaller ,rev_smaller ,num );
    //copy( rev_smaller , smaller ,num ) ;
    (*how)= min_index;
    //squeze_index++;
    //squeze[squeze_index] = min_index ;
    //printf("****Leave CONTRACT*****\n");
        return 0;
}

int dfs(node* matrix[] , int num ,int source , node** smaller ,int* how){
    //printf("****Enter DFS*****\n");
    //node** intermediate = matrix;             //parent = (int*)malloc(sizeof(int)*num);      //dtime =(int*)malloc(sizeof(int)*num);
    //ftime =(int*)malloc(sizeof(int)*num);     
    char* color;
    int num_con=0;
    color =(char*)malloc(sizeof(char)*num);
    for(int k=0;k<num;k++){
        //parent[k]=-1;     //dtime[k]=1000000;
        color[k]='w';
    }
    //time=0;
    
    if(input == 0){ //printf("input=0\n");
        //dfs_visit(matrix , source);
        for(int k=0;k<num ;k++){
            if(color[k]=='w'){
                dfs_visit2(matrix, k ,color);
            }
        }
    }
    else if(input == 1){  //printf("input=1\n");
        //squeze = 0;
        num_con=0;
        for(int k=front;k>=0;k--){
            if(color[stack[k]]=='w'){
                for(int z=0;z<num;z++){ hash[z]=0; }
                count_hash=0;
                dfs_visit(matrix ,stack[k] , color);
                
                //printf("Print Hash\n");
                for(int z=0;z<num;z++){ if(hash[z] == 1)  {     count_hash++;} } //printf("#\n");  // counting no. of hashed verticies and print
                
                if(count_hash>=2){ ////then contract the graphl     //squeze++;
                    cycle = 1;
                    num_con++;
                    contract( matrix ,smaller ,num ,how);    //copy(smaller ,intermediate ,num);
                   // printf("Smaller\n");
                   // printadj2(smaller , num);
                    
                    return 0;
                }
            }
        }
    }
    //printf("****Leave DFS*****\n");
    return num_con;
}
int start;
int finish;
int expand(node* matrix[] , node* transition[] ,node* smaller[] ,node* real_smaller[] ,int num ,int* how){
    node* temp;

    int min_weight;
    int min_valid;
    int value = -14;   
    temp=smaller[(*how)];
    //temp=smaller[squeze[squeze_index]];
    while(temp){
        if(temp->visible ==1){
            min_valid =temp->data;
            min_weight=temp->weight;
            //printf("Check_count");
        }
        temp=temp->next;
    }
    temp=matrix[min_valid];
    while(temp){
        if( temp->weight == min_weight ){
            value = temp->data;
        }
        temp=temp->next;
    }
    start =min_valid;
    finish=value;
    //printf(" alll threee min_valid =%d    min_weight =%d     visible = %d\n", min_valid ,min_weight ,value);
    //squeze_index--;
    //printf("$$$$$$$$$$$value = %d \n$$$$$$$$$$$$$$$$$$min_weight= %d \n",value ,min_weight);
    //printadj2(transition ,num);
     //printf("#\n");
    temp = transition[value];
    while(temp){
        if(temp->data == min_valid){
            temp->visible =1;
        }
        else{
            temp->visible =0;
        }
        temp=temp->next;
    }
    //printadj2(transition ,num);
    //printf("#\n");
}

int precious(node* transition[],int num){
    node* temp;
    temp=transition[finish];
    while(temp){
        if(temp->data == start){
            temp->visible==1;
        }
        else{
            temp->visible=0;
        }
        temp=temp->next;
    }
}


int edmond(node* transition[], int num ,int source){
   // printf("****Enter EDMOND*****\n");
    cycle=0;
    int *how=(int*)malloc(sizeof(int)*1);
    int count_con;
    node** replica = (node**)malloc(sizeof(node*)*num);
    node ** smaller = (node**)malloc(sizeof(node*)*num);
    node** new_matrix = (node**)malloc(sizeof(node*)*num);
    //node** real_smaller = (node**)malloc(sizeof(node*)*num);
    node** matrix = (node**)malloc(sizeof(node*)*num);
    //node** rep_transition = (node**)malloc(sizeof(node*)*num);
    stack = (int*)malloc(sizeof(int)*num);
    hash = (int*)malloc(sizeof(int)*num);
    input=0;
    rear = front = -1;
    //printadj2(matrix ,num);
    
    reverse(transition ,matrix ,num);
    copy(matrix ,new_matrix ,num);
    reverse(matrix ,transition , num);

    reduce_graph(transition, num ) ;
    induce_visible(transition , num);             //for(int k=0 ;k<num ;k++) { matrix[k] =NULL;}
    reverse(transition ,matrix , num);
    //printf("We wnant updated smaller\n");
    //printadj2(matrix ,num);
    
    //================================
    dfs(matrix , num ,source ,smaller ,how);
    //================================
    
    //for(int i=front ;i>=0 ;i--){ printf("%d" , stack[i]); }     printf("\n");
    input=1;
    //================================
    count_con = dfs(transition , num ,source , smaller ,how);
    //================================

    int check;
    node*temp ;
    if(cycle == 0){ //printf("Cycle=0\n");}
    }
    else{ //printf("Cycle=1\n");}
    }
    if(cycle==1){
        
       
        
        check =edmond(smaller ,num ,source);
        if(check==1) precious(transition ,num);
        //update( transition, smaller ,num);
        expand(matrix ,transition ,smaller ,smaller ,num ,how);
    }
    reverse(new_matrix ,replica ,num);
    update(replica ,transition ,num);
    copy(replica , transition ,num);

    //printf("****Leave EDMOND*****\n");
    return cycle ;
    
}



int travel(node* matrix[],int source){
    node* temp=matrix[source];
    
    while(temp){
        if(temp->visible == 1){
            parentlist[temp->data]= source+1;
            reachlist[temp->data]= reachlist[source]+temp->weight;
            travel(matrix , temp->data);
        }
        temp=temp->next;
    }
}

int calculate(node* matrix[] ,int num ,int source){
    parentlist =(int*)malloc(sizeof(int)*num);
    reachlist =(int*)malloc(sizeof(int)*num);
    
    for(int i=0; i<num ;i++){
        parentlist[i]=-1;
        reachlist[i]=-1;
    }
    parentlist[source] =0;
    reachlist[source] =0;
    travel(matrix ,source); 
    
}
struct timeval now;





int clear_visit(node* matrix[] ,int source){
    node* temp ;
    temp=matrix[source];
    while(temp){
            if(temp->visible == 0){
                temp->visible=1;
                clear_visit(matrix ,temp->data);
            }
            temp=temp->next;
        }
}

int clear(node* matrix[] ,int source ,int num){
    clear_visit(matrix ,source);
    node* t2;
    node* t1;
    for(int i=0 ;i<num ;i++){
        t1 = t2 = matrix[i];
        while(t2 && t2->next){
            if(t2->visible == 0){
                
                t2=t2->next;
                t1->next=t2;
            }
            t1=t2;
            t2=t2->next;
        }
    }

    for(int i=0 ;i<num ;i++){
        t1 =matrix[i];
        while(t1){
            t1->visible=0;
            t1=t1->next;
        }
    }
}


int main(){
    gettimeofday(&now ,NULL);
    printf("time = %ld.%06ld\n", now.tv_sec, now.tv_usec);

    node** transpose;
    node** replica;
    node** matrix;
    int num ,source ,check ,edges;
    int test_case,u,v;
    int cost=0;

    scanf("%d",&test_case);
    for(int i=0 ;i<test_case ; i++){
        scanf("%d",&num);
        scanf("%d",&source);
        source = source-1;
        matrix = (node**)malloc(sizeof(node*)*num);
        transpose = (node**)malloc(sizeof(node*)*num);
        replica = (node**)malloc(sizeof(node*)*num);
       
        for(int k=0;k<num;k++){
            matrix[k]=NULL;
            transpose[k]=NULL;
        }
        scanf("%d",&edges);
        for(int j=0; j<edges ;j++){
            scanf("%d",&u);
            scanf("%d",&v); 
            scanf("%d",&check);

            insertend2( matrix , u-1 , v-1 , check , 0);
            //insertend2( transpose , v-1 , u-1 , check , 0);
        }
        clear(matrix ,source , num );
        reverse(matrix ,transpose ,num);

        edmond(transpose ,num ,source );
        reverse(matrix,replica,num);
        update(replica ,transpose ,num);
        reverse(replica ,matrix ,num); 
                
        calculate(matrix ,num ,source);
        cost=min_cost(matrix , matrix ,num);
        printf("%d ",cost); 
        for(int k=0;k<num;k++){ printf("%d ",reachlist[k]);   }     printf("# ");
        for(int k=0;k<num;k++){   printf("%d ",parentlist[k]);   }   printf("\n");        
    gettimeofday(&now ,NULL);
    
    }
    printf("time = %ld.%06ld\n", now.tv_sec, now.tv_usec);
}


/*



Test Cases Working are as follows
4
6 1
9
1 2 10
1 3 2
1 4 10
2 3 1
3 4 4
4 5 2
2 6 8
5 2 2
4 6 4
14 0 10 2 6 8 10 # 0 5 1 3 4 4
8 1
13
1 2 5
1 5 11
5 2 6
6 5 10
6 2 2
2 3 3
2 7 13
7 6 7
3 7 9
3 4 12
4 8 1
8 3 4
7 8 8
47 0 5 8 20 34 24 17 21 # 0 1 2 3 6 7 3 4 
10 10
3
1 2 3
2 3 1
3 5 1
0 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 # -1 -1 -1 -1 -1 -1 -1 -1 -1 0
10 10
5
1 2 3
2 3 3
3 1 2
4 1 3
5 2 6
0 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 # -1 -1 -1 -1 -1 -1 -1 -1 -1 0      




 */

