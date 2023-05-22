#include "tetris.h"
#include <stddef.h>

#define BLOCK_INIT_X 4
#define BLOCK_INIT_Y 0

#define SEED 171768

#define SCORE_LINE 100
#define SCORE_TETRIS 200
#define SCORE_TSPIN_SINGLE 100
#define SCORE_TSPIN_DOUBLE 200
#define SCORE_TSPIN_TRIPLE 300

EFI_GRAPHICS_OUTPUT_BLT_PIXEL debug_color = {0xa9,0xa9,0xa9,0x00};

//shape of blocks
char blocks[7][4][4][4]={
	{  //I-mino
		{//UP
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},{//RIGHT
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},{//DOWN
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},{//LEFT
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},{//O-mino
		{//UP
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},{//RIGHT
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},{//DOWN
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},{//LEFT
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	},{//S-mino
		{//UP
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},{//RIGHT
			{0,1,0,0},
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},{//DOWN
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},{//LEFT
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},{//Z-mino
		{//UP
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},{//RIGHT
			{0,0,1,0},
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},{//DOWN
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},{//LEFT
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{0,0,0,0}
		}
	},{//J-mino
		{//UP
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},{//RIGHT
			{0,1,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},{//DOWN
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},{//LEFT
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},{//L-mino
		{//UP
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},{//RIGHT
			{0,1,1,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		},{//DOWN
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},{//LEFT
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},{//T-mino
		{//UP
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},{//RIGHT
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},{//DOWN
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},{//LEFT
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	}
};

//color of blocks (Blue,Green,Red,Reserved)
EFI_GRAPHICS_OUTPUT_BLT_PIXEL block_colors[9]={
	{0xFF,0xFF,0x00,0x00},
	{0x00,0xFF,0xFF,0x00},
	{0x00,0xFF,0x00,0x00},
	{0x00,0x00,0xFF,0x00},
	{0xFF,0x00,0x00,0x00},
	{0x00,0xA5,0xFF,0x00},
	{0xD3,0x00,0x94,0x00},
	{0xFF,0xFF,0xFF,0x00},	//current color
	{0x00,0x00,0x00,0x00}	//backgrund color
};

//line color
EFI_GRAPHICS_OUTPUT_BLT_PIXEL line_color={0xFF,0xFF,0xFF,0x00};

typedef struct{
	int kind;
	int rotate;
	int x_anchor;
	int y_anchor;
}Block;

typedef struct {
	Console *console;
	Block *current;
	int (*block_buffer)[Y_BUF_LINE];
}Timer;

typedef struct{
	EFI_SYSTEM_TABLE *SystemTable;
	Console *console;
	int (*block_buffer)[Y_BUF_LINE];
}Debug;

/*
	I-tetromino 0 light blue
	O-tetromino 1 yellow
	S-tetromino 2 green
	Z-tetromino 3 red
	J-tetromino 4 blue
	L-tetromino 5 orange
	T-tetromino 6 purple
*/
enum Blocks{
	I_MINO,
	O_MINO,
	S_MINO,
	Z_MINO,
	J_MINO,
	L_MINO,
	T_MINO,
	CURRENT_BLOCK,
	BACKGRUND
};

enum Rotate{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum Score{
	TETRIS=2,
	TSPIN_SINGLE,
	TSPIN_DOUBLE,
	TSPIN_TRIPLE
};

int key_flag[4]={0};
int space_key_flag=0;

int t_spin_flag = 0;

int decide_flag=0;

int delete_Line = 0;

int init_flag = 0;

int gameover_flag = 0;

int score_flag = 0;

unsigned int random = SEED;
unsigned int random_block[7] = {0};
int random_flag = 6;
int next_block_kind = BACKGRUND;

unsigned long long int score;
//Debug
int key_debug[4] = {0};

CHAR16 *itoa(int value, CHAR16 *str){
	int lengh;
	int i,j,m;
	int tmp = value;

	if(value == 0){
		*str=48;
		return str;
	}

	for(lengh=0;tmp != 0;lengh++){
		tmp/=10;
	}

	for(i=lengh;i > 0;i--){
		m=1;
		for(j=i;j > 0;j--)m*=10;
		tmp = value % m;

		m=1;
		for(j=i-1;j > 0;j--)m*=10;
		tmp = tmp / m;

		*(str+lengh-i)=48+tmp;

	}
	return str;
}

void DrawPixel(Console *console, UINT32 x, UINT32 y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color){
	//EFI_GRAPHICS_OUTPUT_BLT_PIXEL *FBB = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)GOP->Mode->FrameBufferBase;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pixel = console->FBB+y*console->width+x;
	pixel->Red = color.Red;
	pixel->Blue = color.Blue;
	pixel->Green = color.Green;
	pixel->Reserved = color.Reserved;
}

void DrawRectangle(Console *console, UINT32 x, UINT32 y, UINT32 xsize, UINT32 ysize, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color){
	int i,j;
	for(i=y;i<=y+ysize;i++){
		for(j=x;j<=x+xsize;j++){
			DrawPixel(console, j, i, color);
		}
	}
}

void WriteBlockToBuffer(int x, int y, int kind, int rotate, int number, int (*block_buffer)[Y_BUF_LINE]){
	int i,j;
	for(i=(y >= 0 ? y:0); i < (y+4 <= Y_BUF_LINE ? y+4:Y_BUF_LINE);i++){
		for(j=(x >= 0 ? x:0); j < (x+4 <= X_BUF_LINE ? x+4:X_BUF_LINE); j++){
			if(blocks[kind][rotate][i-y][j-x] != 0){	//ブロックデータは、行の塊をデータとしているため、一番右側に来るindexは行を表すx座標出なければならない。
				block_buffer[j][i] = number;
			}
		}
	}
}

void UpdateDisplay(Console *console, int x, int y, int xsize, int ysize, int (*block_buffer)[Y_BUF_LINE]){
	int i,j;
	for(i=(y >= Y_BUF_LINE-YLINE ? y:Y_BUF_LINE-YLINE); i < (y+ysize <= Y_BUF_LINE ? y+ysize:Y_BUF_LINE);i++){
		for(j=(x >= XLINE-X_BUF_LINE ? x:XLINE-X_BUF_LINE); j < (x+xsize <= X_BUF_LINE ? x+xsize:X_BUF_LINE); j++){
			DrawRectangle(console, X_DISPLAY_ANCHOR+UNIT*j, Y_DISPLAY_ANCHOR+UNIT*(i-4), UNIT, UNIT, block_colors[block_buffer[j][i]]);
		}
	}
}

//ブロックを落下させる(下矢印キーにより、key_flag[DOWN]=1(DownKeyHandle関数)となったときにKeyTimerHandle関数から呼び出し
//又は、タイマーにより、DownTimerHandle関数が呼ばれたときに呼び出される)
int FallBlock(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	int i,j;
	//落下できるかどうか確かめる。(下にブロックが存在したり、床が存在する場合はreturn)
	for(i=Y_BUF_LINE-1; i >= 0; i--){
		for(j=X_BUF_LINE-1; j >= 0; j--){
			if(block_buffer[j][i] == CURRENT_BLOCK){
				if(i+1 >= Y_BUF_LINE) return 1;
				if(block_buffer[j][i+1] != CURRENT_BLOCK && block_buffer[j][i+1] != BACKGRUND) return 2;
			}
		}
	}
	//block_bufferの更新
	for(i=Y_BUF_LINE-1; i >= 0; i--){
		for(j=X_BUF_LINE-1; j >= 0; j--){
			if(block_buffer[j][i] == CURRENT_BLOCK){
				block_buffer[j][i] = BACKGRUND;
				block_buffer[j][i+1] = CURRENT_BLOCK;
			}
		}
	}
	//描画の更新
	UpdateDisplay(console, current->x_anchor, current->y_anchor, 4,	5, block_buffer);
	//ブロックの基準点の更新
	current->y_anchor++;
	return 0;
}

//ブロックを右へ移動させる(右矢印キーにより、key_flag[RIGHT]=1(RightKeyHandle関数)となったときにKeyTimerHandle関数から呼び出し)
int MoveRight(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	int i,j;
	//右に移動できるかどうか確かめる。(右にブロックが存在したり、床が存在する場合はreturn)
	for(i=Y_BUF_LINE-1; i >= 0; i--){
		for(j=X_BUF_LINE-1; j >= 0; j--){
			if(block_buffer[j][i] == CURRENT_BLOCK){
				if(j+1 >= X_BUF_LINE) return 1;
				if(block_buffer[j+1][i] != CURRENT_BLOCK && block_buffer[j+1][i] != BACKGRUND) return 2;
			}
		}
	}
	//block_bufferの更新
	for(i=Y_BUF_LINE-1; i >= 0; i--){
		for(j=X_BUF_LINE-1; j >= 0; j--){
			if(block_buffer[j][i] == CURRENT_BLOCK){
				block_buffer[j][i] = BACKGRUND;
				block_buffer[j+1][i] = CURRENT_BLOCK;
			}
		}
	}
	//描画の更新
	UpdateDisplay(console, current->x_anchor, current->y_anchor, 5,	4, block_buffer);
	//ブロックの基準点の更新
	current->x_anchor++;
	return 0;
}

int MoveLeft(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	int i,j;
	//左に移動できるかどうか確かめる。(左にブロックが存在したり、床が存在する場合はreturn)
	for(i=Y_BUF_LINE-1; i >= 0; i--){
		for(j=0; j < X_BUF_LINE; j++){
			if(block_buffer[j][i] == CURRENT_BLOCK){
				if(j-1 < 0) return 1;
				if(block_buffer[j-1][i] != CURRENT_BLOCK && block_buffer[j-1][i] != BACKGRUND) return 2;
			}
		}
	}
	//block_bufferの更新
	for(i=Y_BUF_LINE-1; i >= 0; i--){
		for(j=0; j < X_BUF_LINE; j++){
			if(block_buffer[j][i] == CURRENT_BLOCK){
				block_buffer[j][i] = BACKGRUND;
				block_buffer[j-1][i] = CURRENT_BLOCK;
			}
		}
	}
	//ブロックの基準点の更新
	current->x_anchor--;
	//描画の更新
	UpdateDisplay(console, current->x_anchor, current->y_anchor, 5,	4, block_buffer);
	return 0;
}

int RotateBlockCheck(int (*block_buffer)[Y_BUF_LINE], int x_anchor, int y_anchor, int kind, int rotate, int mode){
	//バッファからはみ出てないか（枠外からはみ出てないか）
	if(mode == RIGHT){
		switch (kind){
		case I_MINO:
			switch (rotate){
			case UP:
				if(x_anchor < 0) return 1;
				if(x_anchor > 7) return 1;
				break;
			case RIGHT:
				if(y_anchor > Y_BUF_LINE-4) return 1;
				break;
			case DOWN:
				if(x_anchor < 0) return 1;
				if(x_anchor > 6) return 1;
				break;
			case LEFT:
				if(y_anchor > Y_BUF_LINE-4) return 1;
				break;
			default:
				return 2;
				break;
			}
			break;

		case O_MINO:
			switch (rotate){
				case UP:
				case RIGHT:
				case DOWN:
				case LEFT:
					break;
				default:
					return 2;
					break;
			}
			break;

		case S_MINO:
		case Z_MINO:
		case J_MINO:
		case L_MINO:
		case T_MINO:
			switch (rotate){
				case UP:
					if(x_anchor > 7) return 1;
					break;
				case RIGHT:
					if(y_anchor > Y_BUF_LINE-3) return 1;
					break;
				case DOWN:
					if(x_anchor < 0) return 1;
					break;
				case LEFT:
					break;
				default:
					return 2;
					break;
			}
			break;

		default:
			return 2;
			break;
		}

	}else if(mode == LEFT){
		switch (kind){
		case I_MINO:
			switch (rotate){
			case UP:
				if(x_anchor < 0) return 1;
				if(x_anchor > 6) return 1;
				break;
			case RIGHT:
				if(y_anchor > Y_BUF_LINE-4) return 1;
				break;
			case DOWN:
				if(x_anchor < 0) return 1;
				if(x_anchor > 7) return 1;
				break;
			case LEFT:
				if(y_anchor > Y_BUF_LINE-4) return 1;
				break;
			default:
				return 2;
				break;
			}
			break;

		case O_MINO:
			switch (rotate){
			case UP:
			case RIGHT:
			case DOWN:
			case LEFT:
				break;
			default:
				return 2;
				break;
			}
			break;

		case S_MINO:
		case Z_MINO:
		case J_MINO:
		case L_MINO:
		case T_MINO:
			switch (rotate){
			case UP:
				if(x_anchor < 0) return 1;
				break;
			case RIGHT:
				break;
			case DOWN:
				if(x_anchor > 7) return 1;
				break;
			case LEFT:
				if(y_anchor > Y_BUF_LINE-3) return 1;
				break;
			default:
				return 2;
				break;
			}
			break;

		default:
			return 2;
			break;
		}
	}else{
		return 2;
	}

	int i,j;
	for(i=(y_anchor >= 0 ? y_anchor:0); i < (y_anchor+4 <= Y_BUF_LINE ? y_anchor+4:Y_BUF_LINE);i++){
		for(j=(x_anchor >= 0 ? x_anchor:0); j < (x_anchor+4 <= X_BUF_LINE ? x_anchor+4:X_BUF_LINE); j++){
			if(blocks[kind][rotate][i-y_anchor][j-x_anchor] != 0 && block_buffer[j][i] != CURRENT_BLOCK && block_buffer[j][i] != BACKGRUND) return 1;
		}
	}
	return 0;
}

int RrightRotateCheck(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	int next_rotate = current->rotate == LEFT ? UP : current->rotate+1;
	switch (current->kind){
	case I_MINO:
		switch (next_rotate){
		case UP:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-2, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor+2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->y_anchor += 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-2, current->y_anchor-1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 2;
				current->y_anchor -= 1;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;

		case RIGHT:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-2, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-2, current->y_anchor+1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 2;
				current->y_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor-2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->y_anchor -= 2;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;

		case DOWN:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+2, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor-2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->y_anchor -= 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+2, current->y_anchor+1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 2;
				current->y_anchor += 1;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;


		case LEFT:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+2, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+2, current->y_anchor-1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 2;
				current->y_anchor -= 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor+2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->y_anchor += 2;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;

		default:
			return 2;
			break;
		}
		break;

	case O_MINO:
		current->rotate = next_rotate;
		break;

	case S_MINO:
	case Z_MINO:
	case J_MINO:
	case L_MINO:
	case T_MINO:
		switch (next_rotate){
		case UP:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-2, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 2;
				current->rotate = next_rotate;
				WriteBlockToBuffer(1,1,current->kind,current->rotate, 1, block_buffer);
				UpdateDisplay(console, 0,0,3,3,block_buffer);
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-2, current->y_anchor+1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 2;
				current->y_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor-2, current->kind, next_rotate, RIGHT) == 0){
				current->y_anchor -= 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor-2, current->kind,  next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->y_anchor -= 2;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;
		case RIGHT:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor-1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->y_anchor -= 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+2, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor-1, current->y_anchor+2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor -= 1;
				current->y_anchor += 2;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;
		case DOWN:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor+1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->y_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor-2, current->kind, next_rotate, RIGHT) == 0){
				current->y_anchor -= 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor-2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->y_anchor -= 2;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;
		case LEFT:
			if(RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor-1, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->y_anchor -= 1;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor, current->y_anchor+2, current->kind, next_rotate, RIGHT) == 0){
				current->y_anchor += 2;
				current->rotate = next_rotate;
			}else if (RotateBlockCheck(block_buffer, current->x_anchor+1, current->y_anchor+2, current->kind, next_rotate, RIGHT) == 0){
				current->x_anchor += 1;
				current->y_anchor += 2;
				current->rotate = next_rotate;
			}else{
				return 1;
			}
			break;

		default:
			return 2;
			break;
		}
		break;

	default:
		return 2;
		break;
	}

	return 0;

}

int Rotate(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	WriteBlockToBuffer(current->x_anchor, current->y_anchor, current->kind, current->rotate, BACKGRUND, block_buffer);
	RrightRotateCheck(console, current, block_buffer);
	WriteBlockToBuffer(current->x_anchor, current->y_anchor, current->kind, current->rotate, CURRENT_BLOCK, block_buffer);
	UpdateDisplay(console, current->x_anchor-2, current->y_anchor-2, 8, 8, block_buffer);
	return 0;
}

void DeleteLine(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	int i,j,k,count_block,count_line=0;
	for(i=(current->y_anchor >= Y_BUF_LINE-YLINE ? current->y_anchor:Y_BUF_LINE-YLINE); i < (current->y_anchor+4 <= Y_BUF_LINE ? current->y_anchor+4:Y_BUF_LINE);i++){
		count_block=0;
		for(j=0; j < XLINE; j++){
			if(block_buffer[j][i] != BACKGRUND) count_block++;
		}
		if(count_block == XLINE){
			for(j=0; j < XLINE; j++) block_buffer[j][i] = BACKGRUND;
			count_line++;
		}
	}

	int xsize = (current->kind == I_MINO ? 2 : 1);

	for(i=0; i<count_line; i++){
		for(j=(current->y_anchor+xsize > Y_BUF_LINE ? Y_BUF_LINE : current->y_anchor+xsize); j >= 0; j--){
			for(k=0; k < X_BUF_LINE; k++){
				if(block_buffer[k][j] != BACKGRUND && block_buffer[k][j+1] == BACKGRUND){
					block_buffer[k][j+1] = block_buffer[k][j];
					block_buffer[k][j] = BACKGRUND;
				}
			}
		}
	}

	//calcurate score
	if(count_line > 0) score_flag = 1;
	score += SCORE_LINE * count_line;
	if(count_line == 4){
		score += SCORE_TETRIS;	//tetris
		score_flag = TETRIS;
	}
	if(current->kind == T_MINO && t_spin_flag != 0 && count_line > 0){
		int count_t_spin = 0;
		if(block_buffer[current->x_anchor  ][current->y_anchor  ] != BACKGRUND) count_t_spin++;
		if(block_buffer[current->x_anchor+2][current->y_anchor  ] != BACKGRUND) count_t_spin++;
		if(block_buffer[current->x_anchor  ][current->y_anchor+2] != BACKGRUND) count_t_spin++;
		if(block_buffer[current->x_anchor+2][current->y_anchor+2] != BACKGRUND) count_t_spin++;
		if(count_t_spin >= 3){
			if(count_line == 1){
				//tspin single
				score += SCORE_TSPIN_SINGLE;
				score_flag = TSPIN_SINGLE;
			}else if(count_line == 2){
				score += SCORE_TSPIN_DOUBLE;
				score_flag = TSPIN_DOUBLE;
			}else if(count_line == 3){
				score += SCORE_TSPIN_TRIPLE;
				score_flag = TSPIN_TRIPLE;
			}
		}
	}

	delete_Line += count_line;

	UpdateDisplay(console, 0, 0, X_BUF_LINE, Y_BUF_LINE, block_buffer);
}

int GameOverCheck(int (*block_buffer)[Y_BUF_LINE]){
	int i;
	for(i=0; i<XLINE; i++){
		if(block_buffer[i][Y_BUF_LINE-21] != BACKGRUND && block_buffer[i][Y_BUF_LINE-21] != CURRENT_BLOCK) return 1;
	}
	return 0;
}

void DecideBlock(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	int i,j;
	for(i=(current->y_anchor >= 0 ? current->y_anchor:0); i < (current->y_anchor+4 <= Y_BUF_LINE ? current->y_anchor+4:Y_BUF_LINE);i++){
		for(j=(current->x_anchor >= 0 ? current->x_anchor:0); j < (current->x_anchor+4 <= X_BUF_LINE ? current->x_anchor+4:X_BUF_LINE); j++){
			if(blocks[current->kind][current->rotate][i-current->y_anchor][j-current->x_anchor] != 0) block_buffer[j][i] = current->kind;
		}
	}
	DeleteLine(console, current, block_buffer);
	gameover_flag = GameOverCheck(block_buffer);
	init_flag = 1;
}

//xorshift32
unsigned int Random(){
	random ^= random << 13;
	random ^= random >> 17;
	random ^= random << 5;
	return random;
}

//Fisher-Yates shuffle
void FYS(unsigned int *array, int lengh){
	int i = lengh - 1;
	int k;
	for(k=0; k<lengh; k++){
		int j = Random() % i;
		int tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

void RandomBlock(){
	int i;
	if(random_flag == 6){
		for(i=0;i<7;i++)random_block[i]=i;
		FYS(random_block, 7);
		random_flag = 0;
	}else{
		random_flag++;
	}
}

void Init(Console *console, Block *current, int (*block_buffer)[Y_BUF_LINE]){
	current->x_anchor = BLOCK_INIT_X;
	current->y_anchor = BLOCK_INIT_Y;
	current->rotate=UP;

	//落下ブロックの選定処理
	if(next_block_kind != BACKGRUND){
		current->kind = next_block_kind;
		RandomBlock();
		next_block_kind = random_block[random_flag];
		//choose_kind = (choose_kind == T_MINO ? I_MINO : choose_kind+1);
	}else{	//初期処理
		RandomBlock();
		current->kind = random_block[0];
		next_block_kind = random_block[1];
		random_flag++;
	}

	block_colors[CURRENT_BLOCK] = block_colors[current->kind];

	WriteBlockToBuffer(current->x_anchor, current->y_anchor, current->kind, current->rotate, CURRENT_BLOCK, block_buffer);
	UpdateDisplay(console, current->x_anchor, current->y_anchor, 4,	4, block_buffer);

	init_flag = 0;
}

//タイマーによる落下の処理関数
void DownTimerHandle(IN EFI_EVENT Event,IN VOID *Context){
	Timer *timer = (Timer *)Context;
	if(init_flag == 0){
		key_debug[DOWN] = FallBlock(timer->console, timer->current, timer->block_buffer);
		if(key_debug[DOWN] == 0){
			decide_flag = 0;
		}else{
			decide_flag++;
			if(decide_flag >= 2){
				DecideBlock(timer->console, timer->current, timer->block_buffer);
				decide_flag = 0;
			}
		}
	}
}

EFI_STATUS DownKeyHandle(EFI_KEY_DATA *KeyData){
	key_flag[DOWN]=1;
	return EFI_SUCCESS;
}

EFI_STATUS RightKeyHandle(EFI_KEY_DATA *KeyData){
	key_flag[RIGHT]=1;
	return EFI_SUCCESS;
}

EFI_STATUS LeftKeyHandle(EFI_KEY_DATA *KeyData){
	key_flag[LEFT]=1;
	return EFI_SUCCESS;
}

EFI_STATUS UpKeyHandle(EFI_KEY_DATA *KeyData){
	key_flag[UP]=1;
	return EFI_SUCCESS;
}

EFI_STATUS SpaceKeyHandle(EFI_KEY_DATA *KeyData){
	space_key_flag = 1;
	return EFI_SUCCESS;
}

void KeyTimerHandle(IN EFI_EVENT Event,IN VOID *Context){
	Timer *timer = (Timer *)Context;

	if(init_flag == 0){
		t_spin_flag=0;
		//ブロックの落下(時間又は下矢印キー)
		if(key_flag[DOWN] == 1){
			key_debug[DOWN] = FallBlock(timer->console, timer->current, timer->block_buffer);
			if(key_debug[DOWN] == 0){
				decide_flag = 0;
			}else{
				decide_flag++;
				if(decide_flag >= 2){
					DecideBlock(timer->console, timer->current, timer->block_buffer);
					decide_flag = 0;
				}
			}
			key_flag[DOWN] = 0;
		}else if(key_flag[RIGHT] == 1){
			key_debug[RIGHT] = MoveRight(timer->console, timer->current, timer->block_buffer);
			key_flag[RIGHT] = 0;
		}else if(key_flag[LEFT] == 1){
			key_debug[RIGHT] = MoveLeft(timer->console, timer->current, timer->block_buffer);
			key_flag[LEFT] = 0;
		}else if(key_flag[UP] == 1){
			key_debug[UP] = Rotate(timer->console, timer->current, timer->block_buffer);
			t_spin_flag = 1;
			key_flag[UP] = 0;
		}else if(space_key_flag == 1){
			int i = 0;
			for(;;){
				i = FallBlock(timer->console, timer->current, timer->block_buffer);
				if(i != 0) break;
			}
			space_key_flag = 0;
		}
	}
}

void DebugTimerHandle(IN EFI_EVENT Event,IN VOID *Context){
	Debug *debug = (Debug *)Context;

	int i,j;
	CHAR16 str[128]={0};

	//Debug (move area)
	/*
	debug->SystemTable->ConOut->SetCursorPosition(debug->SystemTable->ConOut,30,0);
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,L"<Move error>");
	debug->SystemTable->ConOut->SetCursorPosition(debug->SystemTable->ConOut,30,1);
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,L"Down :");
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,itoa(key_flag[DOWN],str));
	debug->SystemTable->ConOut->SetCursorPosition(debug->SystemTable->ConOut,30,2);
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,L"Right:");
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,itoa(key_debug[RIGHT],str));
	debug->SystemTable->ConOut->SetCursorPosition(debug->SystemTable->ConOut,30,3);
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,L"Left :");
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,itoa(key_debug[LEFT],str));
	for(i=0;i<4;i++)key_flag[i]=0;
	*/

	//Debug (print block_buffer)
	debug->SystemTable->ConOut->SetCursorPosition(debug->SystemTable->ConOut,50,0);
	debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,L"<Block memory>");
	for(i=0;i < Y_BUF_LINE;i++){
		debug->SystemTable->ConOut->SetCursorPosition(debug->SystemTable->ConOut,50,i+1);
		for(j=0;j < X_BUF_LINE;j++){
			debug->SystemTable->ConOut->OutputString(debug->SystemTable->ConOut,itoa(debug->block_buffer[j][i],str));
		}
	}

	UpdateDisplay(debug->console, X_BUF_LINE-XLINE, Y_BUF_LINE-YLINE, XLINE, YLINE, debug->block_buffer);
}

EFI_STATUS EFIAPI EfiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable){
	//画面のリセット
	SystemTable->ConOut->Reset(SystemTable->ConOut,0);

	//プロトコルのポインタを取得
	EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
	EFI_GUID GOP_GUID = {0x9042a9de,0x23dc,0x4a38,{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}};
	SystemTable->BootServices->LocateProtocol(&GOP_GUID,NULL,(void **)&GOP);
	EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *TIP;
	EFI_GUID TIP_GUID = {0xdd9e7534,0x7762,0x4698,{0x8c,0x14,0xf5,0x85,0x17,0xa6,0x25,0xaa}};
	SystemTable->BootServices->LocateProtocol(&TIP_GUID, NULL,(void**)&TIP);

	//画面の情報を取得
	Console console;
	console.GOP = GOP;
	console.height = GOP->Mode->Info->VerticalResolution;
	console.width  = GOP->Mode->Info->HorizontalResolution;
	console.FBB = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)GOP->Mode->FrameBufferBase;

	//枠の表示
	DrawRectangle(&console, X_DISPLAY_ANCHOR-LINE_SIZE-1 , Y_DISPLAY_ANCHOR-LINE_SIZE-1 , UNIT*XLINE+LINE_SIZE*2+1, LINE_SIZE             , line_color);  //x上
	DrawRectangle(&console, X_DISPLAY_ANCHOR-LINE_SIZE   , Y_DISPLAY_ANCHOR+UNIT*YLINE+1, UNIT*XLINE+LINE_SIZE*2  , LINE_SIZE             , line_color);  //x下
	DrawRectangle(&console, X_DISPLAY_ANCHOR-LINE_SIZE-1 , Y_DISPLAY_ANCHOR-LINE_SIZE   , LINE_SIZE               , UNIT*YLINE+LINE_SIZE+1, line_color);  //y左
	DrawRectangle(&console, X_DISPLAY_ANCHOR+UNIT*XLINE+1, Y_DISPLAY_ANCHOR-LINE_SIZE   , LINE_SIZE               , UNIT*YLINE+LINE_SIZE*2, line_color);  //y右

	int i,j;
	//block_buffer ブロックの位置情報を保持
	int block_buffer[X_BUF_LINE][Y_BUF_LINE];
	for(i=0;i<Y_BUF_LINE;i++){
		for(j=0;j<X_BUF_LINE;j++){
			block_buffer[j][i]=BACKGRUND;
		}
	}

	Block current = {Z_MINO,UP,BLOCK_INIT_X,BLOCK_INIT_Y};

	Timer timer;
	timer.console=&console;
	timer.current=&current;
	timer.block_buffer=block_buffer;

	//fall timer
    EFI_EVENT fall_timer;
    SystemTable->BootServices->CreateEvent(EVT_TIMER|EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DownTimerHandle, &timer, &fall_timer);
    SystemTable->BootServices->SetTimer(fall_timer, TimerPeriodic, 5000000);

	//key timer
	EFI_EVENT key_timer;
    SystemTable->BootServices->CreateEvent(EVT_TIMER|EVT_NOTIFY_SIGNAL, TPL_CALLBACK, KeyTimerHandle, &timer, &key_timer);
    SystemTable->BootServices->SetTimer(key_timer, TimerPeriodic, 500000);

	/*
	//debug timer
	Debug debug = {SystemTable, &console, block_buffer};
	EFI_EVENT debug_timer;
    SystemTable->BootServices->CreateEvent(EVT_TIMER|EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DebugTimerHandle, &debug, &debug_timer);
    SystemTable->BootServices->SetTimer(key_timer, TimerPeriodic, 10000000);
	*/

	//down key
    void *notify_down;
    EFI_KEY_DATA key_data_down={{0x02,0},{0,0}};
    TIP->RegisterKeyNotify(TIP, &key_data_down, DownKeyHandle, &notify_down);

	//right key
    void *notify_right;
    EFI_KEY_DATA key_data_right={{0x03,0},{0,0}};
    TIP->RegisterKeyNotify(TIP, &key_data_right, RightKeyHandle, &notify_right);

	//left key
    void *notify_left;
    EFI_KEY_DATA key_data_left={{0x04,0},{0,0}};
    TIP->RegisterKeyNotify(TIP, &key_data_left, LeftKeyHandle, &notify_left);

	//up key
    void *notify_up;
    EFI_KEY_DATA key_data_up={{0x01,0},{0,0}};
    TIP->RegisterKeyNotify(TIP, &key_data_up, UpKeyHandle, &notify_up);

	//space key
    void *notify_space;
    EFI_KEY_DATA key_data_space={{0x0,0x20},{0,0}};
    TIP->RegisterKeyNotify(TIP, &key_data_space, SpaceKeyHandle, &notify_space);

	CHAR16 str[128]={0};
	CHAR16 strx[128]={0};
	CHAR16 stry[128]={0};
	CHAR16 strs[128]={0};
	CHAR16 strr[128]={0};
	CHAR16 strl[128]={0};

	Init(&console, &current, block_buffer);

	score = 0;

	while(1){
		if(gameover_flag != 0){
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,23);
			SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Game Over");
		}else if(score_flag != 0){
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,20);
			SystemTable->ConOut->OutputString(SystemTable->ConOut,L"score:");
			SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(score,strs));
			if(score_flag == TETRIS){
				SystemTable->ConOut->OutputString(SystemTable->ConOut,L" Tetris        ");
			}else if(score_flag == TSPIN_SINGLE){
				SystemTable->ConOut->OutputString(SystemTable->ConOut,L" T-spin single ");
			}else if(score_flag == TSPIN_DOUBLE){
				SystemTable->ConOut->OutputString(SystemTable->ConOut,L" T-spin double ");
			}else if(score_flag == TSPIN_TRIPLE){
				SystemTable->ConOut->OutputString(SystemTable->ConOut,L" T-spin triple ");
			}else{
				SystemTable->ConOut->OutputString(SystemTable->ConOut,L"               ");
			}
			score_flag = 0;
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,21);
			SystemTable->ConOut->OutputString(SystemTable->ConOut,L"line :");
			SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(delete_Line,strl));
		}else if(init_flag != 0){
			SystemTable->BootServices->CloseEvent(fall_timer);
			Init(&console, &current, block_buffer);
			SystemTable->BootServices->CreateEvent(EVT_TIMER|EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DownTimerHandle, &timer, &fall_timer);
			int fall_time = 5000000/((delete_Line/5)+1) > 500000 ? 5000000/((delete_Line/5)+1) : 500000;
    		SystemTable->BootServices->SetTimer(fall_timer, TimerPeriodic, fall_time);
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,22);
			SystemTable->ConOut->OutputString(SystemTable->ConOut,L"next kind:");
			SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(random_block[random_flag],strr));
		}

		//Debug print anchor
		//DrawRectangle(&console,X_DISPLAY_ANCHOR+UNIT*current.x_anchor,Y_DISPLAY_ANCHOR+UNIT*(current.y_anchor-4),UNIT,UNIT,debug_color);
		//Debug (print block_buffer)
		SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,50,0);
		SystemTable->ConOut->OutputString(SystemTable->ConOut,L"<Block memory>");
		for(i=0;i < Y_BUF_LINE;i++){
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,50,i+1);
			for(j=0;j < X_BUF_LINE;j++){
				SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(block_buffer[j][i],str));
			}
		}
		SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,1);
		SystemTable->ConOut->OutputString(SystemTable->ConOut,L"rotate :");
		SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(current.rotate,str));
		SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,2);
		SystemTable->ConOut->OutputString(SystemTable->ConOut,L"x anchor :");
		SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(current.x_anchor,strx));
		SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,3);
		SystemTable->ConOut->OutputString(SystemTable->ConOut,L"y anchor :");
		SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(current.y_anchor,stry));

			int i,j,y=current.y_anchor,x=current.x_anchor;
		for(i=(y >= 0 ? y:0); i < (y+4 <= Y_BUF_LINE ? y+4:Y_BUF_LINE);i++){
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut,30,i-y+5);
			for(j=(x >= 0 ? x:0); j < (x+4 <= X_BUF_LINE ? x+4:X_BUF_LINE); j++){
				SystemTable->ConOut->OutputString(SystemTable->ConOut,itoa(blocks[current.kind][current.rotate][i-y][j-x],str));
			}
		}
	}
	return EFI_SUCCESS;
}