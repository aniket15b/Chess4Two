#include<ncurses.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
struct position{
    int x;
    int y;
};
struct moves{
    struct position move;
    struct moves *next;
}*head;
int length = 0;
void push(int x, int y){
    struct position next;
    next.x = x;
    next.y = y;
    struct moves *nextmove = malloc(sizeof(struct moves));
    (*nextmove).move = next;
    nextmove->next = head;
    head = nextmove;
    length++;
}
int isEmpty() 
{ 
    return head == NULL; 
} 
struct position peek() 
{ 
    if (!isEmpty()) 
        return head->move; 
    else{
        struct position a;
        a.x = -1;
        a.y = -1;
        return a;
    }    
} 
void pop(){
    struct moves* temp;
    if (head == NULL) { 
        printf("Staack empty"); 
    } else {
        temp = head;
        head = head->next;
        temp->next = NULL;
        free(temp);
        length--;
    }
}
void display()  
{ 
    struct moves* temp; 
    if (head == NULL) { 
        printf("Empty Stack");
    }
    else { 
        temp = head; 
        while (temp != NULL) { 
            printf("x: %d, y: %d\n",temp->move.x, temp->move.y);
            temp = temp->next; 
        } 
    } 
}
int ch1;
int ch2;
int n;
void getinput(WINDOW* win){
    ch1 = wgetch(win);
    ch2 = wgetch(win);
}
void setboard(WINDOW* win){
    for(int i=0;i<2*n;i++){
        for (int j=0;j<2*n;j++)
        {
            if(i%2==0 && j%2==1){
                wmove(win, i, j);
                waddch(win, '|');
            } else if(i%2==1){
                wmove(win, i, j);
                waddch(win, '-');
            }
        }
    }
    wrefresh(win);
}
void draw(WINDOW* win, int board[][n], int n){
    werase(win);
    setboard(win);
    for(int i=0; i<n; i++){
        for (int j = 0; j<n; j++)
        {
            if(board[i][j]==1){
                wmove(win, 2*i, 2*j);
                waddch(win, '*');
            }
        }
    }
    wrefresh(win);
}
int isSafe(int board[][n], int row, int col) {  
    int i, j;  
    /* Check all rows*/  
    for (i = 0; i < n; i++)  
        if (board[row][i]) return 0;  
    /* Check all columns*/  
    for (i = 0; i < n; i++)  
        if (board[i][col]) return 0;  
    /* Check upper diagonal on left side */  
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)  
        if (board[i][j]) return 0;  
    /* Check lower diagonal on left side */  
    for (i = row, j = col; j >= 0 && i < n; i++, j--)  
        if (board[i][j]) return 0;  
    /* Check lower diagonal on right side */  
    for (i = row, j = col; j < 0 && i < 0; i++, j++)  
        if (board[i][j]) return 0;  
    /* Check upper diagonal on right side */  
    for (i = row, j = col; i >= 0 && j < n; i--, j++)  
        if (board[i][j]) return 0; 
    return 1;  
}  
int notin(int x, int y){
    struct moves* temp; 
    if (head == NULL) { 
        return 1;
    }
    else { 
        temp = head; 
        while (temp != NULL) { 
            if(temp->move.x == x && temp->move.y == y){
                return 0;
            }
            temp = temp->next; 
        }
        return 1;
    } 
}
void main(){
    printf("The rules are simple. Enter a value of N and fit N queens on an NxN chessboard such that they do not attack each other.\n");
    printf("Enter the x and y coordinates to place queen(0 to n-1).\n");
    printf("Enter qq to quit and bb to undo move");
    printf("\n N:= ");
    scanf("%d",&n);
    int board[n][n];
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            board[i][j] = 0;
        }
        
    }
    initscr();
    noecho();
    WINDOW * win = newwin(2*n, 2*n, 0, 0); 
    WINDOW * win2 = newwin(4, 4, 2*n, 2*n); 
    setboard(win);
    while (1)
    {
        getinput(win);
        if(ch1=='q'){
            break;
        }
        if(ch1=='b'){
            struct position temp = peek();
            board[temp.x][temp.y] = 0;
            pop();
        }
        if(ch1 != 'b' && ch1-'0'<n && ch2-'0'<n && ch1-'0'>=0 && ch2-'0'>=0){
            wmove(win2, 0, 0);
            waddch(win2, ch1);
            wmove(win2, 0, 2);
            waddch(win2, ch2);
            wrefresh(win2);
            if(isSafe(board, ch1-'0', ch2-'0') && notin(ch1-'0', ch2-'0')){
                board[ch1-'0'][ch2-'0'] = 1;
                push(ch1-'0',ch2-'0');
            }
        }
        draw(win, board, n);
        if(length==n){
            break;
        }
    }
    if(length==n){
        printf("\n\n\n\nYOU WIN!!!");
    }
    delwin(win);
}