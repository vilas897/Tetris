#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <time.h>
#include <vector>
#include <sstream>

using namespace std;

// need to define
static const int bgcolor= 97;
static const int tilesize = 30;
static const double refreshtime = 0.5;
static const int rowmax = 20; //change these accordingly
static const int colmax = 14;
static const int nextboxcolor = 47;
static int partcolor =177;

	void timeractions(void *p);

class MyTile{   // this represent single pixel for one piece. Each piece has 4 tiles.

	int row, col; // x,y value for each pixel

	public:

	void setrow(int x) { row = x; }  // these two define position of tile
	void setcol(int y) { col = y; }

	void changerow(int x) { row = row + x; }// these two modify the position of the tile
	void changecol(int y) { col = col + y; }

	int getrow() { return row; } // functions to return private variables
	int getcol() { return col; }
};

class MyPiece
{ // this object represents each piece on the board

	MyTile tiles[4]; // Each piece has four tiles/pixels
	int color; 		 // color of the tile
	int type; 		 // 7 types of pieces


	public:
		MyPiece(){
			srand(time(NULL));
			color = rand() % 6 + 1;

			int mid = colmax/2;
			int choice = rand() % 7;
			// Creating the 7 pieces
			switch(choice)
			{
				// ---- (I)

				case 0: tiles[0].setrow(0); tiles[0].setcol(mid - 2);
						tiles[1].setrow(0);	tiles[1].setcol(mid - 1);
						tiles[2].setrow(0);	tiles[2].setcol(mid);
						tiles[3].setrow(0);	tiles[3].setcol(mid + 1);
						type = 0;
						break;
				// ---, (J)
				case 1: tiles[0].setrow(0); tiles[0].setcol(mid - 1);
						tiles[1].setrow(0);	tiles[1].setcol(mid);
						tiles[2].setrow(0);	tiles[2].setcol(mid + 1);
						tiles[3].setrow(1);	tiles[3].setcol(mid + 1);
						type = 1;
						break;
				// ,--- (L)
				case 2: tiles[0].setrow(0); tiles[0].setcol(mid);
						tiles[1].setrow(0);	tiles[1].setcol(mid-1);
						tiles[2].setrow(0);	tiles[2].setcol(mid-2);
						tiles[3].setrow(1);	tiles[3].setcol(mid-2);
						type = 2;
						break;
				// --
				// -- (o) this one can't be rotated
				case 3: tiles[0].setrow(0); tiles[0].setcol(mid);
						tiles[1].setrow(0);	tiles[1].setcol(mid-1);
						tiles[2].setrow(1);	tiles[2].setcol(mid);
						tiles[3].setrow(1);	tiles[3].setcol(mid-1);
						type = 3;
						break;
				//   --
			  // --
				case 4: tiles[0].setrow(0); tiles[0].setcol(mid);
						tiles[1].setrow(0);	tiles[1].setcol(mid+1);
						tiles[2].setrow(1);	tiles[2].setcol(mid);
						tiles[3].setrow(1);	tiles[3].setcol(mid-1 );
						type = 4;
						break;

				// -,-

				case 5: tiles[0].setrow(0); tiles[0].setcol(mid);
						tiles[1].setrow(0);	tiles[1].setcol(mid+2);
						tiles[2].setrow(0);	tiles[2].setcol(mid+1);
						tiles[3].setrow(1);	tiles[3].setcol(mid+1);
						type = 5;
						break;
				// --
				//   --
				case 6: tiles[0].setrow(0); tiles[0].setcol(mid);
						tiles[1].setrow(0);	tiles[1].setcol(mid+1);
						tiles[2].setrow(1);	tiles[2].setcol(mid+1);
						tiles[3].setrow(1);	tiles[3].setcol(mid+2);
						type = 6;
						break;
			}
		}

  int get(unsigned int i , unsigned int var)
	{ 		//1 means row and 2 means col
		if(var == 1)
			return tiles[i].getrow() ;
		else if(var == 2)
			return tiles[i].getcol() ;
		else
			return -1 ;
	}

	int getColor()
	{  		//returns color of active piece
		return color ;
	}

	char getType()
	{		//returns type of active piece
		return type ;
	}

  void movedown()
	{
  	for(int i=0; i<4; i++) tiles[i].changerow(1);
  }

  bool can_move_right()
	{
    for(int i=0; i<4;i++)
		{
    	if(tiles[i].getcol() == colmax - 1)
				return false;
    }
    return true;
  }

  bool can_move_left()
	{
    for(int i=0; i<4;i++)
		{
    	if(tiles[i].getcol() == 0) return false;
    }
    return true;
  }

  void moveright(bool filled[rowmax][colmax])
	{
		try
		{
	    if(!can_move_right())
			{
				throw 1;
	    }
			// check boundary condition
			for(int i=0;i<4;i++)
			{
				if(filled[tiles[i].getrow()][tiles[i].getcol() + 1] == true) // is there already a piece ?
					return;
			}
			// possible to move
			for(int i=0; i<4; i++)
				tiles[i].changecol(1);
		}
		catch(int x)
		{
			partcolor = x;
		}
  }
  void moveleft(bool filled[rowmax][colmax])
	{
		try
		{
	    if(!can_move_left())
			{
				throw 1;
	    }
			// check boundary condition
			for(int i=0;i<4;i++)
			{
				if(filled[tiles[i].getrow()][tiles[i].getcol() - 1] == true) // is there already a piece ?
					return;
			}
			// possible to move
			for(int i=0; i<4; i++)
				tiles[i].changecol(-1);
		}
		catch(int x)
		{
			partcolor = x;
		}
  }

  bool can_rotate_cw(int afterrotate[][2], bool filled[rowmax][colmax], int pivr, int pivc){
  		int coldiff[4];
  		int rowdiff[4];
  		for(int i=0;i<4;i++){
  			rowdiff[i] = pivr - tiles[i].getrow();
  			coldiff[i] = tiles[i].getcol() - pivc;
  		}

  		for(int i=0;i<4;i++){
  			afterrotate[i][0] = pivr + coldiff[i]; // new row
  			afterrotate[i][1] = pivc + rowdiff[i]; // new col
  		}

  		for(int i = 0 ; i < 3 ; i++) {
			//check boundary
			if(afterrotate[i][0] >= rowmax || afterrotate[i][0] <= 0 || afterrotate[i][1] >= colmax || afterrotate[i][1] <= 0
			   || filled[afterrotate[i][0]][afterrotate[i][1]] == true)
				return false ;
		}
		return true ;
  }

	bool can_rotate_acw(int afterrotate[][2], bool filled[rowmax][colmax], int pivr, int pivc)
	{
		int coldiff[4];
		int rowdiff[4];
		for(int i=0;i<4;i++){
			rowdiff[i] = pivr - tiles[i].getrow();
			coldiff[i] = tiles[i].getcol() - pivc;
		}
		for(int i=0;i<4;i++)
		{
			afterrotate[i][0] = pivr - coldiff[i]; // new row
			afterrotate[i][1] = pivc - rowdiff[i]; // new col
		}

		for(int i = 0 ; i < 3 ; i++) {
		//check boundary
		if(afterrotate[i][0] >= rowmax || afterrotate[i][0] <= 0 || afterrotate[i][1] >= colmax || afterrotate[i][1] <= 0
			 || filled[afterrotate[i][0]][afterrotate[i][1]] == true)
			return false ;
		}
		return true ;
	}

  void rotate_cw(bool filled[rowmax][colmax])
	{
  		int afterrotate[4][2];
  		int pivr,pivc;
  		// We choose tiles[2] as pivot and rotate wrt it.
  		pivr = tiles[2].getrow();
  		pivc = tiles[2].getcol();
  		if(type!=3)
			{
				try
				{
	  			if(!can_rotate_cw(afterrotate, filled, pivr, pivc))
					{
	  				throw 1;
	  			}
					tiles[0].setrow(afterrotate[0][0]) ; tiles[0].setcol(afterrotate[0][1]) ;
					tiles[1].setrow(afterrotate[1][0]) ; tiles[1].setcol(afterrotate[1][1]) ;
					tiles[3].setrow(afterrotate[3][0]) ; tiles[3].setcol(afterrotate[3][1]) ;
				}
				catch(int x)
				{
					partcolor = x;
				}
  		}
  }

	void rotate_acw(bool filled[rowmax][colmax])
	{
		int afterrotate[4][2];
		int pivr,pivc;
		// We choose tiles[2] as pivot and rotate wrt it.
		pivr = tiles[2].getrow();
		pivc = tiles[2].getcol();

		if(type!=3)
		{
			try
			{
				if(!can_rotate_acw(afterrotate, filled, pivr, pivc))
				{
					throw 1;
				}
				for(int i=0;i<3;i++)
				{
					tiles[0].setrow(afterrotate[0][0]) ; tiles[0].setcol(afterrotate[0][1]) ;
					tiles[1].setrow(afterrotate[1][0]) ; tiles[1].setcol(afterrotate[1][1]) ;
					tiles[3].setrow(afterrotate[3][0]) ; tiles[3].setcol(afterrotate[3][1]) ;
				}
			}
			catch(int x)
			{
				partcolor = x;
			}
		}
	}
};

class Board : public Fl_Widget
{
 	bool filled[rowmax][colmax]; // array that uses 1 to denote presence of a piece and 0 to denote absence
  int fillcolor[rowmax][colmax]; // array to hold color of each color piece
  int nextPiece[5][5]; // grid that displays the next piece
  char* scoreLabel ;	//score label
	Fl_Box* scoreBox ;	//score box
	int score ;

  MyPiece currentpiece, nextpiece; // objects of class Mypiece

 public:

  Board(): Fl_Widget(0, 0, colmax * tilesize + 300, rowmax * tilesize, "Tetris")
  {
    int i,j;

    for(i=0; i<rowmax; i++)
      for(j=0; j<colmax; j++)
    	{	filled[i][j] = false;
        fillcolor[i][j] = bgcolor;
    	}

    for(i=0; i<5; i++)
      for(j=0; j<5; j++)
      nextPiece[i][j] = nextboxcolor;

    score = 0;
  }

	 void setScore(Fl_Box **s)
 	{
    scoreBox = *s;
		score = 0;
		scoreLabel = (char*)malloc(sizeof(char)*10);
 	}

 private:
  bool candown() // Return true if the piece can go down
  {
    for(int i=0; i<4; i++)
    {
			if((currentpiece.get(i , 1) >= rowmax) || (filled[currentpiece.get(i , 1) + 1][currentpiece.get(i , 2)] == true) || (fillcolor[currentpiece.get(i , 1) + 1][currentpiece.get(i , 2)] != bgcolor && fillcolor[currentpiece.get(i , 1) + 1][currentpiece.get(i , 2)] != currentpiece.getColor()))
      {
        return false;
      }
    }
    return true;
  }

 	void draw()				//Draws the entire board
  {
    int i, j;

    for(i=0; i<rowmax; i++)
      for(j=0; j<colmax; j++)
    	{
        if(fillcolor[i][j] != bgcolor)
          fl_draw_box(FL_BORDER_BOX, 5 + j*tilesize, i*tilesize-5, tilesize, tilesize, fillcolor[i][j]);
				else
          fl_draw_box(FL_FLAT_BOX, 5 + j*tilesize, i*tilesize-5, tilesize, tilesize, fillcolor[i][j]);
    	}

  //Next Piece Box
  	for(i = 0; i<5; i++)
			for(j = 0; j<5; j++)
     {
				if(nextPiece[i][j] != nextboxcolor)
					fl_draw_box(FL_BORDER_BOX, (j + colmax + 4)*tilesize, (i + rowmax/2 + 2)*tilesize, tilesize, tilesize, nextPiece[i][j]);
				else
					fl_draw_box(FL_FLAT_BOX, (j + colmax + 4 )*tilesize, (i + rowmax/ 2 + 2)*tilesize , tilesize, tilesize, nextPiece[i][j]);
			}


  //For the partition border
		fl_draw_box(FL_FLAT_BOX, colmax*tilesize + 5, 0, 5, rowmax*tilesize, partcolor);
		fl_draw_box(FL_FLAT_BOX, 0, 0, 5, rowmax*tilesize, partcolor);
		fl_draw_box(FL_FLAT_BOX, 5, 0, colmax*tilesize, 5, partcolor);
		fl_draw_box(FL_FLAT_BOX, 5, rowmax*tilesize-5, colmax*tilesize, 5, partcolor);
  }

  void transferTiles()
  {
    int i;

		for(i = 0; i<4 ; i++)
    {
			fillcolor[currentpiece.get(i , 1)][currentpiece.get(i , 2)] = currentpiece.getColor() ;
			filled[currentpiece.get(i , 1)][currentpiece.get(i , 2)] = true ;
		}

		deleteRow() ; //delete all the rows that are complete
		draw() ; //redraw the board
	}

  void deleteRow()
  {
    bool valid;
    int i, j, k;

    for(i=0; i<rowmax; i++)
    {
      valid = true;

      for(j=0; j<colmax; j++)
        if(filled[i][j] == false)
        	valid = false;

      if(valid)
      {
        //Shift all the rows by 1 step down from that row;

        for(j = i; j>=1; j--)
        {
       			for(k = 0; k < colmax; k++)
            {
							filled[j][k] = filled[j - 1][k];
							fillcolor[j][k] = fillcolor[j - 1][k] ;
						}
        }

        for(k = 0; k<colmax; k++)
        {
          filled[0][k] = false;
          fillcolor[0][k] = bgcolor;
        }

        score += 10;
    	}
    }
  }

	void drawTile() // transfers color values from the matrix to the board
  {
    for(int i=0;i<4;i++)
    {
      fillcolor[currentpiece.get(i , 1)][currentpiece.get(i , 2)] = currentpiece.getColor();
    }
  }


  int handle(int e)
  {
    if(e == 12)
    {

      switch(Fl::event_key())
      {
        case FL_Escape: exit(1);
        case FL_Left:{
          							deleteTile();
          							currentpiece.moveleft(filled);
          							drawTile();
          							break;
        							}
        case FL_Right:{
          							deleteTile();
          							currentpiece.moveright(filled);
          							drawTile();
          							break;
        							}
        case FL_Up:		{
          							deleteTile();
          							currentpiece.rotate_cw(filled);
          							drawTile();
          							break;
        							}
        case FL_Down: {
          							deleteTile();
          							currentpiece.rotate_acw(filled);
          							drawTile();
          							break;
        							}
        case 32:			{
          							while(candown() == true)
                        {
                          deleteTile();
                          currentpiece.movedown();
                          drawTile();
                        }
        							}
      }
    }
    redraw(); // draws the changed board
  }

	bool generatePiece() // function to generate new piece
	{
    currentpiece = nextpiece; // next piece now becomes the current piece
    for(int i=0;i<4;i++)
    {
      if(fillcolor[currentpiece.get(i,1)][currentpiece.get(i,2)] != bgcolor)
        return false; // piece cannot be generated
    }
    return true; // piece generated successfully
  }

  void deleteTile()
  {
    for(int i=0;i<4;i++)
    {
      fillcolor[currentpiece.get(i , 1)][currentpiece.get(i , 2)] = bgcolor;
    }
  }

	void periodic()
	{
		partcolor =177;
	  	if(candown())
	    { deleteTile();
				currentpiece.movedown();
				drawTile();
	    }
	    	else
	      { transferTiles();
	        if(!generatePiece())
	        { 	stringstream strs;
							strs << (int)score;
							string temp_str = strs.str();
							strcpy(scoreLabel,"Gameover. Score : ");
							strcat(scoreLabel,(char*) temp_str.c_str() );

						scoreBox->label(scoreLabel);
						redraw() ;
						Fl :: wait() ;
						exit(1) ;
						Fl::repeat_timeout (0.5,timeractions,this);
	    		}

	       MyPiece newPiece;
	       nextpiece = newPiece;
	       int i, j;

	       for(i = 0; i < 5; i++)
	      	for(j = 0 ; j < 5; j++)
	        	nextPiece[i][j] = nextboxcolor;

	       switch(nextpiece.getType())
	       {
	        case 0: nextPiece[2][0] = nextPiece[2][1] = nextPiece[2][2] = nextPiece[2][3] = nextpiece.getColor();
	                break;
	        case 1: nextPiece[1][1] = nextPiece[1][2] = nextPiece[1][3] = nextPiece[2][3] = nextpiece.getColor();
									break ;
					case 2: nextPiece[1][1] = nextPiece[1][2] = nextPiece[1][3] = nextPiece[2][1] = nextpiece.getColor();
	         				break;
	        case 3: nextPiece[2][1] = nextPiece[2][2] = nextPiece[3][1] = nextPiece[3][2] = nextpiece.getColor();
	         				break;
	        case 4: nextPiece[1][2] = nextPiece[1][3] = nextPiece[2][1] = nextPiece[2][2] = nextpiece.getColor();
	         				break;
	        case 5: nextPiece[1][1] = nextPiece[1][2] = nextPiece[1][3] = nextPiece[2][2] = nextpiece.getColor();
	         				break;
	        case 6: nextPiece[1][1] = nextPiece[1][2] = nextPiece[2][2] = nextPiece[2][3] = nextpiece.getColor();
	         			  break;
	       }
	    }
		  stringstream strs;
			strs << (int)score;
			string temp_str = strs.str();
			strcpy(scoreLabel,"Score : ");
			strcat(scoreLabel,(char*) temp_str.c_str() );

			scoreBox->label(scoreLabel);
			redraw() ;
			Fl::repeat_timeout(refreshtime,timeractions,this);
		}
	  friend void timeractions(void* p);
	};

void timeractions(void *p)
{ //this is the function that provides periodicity to the program
	((Board *)p)->periodic ();
}


int main()
{
  Fl_Window *window = new Fl_Window(800 , rowmax * tilesize , "TETRIS");

  window -> color(bgcolor);

  Board* B = new Board();

  Fl_Box* scorebox = new Fl_Box(tilesize*colmax+40,100,320,80,"Score : 0\0") ;

  scorebox -> box(FL_OVAL_BOX) ;
  scorebox -> labelfont(FL_BOLD) ;
  scorebox -> labeltype(FL_NORMAL_LABEL) ;
  scorebox -> labelsize(30) ;
  scorebox -> color(55);

  B-> setScore(&scorebox);

  window->end() ;
  window->show() ;

  Fl::add_timeout(0.1, timeractions,B) ;
  return(Fl::run()) ;
}
