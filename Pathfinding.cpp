#include "Pathfinding.h"
using Pathfinding::A_star;

std::vector<Point> A_star::get_path()
{
	return algorithm_main()->get_path();
}

A_star::Path_tree_Node* A_star::algorithm_main()
{
	all_points.reserve(BOARD_SIZE * BOARD_SIZE);
	add_to_openlist(startpoint);
	while (!openlist.empty())
	{
		auto &point = *get_point_from_openlist();
		if (point == endpoint)
			return &point;
		auto reachables = get_reachable(point);
		for (auto& reachable_point : reachables)
		{
			if (game_map[reachable_point] == GRID_STATU::NERVER_VISIT)
			{
				add_to_openlist(reachable_point, &point);
			}
			else
			{
				for (const auto& openlist_point : openlist)
					if (*openlist_point == reachable_point)
						openlist_point->update(point);
			}
		}
	}
#ifdef _DEBUG
	assert(true);//No path found

#endif // DEBUG
	return nullptr;
}

A_star::Path_tree_Node* A_star::get_point_from_openlist()
{
	auto min_it = std::min_element(openlist.begin(), openlist.end());
	
	Path_tree_Node* point = *min_it;
	openlist.erase(min_it);
	game_map[*point] = GRID_STATU::IN_CLOSELIST;
	return point;
}

std::vector<Point> A_star::get_reachable(const Point& point) const
{
	std::vector<Point>temp;
	for (const auto& each_direction : Direction::four_neighbor)
	{
		auto next_point = each_direction.walk(point, 1);
		if (!next_point.outofrange() && (game_map[next_point] == GRID_STATU::NERVER_VISIT || game_map[next_point] == GRID_STATU::IN_OPENLIST))
			temp.emplace_back(next_point);
	}
	return temp;
}


void Pathfinding::A_star::add_to_openlist(Point& point, Path_tree_Node* _last)
{
	all_points.emplace_back(point, endpoint, _last);
	game_map[point] = GRID_STATU::IN_OPENLIST;
	openlist.push_back(&all_points.back());
}

std::vector<Point> Pathfinding::A_star::Path_tree_Node::get_path()
{
	std::vector <Point>path_reverse;
	const Path_tree_Node* ptr = this;
	while (ptr)
	{
		path_reverse.emplace_back(ptr->x, ptr->y);
		ptr = ptr->last;
	}
	std::vector <Point>path;
	path.reserve(path_reverse.size());
	for (int Index = path_reverse.size() - 1; Index >= 0; Index--)
	{
		path.push_back(path_reverse[Index]);
	}
	return path;
}
