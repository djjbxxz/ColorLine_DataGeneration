#pragma once

#include "my_func.h"
#include <map>

#ifdef _PRINT
#include <iostream>
#endif // _PRINT

using myfunc::Random;

constexpr auto BOARD_SIZE = 9;
constexpr auto POTENTIAL_MOVE_NUM = BOARD_SIZE * BOARD_SIZE * BOARD_SIZE * BOARD_SIZE;
class Color {
	enum class _color :char
	{
		empty = 0,
		green = 1,
		red = 2,
		dark_blue = 3,
		yellow = 4,
		light_blue = 5,
		purple = 6,
		brown = 7
	};
public:
	Color(_color _value) :value(_value) {};
	Color() :value(_color::empty) {};
public:
	_color value;
	static const std::vector<Color> all_color;
	static const std::vector<Color> all_statu;
	static const Color empty;
public:
	static Color rand_color_except(std::vector<Color>& exclusion);
	static Color rand_color_except(Color _color);
	static Color rand_color();
	//static Color rand_statu();
	static Color rand_statu_except(Color _color);
	bool operator== (const Color& a)const {
		return this->value == a.value;
	}
	bool operator!= (const Color& a)const {
		return this->value != a.value;
	}
#ifdef _PRINT
public:
	int print()const { return (int)value; };
#endif // DEBUG
};

class Point
{
public:
	char x, y;
	Point(char	_x, char _y) :x(_x), y(_y) {};
	Point(int	_x, int _y) :x(_x), y(_y) {};
	Point(char index) { x = index / BOARD_SIZE; y = index % BOARD_SIZE; };
	Point(int index) { x = index / BOARD_SIZE; y = index % BOARD_SIZE; };
	//Point() =delete;
	Point(const Point& point) {
		x = point.x; y = point.y;
	};
	Point& operator=(const Point& point) {
		x = point.x; y = point.y;
		return *this;
	}
	Point(Point&& point)noexcept
		:x(std::move(point.x)), y(std::move(point.y)) {};//move constructor, noexcept is for vector::push_back
	Point() {
		x = -1; y = -1;
	};
	bool operator== (const Point& a)const
	{
		return this->x == a.x && this->y == a.y;
	}
	bool operator!= (const Point& a)const
	{
		return this->x != a.x || this->y != a.y;
	}
	Point operator+ (const Point& a)const
	{
		return { x + a.x, y + a.y };
	}
	Point operator*(const char step)const
	{
		return { x * step,y * step };
	}
	Point operator-(const Point& a)const//����
	{
		return { x - a.x, y - a.y };
	}
	Point operator-()const//ȡ��
	{
		return { -x,-y };
	}
	Point operator+=(const Point& a)
	{
		return { x + a.x,y + a.y };
	}
	char index()const //return 1D index
	{
		return x * BOARD_SIZE + y;
	}
	bool outofrange()const
	{
		return x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE;
	}
	Point walk(const Point& offset, int step)const
	{
		return{ *this + offset * step };
	}
	Point walk_r(const Point& offset, int step)const
	{
		return{ *this - offset * step };
	}
	float distance(const Point& point)const
	{
		return std::sqrt(std::pow((point.x - x), 2) + std::pow((point.y - y), 2));
	}


#ifdef _PRINT
	void print()const { printf("(%d,%d)", (int)x, (int)y); };
#endif // DEBUG

private:

};

class Direction
{
public:
	enum class _direction :char
	{
		north,
		northeast,
		east,
		southeast,
		south,
		southwest,
		west,
		northwest,
	};
public:
	Direction(_direction d) :direction(d) {};
	Direction() :direction(_direction::north) {};
public:
	_direction direction;
	const Point& point()const { return direct2point.find(direction)->second; };
	Point walk(const Point& startpoint, int step)const { return startpoint.walk(point(), step); }
	Point walk_r(const Point& startpoint, int step)const { return startpoint.walk(-point(), step); }//walk reversely
	Direction operator-()const {
		switch (direction)
		{
		case Direction::_direction::north:
			return{ _direction::south };
		case Direction::_direction::northeast:
			return{ _direction::southwest };
		case Direction::_direction::east:
			return{ _direction::west };
		case Direction::_direction::southeast:
			return{ _direction::northwest };
		case Direction::_direction::south:
			return{ _direction::north };
		case Direction::_direction::southwest:
			return{ _direction::northeast };
		case Direction::_direction::west:
			return{ _direction::east };
		case Direction::_direction::northwest:
			return{ _direction::southeast };
		default:
			return{};
		}
	};
public:
	static const std::map < Direction::_direction, Point > direct2point;
	static const std::vector <Direction> eight_neighbor;
	static const std::vector <Direction> four_neighbor;
	static const std::vector <Direction> scan_direction;

};

class Move
{
public:
	Point start;
	Point end;
	Move(const Point& p1, const Point& p2) :start(p1), end(p2) {};
	bool operator==(const Move& a)const
	{
		return start == a.start && end == a.end;
	}
	Move() :start(Point()), end(Point()) {};
	operator int() const {
		return to_densed();
	};
	static Move from_densed(int densed)
	{
		char x1, y1, x2, y2;
		x1 = densed / (BOARD_SIZE * BOARD_SIZE * BOARD_SIZE);
		y1 = densed / (BOARD_SIZE * BOARD_SIZE) - x1 * BOARD_SIZE;
		x2 = densed / BOARD_SIZE - x1 * BOARD_SIZE * BOARD_SIZE - y1 * BOARD_SIZE;
		y2 = densed % BOARD_SIZE;
		return{ {x1,y1},{x2,y2} };
	}
	int to_densed()const {
		return start.x * BOARD_SIZE * BOARD_SIZE * BOARD_SIZE + start.y * BOARD_SIZE * BOARD_SIZE + end.x * BOARD_SIZE + end.y;
	};
#ifdef _PRINT
	void print()
	{
		std::cout << "Move from ";
		start.print();
		std::cout << " to ";
		end.print();
		std::cout << std::endl;
	}
#endif // _PRINT
};

template<typename _Statu>
class _MAP
{
public:
	void set(const Point& point, const _Statu& _statu)
	{
#ifdef _DEBUG
		assert(_data.size() > point.index());
#endif // _DEBUG
		_data[point.index()] = _statu;
	};
	void set(int index, const _Statu& _statu)
	{
#ifdef _DEBUG
		assert(_data.size() > index);
#endif // _DEBUG
		_data[index] = _statu;
	};
	void set_all(const std::vector<Point>& points, const _Statu& _statu)
	{
		for (auto&& point : points)
			set(point, _statu);
	}
	const _Statu& get(const Point& point)const
	{
#ifdef _DEBUG
		assert(_data.size() > point.index());
#endif // _DEBUG
		return _data[point.index()];
	}

	void reset(int num, _Statu value)
	{
		_data = std::vector<_Statu>(num, value);
	};

	template<typename FUNC>
	static std::vector<Point> get_surround_by_filter(const Point& point, FUNC condition)
	{
		std::vector<Point>temp;
		for (auto&& direction : Direction::four_neighbor)
		{
			auto new_point = direction.walk(point, 1);
			if (condition(new_point))
				temp.push_back(new_point);
		}
		return temp;
	}

	std::vector<Point> dilate(const std::vector<Point>& points)const
	{
		std::vector<Point>result;
		_MAP<bool> bool_map;
		bool_map.reset(BOARD_SIZE * BOARD_SIZE, false);
		for (auto&& point : points)
			bool_map.set(point, true);
		for (auto&& point : points)
		{
			auto surround_points = get_surround_by_filter(point, [&](const Point& new_point)
				{return (!new_point.outofrange()) && (bool_map.get(new_point) == false); });
			for (auto&& point : surround_points)
			{
				result.push_back(point);
				bool_map.set(point, true);
			}
		}
		return result;
	}
public:
	std::vector<_Statu> _data;
	};

class Game_map :public _MAP<Color>
{
public:
	Game_map() {
		reset();
	};
	Game_map& operator= (const Game_map& other)
	{
		_data = other._data;
		next_three = other.next_three;
		return *this;
	}//Copy assignment
	Game_map(Game_map&& g)noexcept
	{
		_data = std::move(g._data);
		next_three = std::move(g.next_three);
	};
	Color& operator[](int index)
	{
		return _data[index];
	}
	const Color& operator[](int index)const
	{
		return _data[index];
	}
public:
	const Point& pick_a_spot(const std::vector<Point>& exclusion)const;
	const Point& pick_a_spot()const;
	void reset() {
		_data = std::vector<Color>(BOARD_SIZE * BOARD_SIZE);
		next_three = std::vector<Color>(3);
	};
	void set_next_three(const std::vector<Color>& _next_three) {
		next_three = _next_three;
	}
	Color* get__data_ptr() {
		return _data.data();
	};
public:
	const static std::vector<Point> _empty;
private:
	static std::vector<Point> _empty_init();
public:
	std::vector<Color> next_three;
#ifdef _PRINT
public:
	void print()const;
#endif // _DEBUG
};

class Lined_chess
{
public:
	Lined_chess(Game_map* game_map, Point lined_startpoint, Direction _lined_direction, int _lined_num) :
		endpoint1(lined_startpoint),
		lined_direction(_lined_direction),
		lined_num(_lined_num)
	{
		endpoint2 = _lined_direction.walk(lined_startpoint, _lined_num - 1);
		points.reserve(_lined_num);
		points.push_back(endpoint1);
		for (int i = 0; i < _lined_num - 1; i++)
			points.emplace_back(_lined_direction.walk(endpoint1, i + 1));
		std::vector<Point> not_safe_destinations;
		not_safe_destinations.emplace_back(lined_direction.walk(endpoint2, 1));
		not_safe_destinations.emplace_back(lined_direction.walk_r(endpoint1, 1));
		for (auto& point : not_safe_destinations)
			if (!point.outofrange())
				possible_destinations.emplace_back(point);
		if (possible_destinations.empty())
			return;
		destination = Random::rand_choice(possible_destinations);
		exclusion.insert(exclusion.end(), points.begin(), points.end());
		exclusion.insert(exclusion.end(), possible_destinations.begin(), possible_destinations.end());
		startpoint = game_map->pick_a_spot(exclusion);
		exclusion.emplace_back(startpoint);
#ifdef _DEBUG
		assert(!myfunc::is_inlist(startpoint, points));
#endif // _DEBUG

	}
	Lined_chess() {};
public:
	int lined_num;
	Direction lined_direction;
	Point endpoint1;
	Point endpoint2;
	Point startpoint;
	Point destination;
	std::vector<Point> possible_destinations;
	std::vector<Point> points;
	std::vector<Point>exclusion;
private:

};


