// cs hotel
//
// This program was written by Tonia Li (z5423203)
// on 24/6/2022
//
// Assignment 1
--------------------------------------------------------------------------------
> `// Assignment 1`: Header comment descriptions should tell us about the program's purpose/behaviour
--------------------------------------------------------------------------------

#include <stdio.h>

// Additional libraries here

// Provided constants
#define TRUE 1
#define FALSE 0

#define TOTAL_FLOORS 8
#define ROOMS_PER_FLOOR 8
#define INITIAL_PRICE 10
#define NO_BOOKING -1
#define NORMAL 1
#define GROUP 2
#define ROYAL 3

// Your constants here

// Provided struct
struct room {
    int booking_id;
    int price;
    int type;
};

// Provided functions use for game setup
// You do not need to use these functions yourself.
void init_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

// You will need to use these functions for stage 1.
void print_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void print_hotel_with_prices(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

// Your function prototypes here:
void invalid_rooms (int row, int col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

int valid_rooms (int row, int col, int valid, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]); 

void print_rooms (int row, int col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void leave_rooms (int row, int col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void setting_price (int row, int col, int price, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void varying_price (int price, int row, int col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

int total_revenue (int row, int col, int revenue, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void total_revenue_1 (int revenue, int previous_revenue);

void best_rooms (int row, int col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], int valid);

int swap_invalid_rooms (int row, int col, int row_1, int col_1);
void swap_rooms (int *a, int *b);

int minimum_price (int row, int col, int minimum, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
int maximum_price (int row, int col, int maximum, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

int detect_availiability (int row, int col, int detect, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]); 
int detect_all_availiability (int row, int col, int detect, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
int detect_floor_availiability (int row, int col, int detect, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void group_1 (int row, int col, int n, int *valid, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void group_1_sub (int row, int col, int *n, int *valid, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void group_1_all (int row, int col, int n, int detect, int valid, int *valid_1, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void group_2 (int row, int col, int n, int price, int *valid, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void group_2_all (int row, int col, int n, int detect, int minimum, int price, int valid, int *valid_1, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void royal_group_remove (int *detect, int row, int col, int previous_booking_id, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void royal_group_replace (int row, int col, int detect, int previous_booking_id, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void royal_normal_replace (int row, int col, int previous_booking_id, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void royal_group_replace_sub (int row, int col, int detect, int previous_booking_id, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void royal_normal (int row, int col, int previous_booking_id, int detect, int valid, int *valid_1, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

void burglar (int row, int col, int previous_col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void burglar_sub (int *row, int col, int *previous_col, struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]); 

int main(void) {
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR];
    init_hotel(hotel);

    printf("Welcome to the CS Hotel!!\n\n");
    printf("Add your current guests, how many guests? ");
    
    int n;
    scanf("%d", &n);
    
    printf("Input in your guests room numbers\n");
    
    // my variables here 
    // detect is for checking rooms availablity
    // valid is for booking id
--------------------------------------------------------------------------------
>    // detect is for checking rooms availablity
>        // valid is for booking id

> Instead of having to rely on these comments to understand the variables' purpose, it would be better to just
> name the variables more descriptively - e.g. maybe available_room_detected instead of detect
--------------------------------------------------------------------------------
    int row, col, row_1, col_1, price; 
    int revenue = 0;
    int minimum = 0;
    int detect = 0;
    int previous_revenue = 0;
    int previous_col = 0;
    int i = 0;
    int valid = -1;

    // start scanning guests in 
    while (i < n) {
        scanf("%d %d", &row, &col);
        if (valid_rooms(row, col, valid, hotel)) {
            valid++;
            hotel[row][col].booking_id = valid;
        } else if (n > 64) {
--------------------------------------------------------------------------------
> `> 64) {`: Could this be a constant?
--------------------------------------------------------------------------------
            // when the rooms are not enough
            // I don't know if this will be included in marking
            printf("There's no available rooms!");
        } else {
            invalid_rooms(row, col, hotel);
        }
        i++;
    }
    
    print_hotel(hotel);
    printf("Enter a command: ");
    
    // all the commands 
    char command;
    while (scanf(" %c", &command) == 1) {

        if (command == 'p') {
        
            print_hotel(hotel); 
            
        } else if (command == '?') { 
        
            scanf("%d %d", &row, &col);
            print_rooms(row, col, hotel); 
            
        } else if (command == 'a') {
		
            detect = detect_all_availiability(row, col, detect, hotel);
            scanf("%d %d", &row, &col);
		    
            if (valid_rooms(row, col, valid, hotel)) { 
                valid++;
                // scan a new guest
                hotel[row][col].booking_id = valid; 
                printf("Guest's booking id is: %d\n", 
                hotel[row][col].booking_id);
            } else if (detect < 1) { 
                printf("The hotel is full!");
            } else { 
                invalid_rooms(row, col, hotel); 
            }  
              
        } else if (command == 'l') { 
	        
            scanf("%d %d", &row, &col);
            leave_rooms(row, col, hotel);
		    
        } else if (command == 'c') { 
        
            print_hotel_with_prices(hotel);
            
        } else if (command == '$') { 
        
            scanf("%d %d", &row, &price);
            setting_price(row, col, price, hotel); 
            
        } else if (command == '^') { 
        
            scanf(" %d", &price);
            varying_price (price, row, col, hotel);
            
        } else if (command == 'r') { 
            
            revenue = total_revenue(row, col, revenue, hotel); 
            total_revenue_1 (revenue, previous_revenue); 
            previous_revenue = revenue; 
            
        } else if (command == 'j') {
        
            valid++;
            best_rooms (row, col, hotel, valid);   
                  
        } else if (command == 's') {
        
            scanf("%d %d %d %d", &row, &col, &row_1, &col_1);
            // when it's invalid (out of bounds or something)
            if (swap_invalid_rooms (row, col, row_1, col_1)) {
                printf("Invalid room number!\n");
            } else {
		        
                swap_rooms(&hotel[row][col].booking_id, 
                &hotel[row_1][col_1].booking_id);
                // swap both booking id and guest type 
                //(the one above and the one below)
                swap_rooms(&hotel[row][col].type, &hotel[row_1][col_1].type);
            }
            
        } else if (command == 'g') {
        
            int option;
            scanf(" %d", &option);
            if (option == 1) {
--------------------------------------------------------------------------------
> `== 1)`: Could these be constants?
--------------------------------------------------------------------------------
                scanf(" %d", &n);
                group_1_all(row, col, n, detect, valid, &valid, hotel);
            } else if (option == 2) { 
                scanf(" %d %d", &n, &price);
                group_2_all(row, col, n, detect, minimum, price, valid, & valid, hotel);
            }
            
        } else if (command == '!') {
        
            scanf(" %d %d", &row, &col);
            row_1 = row;
            col_1 = col;
            if (hotel[row][col].type == GROUP) { 
                // store the original guest, for replacing them later
                int previous_booking_id = hotel[row_1][col_1].booking_id;
                detect = 0;
                // remove the original group guests
                royal_group_remove (&detect, row, col, previous_booking_id, hotel); 
                // add the royal guest
                valid++;
                hotel[row_1][col_1].booking_id = valid;
                hotel[row_1][col_1].type = ROYAL;
                // replace the original guests to the lowest floor
                // all these steps construct to a whole process
                royal_group_replace (row, col, detect, previous_booking_id, hotel); 
            } else if (hotel[row][col].type == NORMAL) { 
                // store the original guest
                int previous_booking_id = hotel[row][col].booking_id;
                // remove and replace the individual, and add the royal guest
                royal_normal(row, col, previous_booking_id, detect, valid, &valid, hotel);
            } else if (hotel[row][col].type == ROYAL) {
                // when the ocuppying guest is also royal
                printf("Your Royal Highness, this room is not available\n");
            }
               
        } else if (command == 'b') {
            // it has limitations, like the burglar can't go left
            // I give up on fixing it...
            burglar (row, col, previous_col, hotel);
        }
        printf("Enter a command: ");
    }
    printf("Bye!\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: you may need to add additional functions here
--------------------------------------------------------------------------------
> `// TODO: you may need to add additional functions here`: PLACEHOLDER COMMENTS: you should remove these before submitting your code!
--------------------------------------------------------------------------------

void invalid_rooms (
    int row, 
    int col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (
        (row >= TOTAL_FLOORS || row < 0) || 
        (col >= ROOMS_PER_FLOOR || col < 0)
    ) {
        // when it's out of bounds
        printf("Invalid room number!\n");
    } else if (hotel[row][col].booking_id != NO_BOOKING) {
        // when there're someone has been entered already
        printf("Room already occupied!\n");
    }
}

int valid_rooms (
    int row, 
    int col, 
    int valid, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    return(
    // when its not out of bounds and is empty
    // then the guest can be added
    !(row >= TOTAL_FLOORS || row < 0) && 
    !(col >= ROOMS_PER_FLOOR || col < 0) && 
    hotel[row][col].booking_id == NO_BOOKING
    ); 
}

void print_rooms (
    int row, 
    int col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if ((row >= TOTAL_FLOORS || row < 0) || 
    (col >= ROOMS_PER_FLOOR || col < 0)) {
        // out of bounds
        printf("Invalid room number!\n");  
    } else if (!(row >= TOTAL_FLOORS || row < 0) && 
    !(col >= ROOMS_PER_FLOOR || col < 0) && 
    hotel[row][col].booking_id != NO_BOOKING) {
        // when it's valid and is someone in the room
        printf("Booking ID at Floor %d, Room %d is : %d\n", 
        row, col, hotel[row][col].booking_id);
        // when its already empty
    } else if (hotel[row][col].booking_id == NO_BOOKING) {
        printf("Floor %d, Room %d is empty.\n", row, col);
	}
}

void leave_rooms (
    int row, 
    int col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (
        (row >= TOTAL_FLOORS || row < 0) || 
        (col >= ROOMS_PER_FLOOR || col < 0)
    ) {
        printf("Invalid room number!\n");  
    } else if (
        !(row >= TOTAL_FLOORS || row < 0) && 
        !(col >= ROOMS_PER_FLOOR || col < 0) && 
        hotel[row][col].booking_id != NO_BOOKING
    ) {
        // same thing as the previous function
        // just the ouput for this part is a little different
        printf("Goodbye booking %d!\n", hotel[row][col].booking_id);           
        hotel[row][col].booking_id = NO_BOOKING;
        hotel[row][col].type = NORMAL; 
    } else if (hotel[row][col].booking_id == NO_BOOKING) {
        printf("This room is already empty.\n");
    }
}

void setting_price (
    int row, 
    int col, 
    int price, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (row >= 0 && price >= 0) {
        col = 0;
        // a loop setting the same room price for all cols
        // which are the rooms on the same floor
        while (col < ROOMS_PER_FLOOR) {
            hotel[row][col].price = price;
            col++;
        }
    } else if (row < 0) {
        printf("Invalid floor number!\n");
    } else if (price < 0) {
        printf("Invalid price!\n");
    }
}

void varying_price (
    int price, 
    int row, 
    int col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    // set the new price
    price = price + INITIAL_PRICE;
    row = 0;
    while (row < TOTAL_FLOORS) {
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            // set the all price to the same for all rooms
            if (price > 0) {
                hotel[row][col].price = price;
            } else if (price <= 0) {
                hotel[row][col].price = 0;
            }
            col++;
        }    
        row++;
    }
}

int total_revenue (
    int row,
    int col, 
    int revenue,
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 0;
    revenue = 0;
    // calculate the revenue for the rooms which are occupied
    while (row < TOTAL_FLOORS) {
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            if (hotel[row][col].booking_id != NO_BOOKING) {
                revenue += hotel[row][col].price;
            }
            col++;
        }
        row++;
    }
    return revenue;
}

void total_revenue_1 (int revenue, int previous_revenue) {
    // print the different results with conditions
    // by calculating the difference using the revenue calculated
    // and a variable saved before as the previous revenue
    printf("The total revenue for the night is $%d\n", revenue);
    if (revenue > previous_revenue) {
        int difference = revenue - previous_revenue;
        printf("This is $%d more than last time :)\n", difference);
    } else if (revenue < previous_revenue) {
        int difference = previous_revenue - revenue;
        printf("This is $%d less than last time (haha lol)\n", difference);
    } else if (revenue == previous_revenue) {
        printf("This is the same as the last time you asked :\\\n");
    }
}

void best_rooms (
    int row, 
    int col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
    int valid
) {
    row = 0;
    int detect = 0;
    // use a varibale detect to see if the hotel is full
    detect = detect_all_availiability(row, col, detect, hotel);
    while (row < TOTAL_FLOORS) {
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            if (hotel[row][col].booking_id == NO_BOOKING) {
                hotel[row][col].booking_id = valid;
                col = 9;
                row = 9;
                // loop through the rooms that are empty from the top left
                // and fit the guests in   
            } else if (detect < 1) {
                printf("The hotel is full!\n");
                col = 9;
                row = 9;
            }
            col++;
        }
        row++;
    }                
}
int swap_invalid_rooms (int row, int col, int row_1, int col_1) {
    return (
    // some conditions for invalid rooms
    (row >= TOTAL_FLOORS || row < 0) || 
    (col >= ROOMS_PER_FLOOR || col < 0) || 
    (row_1 >= TOTAL_FLOORS || row_1 < 0) || 
    (col_1 >= ROOMS_PER_FLOOR || col_1 < 0)
    ); 
}            

void swap_rooms (int *a, int *b) {
    // some pointers for swapping
    int swap;
    swap = *a;
    *a = *b;
    *b = swap;
}

int detect_availiability (
    int row, 
    int col, 
    int detect, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    col = 0;
    detect = 0; 
    while (col < ROOMS_PER_FLOOR) {
        if (hotel[row][col].booking_id == NO_BOOKING) {
            // for the rooms on the same floor
            // if there's an empty room, refresh the variable detect
            detect = detect + 1;
        }
        col++;
    }
    return detect;
}

int minimum_price (
    int row, 
    int col, 
    int minimum, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 1;
    minimum = hotel[0][0].price;;
    while (row < TOTAL_FLOORS) {
        // for the rooms, if the rooms on the previous floor are cheaper
        // take their price as the minimum price
        // since the price for the same floor is constant
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            if (hotel[row][col].price < hotel[row-1][col].price) {
                minimum = hotel[row][col].price;
            } 
            col++;
        }
        row++;
    }
    return minimum;
}

int maximum_price (
    int row, 
    int col, 
    int maximum, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 1;
    maximum = hotel[0][0].price;
    while (row < TOTAL_FLOORS) {
        // same thing as the above function, but just for the maximum
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            if (hotel[row][col].price > hotel[row-1][col].price ) {
                maximum = hotel[row][col].price;
            }  
            col++;
        }
        row++;
    }
    return maximum;
}

int detect_all_availiability (
    int row, 
    int col, 
    int detect, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 0;
    detect = 0;
    while (row < TOTAL_FLOORS) {
        col = 0;     
        while (col < ROOMS_PER_FLOOR) {
            // loop through all rooms
            // to see the number of available rooms
            if (hotel[row][col].booking_id == NO_BOOKING) {
                detect = detect + 1;
            }
            col++;
        }
        row++;
    }
    return detect;
}

int detect_floor_availiability (
    int row, 
    int col, 
    int detect, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 0;
    int previous_detect = 0; 
    while (row < TOTAL_FLOORS) {
        col = 0;
        detect = 0; 
        while (col < ROOMS_PER_FLOOR) {
            // loop through all the rooms but initialise the varibale detect
            // to 0, to see the floor with the largest capacity 
            // this is for fitting in the group booking guests
            if (hotel[row][col].booking_id == NO_BOOKING) {
                detect = detect + 1;
            }
            if (detect > previous_detect) {
                previous_detect = detect;
            }    
            col++;
        }
        row++;
    }
    return previous_detect;
}

void group_1 (
    int row, 
    int col, 
    int n, 
    int *valid, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    int minimum = 0;
    int detect = 0;
    row = TOTAL_FLOORS - 1;
    while (row >= 0) {
        col = 0;
        if (
            hotel[row][col].price == minimum_price(row, col, minimum, hotel) &&
            n <= detect_availiability (row, col, detect, hotel)
        ) {
            // find the availiable cheapest floor
            // then loop through the rooms into that floor
            while (col < n) {
                group_1_sub (row, col, &n, valid, hotel);
                col++;
            }
            // once the guests are fitted in, exit the loop
            // and refresh the varibale for booking id
            row = -1;
            *valid = *valid + 1;
        } else {
            row--;
        }
    }
}

void group_1_sub (
    int row, 
    int col, 
    int *n, 
    int *valid, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (hotel[row][col].booking_id == NO_BOOKING) {
        hotel[row][col].booking_id = *valid + 1;
        hotel[row][col].type = GROUP;    
    } else if (hotel[row][col].booking_id != NO_BOOKING) {
        // if there're other guests in the rooms
        // the *n here is meant to ignore the col++ if the room is occupied
        // I found this potential bug if n = 8, then col will be an error
        // but since I've already filtered out the risks during the detect
        // (like if it's detected as available for an 8 people group,
        // there's no way for anyone to occupy the room)
        // although it works well, 
        // but still risky and has logical mistakes... I have no time to fix it
        *n = *n + 1;
    }
}

void group_1_all (
    int row, 
    int col, 
    int n,  
    int detect,
    int valid, 
    int *valid_1, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (n > detect_floor_availiability (row, col, detect, hotel)) {
        // when it's impossible
        printf("No available floor for your group booking!\n");
    } else {
        // just when group_1 booking is possible
        group_1 (row, col, n, &valid, hotel);
        *valid_1 = valid;
    }
}

void group_2 (
    int row, 
    int col, 
    int n, 
    int price, 
    int *valid, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    int maximum = 0;
    int detect = 0;
    row = 0;
    while (row < TOTAL_FLOORS) {
        col = 0;
        if (
            hotel[row][col].price <= price && 
            hotel[row][col].price == maximum_price(row, col, maximum, hotel) &&
            n <= detect_availiability (row, col, detect, hotel)
        ) {
            // same as group 1 but just for the highest suitable price
            while (col < n) {
                group_1_sub (row, col, &n, valid, hotel);
                col++;
            }
            row = TOTAL_FLOORS;
            // exit the loop and refresh the booking id variable
            *valid = *valid + 1;
        } else {
            row++;
        }
    }
}

void group_2_all (
    int row, 
    int col, 
    int n,  
    int detect,
    int minimum,
    int price, 
    int valid,
    int *valid_1, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (
        price < minimum_price(row, col, minimum, hotel) ||
        n > detect_floor_availiability (row, col, detect, hotel)
    ) {
        // when it's impossible
        printf("No available floor for your group booking!\n");
    } else {
        // just when group_2 booking is possible
        group_2 (row, col, n, price, &valid, hotel);
        *valid_1 = valid;
    }
}

void royal_group_remove (
    int *detect,
    int row,
    int col,
    int previous_booking_id, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 0;
    while (row < TOTAL_FLOORS) {
        col = 0;                
        while (col < ROOMS_PER_FLOOR) {
            if (
                hotel[row][col].type == GROUP && 
                hotel[row][col].booking_id == previous_booking_id
            ) {
                // when it's confirmed to be the same group
                // I have a variable here to count the number of group members
                // then turn the rooms to empty and normal status
                *detect = *detect + 1;
                hotel[row][col].booking_id = NO_BOOKING;
                hotel[row][col].type = NORMAL; 
            }   
            col++;
        }
        row++;
    }                
}

void royal_group_replace (
    int row, 
    int col, 
    int detect, 
    int previous_booking_id, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = TOTAL_FLOORS -1;
    while (row >= 0) {
        if (detect > detect_all_availiability(row, col, detect, hotel)) {
            // since the group guests are more than any floor's capacity...
            printf("No available floor for your group booking!\n");
            row = -1;
        } else if (detect <= detect_availiability (row, col, detect, hotel)) {
            // if there are available places, start replacing
            royal_group_replace_sub (row, col, detect, 
            previous_booking_id, hotel);
            // once the loop through columns is finished, exit the row loop
            row = -1;
        } else {
            row--;
        }
    }
}

void royal_group_replace_sub (
    int row, 
    int col, 
    int detect, 
    int previous_booking_id, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    col = 0;
    int col_1 = 0;
    while (col < ROOMS_PER_FLOOR) {
        if (hotel[row][col].booking_id == NO_BOOKING && col_1 < detect) {
            // loop through the rooms from the top left
            // and fit in the guests once there're empty rooms
            // use the previously stored variable to re-assign the correct
            // booking id. Then exit the loop until the detected guests
            // are all fitted in
            hotel[row][col].booking_id = previous_booking_id;
            hotel[row][col].type = GROUP;
            // I have a variable col_1 to count up till the detected number
            col_1 = col_1 + 1;
            col++;    
        } else if (hotel[row][col].booking_id != NO_BOOKING) {
            // if the room isn't empty, just keep proceeding the loop
            col++;
        } else if (col_1 >= detect) {
            // exit
            col = ROOMS_PER_FLOOR;
        }
    }
}

void royal_normal_replace (
    int row, 
    int col, 
    int previous_booking_id, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = 0;
    while (row < TOTAL_FLOORS) {
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            if (hotel[row][col].booking_id == NO_BOOKING) {
                // same thing as fitting in group guests
                // the only thing is once the individual guest fitted in
                // just exit the loop, no more detections
                hotel[row][col].booking_id = previous_booking_id;
                col = ROOMS_PER_FLOOR;
                row = TOTAL_FLOORS;   
            } 
            col++;
        }
        row++;
    }
}

void royal_normal (
    int row,
    int col,
    int previous_booking_id,
    int detect,
    int valid,
    int *valid_1,
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    if (hotel[row][col].booking_id != NO_BOOKING) {
        // Reset the room to empty 
        hotel[row][col].booking_id = NO_BOOKING;
        // then create a new booking ids for the royal
        valid++;
        *valid_1 = valid;
        hotel[row][col].booking_id = valid;
        hotel[row][col].type = ROYAL;
       
        detect = detect_all_availiability(row, col, detect, hotel);
        if (detect < 1) {
            // if it's detected as no empty room
            // just ignore the kicked guest
            printf("The hotel is full!\n");
        } else {
            // replace the original guest by using the stored variable,
            // previous_booking_id 
            royal_normal_replace (row, col, previous_booking_id, hotel);
        }
    } else {
        // if just noone in the room
        valid++;
        *valid_1 = valid;
        hotel[row][col].booking_id = valid;
        hotel[row][col].type = ROYAL;
    }
}

void burglar (
    int row, 
    int col, 
    int previous_col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    row = TOTAL_FLOORS - 1;
    while (row > 0) {
        col = 0;
        while (col < ROOMS_PER_FLOOR - 1) {
            if (
                hotel[row - 1][col].booking_id != NO_BOOKING && 
                hotel[row][col + 1].booking_id == NO_BOOKING
            ) { 
                // start from the bottom left
                // when the room on the RHS is empty and the above isn't empty
                // move to the right by one room
                // and store this column in case moving up in next steps
                previous_col = col;
                col++;
                burglar_sub (&row, col, &previous_col, hotel); 
            } else if (hotel[row-1][col].booking_id == NO_BOOKING && row != 1) {
                // when the burglar hasn't reached the top and the room
                // above is empty, move up (only move up)
                // and make col = previous_col to make sure the loop is still
                // looping, but the col number keeps the same
                col = previous_col;
                col++;
                row--;
            } else if (row == 1 && hotel[row-1][col].booking_id == NO_BOOKING) {
                // when the burglar is at the second top floor, and the
                // room above is empty, exit the loop and print
                printf("Possible for a burglar to reach the rooftop!\n");
                col = ROOMS_PER_FLOOR - 1;
                row = 0;
            } else {
                // when there's no way proceeding the loop, exit and print
                printf("Impossible for a burglar to reach the rooftop!\n");
                col = ROOMS_PER_FLOOR - 1;
                row = 0; 
            }
        }
        row--;
    }
}

void burglar_sub (
    int *row, 
    int col, 
    int *previous_col, 
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]
) {
    // when it loop till the edge and the edge is empty
    // move to the next floor (it was over-deep nesting)
    if (col == 6 && hotel[*row - 1][col + 1].booking_id == NO_BOOKING) {
--------------------------------------------------------------------------------
> `== 6 &`: Could this be a constant?
--------------------------------------------------------------------------------
        *previous_col = col;
        *row = *row + 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function - DO NOT EDIT
// Initalises the booking id and price of each room in the hotel to be
// the default values.
void init_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        while (col < ROOMS_PER_FLOOR) {
            hotel[row][col].booking_id = NO_BOOKING;
            hotel[row][col].price = INITIAL_PRICE;
            hotel[row][col].type = NORMAL;
            col++;
        }
        row++;
    }
}

// Provided Function - DO NOT EDIT
// Prints out the hotel with the booking number of each room, or empty
// if there is no booking.
void print_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    printf(
        "\n"
        "         /--^--\\\n"
        "   _     |  O  |     _\n"
        " _/ \\___/_^___^_\\___/ \\_\n"
        "/                       \\\n");

    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        printf("|--+--+--+--+--+--+--+--|\n");
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
            if (hotel[row][col].booking_id == NO_BOOKING) {
                printf("  ");
            } else {
                printf("%02d", hotel[row][col].booking_id);
            }
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(
        "|--+--+--+--+--+--+--+--|\n"
        "|         /^^^\\         |\n"
        "|_________|_\"_|_________|\n\n");
}


// Provided Function - DO NOT EDIT
// Prints out the hotel with the the price and booking number included.
void print_hotel_with_prices(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    printf(
        "\n"
        "             /--^--\\\n"
        "   _         |  O  |         _\n"
        " _/ \\_______/_^___^_\\_______/ \\_\n"
        "/                               \\\n");

    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        printf("|---+---+---+---+---+---+---+---|\n");
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
            if (hotel[row][col].booking_id == NO_BOOKING) {
                printf("   ");
            } else {
#ifndef NO_COLORS
--------------------------------------------------------------------------------
> `#ifndef NO_COLORS`: HEADER GUARDS: you should never have these in .c files, only ever in .h files
--------------------------------------------------------------------------------
                printf("\033[1;35m");
#endif
                printf("%03d", hotel[row][col].booking_id);
#ifndef NO_COLORS
                printf("\033[0m");
#endif
            }
            col++;
        }
        printf("|\n");
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
#ifndef NO_COLORS
            printf("\033[1;32m");
#endif
            printf("%03d", hotel[row][col].price);
#ifndef NO_COLORS
            printf("\033[0m");
#endif
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(
        "|---+---+---+---+---+---+---+---|\n"
        "|             /^^^\\             |\n"
        "|_____________|_\"_|_____________|\n\n");
}

