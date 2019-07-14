
# include <Windows.h>
# include <string>

/*sdf*/
/*
Welcome to the Snake game programming challenge. You are required to add code to the following three functions to implement the Snake game.
You can of course write any code that you need to outside of these functions as well.
The functions are:

Initialize()
OnTimer()
OnKeyDown()

The file shows you how to draw rectangles with specific colors and how to draw text if you need to.
You should not need anything else because we are looking to create a visually-simple Snake game, so flat squared should be good enough.

Good luck!
*/


// COLORREF is a Windows data type. It is used by the API to represent 32-bit colors.
// Create a COLORREF value using the RGB() macro:
// COLORREF myColor = RGB(red, green, blue);
// where red, green, and blue are integers from 0-255.

// Another Windows data type is RECT. It represents a rectangle. It has the members:
// top, left, right, bottom.


// Use this to fill a rectangle of the hDC with a specific color.

void FillRect(HDC hDC, const RECT* pRect, COLORREF color)
{
	COLORREF oldColor = SetBkColor(hDC, color);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, pRect, TEXT(""), 0, 0);
	SetBkColor(hDC, oldColor);
}

// Use this to draw text if you need to.
void DrawTextLine(HWND hWnd, HDC hDC, const char* sText, RECT* pr_topext, COLORREF clr);


namespace game
{
	unsigned int windowWidth = 0;
	unsigned int windowHeight = 0;
	bool OnKeyDown(WPARAM keyCode);
	void BuildSnake(RECT Rec);
	bool eat_itself();
	bool hit_wall();


    RECT r;
	int r_left, tmp_left;
	int r_top;
	char direction = ' ';
	bool mov = false;
	int AvalibalePosition[] = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 420, 440, 460, 480, 500};//avaliable position for apple 
	int Xindex = 0, Yindex = 0; // random x and y indexes for avaliable position for apple
	int x = 0 , y = 0; // apple position
	bool eaten_apple = false; 
	bool active = true; 
	

	struct node
	{
		RECT rec;
		node* next;
		node* prev;
	};
	node* head=NULL;
	node* tmp;
	node* t;
	node* tail;
	node* op = tail;
	
	void FirstSnake(HWND hWnd)
	{
		head=NULL;
		HDC hDC = GetDC(hWnd);
			for(int i=0;i<4;i++)
			{
				if(i==0)
					tmp_left = r_left;
				r.left = r_left;
				r.top = r_top;
				r.right = r_left + 15;
				r.bottom = r_top + 15;
				FillRect(hDC, &r, RGB(255, 255, 0)); //Draw a red square.
				BuildSnake(r);
				r_left = r_left + 20;
			}
			r_left = tmp_left;

	}
	bool Initialize(HWND hWnd)
	{
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		windowWidth = rClient.right - rClient.left; // rClient.top and rClient.left are always 0.
		windowHeight = rClient.bottom - rClient.top;
		windowHeight+=5;
		windowWidth+=5;

		r_left = 40;
		r_top = 60;

		x = rand() % windowWidth ;
		y = rand() % windowHeight ;

		int newx = x-20;
		int newy = y-20;

		x = x-newx;
		y = y-newy;

		
		mov = false;
		
		OutputDebugStringA("My game has been initialized. This text should be shown in the 'output' window in VS");	
		HDC hDC = GetDC(hWnd);
		FirstSnake(hWnd);

        active = true;
		return true;
	}

	void BuildSnake(RECT Rec)
	{
		tmp = new node;
		tmp->rec = Rec;
		tmp->next = NULL;

		if(head==NULL)
		{
			head = tmp;
			tail = tmp;
		}
		else
		{
			head->next = tmp;
			head = tmp;
		}
	}

	//decide direction of snake 
	void decide_direction()
	{
		if(direction == 'd')//Left direction
		{
			r_left = r_left-20;
		}
		else if(direction == 's')//down direction
		{
			r_top = r_top+20;
		}
		else if(direction == 'a')//right direction
		{
			r_left = r_left+20;
		}
		else if(direction == 'w')//top direction
		{
			r_top = r_top-20;		
		}

	}

	void BuildNewRec()
	{
		r.left = r_left;
		r.top = r_top;
		r.right = r_left + 15;
		r.bottom = r_top + 15;

		tmp = new node();
		tmp->rec = r;
		head->next = tmp;
		head = tmp;
	}

	bool EatenApple()
	{
		if((head->rec).left == x && (head->rec).top == y)
			return true;

		return false;
	}

	bool hit_wall()
	{
		if (direction == 'a')
		{
			if( (head->rec).right == windowWidth )
				return true;
		}
		else if (direction == 'd') 
		{
			if( (head->rec).left == 0)
				return true;
		}
		else if (direction == 'w') 
		{
			if( (head->rec).top == 0)
				return true;
		}
		else if (direction == 's') 
		{
			if( (head->rec).bottom == windowHeight)
				return true;
		}

		return false;
	}
	
	bool eat_itself()
	{
	   node* t = tail;
		while ( t != head )
		{
			if( (head->rec).left == (t->rec).left  && (head->rec).top == (t->rec).top)
				return true;
			
				t = t->next;
		}
		return false;
	}
	// This is called periodically. Use it to update your game state and draw to the window.
	void CALLBACK OnTimer(HWND hWnd, UINT Msg, UINT_PTR idTimer, DWORD dwTime)
	{
		if(!active)
			return ;

		HDC hDC = GetDC(hWnd);
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		FillRect(hDC, &rClient, RGB(0,0,128)); // Clear the window to blackness.
		char text[256] = { 0 };
		sprintf_s(text, "OnTimer. Time: %d %d %d %d ", r_left , r_top, x, y );
		RECT r_topext = { 0, 0, rClient.right, 15 };
		DrawTextLine(hWnd, hDC, text, &r_topext, RGB(120, 120, 120));
		RECT r_apple;

		//generate apple 
			r_apple.left = x;
			r_apple.top = y;
			r_apple.right = x+15;
			r_apple.bottom = y+15;
			FillRect(hDC, &r_apple, RGB(255, 20 , 255));	

		// check if snake eat itself
			if( eat_itself() )
			{
				active = false;
				MessageBox(hWnd, TEXT("YOU LOST!, EatItself"), TEXT("Snake"), MB_OK );
				Initialize(hWnd);
			}
			
		// check if snake hit the wall
			if( hit_wall() )
			{
				active = false;
				MessageBox(hWnd, TEXT("YOU LOST!!!,HitWall"), TEXT("Snake"), MB_OK );
				Initialize(hWnd);
			}	

		
		if( !mov )
			FirstSnake( hWnd);
			
		else 
		{
			r_left = (head->rec).left;
			r_top = (head->rec).top;

			decide_direction();
			BuildNewRec();
			
			node *p=tail;
			tail = tail->next;
			delete(p);

		
			node* t = tail;
			while(t != head)
			{
				FillRect(hDC, &(t->rec), RGB(255, 255, 0));
				t = t->next;
			}
			FillRect(hDC, &(head->rec), RGB(255, 255, 0));
			
			if(EatenApple())
			{
				eaten_apple = 1;
				FillRect(hDC, &r_apple, RGB(0,0,128)); 

				decide_direction();
				BuildNewRec();

				//generateFood( hWnd);
				Xindex = rand() % 20;
				Yindex = rand() % 20;

				x = AvalibalePosition[Xindex];
				y = AvalibalePosition[Yindex];

			}
	
		}
	}
		
	
	// This is called when the user presses a key on the keyboard.
	// Use this to change the direction of your snake.
	bool OnKeyDown(WPARAM keyCode)
	{
		
		if(keyCode == VK_UP && direction != 's')
			direction = 'w';
		else if(keyCode == VK_DOWN && direction != 'w')
			direction = 's';
		else if(keyCode == VK_LEFT && direction != 'a')
			direction = 'd';
		else if(keyCode == VK_RIGHT && direction != 'd')
			direction = 'a';

		if (keyCode == VK_UP || keyCode == VK_DOWN || keyCode == VK_LEFT || keyCode == VK_RIGHT){
			mov = 1;
			return true;
		}
		
		return false; // They key pressed does not interest us. Let the OS handle it.
	}
		


}
