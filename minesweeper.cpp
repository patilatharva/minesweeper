// minesweeper

#include<iostream.h>
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<process.h>
#include<constrea.h>
#include<time.h>
#include<iomanip.h>
#include<fstream.h>
#include<string.h>

const int rows=10, cols=10; //dimensions of Minesweeper field
const int chance=5; // no. of tiles out of which 1 is a mine

char field[rows][cols];
char status[rows][cols];
int i, j, defeat=0;

clock_t start, end; // starting and ending time
float gameTime; // time taken to complete game

void startGame(); // starts a new game
void placeMines(); // randomly places mines on field
void placeClues(); // non-mine tiles indicate no. of mines around them
void display(); // displays the field after each action
void check(); // checks if the user has won
void revealMines(); // reveals all mines on defeat
void addScore(); // adds score (time) to data file

class score // stores player name and game time
{
	float gtime;
	char player[20];

	public:
		void clean()
		{
			gtime = NULL;
			strcpy(player, "");
		}

		void getData()
		{
			gtime = gameTime;
			cout << "\nPlayer Name: ";
			gets(player);
		}

		void putData()
		{
			cout << setprecision(2) << gtime << '\t' << player;
		}

		float getTime()
		{
			return gtime;
		}
}newscore, record;

void addScore() // adds score (time) to data file
{
	newscore.clean();
	newscore.getData();
	getch();

	ifstream filin("scores.dat", ios::nocreate|ios::binary);
	char last='y';

	if(filin) // if scores.dat already exists
	{		
		ofstream filout("temp.dat", ios::out|ios::binary);

		while(!filin.eof())
		{
			filin.read((char*)&record, sizeof(record));
			if(newscore.getTime()<=record.getTime())
			{
				cout << "\nif1";
				filout.write((char*)&newscore, sizeof(newscore));
				last='n';
				break;
			}

			else
			{
				filout.write((char*)&record, sizeof(record));
				cout << "\nelse1";
			}
		}

		if(last=='y')
		{
			cout << "\nif2";
			filout.write((char*)&newscore, sizeof(newscore));
		}
		
		else if (!filin.eof())
		{
			while(!filin.eof())
			{
				cout << "\nelse2";
				filin.read((char*)&record, sizeof(record));
				filout.write((char*)&record, sizeof(record));
			}
		}
		filin.close();
		filout.close();
		remove("scores.dat");
		rename("temp.dat", "scores.dat");
	}

	else // scores.dat is created if it doesn't exist
	{
		ofstream filout("scores.dat", ios::noreplace|ios::binary);
		filout.write((char*)&newscore, sizeof(newscore));
		filout.close();
	}

	startGame(); // start next game
}

void placeMines() // randomly places mines on field
{
	randomize();
	for(i=0; i<rows; i++)
	{
		for(j=0; j<cols; j++)
		{
			if(random(chance-1)==0)
				field[i][j] = '*';
			status[i][j] = 0;
		}
	}
}

void placeClues() // non-mine tiles indicate no. of mines around them
{
	int count=0;

	for(i=0; i<rows; i++)
	{
		for(j=0; j<cols; j++)
		{
			if(field[i][j]=='*')
				continue;

			if(i!=0)
			{
				if(j!=0&&field[i-1][j-1]=='*')		// top left
					count++;
				if(field[i-1][j]=='*')			// above
					count++;
				if(j!=cols-1&&field[i-1][j+1]=='*')	// top right
					count++;
			}

			if(j!=0&&field[i][j-1]=='*')			// left
				count++;
			if(j!=cols-1&&field[i][j+1]=='*')		// right
				count++;

			if(i!=rows-1)
			{
				if(j!=0&&field[i+1][j-1]=='*')		// bottom left
					count++;
				if(field[i+1][j]=='*')			// below
					count++;
				if(j!=cols-1&&field[i+1][j+1]=='*')	// bottom right
					count++;
			}

			if(count==0)
				field[i][j]='0';
			else
				field[i][j] = count+48;
			count = 0;
		}
	}
}

void revealMines() // reveals all mines on defeat
{
	for(i=0; i<rows; i++)
		for(j=0; j<cols; j++)
			if(field[i][j]=='*')
				status[i][j]=1;
}

void display() // displays the field after each action
{
	cout << "    ";
	for(j=0; j<cols; j++)
		cout << j+1 << "   ";
	cout << "\n\n";

	for(i=0; i<rows; i++)
	{
		for(j=0; j<cols; j++)
		{
			if(j==0)
			{
				cout << i+1 << "  ";
				if(i<9)
				cout << ' ';
			}

			if(status[i][j]==1)
			{
				cout << field[i][j] << "   ";
				if(field[i][j]=='*'&&!defeat)
				{
					textcolor(RED);
					defeat++;
					revealMines();
					clrscr();
					display();
				}
			}

			else if(status[i][j]==2)
				cout << 'f' << "   ";

			else
				cout << char(177) << "   ";

		}

		if(i==3)
			cout << "\t[== M I N E S W E E P E R ==]";
		if(i==6)
			cout << "\tPress \'f\' to flag a spot.";
		if(i==7)
			cout << "\tPress \'t\' to target a spot.";
		if(i==8)
			cout << "\tPress \'s\' to view high scores.";
		if(i==9)
			cout << "\tPress \'e\' to exit the game.";


		cout << "\n\n";
	}
	if(defeat)
	{
		cout << "\nY O U  L O S E";
		getch();
		exit(0);
	}
}

void check() // checks if the user has won
{
	int victory=1;

	for(i=0; i<rows; i++)
	{
		for(j=0; j<cols; j++)
		{
			if(isdigit(field[i][j])&&status[i][j]!=1)
				victory=0;
		}
	}

	if(victory)
	{
		end = clock();
		gameTime = ((float)(end - start))/CLOCKS_PER_SEC;
		textcolor(GREEN);
		clrscr();
		display();

		cout << "Y O U  W I N !\n\nYour time: " << setprecision(2) << gameTime << " seconds";
		addScore();
		getch();
		exit(0);
	}
}

void startGame() // starts a new game
{
	textcolor(3);
	placeMines();
	placeClues();
	start = clock();
	while(1)
	{
		clrscr();
		display();
		check();

		char choice;
		int row, col;

		cout << "Action (t/f/s/e): ";
		cin >> choice;

		switch(choice)
		{
			case 't': cout << "Target Row: ";
				  cin >> row;
				  cout << "Target Column: ";
				  cin >> col;
				  status[row-1][col-1] = 1;
				  break;

			case 'f': cout << "Flag Row: ";
				  cin >> row;
				  cout << "Flag Column: ";
				  cin >> col;
				  status[row-1][col-1] = 2;
				  break;

			case 's': clrscr();
				  cout << "H I G H  S C O R E S";
				  cout << "\n\nTIME\tPLAYER";

				  ifstream filin("scores.dat", ios::in);
				  while(!filin.eof())
				  {
					filin.read((char*)&record, sizeof(record));
					if(filin.eof())
						break;
					cout << "\n";
					record.putData();
				  }
				  filin.close();
				  getch();
				  break;

			case 'e': exit(0);
			default: continue;
		}
	}
}

int main()
{
	startGame();
  return 1;
}
