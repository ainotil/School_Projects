//
// This program was written by Tonia Li (z5423203)
// on 20/07/2022
//
// This program is to contruct a simulated sushi trian, which is able to
// set up a restaurant and arrange orders to the customers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Additional libraries here

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CONSTANTS  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define TRUE 1
#define FALSE 0

#define MAX_STRING_LENGTH 100
#define EMPTY_CUSTOMER "EMPTY"

#define COMMAND_ADD_TABLE 'a'
#define COMMAND_ADD_CUSTOMER 'c'
#define COMMAND_PRINT_RESTAURANT 'p'
#define COMMAND_ADD_PLATE 'r'
#define COMMAND_PRINT_TRAIN 't'
#define COMMAND_ASSIGN_ORDER 'o'
#define COMMAND_CALCULATE_BILLS 'b'
#define COMMAND_CLOSE_RESTAURANT 'q'
#define COMMAND_REVERSE 's'
#define COMMAND_ROTATE 'm'

// Provided Enums
// the numerical value is the cost of that plate color
enum plate_colour { RED = 3, GREEN = 4, BLUE = 5, PURPLE = 6, ORANGE = 7 };
enum sushi_type { VEGETARIAN, SEAFOOD, CHICKEN, TEMPURA };

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  STRUCTS  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided structs
struct table {
    char customer[MAX_STRING_LENGTH];
    struct plate *orders;
    struct table *next;
};

struct plate {
    char roll_name[MAX_STRING_LENGTH];
    enum plate_colour colour;
    enum sushi_type type;
    struct plate *next;
};

struct restaurant {
    struct table *tables;
    struct plate *plates;
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTION PROTOTYPES  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Helper Functions
void print_restaurant(struct restaurant *restaurant);
int scanf_command(char *command);
void interpret_line(
    char buffer[MAX_STRING_LENGTH], char name[MAX_STRING_LENGTH],
    enum plate_colour *colour, enum sushi_type *type);

void interpret_order(
    char buffer[MAX_STRING_LENGTH], char customer[MAX_STRING_LENGTH],
    char roll[MAX_STRING_LENGTH]);

enum plate_colour string_to_colour(char colour[MAX_STRING_LENGTH]);
enum sushi_type string_to_type(char type[MAX_STRING_LENGTH]);
void remove_newline(char input[MAX_STRING_LENGTH]);
char color_to_char(enum plate_colour colour);
void to_type(char types[MAX_STRING_LENGTH], enum sushi_type type);
void to_colour(char cols[MAX_STRING_LENGTH], enum plate_colour colour);

// My prototypes
// All prototypes related to tables
struct table *create_table(struct table *next); 
struct table *add_table(struct restaurant *sushi_restaurant);
struct table *add_customer (struct restaurant *sushi_restaurant, char *value);
struct table *remove_orders (struct restaurant *sushi_restaurant, char customer[MAX_STRING_LENGTH]);

// All prototypes related to plates
struct plate *arrange_plate (struct restaurant *sushi_restaurant, struct plate **vegetarian, struct plate **seafood, struct plate **chicken, struct plate **tempura, int reverse_switch);
struct plate *create_plate (struct plate *next, char dish_name[MAX_STRING_LENGTH], int colour, int type);
struct plate *add_group (struct plate *plate, char dish_name[MAX_STRING_LENGTH], int colour, int type);
struct plate *group_plate (struct restaurant *sushi_restaurant, char dish_name[MAX_STRING_LENGTH], int colour, int type);
struct plate *merge_lists (struct plate *old, struct plate *new);
struct plate *reverse_merge_lists (struct plate *old, struct plate *new);
struct plate *remove_plate (struct restaurant *sushi_restaurant, char dish_name[MAX_STRING_LENGTH]); 
struct plate *copy_plate (struct restaurant *sushi_restaurant, char dish_name[MAX_STRING_LENGTH], char customer [MAX_STRING_LENGTH]);
struct plate *assign_orders (char customer[MAX_STRING_LENGTH], char dish_name[MAX_STRING_LENGTH], int colour, int type, struct restaurant *sushi_restaurant);

// All prototypes for other operations
void print_linked_list(struct restaurant *restaurant);
int calculate_bill (struct restaurant *sushi_restaurant, char customer[MAX_STRING_LENGTH]);
struct restaurant *free_everything (struct restaurant *sushi_restaurant);
struct plate *reverse(struct restaurant *sushi_restaurant);
struct plate *rotate(struct restaurant *sushi_restaurant, int rotate_number);

// All prototypes for detections
int detect_customer(struct restaurant *sushi_restaurant, char customer[MAX_STRING_LENGTH], int detect_table);
int detect_group(struct restaurant *sushi_restaurant, int type);
int detect_same_plate(struct restaurant *sushi_restaurant, char dish_name[MAX_STRING_LENGTH], int detect);
int detect_number_of_nodes (struct restaurant *sushi_restaurant);
int detect_smallest_group (struct plate *old, struct plate *new); 
////////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION IMPLEMENTATIONS  //////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {

    // here's my setups
    struct restaurant *sushi_restaurant;
    printf("Welcome to CSE Sushi Restaurant!\n");
    sushi_restaurant = malloc(sizeof(struct restaurant));
    sushi_restaurant->plates = NULL;
    sushi_restaurant->tables = NULL;
    
    // This is to arrange plates into groups for nesting linked lists
    struct plate *tempura = NULL;
    struct plate *chicken = NULL;
    struct plate *seafood = NULL;
    struct plate *vegetarian = NULL;

    print_restaurant(sushi_restaurant);
    printf("Enter command: ");
    
    // My variables
    char command;
    int reverse_switch = 1;
    int detect_table = FALSE;
    int detect_plate = FALSE;
    int detect_nodes = FALSE;
    
    // if the restaurant is empty, quit the commands
    while (sushi_restaurant != NULL && scanf_command(&command) == TRUE) {
    
        if (command == COMMAND_PRINT_RESTAURANT) {
        
--------------------------------------------------------------------------------
> ``: Having too much vertical whitespace can decrease the readability of your code, especially if it's single lines!
--------------------------------------------------------------------------------
            print_restaurant(sushi_restaurant);
            
        } else if (command == COMMAND_ADD_TABLE) {
            
            // If the restaurant is empty, create the first table and directly
            // connect it to the restaurant for further ado. 
            if (sushi_restaurant->tables == NULL) {
                struct table *new_table = create_table(NULL);
                sushi_restaurant->tables = new_table;
            } else {
                sushi_restaurant->tables = add_table(sushi_restaurant);
            }
            
        } else if (command == COMMAND_ADD_CUSTOMER) {
            
            printf("Enter customer name: ");
            char value [MAX_STRING_LENGTH];
            fgets(value, MAX_STRING_LENGTH, stdin);
            remove_newline(value);
            
            // we detect the availability of tables
            if (sushi_restaurant->tables != NULL) {
                detect_table = detect_customer(sushi_restaurant, 
                EMPTY_CUSTOMER, detect_table);
            }
            
            // if the restaurant has tables and spare spots, add the customer
            if (sushi_restaurant->tables == NULL || detect_table == FALSE) {
                printf("No empty tables!\n");
            } else {
                add_customer(sushi_restaurant, value); 
            }
            
        } else if (command == COMMAND_ADD_PLATE) {
        
            // To avoid overdeep nesting and this step is essential to be recalled 
            // in other steps, I made a function that combines everything.
            // For further info, the function is down below.
            arrange_plate(sushi_restaurant, &vegetarian, &seafood, &chicken, 
            &tempura, reverse_switch);
            
        } else if (command == COMMAND_PRINT_TRAIN) {
            
            print_linked_list(sushi_restaurant);
            
        } else if (command == COMMAND_ASSIGN_ORDER) {
            
            // We set the inputs to usable values
            char remaining_input[MAX_STRING_LENGTH];
            fgets(remaining_input, MAX_STRING_LENGTH, stdin);
            char customer[MAX_STRING_LENGTH];
            char dish_name[MAX_STRING_LENGTH];
            interpret_order(remaining_input, customer, dish_name);
            
            // We detect the tables with customers and the sushi train
            detect_table = 
            detect_customer(sushi_restaurant, customer, detect_table);
            if (sushi_restaurant->plates != NULL) {
                detect_plate = detect_same_plate(sushi_restaurant, dish_name, 
                detect_plate); 
            }
            
            // if the trian isn empty and there is an availiable plate such customer 
            if (
                sushi_restaurant->plates != NULL && 
                detect_table == TRUE && detect_plate == TRUE
            ) {
                // we copy the original plate, assign it to the customer's order
                // then remove it from the train.
                copy_plate (sushi_restaurant, dish_name, customer);
                remove_plate(sushi_restaurant, dish_name); 
            } else if (detect_table == FALSE) {
                // if its'detected that such customer doesn't exist
                printf("There is no customer with that name!\n");
            } else if (
                (detect_plate == FALSE && detect_table == TRUE) || 
                sushi_restaurant->plates == NULL
                ) {
                // if such plate doesn't exist, including the case that the 
                // trian is empty, we add a new plate to the trian then assign
                // it to the orders then remove it.
                printf("Enter dish details: ");
                char input[MAX_STRING_LENGTH];
                fgets(input, MAX_STRING_LENGTH, stdin);
                char dish[MAX_STRING_LENGTH];
                enum plate_colour colour;
                enum sushi_type type;
                interpret_line(input, dish, &colour, &type);
                assign_orders (customer, dish, colour, type, sushi_restaurant);
            }
            
        } else if (command == COMMAND_CALCULATE_BILLS) {
        
            printf("Enter customer name: ");
            char value [MAX_STRING_LENGTH];
            fgets(value, MAX_STRING_LENGTH, stdin);
            remove_newline(value);
            
            // we detect if such customer exists. If does, then calculate the
            // bill according to the plate colours, then remove the plates
            detect_table = detect_customer(sushi_restaurant, value, detect_table);
            if (detect_table == FALSE) {
                printf("There is no customer with that name!\n");
            } else {
                int bill = calculate_bill (sushi_restaurant, value);
                printf(
                "Thank you for dining with CSE Sushi! Your total is: $%d\n"
                , bill
                );
                remove_orders (sushi_restaurant, value);
            }
            
        } else if (command == COMMAND_CLOSE_RESTAURANT) {
        
            // we have a fucntion to free the tables and plates, then free
            // the restaurant structure.
            free_everything(sushi_restaurant);
            free(sushi_restaurant);
            sushi_restaurant = NULL;
            
        } else if (command == COMMAND_REVERSE) {
        
            // we have a function to reverse the trian.
            // Note that every time this command is called, we turn the
            // reverse_switch to the opposite.
            if (sushi_restaurant->plates != NULL) {
                sushi_restaurant->plates = reverse(sushi_restaurant);
            }
            reverse_switch = -reverse_switch;
        } else if (command == COMMAND_ROTATE) {
            // input the rotate number
            char value[MAX_STRING_LENGTH];
            fgets(value, MAX_STRING_LENGTH, stdin);
            int rotate_number = atoi(value);
            detect_nodes = detect_number_of_nodes(sushi_restaurant);
            // compare if the rotate number is greater than this amount;
            // if is greater, take the remainder else just take it
            if (abs(rotate_number) > detect_nodes && detect_nodes != 0) {
--------------------------------------------------------------------------------
> `if (abs(rotate_number) > detect_nodes && detect_nodes != 0) {`: Consider if these 3 if statements could have been simplified into one, (through a function or otherwise)!
--------------------------------------------------------------------------------
                rotate_number = rotate_number % detect_nodes;
            }
            // i will have two functions one for negative one for positive
            if (rotate_number > 0) {
                rotate(sushi_restaurant, rotate_number);
            } else if (rotate_number < 0) {
                rotate_number = rotate_number + detect_nodes;
                rotate(sushi_restaurant, rotate_number);
            }  
        }
        // this determination here is to avoid printing after the restaurant
        // is closed
        if (command != COMMAND_CLOSE_RESTAURANT) {
            printf("Enter command: ");
        }
    }
    // This is to ensure that no memory leak if the program ends without
    // closing the restaurant
    if (sushi_restaurant != NULL) {
        free_everything(sushi_restaurant);
        free(sushi_restaurant);
        sushi_restaurant = NULL;
    }
    
    printf("Thank you for dining with CSE Sushi Restaurant!\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct plate *rotate (struct restaurant *sushi_restaurant, int rotate_number) {
    if (sushi_restaurant->plates != NULL) {
        int i = 1;
        struct plate *current = sushi_restaurant->plates;
        struct plate *head = sushi_restaurant->plates;
        while (current->next != NULL && i < rotate_number) {
            // we loop till the tail of the list
            current = current->next;
            i++;
        }
        // store the tail of the first part to a variable
        struct plate *temp = current;
        while (current->next != NULL) {
            // we loop through the rest of the original list
            current = current->next;
        }
        // now we connect the lists by assiging the the second part's next to
        // the head of the first part splitted list. Then we make the new 
        // head as the head of the second part. Lastly set the tail of the
        // second to null.
        current->next = head;
        head = temp->next;
        temp->next = NULL;
        sushi_restaurant->plates = head;
    }
    return sushi_restaurant->plates;
}

// My functions
// functions related to tables
struct table *create_table(struct table *next) {
    // we create a table and assign some memory to it
    struct table *new_table = malloc(sizeof(struct table));
    // initialise it to empty status and return it
    strcpy(new_table->customer, EMPTY_CUSTOMER);
    new_table->next = next;
    new_table->orders = NULL;
    return new_table;
}

struct table *add_table(struct restaurant *sushi_restaurant) {
    // we add tables by inserting nodes at the tail
    struct table *current = sushi_restaurant->tables;
    while (current->next != NULL) {
        // loop till reaches the last node
        current = current->next;
    }
    // now create a new node
    struct table *new_table = create_table(NULL);
    // make it as the last node and return the head node
    new_table->next = current->next;
    current->next = new_table;
    return sushi_restaurant->tables;
}

struct table *add_customer (struct restaurant *sushi_restaurant, char *value) {
    // to add a customer, we loop till the empty table and assign the customer
    // name as the input value
    struct table *current = sushi_restaurant->tables;
    while (
        current != NULL && 
        strcmp(current->customer, EMPTY_CUSTOMER) != 0
    ) {
        current = current->next;
    }
    strcpy(current->customer, value);
    remove_newline(current->customer);
    return sushi_restaurant->tables;
}

struct table *remove_orders (
    struct restaurant *sushi_restaurant, 
    char customer[MAX_STRING_LENGTH]
) {
    struct table *current = sushi_restaurant->tables;
    // we first loop till the target customer
    while (current->next != NULL && strcmp(current->customer, customer) != 0) {
        current = current->next;
    }
    // then we turn over to its orders
    struct plate *temp = current->orders; 
    while (current->orders != NULL) { 
        // we loop through the orders, everytime we pass a node, just free it.
        temp = temp->next;
        free(current->orders);
        current->orders = temp;
    }
    // lastly we reset the customer to empty
    strcpy(current->customer, EMPTY_CUSTOMER);
    return sushi_restaurant->tables;
}

// funtions related to plates
struct plate *arrange_plate (
    struct restaurant *sushi_restaurant, 
    struct plate **vegetarian, 
    struct plate **seafood, 
    struct plate **chicken, 
    struct plate **tempura, 
    int reverse_switch
) {
    // first we decalre some variables and turn the inputs to usable
    char input[MAX_STRING_LENGTH];
    fgets(input, MAX_STRING_LENGTH, stdin);
    char dish_name[MAX_STRING_LENGTH];
    enum plate_colour colour;
    enum sushi_type type;
    interpret_line(input, dish_name, &colour, &type);
    // then we have a current list for storing the clarified linked list
    struct plate *current = NULL;
    // we detect the existence of the group that we entered
    int detect_groups = detect_group(sushi_restaurant, type);
    if (detect_groups == FALSE) {
        // if the group doesn't exist, we create the group and set the
        // current to the group.   
        if (type == TEMPURA) {
            *tempura = add_group(*tempura, dish_name, colour, type);
            current = *tempura;
        } else if (type == CHICKEN) {
            *chicken = add_group(*chicken, dish_name, colour, type);
            current = *chicken;
        } else if (type == SEAFOOD) {
            *seafood = add_group(*seafood, dish_name, colour, type);
            current = *seafood;
        } else if (type == VEGETARIAN) {
            *vegetarian = add_group(*vegetarian, dish_name, colour, type);
            current = *vegetarian;
        }
    } else {
        // else if the group exists, we just go inside that group and insert 
        // the new node at the tail.
        sushi_restaurant->plates = group_plate(sushi_restaurant, dish_name, colour, type);
    }
    // if the group is the first group in the restaurant, just sign it 
    // without further merging
    if (sushi_restaurant->plates == NULL) {
        if (type == TEMPURA) { 
            sushi_restaurant->plates = *tempura;
        } else if (type == CHICKEN) {
            sushi_restaurant->plates = *chicken;
        } else if (type == SEAFOOD) {
            sushi_restaurant->plates = *seafood;
        } else if (type == VEGETARIAN) {
            sushi_restaurant->plates = *vegetarian;
        }
    } else {
        // if the group is new then it needs to be sorted.
        if (detect_groups == FALSE && reverse_switch == 1) {
            // sorted in positive order
            sushi_restaurant->plates = 
            merge_lists(sushi_restaurant->plates, current);
        } else if (detect_groups == FALSE && reverse_switch == -1) {
            // sorted in negative order
            sushi_restaurant->plates = 
            reverse_merge_lists(sushi_restaurant->plates, current);
        }
    }
    return sushi_restaurant->plates;
}

struct plate *create_plate (
    struct plate *next, 
    char dish_name[MAX_STRING_LENGTH], 
    int colour, 
    int type
) {
    // this function is to create a plate as the fundemental of all plates
    // operations. We create a new plate by assignning the memory and 
    // initialise it.
    struct plate *new_plate = malloc(sizeof(struct plate));
    strcpy(new_plate->roll_name, dish_name);
    new_plate->next = next;
    new_plate->colour = colour;
    new_plate->type = type;
    return new_plate;
}

struct plate *add_group (
    struct plate *plate, 
    char dish_name[MAX_STRING_LENGTH], 
    int colour, 
    int type
) {
    // This function is called if there's no such group in the input exists
    struct plate *new_plate = create_plate(NULL, dish_name, colour, type);
    plate = new_plate;
    return plate;
}


struct plate *group_plate (
    struct restaurant *sushi_restaurant, 
    char dish_name[MAX_STRING_LENGTH], 
    int colour, 
    int type
) {
    // when the group exists, we do some operations inside these sub linked lists
    // Note that these linked lists have already been combined into another 
    // linked lists, so the process will be complicated and needed to be broke
    // into parts
    struct plate *current = sushi_restaurant->plates;
    while (current->next != NULL && current->type != type) {
        // To add the new plate, we firstly loop till it reaches the group
        // where it belongs to.
        current = current->next;
    }
    
    // then we loop till the end of that group. We have a previous variable
    // here because there will be two cases below.
    struct plate *previous = current;
    while (current->next != NULL && current->type == type) {
        previous = current;
        current = current->next;
    }  
    
    // we have two cases due to the different position we're at.
    // 1. We will be able to proceed to the head of the next group, if our 
    // target is at the middle of the list.
    // 2. We are stopped since our target group is the tail of the list.
    if (current->type != type) {
        struct plate *new_plate = create_plate(NULL, dish_name, colour, type);
        new_plate->next = previous->next;
        previous->next = new_plate; 
    } else if (current->type == type) {
        struct plate *new_plate = create_plate(NULL, dish_name, colour, type);
        new_plate->next = current->next;
        current->next = new_plate;
    }   
    return sushi_restaurant->plates;
}

struct plate *merge_lists (struct plate *old, struct plate *new) {
    // We have two major cases in this process, whether the list contains a smaller
    // type of group compared to the new added group.
    // I intended to directly add the group after the larger one, which was easier
    // since linked lists are formed from tail to head. However after rotations
    // it was all messed up, so I devour to make it more complicated but correct
     
    int detect_smallest = detect_smallest_group(old, new);
    if (new->type < old->type && detect_smallest == FALSE) {
        // 1. if there's no smaller group in the old list, we just add the new
        // node as the head of the list. (if all nodes in the list have a smaller
        // type)
        struct plate *current = new;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = old;
        old = current;
    } else {
        // 2. case 2 is more complicated, since we confirmed that there's a smaller
        // group in the list, we need to reach it in its position.
        struct plate *current = old;
        int loop_switch = TRUE;
        while (current->next != NULL && loop_switch == TRUE) {
            // when we reach it, quit the loop
            if (current->type < new->type && current->next->type > new->type) {
                loop_switch = FALSE;
            } else {
                current = current->next;
            }
        }
        // now we are at the tail node of the smaller group
        new->next = current->next;
        current->next = new;
    }
    return old;
}

struct plate *reverse_merge_lists (struct plate *old, struct plate *new) {
    // similar to above, but the comparison operators are different
    int detect_smallest = detect_smallest_group(old, new);
    if (new->type > old->type && detect_smallest == TRUE) {
        struct plate *current = new;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = old;
        old = current;
    } else {
        struct plate *current = old;
        int loop_switch = TRUE;
        while (current->next != NULL && loop_switch == TRUE) {
            if (current->type > new->type && current->next->type < new->type) {
                loop_switch = FALSE;
            } else {
                current = current->next;
            }
        }
        new->next = current->next;
        current->next = new;
    }
    
    return old;
}

struct plate *copy_plate (
    struct restaurant *sushi_restaurant, 
    char dish_name[MAX_STRING_LENGTH], 
    char customer[MAX_STRING_LENGTH]
) { 
    struct plate *current = sushi_restaurant->plates;
    while (current->next != NULL && strcmp(current->roll_name, dish_name) != 0) {
        // we loop till the target node which contains the same name as the entered
        // value.    
        current = current->next;
    }
    // then we create a new node which copies its info.
    struct plate *temp = 
    create_plate(NULL, current->roll_name, current->colour, current->type);
    
    // now we assign the node to the orders
    struct table *curr = sushi_restaurant->tables;
    while (curr != NULL && strcmp(curr->customer, customer) != 0) {
        // we loop till the node with the target customer
        curr = curr->next;
    }
    // then if the orders are empty, we directly assign the copied node to it.
    if (curr->orders == NULL) { 
        curr->orders = temp;
    } else {
        // otherwise we add the new node to the order list as the tail
        while (curr->orders->next != NULL) {
            curr->orders = curr->orders->next;
        }
        temp->next = curr->orders->next;
        curr->orders->next = temp;
    }
    return temp;
}

struct plate *remove_plate (
    struct restaurant *sushi_restaurant, 
    char dish_name[MAX_STRING_LENGTH]
) {
    struct plate *current = sushi_restaurant->plates;
    struct plate *previous = NULL;
    // we loop till the target node
    while (current->next != NULL && strcmp(current->roll_name, dish_name) != 0) {    
        previous = current;
        current = current->next;
    }
    // then there're four cases.
    if (previous == NULL) {
        // 1. if the node is the first, we point it to NULL and free it.
        sushi_restaurant->plates = sushi_restaurant->plates->next;
        free(current);
        return NULL;
    } else if (previous != sushi_restaurant->plates && current->next == NULL) {
        // 2. if current is the last node, we first split current away from the 
        // list. Then we free and set it to null.
        previous->next = NULL;
        free(current);
        current = NULL;
    } else {
        // 3. if current is at the middle of the list, we split current away 
        // from the list. Then we free and set it to null.
        previous->next = current->next;
        current->next = current->next->next;
        free(current);
        current = NULL;
    }
    return sushi_restaurant->plates;
}

struct plate *assign_orders (
    char customer[MAX_STRING_LENGTH], 
    char dish_name[MAX_STRING_LENGTH], 
    int colour, 
    int type, 
    struct restaurant *sushi_restaurant
) {
    struct plate *temp = 
    create_plate(NULL, dish_name, colour, type);
    
    // now we assign the node to the orders
    struct table *curr = sushi_restaurant->tables;
    while (curr != NULL && strcmp(curr->customer, customer) != 0) {
        // we loop till the node with the target customer
        curr = curr->next;
    }
    // then if the orders are empty, we directly assign the copied node to it.
    if (curr->orders == NULL) { 
        curr->orders = temp;
    } else {
        // otherwise we add the new node to the order list as the tail
        struct plate *current = curr->orders;
        while (current->next != NULL) {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;
    }
    return sushi_restaurant->tables->orders;
}

// functions for other operations
void print_linked_list(struct restaurant *restaurant) {
    struct plate *current = restaurant->plates;
    if (current == NULL) {
        printf("    empty :(\n");
    } else {
        while (current != NULL) {
            // for everynode, print its info
            char types[MAX_STRING_LENGTH];
            char cols[MAX_STRING_LENGTH];
            to_type(types, current->type);
            to_colour(cols, current->colour);
            printf("    %s-%s-%s\n", current->roll_name, types, cols);
            printf("      |\n");
            printf("      v\n");
            current = current->next;
        }
    }
}

int calculate_bill (
    struct restaurant *sushi_restaurant, 
    char customer[MAX_STRING_LENGTH]
) {
    struct table *current = sushi_restaurant->tables;
    while (current->next != NULL && strcmp(current->customer, customer) != 0) {
        // we loop till the target customer
        current = current->next;
    }
    // we then enter its order list
    struct plate *curr = current->orders;
    int count = 0;
    while (curr != NULL) {
        // we count the price by summing up
        count += curr->colour;
        curr = curr->next;
    }
    return count;
}

struct restaurant *free_everything (struct restaurant *sushi_restaurant) {
    if (sushi_restaurant->tables != NULL) {
        struct table *current = sushi_restaurant->tables;
        while ((current = sushi_restaurant->tables) != NULL) {
            // we loop through every node and free the orders inside
            // I didn't create much variables here because the process isn't
            // complicated, but require so many variables names if I tried to
--------------------------------------------------------------------------------
>    // I didn't create much variables here because the process isn't
>                // complicated, but require so many variables names if I tried to

> No problem!
--------------------------------------------------------------------------------
            // create. And I would come up with names like curr, curr1, curr2 ect.
            struct plate *temp = sushi_restaurant->tables->orders;
            while ((temp = sushi_restaurant->tables->orders) != NULL) { 
                sushi_restaurant->tables->orders = 
                sushi_restaurant->tables->orders->next;
                free(temp);
                temp = NULL;
            }
            sushi_restaurant->tables = sushi_restaurant->tables->next;
            free(current);
            current = NULL;
        }
    }
    // Similar to freeing tabls above, but no need to have a nested loop
    if (sushi_restaurant->plates != NULL) { 
        struct plate *curr = sushi_restaurant->plates;
        while ((curr = sushi_restaurant->plates) != NULL) {
            sushi_restaurant->plates = sushi_restaurant->plates->next;
            free(curr);
            curr = NULL;
        }
    }   
    return sushi_restaurant;    
}

struct plate *reverse(struct restaurant *sushi_restaurant) {
    
    struct plate *current = sushi_restaurant->plates;
    struct plate *previous = NULL;
    while (current != NULL) {
        // we save the current->next into other variable, since we will have
        // some arrangements on it.
        struct plate *next = current->next;
        // we reassign the next node of current to previous for every node
        // which is similar to changing the direction of the nodes.
        current->next = previous;
        // then we progress the looping
        previous = current;
        current = next;      
    }
    return previous;
}

// function for detections
int detect_customer(
    struct restaurant *sushi_restaurant, 
    char customer[MAX_STRING_LENGTH], 
    int detect_table
) {
    // this function is to detect if any requiring customer exists. It can be 
    // used to detect availiable tables by assigning customer to EMPTY_CUSTOMER
    struct table *current = sushi_restaurant->tables;
    while (current->next != NULL && strcmp(current->customer, customer) != 0) {
        current = current->next;
    }
    if (strcmp(current->customer, customer) == 0) {
        detect_table = TRUE;
    } else {
        detect_table = FALSE;
    }
    return detect_table;
}

int detect_same_plate(
    struct restaurant *sushi_restaurant, 
    char dish_name[MAX_STRING_LENGTH], 
    int detect
) {
    // this function is to detect if any requiring plates exist in accordance
    // of roll name
    struct plate *current = sushi_restaurant->plates;
    while (
        current->next != NULL && 
        strcmp(current->roll_name, dish_name) != 0
    ) {
        current = current->next;
    }
    if (strcmp(current->roll_name, dish_name) == 0) {
        detect = TRUE;
    } else {
        detect = FALSE;
    }
    return detect;
}

int detect_group(
    struct restaurant *sushi_restaurant, 
    int type
) {
    // this function is to detect any plates with the same requiring type exists
    int detect = FALSE;
    struct plate *current = sushi_restaurant->plates;
    if (current == NULL) {
        return detect;
    } else {
        while (
            current->next != NULL && 
            current->type != type
        ) {
            current = current->next;
        }
        if (current->type == type) {
            detect = TRUE;
        }
    } 
    return detect;
}

int detect_smallest_group (struct plate *old, struct plate *new) {
    // since the rotations make plates addition more complicated
    // we need a function to detect whether there's a smaller type of group
    // in the tran.
    int loop_switch = TRUE;
    int detect_smallest = FALSE;
    struct plate *current = old;
    while (current->next != NULL && loop_switch == TRUE) {
        // if loop till the target node, we quit the loop
        if (current->type < new->type) {
            loop_switch = FALSE;
        } else {
            current = current->next;
        }
    }
    if (loop_switch == FALSE) {
        detect_smallest = TRUE;
    }  
    return detect_smallest;
}

int detect_number_of_nodes (struct restaurant *sushi_restaurant) {
    // we detect the number of plates for rotations' remainder
    int count = 0;
    if (sushi_restaurant->plates != NULL) {
        // if the train isn't empty, then it should be initially 1 plate
        count = 1;
        struct plate *current = sushi_restaurant->plates;
        while (current->next != NULL) {
            // then we tranverse through the list and count the plates
            current = current->next;
            count++;
        }
    }
    return count;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function - DO NOT EDIT
void print_restaurant(struct restaurant *restaurant) {
    int i;
    if (restaurant == NULL) {
        printf("ERROR: No restaurant!\n");
        return;
    };

    printf("CSE Sushi Restaurant\n");
    struct table *table = restaurant->tables;
    int table_num = 0;
    while (table != NULL) {
        int table_length = 5;
        if (strcmp(table->customer, EMPTY_CUSTOMER) != 0) {
            table_length = strlen(table->customer) + 4;
        }
        // calculate number of plates
        int number_of_plates = 0;
        struct plate *curr = table->orders;
        while (curr != NULL) {
            number_of_plates++;
            curr = curr->next;
        }

        printf(" ");
        for (i = 0; i < table_length; i++) {
            printf("_");
        }
        printf(" ");

        // print out the top row
        for (i = 0; i < number_of_plates; i++) {
            printf("    _____ ");
        }

        printf("\n");

        // print out the data
        int occupied = FALSE;
        if (strcmp(table->customer, "EMPTY") != 0) {
            occupied = TRUE;
        }
        if (occupied) {
            printf("|  %s  |-->", table->customer);
        } else {
            printf("|  %d  |-->", table_num);
        }

        // print out the plates
        struct plate *order = table->orders;
        while (order != NULL) {
            printf("|  %c  |-->", color_to_char(order->colour));

            order = order->next;
        }

        printf("\n");
        printf(".");
        for (i = 0; i < table_length; i++) {
            printf("-");
        }
        printf(".");
        // print out the top row
        for (i = 0; i < number_of_plates; i++) {
            printf("   |-----|");
        }

        printf("\n");
        table = table->next;
        table_num++;
    }
}

// Helper Function
// scans a single character from the user and returns the value of scanf;
//
// Parameters:
//  - command: a pointer to a character to store the command
//
// Returns:
//  - the value that scanf returns (this will be 1 if it was successfully read);
//
// Usage:
//  ```
//  char command;
//  while (scanf_command(&command) == 1) {
//  ```
// this function does some wizadry that you **do not need to understand**
// for you to ensure that subsequent
// calls to fgets works correctlly.
int scanf_command(char *command) {
    // read in character + newline + null-terminator
    char buffer[3];
    char *result = fgets(buffer, 3, stdin);
    if (result == NULL) {
        return EOF;
    }

    // a non a-z character probably means not a char
    if (buffer[0] < 'a' || buffer[0] > 'z') {
        return EOF;
    }

    // set the character
    *command = buffer[0];
    return 1;
}

// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the format: [string] [enum1] [enum2]
// This function will extract the relevant values into the given variables.
// The function will also remove any newline characters.
//
// For example, if given: "salmon red seafood"
// The function will copy the string:
//     "salmon" into the 'name' array
// And will copy the enums:
//     red      into the colour pointer
//     seafood  into the type pointer
//
// If you are interested, `strtok` is a function which takes a string,
// and splits it up into before and after a "token" (the second argument)
//
// Parameters:
//     buffer  = A null terminated string in the following format
//               [string] [enum1] [enum2]
//     name    = An array for the [string] to be copied into
//     colour  = A pointer to where [enum1] should be stored
//     type    = A pointer to where [enum2] should be stored
// Returns:
//     None
void interpret_line(
    char buffer[MAX_STRING_LENGTH], char name[MAX_STRING_LENGTH],
    enum plate_colour *colour, enum sushi_type *type) {
    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as string (sushi roll name)
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        // Extract word
        strcpy(name, name_str);
    }

    // Extract value 2 as str, then enum
    char *colour_str = strtok(NULL, " ");
    if (colour_str != NULL) {
        *colour = string_to_colour(colour_str);
    }

    // Extract value 2 as str, then enum
    char *type_str = strtok(NULL, " ");
    if (type_str != NULL) {
        *type = string_to_type(type_str);
    }

    if (name_str == NULL || colour_str == NULL || type_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }
}

// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the following foramt: [string1] [string2]
// This function will extract the relevant values into the given variables.
// The function will also remove any newline characters.
//
// For example, if given: "Gab salmon"
// The function will put:
//     "Gab" into the 'customer' array
//     "salmon" into the 'roll' array
//
// Parameters:
//     buffer   = A null terminated string in the following format
//                [string1] [string2]
//     customer = An array for the [string1] to be copied into
//     roll     = An array for the [string2] to be copied into
// Returns:
//     None
void interpret_order(
    char buffer[MAX_STRING_LENGTH], char customer[MAX_STRING_LENGTH],
    char roll[MAX_STRING_LENGTH]) {
    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as string
    char *customer_str = strtok(buffer, " ");
    if (customer_str != NULL) {
        strcpy(customer, customer_str);
    }

    // Extract value 2 as string
    char *roll_str = strtok(NULL, " ");
    if (roll_str != NULL) {
        strcpy(roll, roll_str);
    }

    if (customer_str == NULL || roll_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }
}

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will remove and first newline it sees.
// The newline character wil be replaced with a null terminator ('\0')
void remove_newline(char input[MAX_STRING_LENGTH]) {
    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}

// Helper Function
// You likely do not need to change this function.
//
// Given an enum plate_colour will return the first letter of enum, or ? if the
// value is not one of the plate_colour values.
// Parameters:
//     colour  = enum plate_colour for a plate
// Returns:
//     char    = character represtning the first letter of the plate colour
char color_to_char(enum plate_colour colour) {
    if (colour == RED) {
        return 'r';
    } else if (colour == GREEN) {
        return 'g';
    } else if (colour == BLUE) {
        return 'b';
    } else if (colour == PURPLE) {
        return 'p';
    } else if (colour == ORANGE) {
        return 'o';
    } else {
        return '?';
    }
}

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will return the corresponding enum plate_colour, or FALSE
// if the string doesn't correspond with the enums.
// Parameters:
//     colour  = string representing the corresponding enum plate_colour value
// Returns:
//     enum plate_colour
enum plate_colour string_to_colour(char colour[MAX_STRING_LENGTH]) {
    if (strcmp(colour, "red") == 0) {
        return RED;
    } else if (strcmp(colour, "green") == 0) {
        return GREEN;
    } else if (strcmp(colour, "blue") == 0) {
        return BLUE;
    } else if (strcmp(colour, "purple") == 0) {
        return PURPLE;
    } else if (strcmp(colour, "orange") == 0) {
        return ORANGE;
    }
    return FALSE;
}

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will return the corresponding enum sushi_type, or FALSE
// if the string doesn't correspond with the enums.
// Parameters:
//     type  = string representing the corresponding enum sushi_type value
// Returns:
//     enum sushi_type
enum sushi_type string_to_type(char type[MAX_STRING_LENGTH]) {
    if (strcmp(type, "vegetarian") == 0) {
        return VEGETARIAN;
    } else if (strcmp(type, "seafood") == 0) {
        return SEAFOOD;
    } else if (strcmp(type, "chicken") == 0) {
        return CHICKEN;
    } else if (strcmp(type, "tempura") == 0) {
        return TEMPURA;
    }
    return FALSE;
}

// Helper Function
// You likely do not need to change this function.
//
// Given an enum sushi_type will return the enum name as a string, or "ERROR" if
// the value was not one of the given enums.
// Parameters:
//     types  = string to store the name of the enum in
//     type   = sushi_type enum to have the name stored in types.
// Returns:
//     None
void to_type(char types[MAX_STRING_LENGTH], enum sushi_type type) {
    if (type == VEGETARIAN) {
        strcpy(types, "vegetarian");
    } else if (type == SEAFOOD) {
        strcpy(types, "seafood");
    } else if (type == CHICKEN) {
        strcpy(types, "chicken");
    } else if (type == TEMPURA) {
        strcpy(types, "tempura");
    } else {
        strcpy(types, "ERROR");
    }
}

// Helper Function
// You likely do not need to change this function.
//
// Given an enum plate_colour will return the enum name as a string, or "ERROR"
// if the value was not one of the given enums.
// Parameters:
//     cols   = string to store the name of the enum in
//     colour = plate_colour enum to have the name stored in cols
// Returns:
//     None
void to_colour(char cols[MAX_STRING_LENGTH], enum plate_colour colour) {
    if (colour == RED) {
        strcpy(cols, "red");
    } else if (colour == BLUE) {
        strcpy(cols, "blue");
    } else if (colour == GREEN) {
        strcpy(cols, "green");
    } else if (colour == PURPLE) {
        strcpy(cols, "purple");
    } else if (colour == ORANGE) {
        strcpy(cols, "orange");
    } else {
        strcpy(cols, "ERROR");
    }
}
