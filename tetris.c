#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define DELAY_TIME 2000
#define field_height 21
#define field_width 12

typedef struct{
	int x;
	int y;
	int type;
	int state;
}block;

block act;

void screen_printer(int*, int);
int action(int*, char, int);
void act_to_stable(int*);
void randomlizer(int*);
int detect_move(int*, int);
void rotate(int*);
void line_dealer(int*);

int main() {
	// initialize
	int time0 = time(NULL);
	int field[field_height][field_width] = {};			// 0 == nothing
	for(int i = 0; i < field_height; i++){
		field[i][11] = 9;			// 9 == wall
		field[i][ 0] = 9;			// 9 == wall
	}
	for(int i = 0; i < field_width; i++){
		field[20][i] = 8;			// 8 == floor
	}
	int* p = (int*)field;
	srand(time(NULL));
	
	//end of initialize
	
	randomlizer(p);
	screen_printer(p, time0);
	
	// In game logic
	char instuction = 'S';
	screen_printer(p,time0);
	int test = 0;
	test = action(p, instuction, test);
	int t0 = clock() + DELAY_TIME;
	
	while(1){
		int t1 = clock();
		
		if(kbhit()){
			instuction = getche();
			switch(instuction){
				case 'a':
					instuction = 'A';
					break;
				case 's':
					instuction = 'S';
					break;
				case 'd':
					instuction = 'D';
					break;
			}
			test = action(p, instuction, test);
			line_dealer(p);
			screen_printer(p, time0);
		}
		
		t1 = clock();
		
		if(t0 < t1){
			t0 = clock() + DELAY_TIME;
			instuction = 'S';
			test = action(p, instuction, test);
			line_dealer(p);
			screen_printer(p,time0);
		}
		if(field[0][5] == 1)	break;
	}
	//end of in game logic
	
	printf("\nyou are a fucking loooooooser");
	return 0;
}

void screen_printer(int* p, int time0){
	system("cls");
	for(int i = 0; i < field_width * field_height; i += 1){
		switch(*(p + i)){
			case 0:					// 0 is nothing
				printf("--");
				break;
			case 1:					// 1 is stop-block
				printf("# ");
				break;
			case 2:					// 2 is active-block
				printf("# ");
				break;
			case 8:					// 8 is floor
				printf("==");
				break;
			case 9:					// 9 is wall
				printf("||");
				break;
		}
		if((i + 1) % field_width == 0 && i != 0){
			printf("\n");
		}
	}
	printf("Playing Time:%d", time(NULL) - time0);
	printf("\nx:%d, y:%d, type:%d, state:%d\n", act.x, act.y, act.type, act.state);
}

int action(int* p, char instuction, int test){
	int check = detect_move(p, test);
	switch(instuction){
		case 'A':
			if(check == 0 || check == 1 || check == 3 || check == 5){
				for(int i = 0; i < field_width * field_height; i += 1){
					if(*(p + i) == 2 && *(p + i - 1) == 0){
						*(p + i) = 0;
						*(p + i - 1) = 2;
					}
				}
				act.x--;
			}			
			break;
		
		case 'S':
			if(check == 0 || check == 2 || check == 3 || check == 6){
				for(int i = field_width * field_height - 1; i >= 0; i -= 1){
					if(*(p + i) == 2){
						*(p + i) = 0;
						*(p + i + field_width) = 2;
					}
				}
				act.y++;
			}
			break;
		
		case 'D':
			if(check == 0 || check == 1 || check == 2 || check == 4){
				for(int i = field_width * field_height - 1; i >= 0; i -= 1){
					if(*(p + i) == 2 && *(p + i + 1) == 0){
						*(p + i) = 0;
						*(p + i + 1) = 2;
					}
				}
				act.x++;
			}
			break;
			
		case 'j':
			rotate(p);
			break;
		
		default:
			system("pause");
			break;
			
	}
	if(check == 7)	act_to_stable(p);
	return check;
}

int detect_move(int* p, int test){
	
	// initialize
	int t_R = 0, t_L = 0, t_D = 0;
	//end of initialize
	
	// detect
	for(int i = 0; i < field_width * field_height; i += 1){
		if(*(p + i) == 2 && *(p + i + 1) == 9){
			t_R++;
		}
		if(*(p + i) == 2 && *(p + i - 1) == 9){
			t_L++;
		}
		if(*(p + i) == 2 && *(p + i + 1) == 1){
			t_R++;
		}
		if(*(p + i) == 2 && *(p + i - 1) == 1){
			t_L++;
		}
		if(*(p + i) == 2 && *(p + i + field_width) != 2 && *(p + i + field_width) != 0){
			t_D++;
		}
	}
	// end of detect
	
	// export result
	test = test == 1 || test == 4 || test == 5 || test == 7;
	if(test != 0 && t_D != 0){
		return 7;
	}					
	
	if(t_R == 0 && t_L == 0 && t_D == 0)		return 0;		//can go everywhere
	else if (t_R == 0 && t_L == 0)				return 1;		//can go right and left
	else if (t_R == 0 && t_D == 0)				return 2;		//can go right and down
	else if (t_L == 0 && t_D == 0)				return 3;		//can go left and down
	else if (t_R == 0)							return 4;		//can go right
	else if (t_L == 0)							return 5;		//can go left
	else if (t_D == 0)							return 6;		//can go down
	else										return 7;		//can go nowhere						
}

void rotate(int*p){
	int *posi = p + act.x + field_width * act.y;
	if(act.y == 0){
		return;
	}
	switch(act.type){
		case 1:
			if(act.state == 0){
				if(*(posi + field_width) == 0 && *(posi + 2*field_width) == 0 && *(posi - field_width) == 0){
					*(posi +   field_width) = 2;
					*(posi + 2*field_width) = 2;
					*(posi -   field_width) = 2;
					*(posi - 1) = 0;
					*(posi + 1) = 0;
					*(posi + 2) = 0;
					act.state = 1;
				}
			}
			else{
				if(*(posi - 1) == 0 && *(posi + 1) == 0 && *(posi + 2) == 0){
					*(posi +   field_width) = 0;
					*(posi + 2*field_width) = 0;
					*(posi -   field_width) = 0;
					*(posi - 1) = 2;
					*(posi + 1) = 2;
					*(posi + 2) = 2;
					act.state = 0;
				}
			}
			break;
		
		case 2:
			if(act.state == 0){
				if(*(posi- field_width) == 0 && *(posi + field_width + 1) == 0){
					*(posi + field_width    ) = 0;
					*(posi + field_width - 1) = 0;
					*(posi - field_width    ) = 2;
					*(posi + field_width + 1) = 2;
					act.state = 1;
				}
			}
			else if(act.state == 1){
				if(*(posi + field_width) == 0 && *(posi + field_width - 1) == 0){
					*(posi + field_width    ) = 2;
					*(posi + field_width - 1) = 2;
					*(posi - field_width    ) = 0;
					*(posi + field_width + 1) = 0;
					act.state = 0;
				}
			}
			break;
			
		case 3:
			if(act.state == 0){
				if(*(posi - field_width) == 0 && *(posi + field_width - 1) == 0){
					*(posi - field_width    ) = 2;
					*(posi + field_width - 1) = 2;
					*(posi + field_width    ) = 0;
					*(posi + field_width + 1) = 0;
					act.state = 1;
				}
			}
			else if(act.state == 1){
				if(*(posi + field_width) == 0 && *(posi + field_width + 1) == 0){
					*(posi - field_width    ) = 0;
					*(posi + field_width - 1) = 0;
					*(posi + field_width    ) = 2;
					*(posi + field_width + 1) = 2;
					act.state = 0;
				}
			}
			break;
		
		case 4:
			if(act.state == 0){
				if(*(posi - field_width) == 0){
					*(posi - field_width) = 2;
					*(posi - 1          ) = 0;
					act.state = 1;
				}
			}
			else if(act.state == 1){
				if(*(posi - 1) == 0){
					*(posi + field_width) = 0;
					*(posi - 1          ) = 2;
					act.state = 2;
				}
			}
			else if(act.state == 2){
				if(*(posi + field_width) == 0){
					*(posi + field_width) = 2;
					*(posi + 1          ) = 0;
					act.state = 3;
				}
			}
			else if(act.state == 3){
				if(*(posi + 1) == 0){
					*(posi - field_width) = 0;
					*(posi + 1          ) = 2;
					act.state = 0;
				}
			}
			break;
		
		case 5:
			if(act.y < 2){
				return;
			}
			if(act.state == 0){
				if(*(posi - field_width) == 0 && *(posi - 2*field_width) == 0){
					*(posi -   field_width) = 2;
					*(posi - 2*field_width) = 2;
					*(posi +   field_width) = 0;
					*(posi - 2            ) = 0;
					act.state = 1;
				}
			}
			else if(act.state == 1){
				if(*(posi + 1) == 0 && *(posi + 2) == 0){
					*(posi + 1            ) = 2;
					*(posi + 2            ) = 2;
					*(posi - 1            ) = 0;
					*(posi - 2*field_width) = 0;
					act.state = 2;
				}
			}
			else if(act.state == 2){
				if(*(posi + field_width) == 0 && *(posi + 2*field_width) == 0){
					*(posi +   field_width) = 2;
					*(posi + 2*field_width) = 2;
					*(posi -   field_width) = 0;
					*(posi + 2            ) = 0;
					act.state = 3;
				}
			}
			else if(act.state == 3){
				if(*(posi - 1) == 0 && *(posi - 2) == 0){
					*(posi - 1            ) = 2;
					*(posi - 2            ) = 2;
					*(posi + 1            ) = 0;
					*(posi + 2*field_width) = 0;
					act.state = 0;
				}
			}
			break;
		
		case 6:
			if(act.state == 0){
				if(*(posi - 1) == 0 && *(posi + 2*field_width) == 0){
					*(posi - 1            ) = 2;
					*(posi + 2*field_width) = 2;
					*(posi + 1            ) = 0;
					*(posi + 2            ) = 0;
					act.state = 1;
				}
			}
			else if(act.state == 1){
				if(*(posi - field_width) == 0 && *(posi - 2) == 0){
					*(posi -  field_width) = 2;
					*(posi -  2          ) = 2;
					*(posi +  field_width) = 0;
					*(posi +2*field_width) = 0;
					act.state = 2;
				}
			}
			else if(act.state == 2){
				if(*(posi + 1) == 0 && *(posi - 2*field_width) == 0){
					*(posi + 1            ) = 2;
					*(posi - 2*field_width) = 2;
					*(posi - 1            ) = 0;
					*(posi - 2            ) = 0;
					act.state = 3;
				}
			}
			else if(act.state == 3){
				if(*(posi + field_width) == 0 && *(posi + 2) == 0){
					*(posi +  field_width) = 2;
					*(posi +  2          ) = 2;
					*(posi -  field_width) = 0;
					*(posi -2*field_width) = 0;
					act.state = 0;
				}
			}
			break;		
	}
}

void act_to_stable(int *p){
	for(int i = 0; i < field_width * field_height; i += 1){
		if(*(p + i) == 2) *(p + i) = 1;
	}
	if(*(p + 5) != 1){
		randomlizer(p);	
	}
}

void randomlizer(int *p){
	//int next = 6; 
	int next = rand() % 7;
	act.type = next;
	act.state = 0;
	act.x = 5;
	act.y = 0;
	
	switch(next){
		case 0:
			*(p +  4)= 2;
			*(p +  5)= 2;
			*(p + 16)= 2;
			*(p + 17)= 2;
			break;
		
		case 1:
			*(p + 4) = 2;
			*(p + 5) = 2;
			*(p + 6) = 2;
			*(p + 7) = 2;
			break;
			
		case 2:
			*(p +  5)= 2;
			*(p +  6)= 2;
			*(p + 16)= 2;
			*(p + 17)= 2;
			break;
		
		case 3:
			*(p +  4)= 2;
			*(p +  5)= 2;
			*(p + 17)= 2;
			*(p + 18)= 2;
			break;
			
		case 4:
			*(p +  4)= 2;
			*(p +  5)= 2;
			*(p +  6)= 2;
			*(p + 17)= 2;
			break;
		case 5:
			*(p +  3)= 2;
			*(p +  4)= 2;
			*(p +  5)= 2;
			*(p + 17)= 2;
			break;
			
		case 6:
			*(p +  5)= 2;
			*(p +  6)= 2;
			*(p +  7)= 2;
			*(p + 17)= 2;
			break;
	}
}

void line_dealer(int* p){
	int line = 0, y = 0, x = 0, test = 0;
	for(y = 20; y >= 0; y--){
		test = 0;
		for(x = 1; x < 11; x++){
			if(*(p + field_width * y + x) == 1){
				test++;
			}
		}
	
		if(test == 10 && y != 0){
			line++;
			for(int i = y; i > 0; i--){
				for(x = 1; x < 11; x++){	
					*(p + field_width * i + x) = *(p + field_width * (i - 1) + x);
					*(p + field_width * (i - 1) + x) = 0;
				}
			}
		}
		else if(test == 10){
			line++;
			for(x = 1; x < 11; x++){
				*(p + 11 * y + x) = 0;
			}
		}
	}
	if(line != 0){
		act.y += line;
	}
}