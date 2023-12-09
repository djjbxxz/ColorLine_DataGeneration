# ColorLine
ColorLine is a puzzle board game. Score points by moving pieces of the same color together on a fixed size board(default is 5 by 5). Pieces can only be moved on empty spots and cannot jump over other pieces. When 5 pieces are connected horizontally, vertically and diagonally (45��), the connected pieces are eliminated and the score is based on the number of eliminated pieces. 

ColorLine ��һ���������������Ϸ���ڹ̶������̳ߴ��ϣ�Ĭ��Ϊ5��5����ͨ������ͬ��ɫ�������ƶ���һ��ȡ�÷���������ֻ���ڿյ�λ�����ƶ������������������ӡ���������ˮƽ����ֱ���Խ���б(45��)�����������ﵽ5��ʱ�����������ӽ��ᷢ���������������������ӵĸ����÷֡�


## Game Rule
At the beginning of the game, 5 pieces are placed on the board and the color of the next 3 pieces will be displayed (the position on the board is unknown). The user specifies a piece and moves it to the desired place. If the move does not result in 5 or more pieces of the same color being connected on the board, 3 new pieces will randomly appear in the empty space on the board; if the move results in 5 or more pieces of the same color being connected on the board, these pieces will be eliminated, rewarded. No new pieces will appear on the board in the next round.

The game ends when there is no space on the board for a new piece, or when no piece can move after a new piece is placed.

����Ϸ��ʼʱ�������ϻ����5�����ӣ����һ���ʾ�´ν�����ֵ�3�����ӵ���ɫ���������ϳ��ֵ�λ����δ֪�ģ����û�ָ��һ�����ӣ������ƶ�����Ҫ�ĵط���������ƶ�û��ʹ�������ϳ���5�Ż�������ͬ��ɫ�������������򽫻���3���µ�������������������ϵĿ�λ�ã�������ƶ�ʹ�������ϳ�����5�ż�����������ͬ��ɫ����������������Щ���ӽ����������õ������������û��ܹ��ٴν����ƶ��������ϲ�������µ����ӡ�

��������û�п�λ�����µ����ӣ����߷������µ����Ӻ��κ����Ӷ��޷������ƶ�ʱ����Ϸ�ﵽ�վ֡�


## Game configuration
The following parameters ard configurable in [`Base.h`](Base.h)

| Configurable params          | Default        | Description |
| -----------------------------|----------------|------------------|
| COLOR_NUM                    | 4              | The number of color |
| BOARD_SIZE                   | 5              | The size of the board |
| COMING_CHESS_NUM             | 2              | The number of the chess will be placed in every round (if applicable) |
| MIN_ELEMINATABLE_NUM         | 4              | The minimal number of the connnected chess that can be eleminated |
| EACH_CHESS_ELEMINATED_REWARD | 2              | The reward from every eleminated chess |


## Compilation
C++14 standard required

For Linux, g++ version >=5 
**[C++ standrad and gcc version] (https://gcc.gnu.org/projects/cxx-status.html#cxx14)**

For Windows, use latest MSVC

### CMake
```bash
mkdir build
cd build
cmake ..
make
```

##Misc
This is a submodule, and is designed for **[this](https://github.com/djjbxxz/sac-discrete-tf-colorline)** project.

Please make sure the game configuration in this submodule is the same as the one in **[here](https://github.com/djjbxxz/sac-discrete-tf-colorline)**.