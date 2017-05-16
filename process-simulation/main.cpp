#include <iostream>
#include <windows.h>
#include <stdio.h>

#define STATUS_RUNNING 0
#define STATUS_READY   1
#define STATUS_BLOCKED 2
#define STATUS_FINISHED 3

using namespace std;
void text_color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


/**** data structure definition ****/

/* class pcb defini*/
class c_pcb{
public:
    string name;        // name of process
    int pid;            // id for process
    int status;         // status of process
    int priority;       // priority of the process
    int time_reach;    // when the process is create
    int time_exp;       // time slice(s) expected for the process
    int time_left;


    c_pcb(){}
    c_pcb(string name, int pid, int status, int priority, int time_exp, int time_reach){
        this->name = name;
        this->pid = pid;
        this->status = status;
        this->time_exp = time_exp;
        this->priority = priority;
        this->time_reach = time_reach;
        this->time_left = time_exp;
    }
    /*
    string get_name(){return name;    }
    int get_pid(){return pid;    }
    int get_status(){return status;    }
    int get_time_exp(){return time_exp;    }
    int get_priority(){return priority;    }

    void set_pid(int pid){this->pid = pid;    }
    void set_name(string name){this->name = name;    }
    void set_status(int status){this->status = status;    }
    void set_priority(int priority){this->priority = priority;    }
    void set_time_exp(int time_exp){this->time_exp = time_exp;    }
    */

};

typedef class pcb_node{
    public:
    c_pcb pcb_data;
    pcb_node *next = NULL;

    pcb_node(){}/*
    pcb_node(c_pcb pcb){
        this.pcb_data = pcb;
    }
*/
}*link_list, *p_node;

/*cpu class*/
class cpu{
private:
    int time_slice;     //length of tie slice
public:
    cpu(){}
    void create_process();
    void block_process();
    bool delete_process(int pid);
    void wakeup_process();
    void run_process();
    void print_status();
    void sort_priority(link_list process_list, int length);
};


/***** global variables definition *****/
//p_node pcb_ant = (p_node)malloc(sizeof(pcb_node));
cpu ant_cpu;
p_node create_list();
link_list ready_list = create_list();
link_list blocked_list = create_list();
link_list finished_list = create_list();
p_node running = ready_list->next;
int cpu_time = 0;

/**** function definition ****/
int get_length(link_list head);
bool insert_node(p_node head, int position, p_node pcb_node);
c_pcb get_node(link_list head, int position, int flag);
void set_node(link_list head, int position, c_pcb data);
void delete_node(link_list head);
bool is_duplicated(int pid);

/**** function for cpu class ****/
void cpu::create_process(){

    string name; int pid; int time_exp; int priority;
    int status = STATUS_READY;
    int time_reach = cpu_time;
    while(1){
        text_color(0x07);
        cout << "You are creating a new process, please type in the info:" << endl;
        cout << "        Process name: ";
        cin >> name;
        cout << "                 Pid: ";
        cin >> pid;
        cout << "Expected time slices: ";
        cin >> time_exp;
        cout << "            Priority: ";
        cin >> priority;
        //cout << "length1:" << get_length(ready_list) << endl;
        if(!is_duplicated(pid)){

            //set_pcb(pcb_ant, name, pid, status, priority, time_exp);
            pcb_node* pcb_ant = new pcb_node();
            pcb_ant->pcb_data = c_pcb(name, pid, status, priority, time_exp, time_reach);
            //cout << "length2:" << get_length(ready_list) << endl;
            insert_node(ready_list, get_length(ready_list) + 1, pcb_ant);
            text_color(0x0a);
            cout << "Process " << pcb_ant->pcb_data.name <<" created successfully!\n" << endl;
            text_color(0x07);
            break;
        }
        //cout << "length3:" << get_length(ready_list) << endl;
    }



}

bool cpu::delete_process(int pid){
    p_node temp;
    temp = running;
    if(temp != NULL){
        if(temp->pcb_data.pid == pid){
            running = NULL;
            text_color(0x0a);
            cout << "Process " << temp->pcb_data.pid <<" deleted successfully!\n" << endl;
            text_color(0x07);
            return true;
        }
    }
    temp = ready_list;
    while(temp->next != NULL){

        if(temp->next->pcb_data.pid == pid){
            text_color(0x0a);
            cout << "Process " << temp->next->pcb_data.pid <<" deleted successfully!\n" << endl;
            text_color(0x07);
            temp->next = temp->next->next;
            return true;
        }
        temp = temp->next;
    }

    temp = blocked_list;
    while(temp->next != NULL){

        if(temp->next->pcb_data.pid == pid){
            text_color(0x0a);
            cout << "Process " << temp->next->pcb_data.pid <<" deleted successfully!\n" << endl;
            text_color(0x07);
            temp->next = temp->next->next;
            return true;
        }
        temp = temp->next;
    }
    text_color(0x0c);
    cout << "Process not found." << endl << endl;
    text_color(0x07);
}

void cpu::block_process(){

    running->pcb_data.status = STATUS_BLOCKED;
    insert_node(blocked_list, get_length(blocked_list), running);
    running = NULL;
    //delete_node(ready_list);
}

void cpu::wakeup_process(){
    blocked_list->next->pcb_data.status = STATUS_READY;
    insert_node(ready_list, get_length(ready_list), blocked_list->next);
    ant_cpu.sort_priority(ready_list, get_length(ready_list));
    delete_node(blocked_list);
}

void cpu::run_process(){
        delete_node(ready_list);

        if(running->pcb_data.time_left > 0){
            running->pcb_data.priority --;
            running->pcb_data.time_left --;

            if(running->pcb_data.time_left == 0){
                running->pcb_data.status = STATUS_FINISHED;
                insert_node(finished_list, get_length(finished_list) + 1, running);
                running = NULL;
            }
        }
        ant_cpu.print_status();

        //cout << running->pcb_data.name << " is running." << endl;
}

void cpu::print_status(){
    text_color(0x0e);
    cout << "Present CPU time: " << cpu_time << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Running Process: " << endl;
    if(running == NULL){
        cout << "No running process." << endl << endl;
    }else{
        printf("PID\tNAME\tPRIORITY\tSTATUS\t\tTIME-LEFT\tTIME-REACH\n");
        //text_color(0x0a);
        printf("%-8d", running->pcb_data.pid);
        cout << running->pcb_data.name;
        printf("\t%-8d\t%-8d\t%-16d%-4d\n", running->pcb_data.priority, running->pcb_data.status, running->pcb_data.time_left, running->pcb_data.time_reach);
        cout << endl;
    }

    cout << "Ready List: " << endl;
    printf("PID\tNAME\tPRIORITY\tSTATUS\t\tTIME-LEFT\tTIME-REACH\n");
    //text_color(0x0a);
    for(int i = 1; i <= get_length(ready_list); i++)
        get_node(ready_list, i, 1);
    cout << endl;

    cout << "Blocked List: " << endl;
    printf("PID\tNAME\tPRIORITY\tSTATUS\t\tTIME-LEFT\tTIME-REACH\n");
    //text_color(0x0a);
    for(int i = 1; i <= get_length(blocked_list); i++)
        get_node(blocked_list, i, 1);
    cout << endl;

    cout << "Finished List: " << endl;
    printf("PID\tNAME\tPRIORITY\tSTATUS\t\tTIME-LEFT\tTIME-REACH\n");
    //text_color(0x0a);
    for(int i = 1; i <= get_length(finished_list); i++)
        get_node(finished_list, i, 1);
    cout << "------------------------------------------------------------" << endl << endl;
}

void cpu::sort_priority(link_list process_list, int length){
    c_pcb tmp;
    for(int i = 1; i <= length - 1; i++){
        for(int j = 1; j <= length - 1; j++){
            if(get_node(process_list, j, 0).priority < get_node(process_list, j + 1, 0).priority){
                tmp = get_node(process_list, j, 0);
                set_node(process_list, j, get_node(process_list, j + 1, 0));
                set_node(process_list, j + 1, tmp);
            }else if(get_node(process_list, j, 0).priority == get_node(process_list, j + 1, 0).priority){
                if(get_node(process_list, j, 0).time_reach > get_node(process_list, j + 1, 0).time_reach){
                    tmp = get_node(process_list, j, 0);
                    set_node(process_list, j, get_node(process_list, j + 1, 0));
                    set_node(process_list, j + 1, tmp);
                }
            }
        }
    }
}

/**** function for pcb structure ****/
link_list create_list(){
    link_list head = (p_node)malloc(sizeof(pcb_node));
    if(head == NULL){
        text_color(0x0c);
        cout << "Failed to allocate memory." << endl;
        exit(-1);
    }
    else{
        head->next = NULL;
    }
    return head;
}

/** \brief say we have a list head(0)->1->2->3, and we want to insert a node called x at position 2
 *         then we should put it after node 1(position - 1), which should like head(0)->1->x->2->3
 *         in this case, max position value is 3(length of the list, except head node, included)
 * \param position: where to put new node. starts from 1
 * \param
 * \return
 *
 */
bool insert_node(p_node head, int position, p_node pcb_node_){
    int i = 0;
    p_node temp_f = head, temp_b;
    while(temp_f->next != NULL && i < position){
        temp_f = temp_f->next;

    }
    //cout << "temp_f:" << temp_f->pcb_data.name << endl;
    temp_b = temp_f->next;
    if(temp_f == NULL){
        return false;
    }
    //p_node node_process = (p_node)malloc(sizeof(pcb_node));
    //p_node node_process = new pcb_node()
/*
    if(node_process == NULL){
        text_color(0x0c);
        cout << "Failed to allocate memory." << endl;
        exit(-1);
    }else{*/
        /*
        cout << "node_process->data:" << &node_process->pcb_data << endl;
        cout << "pcb_node_->data:" << pcb_node_->pcb_data.name << endl;
        node_process->pcb_data = pcb_node_->pcb_data;
        cout << "node_process->data:" << node_process->pcb_data.name << endl;
*/
        temp_f->next = pcb_node_;
        pcb_node_->next = temp_b;
        //cout << "tmp_b:" << temp_b << endl;
   // }

}

int get_length(link_list head){
    int length = 0;
    link_list tmp = head;
    while(tmp->next != NULL){
        length++;
        tmp = tmp->next;
    }
    return length;
}

c_pcb get_node(link_list head, int position, int flag){
    int i = 0;
    link_list tmp = head;
    while(tmp->next != NULL && i < position){
        tmp = tmp->next;
        i++;
    }
    if(flag){
        printf("%-8d", tmp->pcb_data.pid);
        cout << tmp->pcb_data.name;
        printf("\t%-8d\t%-8d\t%-16d%-4d\n", tmp->pcb_data.priority, tmp->pcb_data.status, tmp->pcb_data.time_left, tmp->pcb_data.time_reach);
    }
    return tmp->pcb_data;
}
bool is_duplicated(int pid){
    p_node temp;
    temp = running;
    if(temp != NULL){
        if(temp->pcb_data.pid == pid){
            text_color(0x0c);
            cout << "Process " << temp->pcb_data.pid <<" has been created, please try another pid:\n" << endl;
            text_color(0x07);
            return true;
        }
    }
    temp = ready_list;
    while(temp->next != NULL){

        if(temp->next->pcb_data.pid == pid){
            text_color(0x0c);
            cout << "Process " << temp->next->pcb_data.pid <<" has been created, please try another pid:\n" << endl;
            text_color(0x07);
            return true;
        }
        temp = temp->next;
    }

    temp = blocked_list;
    while(temp->next != NULL){

        if(temp->next->pcb_data.pid == pid){
            text_color(0x0c);
            cout << "Process " << temp->next->pcb_data.pid <<" has been created, please try another pid:\n" << endl;
            text_color(0x07);
            return true;
        }
        temp = temp->next;
    }
}
void delete_node(link_list head){
    if(head->next == NULL){
            cout << "NULL" << endl;
        exit(-1);
    }
    head->next = head->next->next;
    //cout << "DELETE:" << head->next << endl;
}
bool is_list_empty(link_list process_list){
    if(process_list->next == NULL) return true;
    else return false;
}
void set_node(link_list head, int position, c_pcb data){
    int i = 0;
    link_list tmp = head;
    while(tmp->next != NULL && i < position){
        tmp = tmp->next;
        i++;
    }
    tmp->pcb_data = data;
}

/**** function from main() ****/
void print_menu(){
    text_color(0x0B);
    cout << "****** A Process Handling Simulation Program ******" << endl;
    cout << "       1. Create a process" << endl;
    cout << "       2. Delete a process" << endl;
    cout << "       3. Block a process" << endl;
    cout << "       4. Wake up a process" << endl;
    cout << "       5. Run a process" << endl;
    cout << "       q. Exit" << endl;
    cout << "****** ************************************* ******" << endl;
    text_color(0x07);
    cout << "Please choose an option: ";
}

int main()
{
    char option;
    while(1){
        print_menu();
        cin >> option;
        if(option == '1'){
            //int num;
            //cout << "Number of process you want to create: ";
            //cin >> num;
            //for(int i = 0; i < num; i++)
                ant_cpu.create_process();
            ant_cpu.sort_priority(ready_list, get_length(ready_list));
        }
        else if(option == '2'){
            int pid;
            cout << "Please type in the pid of the process to delete: ";
            cin >> pid;
            ant_cpu.delete_process(pid);
        }
        else if(option == '3'){
            if(running == NULL){
                text_color(0x0c);
                cout << "No process running, please run a process." << endl << endl;
                text_color(0x07);
                //ant_cpu.print_status();
                continue;
            }
            ant_cpu.block_process();
        }
        else if(option == '4'){
            if(blocked_list->next == NULL){
                text_color(0x0c);
                cout << "No process to wake up." << endl << endl;
                text_color(0x07);
                //ant_cpu.print_status();
                continue;
            }
            ant_cpu.wakeup_process();
        }
        else if(option == '5'){
            if(running != NULL){
                insert_node(ready_list, get_length(ready_list) + 1, running);
                ant_cpu.sort_priority(ready_list, get_length(ready_list));
                running = NULL;
            }
            running = ready_list->next;

            if(running == NULL){
                text_color(0x0c);
                cout << "No process exiting, please create a process." << endl << endl;
                text_color(0x07);
                ant_cpu.print_status();
                continue;
            }

            running->pcb_data.status = STATUS_RUNNING;
            ant_cpu.run_process();

        }
        else if(option == 'q'){
            break;
        }
        if(option != '5')
            ant_cpu.print_status();
        cpu_time++;
    }
    return 0;
}
