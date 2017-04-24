#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void text_color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

typedef struct node{
    int start_addr;
    int mem_size;
    struct node *next;
}*mem_node, *link_list;

//function declaration
link_list create_list();
//void insert_node(mem_node head, mem_node new_node);

//global variable definition
link_list available_mem, occupied_mem;


link_list create_list(){
    mem_node head = (mem_node)malloc(sizeof(struct node));
    if(head == NULL){
        text_color(0x0c);
        printf("Failed to allocate memory.\n");
        text_color(0x07);
    }
    else{
        head->next = NULL;
    }
    return head;
};

void insert_node(mem_node head, int start_addr, int mem_size){
    mem_node tmp = head;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }

    mem_node new_node = (mem_node)malloc(sizeof(struct node));
    new_node->mem_size = mem_size;
    new_node->start_addr = start_addr;

    tmp->next = new_node;
    new_node->next = NULL;
}

void set_node(mem_node node, int start_addr, int mem_size){
    node->start_addr = start_addr;
    node->mem_size = mem_size;
}

void create_partitions(){
    int start_addr, mem_size;
    printf("Please type in the info:\n");
    printf("Partition Start Address:");
    scanf("%d", &start_addr);
    printf("         Partition size:");
    scanf("%d", &mem_size);
    printf("\n");
    insert_node(available_mem, start_addr, mem_size);

}

void print_status(){
    mem_node tmp = available_mem;
    printf("\n");
    while(tmp->next != NULL){
        tmp = tmp->next;
        printf("%d\n", tmp->mem_size);
    }

}

int get_length(link_list head){
    int length = 0;
    mem_node tmp = available_mem;
    while(tmp->next != NULL){
        tmp = tmp->next;
        length++;
    }
    return length;
}

void print_menu(){
    text_color(0x0b);
    printf("------ Memory Management Simulation Program ------\n");
    printf("---    1. New available partition(s)           ---\n");
    printf("---    2. Allocate to process                  ---\n");
    printf("---    3. Free memory                          ---\n");
    printf("---    q. Exit                                 ---\n");
    printf("------ ------------------------------------ ------\n");
    text_color(0x07);
    printf("Please choose an option: ");
}

int main()
{
    //text_color(0x0c);
    //mem_node tmp = (mem_node)malloc(sizeof(struct node));
    available_mem = create_list();
    char option;
    while(1){
        print_menu();
        fflush(stdin);
        scanf("%c", &option);

        if(option == '1'){
            int num;
            printf("Please type in the number of partitions: ");
            scanf("%d", &num);
            printf("\n");
            int i;
            for(i = 0; i < num; i++)
                create_partitions();
        }
        else if(option == '2'){

        }
        else if(option == '3'){

        }
        else if(option == 'q'){
            break;
        }

        print_status();
    }





    //printf("Hello world!\n");
    return 0;
}
