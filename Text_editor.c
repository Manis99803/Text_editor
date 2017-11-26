#include <stdio.h>
#include <stdlib.h>

#define OP_INSERT_CHAR 		'I' //Insert
#define OP_PRESSED_ENTER 	'N' //Newline
#define OP_PRESSED_DELETE 	'D' //Delete 
#define OP_PRESSED_LEFT 	'h' //Left arrow equivalent in vi editor
#define OP_PRESSED_DOWN 	'j' //Down arrow equivalent in vi editor
#define OP_PRESSED_UP 		'k' //Up arrow equivalent in vi editor
#define OP_PRESSED_RIGHT 	'l' //Right arrow equivalent in vi editor
#define OP_PRESSED_HOME 	'H' //Home
#define OP_PRESSED_END 		'E' //End
#define OP_PRINT_LINE 		'P' //Print the current line
#define OP_PRINT_TEXT 		'T' //print the whole Text
#define OP_QUIT 			'Q' //Quit the editor

//Forward declaration; see the definition of struct for more details.
struct text_node;

//A node in 
struct line_node {
	char ch;
	struct line_node *next_char;
	struct line_node *prev_char;
	struct text_node *line_header;
};

struct text_node {
	struct line_node *line_begin;
	struct text_node *next_line;
	struct text_node *prev_line;
};

//Creates the empty text. 
//Empty text has one empty line, which has a newline '\n' as end of line.
struct text_node *create_text(struct line_node **pcursor);

//Free the whole text
void free_text(struct text_node *text);

//Insert a char just before the cursor. 
//Cursor always points to a line_node. 
//A line has a newline char as the end of line.
struct line_node *op_insert_char(struct line_node *cursor, char ch);

//“Enter” (i.e. create) a new line and the cursor moves to the new line. 
//If the cursor was at the end of line, a new empty line is created, otherwise 
//the current line is split into a new line starting from the character at the cursor.
struct line_node *op_pressed_enter(struct line_node *cursor);

//“Delete” a character at the cursor, or the end of line if the cursor is 
//at the end of a line. Deleting an end of line is essentially concatenating 
//the next line with the current line. 
//No operation if the cursor is at the end of line of the last line.
struct line_node *op_pressed_delete(struct line_node *cursor);

//Move the cursor “Left” by a character (or to the end of line of 
//above line if the cursor is at the beginning of a line). 
//No operation if the cursor is the beginning of the first line.
struct line_node *op_pressed_left(struct line_node *cursor);

//Move the cursor “Down” to the below line (beginning of the below line). 
//No operation if the cursor is already at the last line.
struct line_node *op_pressed_down(struct line_node *cursor);

//Move the cursor “Up” to the above line (beginning of the above line). 
//No operation if the cursor is already at the first line.
struct line_node *op_pressed_up(struct line_node *cursor);

//Move the cursor “Right” by a character (or to the beginning of below line 
//if the cursor is at the end of a line). 
//No operation if the cursor is the end of line of the last line.
struct line_node *op_pressed_right(struct line_node *cursor);

//Move the cursor to “Home”, that is, beginning of the current line. 
//No operation if the cursor is at the beginning of a line.
struct line_node *op_pressed_home(struct line_node *cursor);

//Move the cursor to “End”, that is, end of line the current line. 
//No operation if the cursor is at the end of a line.
struct line_node *op_pressed_end(struct line_node *cursor);

//“Print Line” prints the current line.
void op_print_line(struct line_node *cursor);

//“Print Text” prints the whole text.
void op_print_text(struct text_node *text);

int main() {
	struct line_node *cursor;
	struct text_node *text = create_text(&cursor);
	char op[4];

	do {
	    op[2] = ' ';
		scanf(" %[^\n]", op);
		switch(op[0]) {
	        case OP_INSERT_CHAR:
	            cursor = op_insert_char(cursor, op[2]);
	            break;
	        case OP_PRESSED_ENTER:
	            cursor = op_pressed_enter(cursor);
	            break;
	        case OP_PRESSED_DELETE:
	            cursor = op_pressed_delete(cursor);
	            break;
	        case OP_PRESSED_LEFT:
	            cursor = op_pressed_left(cursor);
	            break;
	        case OP_PRESSED_DOWN:
	            cursor = op_pressed_down(cursor);
	            break;
	        case OP_PRESSED_UP:
	            cursor = op_pressed_up(cursor);
	            break;
	        case OP_PRESSED_RIGHT:
	            cursor = op_pressed_right(cursor);
	            break;
	        case OP_PRESSED_HOME:
	            cursor = op_pressed_home(cursor);
	            break;
	        case OP_PRESSED_END:
	            cursor = op_pressed_end(cursor);
	            break;
	        case OP_PRINT_LINE:
	            op_print_line(cursor);
	            break;
	        case OP_PRINT_TEXT:
	            op_print_text(text);
	            break;	        
	    }
	} while(op[0] != OP_QUIT);
	
	free_text(text);
	return 0;
}
struct text_node *create_text(struct line_node **pcursor)
{
    struct text_node *newText = (struct text_node *)malloc(sizeof(struct text_node));
    newText->prev_line = NULL;
    newText->next_line = NULL;
    *pcursor = (struct line_node *)malloc(sizeof(struct line_node));
    (*pcursor)->ch = '\n';
    (*pcursor)->prev_char = NULL;
    (*pcursor)->next_char = NULL;
    (*pcursor)->line_header = newText;
    newText->line_begin = *pcursor;
    return newText;
     
}
void free_text(struct text_node *text)
{
    free(text);
}
struct line_node *op_insert_char(struct line_node *cursor,char ch)
{
    struct line_node *newChar = (struct line_node *)malloc(sizeof(struct line_node));

    newChar->ch = ch;
    if(cursor->prev_char == NULL)
    {
    	op_pressed_home(cursor)->line_header->line_begin = newChar;
        newChar->prev_char = cursor->prev_char;
        newChar->next_char = cursor;
        newChar->line_header = cursor->line_header;
        cursor->line_header = NULL;
        cursor->prev_char = newChar;
        return cursor;
    }
    else
    {

        newChar->prev_char = cursor->prev_char;
        newChar->next_char = cursor;
        newChar->line_header = cursor->line_header;
        cursor->prev_char->next_char = newChar;
        cursor->prev_char = newChar;
        return cursor;
    }
}

struct line_node *op_pressed_enter(struct line_node *cursor)
{
    struct line_node *newCursor;
    struct text_node *newLine = create_text(&newCursor);
    //printf("address of cursor = %p\n",cursor);
    if(cursor->ch == '\n')
    {
    	if(cursor->prev_char!=NULL)
    	{
    		cursor->line_header = newLine;
    	}
        newLine->prev_line = op_pressed_home(cursor)->line_header;
        op_pressed_home(cursor)->line_header->next_line = newLine;
        newLine->next_line = NULL;
		return newCursor;
    }
    struct line_node *backslash = newLine->line_begin;
    newLine->prev_line = op_pressed_home(cursor)->line_header;
    op_pressed_home(cursor)->line_header->next_line = newLine;
    newLine->next_line = NULL;
    newLine->line_begin = cursor;
    struct line_node *temp = newLine->line_begin;
    while(temp->next_char!=NULL)
    {
    	temp = temp->next_char;
    }
    temp->next_char = backslash;
    backslash->prev_char = temp;
   
    cursor->prev_char->next_char = NULL;
    cursor->prev_char->line_header = newLine;
    newCursor = newLine->line_begin;
    cursor->prev_char = NULL;
    return newCursor;
}

struct line_node *op_pressed_home(struct line_node *cursor)
{
    struct line_node *temp_cursor = cursor;
	//printf("temp->cursor->ch in home = %c\n",temp_cursor->ch);    
    while(temp_cursor->prev_char!=NULL)
    {
    	//printf("Holding\n");
        temp_cursor = temp_cursor->prev_char;
    }

    return temp_cursor;
}

struct line_node *op_pressed_end(struct line_node *cursor)
{
	struct line_node *temp_cursor = cursor;
	while(temp_cursor->ch!='\n')
	{
		temp_cursor = temp_cursor->next_char;
	}
	//printf("temp->cursor = %p\n temp_cursor->ch = %c\n",temp_cursor,temp_cursor->ch);
	return temp_cursor;
}

void op_print_line(struct line_node *cursor)
{
    if(cursor->prev_char == NULL && cursor->ch == '\n')
    {
    	return;
    }
    //printf("%d\n",cursor->ch);
    struct line_node *temp_cursor = op_pressed_home(cursor);
    //printf("temp_cursor->ch = %c\n",temp_cursor->ch);
    while(temp_cursor->ch!='\n')
    {
        printf("%c",temp_cursor->ch);
        temp_cursor = temp_cursor->next_char;
        
    }
    printf("\n");
}

void op_print_text(struct text_node *text)


	struct line_node *newCursor;
	while(text!=NULL)
	{
		newCursor = text->line_begin;
		op_print_line(newCursor);
		text = text->next_line;
	}

}

struct line_node *op_pressed_delete(struct line_node *cursor)
{
    if(cursor->ch == '\n')
    {
    	if(cursor->line_header == NULL)
    	{
    		//free(cursor);
    		return cursor;
    	}
    	if(cursor->prev_char == NULL)
    	{
    		if(cursor->line_header->prev_line == NULL)
    		{
    			return cursor;
    		}
    		struct line_node *newCursor = op_pressed_end(cursor->line_header->prev_line->line_begin);
    		free(cursor);
    		return newCursor;
    	}
    	struct line_node *nextLine = cursor->line_header->line_begin;
    	struct line_node *prevCursor = cursor->prev_char;
    	prevCursor->next_char = nextLine;
    	prevCursor->line_header->next_line = prevCursor->line_header->next_line->next_line;
    	free(cursor);
    	cursor = prevCursor;
    	return cursor;
    }
    	
    if(cursor->prev_char == NULL)
    {
    	struct line_node *newLine = cursor->next_char;
    	//printf("newLine->ch = %c\n",newLine->ch);
    	newLine->line_header = cursor->line_header;
    	//printf("Free cursor\n");
    	free(cursor);
    	newLine->prev_char = NULL;
    	return newLine;
    }
    cursor->next_char->prev_char = cursor->prev_char;
    cursor->prev_char->next_char = cursor->next_char;
    struct line_node *prevCursor = cursor->prev_char;
    free(cursor);
    cursor = prevCursor->next_char;
    return cursor;
    
}

struct line_node *op_pressed_left(struct line_node *cursor)
{
	if(op_pressed_home(cursor) == cursor && op_pressed_home(cursor)->line_header->prev_line == NULL)
	{
		return cursor;
	}
	if(op_pressed_home(cursor) == cursor)
	{
		//printf("Hello\n");
		cursor = op_pressed_end(cursor->line_header->prev_line->line_begin);
		return cursor;
	}
	cursor = cursor->prev_char;
	return cursor;
}

struct line_node *op_pressed_right(struct line_node *cursor)
{
	if(cursor == op_pressed_end(cursor))
	{
		if(cursor->line_header == NULL)
		{
			return cursor;
		}
		else
		{
			cursor = cursor->line_header->line_begin;
			return cursor;
		}
	}
	cursor = cursor->next_char;
	return cursor;

}

struct line_node *op_pressed_up(struct line_node *cursor)
{
	struct line_node *temp_cursor = op_pressed_home(cursor);
	if(temp_cursor->line_header->prev_line == NULL)
	{
		return cursor;
	}
	struct text_node *prevNode = temp_cursor->line_header->prev_line;
	temp_cursor = temp_cursor->line_header->prev_line->line_begin;
	//printf("temp_cursor = %p\n",temp_cursor);
	temp_cursor = op_pressed_home(temp_cursor);
	return temp_cursor;
}

struct line_node *op_pressed_down(struct line_node *cursor)
{

	struct line_node *temp_cursor = op_pressed_home(cursor);
	struct text_node *sameLine = temp_cursor->line_header;
	if(temp_cursor->line_header->next_line == NULL)
	{
		return cursor;
	}

	temp_cursor = temp_cursor->line_header->next_line->line_begin;
	return temp_cursor;
}