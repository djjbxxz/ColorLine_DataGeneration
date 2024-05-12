#include "gen_data.h"
#include <iostream>
#include "Pathfinding.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <sstream>
#include <string>
#include <iostream>
#include "CompileTimeMsg.h"
#include <pybind11/numpy.h>

constexpr auto INPUT_CHANNEL_SIZE = (1 + COMING_CHESS_NUM) * COLOR_NUM;
namespace export_bind
{
	class Game_statu_9928;
	class Game_statu_994;
	int color_one_hot_to_index(int* one_hot)
	{
		FOR_RANGE(i, COLOR_NUM)
		{
			if (one_hot[i] == 1)
				return i + 1;
		}
		return 0;
	}
	class Game_statu_9928
	{
	public:
		Game_statu_9928() {};
		Game_statu_9928(const Game_map& game_map)
		{
			auto&& game_board = game_map._data;
			auto&& coming_chess = game_map.coming_chess;

			FOR_RANGE(i, BOARD_SIZE)
				FOR_RANGE(j, BOARD_SIZE)
			{
				int color = (int)game_board[i * BOARD_SIZE + j];
				if (color)
					value[i][j][color - 1] = 1;
			}
			FOR_RANGE(index, COMING_CHESS_NUM)
			{
				int color = (int)coming_chess[index];
				FOR_RANGE(i, BOARD_SIZE)
					FOR_RANGE(j, BOARD_SIZE)
					value[i][j][(COLOR_NUM - 1) + index * COLOR_NUM + color] = 1;
			}
		}

		Game_map to_game_map()
		{
			Game_map game_map;
			auto&& game_board = game_map._data;
			auto&& coming_chess = game_map.coming_chess;
			game_board = { 0 };
			coming_chess = { 0 };
			FOR_RANGE(i, BOARD_SIZE)
			{
				FOR_RANGE(j, BOARD_SIZE)
				{
					auto color_one_hot = std::array<int, COLOR_NUM>{0};
					FOR_RANGE(k, COLOR_NUM)
						color_one_hot[k] = value[i][j][k];

					int color = color_one_hot_to_index(color_one_hot.data());
					game_board[i * BOARD_SIZE + j] = color;
				}
			}
			FOR_RANGE(index, COMING_CHESS_NUM)
			{
				auto color_one_hot = std::array<int, COLOR_NUM>{0};
				FOR_RANGE(k, COLOR_NUM)
					color_one_hot[k] = value[0][0][COLOR_NUM + index * COLOR_NUM + k];

				int color = color_one_hot_to_index(color_one_hot.data());
				coming_chess[index] = color;
			}
			return game_map;
		}
		Game_statu_9928(const int* data)
		{
			int* value_ptr = reinterpret_cast<int*>(value);
			FOR_RANGE(p, BOARD_SIZE * BOARD_SIZE * INPUT_CHANNEL_SIZE)
				* (value_ptr + p) = *(data + p);
		}
		int value[BOARD_SIZE][BOARD_SIZE][INPUT_CHANNEL_SIZE]{ 0 };

	};

	class Game_statu_994
	{
	public:
		Game_statu_994() {};
		Game_statu_994(std::array<int, BOARD_SIZE* BOARD_SIZE* (COMING_CHESS_NUM + 1)>& game_statu_9928)
		{
			auto ptr_game_map_9928 = reinterpret_cast<int(*)[BOARD_SIZE][INPUT_CHANNEL_SIZE]>(game_statu_9928.data());
			int game_board[BOARD_SIZE][BOARD_SIZE]{ 0 };
			int coming_chess[COMING_CHESS_NUM]{ 0 };
			FOR_RANGE(i, BOARD_SIZE)
			{
				FOR_RANGE(j, BOARD_SIZE)
				{
					std::array<int, COLOR_NUM> color_one_hot{ 0 };
					FOR_RANGE(k, COLOR_NUM)
						color_one_hot[k] = ptr_game_map_9928[i][j][k];
					int color = color_one_hot_to_index(color_one_hot.data());
					game_board[i][j] = color;
				}
			}
			FOR_RANGE(i, COMING_CHESS_NUM)
			{
				std::array<int, COLOR_NUM> color_one_hot{ 0 };
				FOR_RANGE(j, COLOR_NUM)
					color_one_hot[j] = ptr_game_map_9928[0][0][COLOR_NUM + i * COLOR_NUM + j];
				coming_chess[i] = color_one_hot_to_index(color_one_hot.data());
			}

			FOR_RANGE(i, BOARD_SIZE)
				FOR_RANGE(j, BOARD_SIZE)
				value[i][j][0] = game_board[i][j];
			FOR_RANGE(k, COMING_CHESS_NUM)
				FOR_RANGE(i, BOARD_SIZE)
				FOR_RANGE(j, BOARD_SIZE)
				value[i][j][k + 1] = coming_chess[k];
		}
		Game_statu_994(int* ptr)
		{
			int* value_ptr = reinterpret_cast<int*>(value);
			FOR_RANGE(p, BOARD_SIZE * BOARD_SIZE * (COMING_CHESS_NUM + 1))
				* (value_ptr + p) = *(ptr + p);
		}
		Game_statu_994(const Game_map& game_map)
		{
			FOR_RANGE(i, BOARD_SIZE)
				FOR_RANGE(j, BOARD_SIZE)
				value[i][j][0] = int(game_map._data[i * BOARD_SIZE + j]);
			FOR_RANGE(k, COMING_CHESS_NUM)
				FOR_RANGE(i, BOARD_SIZE)
				FOR_RANGE(j, BOARD_SIZE)
				value[i][j][k + 1] = int(game_map.coming_chess[k]);
		}
		Game_map to_game_map()
		{
			Game_map game_map;
			FOR_RANGE(i, BOARD_SIZE)
				FOR_RANGE(j, BOARD_SIZE)
				game_map._data[i * BOARD_SIZE + j] = value[i][j][0];
			FOR_RANGE(k, COMING_CHESS_NUM)
				game_map.coming_chess[k] = value[0][0][k + 1];
			return game_map;
		}
		int value[BOARD_SIZE][BOARD_SIZE][COMING_CHESS_NUM + 1]{ 0 };

	};

	class Point_int
	{
	public:
		Point_int() = delete;
		Point_int(const Point& point) :x(point.x), y(point.y) {};
		Point_int(int x, int y) :x(x), y(y) {};
		int x;
		int y;
	};
	Game_map generate_data(int lined_num, float fill_ratio)
	{
		lined_num = lined_num >= 2 ? lined_num : 2;
		lined_num = lined_num <= 8 ? lined_num : 8;
		fill_ratio = fill_ratio >= 0.05 ? fill_ratio : 0.05;
		fill_ratio = fill_ratio < 0.99 ? fill_ratio : 0.99;
		Game_map game_map;
		auto a = Gen_data(lined_num, fill_ratio, &game_map);
		a.go();
		return game_map;
	}

	Moveable_mask get_valid_mask(const Game_map& game_map)
	{
		return Legal_mask(game_map).get_result();
	}

	int rule(Game_map& game_map, int move)
	{
		return Game_rule(game_map, Move(move)).rule();
	}

	auto get_path(pybind11::array_t<int> array, int move)
	{
		pybind11::buffer_info info = array.request();

		int* data = static_cast<int*>(info.ptr);
		int size = info.size;
		Move move_ = Move(move);
		auto start = move_.start;
		auto end = move_.end;
		auto path = Pathfinding::A_star{ start, end,{ data } }.get_path();
		return std::vector<Point_int>(path.begin(), path.end());
	}

	void seed(int seed)
	{
		Random::seed(seed);
	}
}
