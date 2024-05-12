#ifdef _EXPORT
#include "Export.h"

namespace py = pybind11;
using namespace export_bind;

PYBIND11_MODULE(gen_colorline_data_tensorflow, m)
{
	std::ostringstream oss;
	oss << std::endl << "***********Configuration***********" << std::endl << std::endl;
	oss << "COLOR_NUM: " << COLOR_NUM << std::endl;
	oss << "BOARD_SIZE: " << BOARD_SIZE << std::endl;
	oss << "COMING_CHESS_NUM: " << COMING_CHESS_NUM << std::endl;
	oss << "MIN_ELEMINATABLE_NUM: " << MIN_ELEMINATABLE_NUM << std::endl;
	oss << "INVALID_MOVE_PENALTY: " << INVALID_MOVE_PENALTY << std::endl;
	oss << "EACH_CHESS_ELEMINATED_REWARD: " << EACH_CHESS_ELEMINATED_REWARD << std::endl;
	oss << std::endl << "Compiled for Tensorflow" << " | Python version : " << PYVERSION \
		<< " | System : " << SYSTEM_NAME << " " << SYSTEM_VERSION << " | Compile time : " << COMPILE_TIME << std::endl;

	oss << std::endl << "***********Configuration***********" << std::endl << std::endl;

	std::string str = oss.str();
	m.doc() = str;
	//std::cout << str << std::endl;

	m.def("get_random_start", &generate_data);
	m.def("get_valid_mask", &get_valid_mask);
	m.def("rule", &rule);
	m.def("get_path", &get_path);
	m.def("seed", &seed);

	pybind11::class_<Game_map>(m, "Game_map", pybind11::buffer_protocol())
		.def_buffer([](Game_map& m) -> pybind11::buffer_info {
		return pybind11::buffer_info(
			m.get__data_ptr(),								/* Pointer to buffer */
			sizeof(char),									/* Size of one scalar */
			pybind11::format_descriptor<char>::format(),	/* Python struct-style format descriptor */
			2,												/* Number of dimensions */
			{ BOARD_SIZE,BOARD_SIZE },						/* Buffer dimensions */
			{ sizeof(char) * BOARD_SIZE, sizeof(char) }   /* Strides (in bytes) for each index */
	);

			})
		.def(pybind11::init([](py::array arr) ->Game_map {
				py::buffer_info info = arr.request();
			if (arr.dtype().kind() != 'i') {
				std::cout << "Invalid data type, must be int" << std::endl;
				return Game_map();
			}
			if (info.shape[0] != BOARD_SIZE || info.shape[1] != BOARD_SIZE) {
				std::cout << "Invalid data shape, must be " << BOARD_SIZE << "x" << BOARD_SIZE << std::endl;
				return Game_map();
			}
			if (info.ndim == 2) {
				//std::cout << "Warning: comming chess is not set" << std::endl;
				return Game_map((int*)(arr.data()));
			}
			if (info.ndim == 3 && info.shape[2] == (COMING_CHESS_NUM+1))
			{
				return Game_statu_994((int*)(arr.data())).to_game_map();
			}
			if (info.ndim == 3 && info.shape[2] == INPUT_CHANNEL_SIZE)
			{
				return Game_statu_9928((int*)(arr.data())).to_game_map();
			}
			std::cout << "Invalid data" << std::endl << "number of dim: " << info.ndim << std::endl;
			std::cout << "shape: ";
			FOR_RANGE(d, info.ndim)
				std::cout << info.shape[d] << " ";
			std::cout << std::endl;
			return Game_map();
			}))
		.def(pybind11::init([](Game_statu_994 _994) ->Game_map {
			return _994.to_game_map();
		}))
		.def(pybind11::init([](Game_statu_9928 _9928) ->Game_map {
			return _9928.to_game_map();
		}))
		.def("set_coming_chess", [](Game_map& m, py::array arr) {
			py::buffer_info info = arr.request();
		if (arr.dtype().kind() != 'i') {
			std::cout << "Invalid data type, must be int" << std::endl;
			return;
		}
		if (info.ndim != 1) {
			std::cout << "Invalid data shape, must be 1D" << std::endl;
			return;
		}
		if (info.shape[0] != COMING_CHESS_NUM) {
			std::cout << "Invalid data shape, must be " << COMING_CHESS_NUM << std::endl;
			return;
		}
		auto ptr = static_cast<int*>(info.ptr);
		std::array<Color, COMING_CHESS_NUM> coming_chess;
		FOR_RANGE(i, COMING_CHESS_NUM)
			coming_chess[i] = Color(ptr[i]);

		m.set_coming_chess(coming_chess);
		})
		.def("get_coming_chess", [](Game_map& m) {
			py::array_t<int> arr({ COMING_CHESS_NUM });
			auto ptr = arr.mutable_unchecked<1>();
			FOR_RANGE(i, COMING_CHESS_NUM)
				ptr(i) = char(m.coming_chess[i].value);
			return arr;
			})
		;
		
	pybind11::class_<Moveable_mask>(m, "Moveable_mask", pybind11::buffer_protocol())
		.def_buffer([](Moveable_mask& m) -> pybind11::buffer_info {
		return pybind11::buffer_info(
			m.data(),								/* Pointer to buffer */
			sizeof(int),									/* Size of one scalar */
			pybind11::format_descriptor<int>::format(),	/* Python struct-style format descriptor */
			1,												/* Number of dimensions */
			{ POTENTIAL_MOVE_NUM },						/* Buffer dimensions */
			{ sizeof(int) }   /* Strides (in bytes) for each index */
	);
			});

	pybind11::class_<Color>(m, "Color", pybind11::buffer_protocol())
		.def_buffer([](Color& m) -> pybind11::buffer_info {
		return pybind11::buffer_info(
			&m.value,								/* Pointer to buffer */
			sizeof(char),									/* Size of one scalar */
			pybind11::format_descriptor<char>::format(),	/* Python struct-style format descriptor */
			1,												/* Number of dimensions */
			{ 1 },						/* Buffer dimensions */
			{ sizeof(char) }   /* Strides (in bytes) for each index */
	);

			});

	pybind11::class_<Game_statu_9928>(m, "Game_statu_9928", pybind11::buffer_protocol())
		.def_buffer([](Game_statu_9928& m) -> pybind11::buffer_info {
		return pybind11::buffer_info(
			&m.value,								/* Pointer to buffer */
			sizeof(int),									/* Size of one scalar */
			pybind11::format_descriptor<int>::format(),	/* Python struct-style format descriptor */
			3,												/* Number of dimensions */
			{ BOARD_SIZE,BOARD_SIZE ,INPUT_CHANNEL_SIZE },						/* Buffer dimensions */
			{ sizeof(int) * BOARD_SIZE * INPUT_CHANNEL_SIZE,sizeof(int) * INPUT_CHANNEL_SIZE,sizeof(int) }   /* Strides (in bytes) for each index */
			);})
		.def(pybind11::init([](py::array arr) ->Game_statu_9928 {
				py::buffer_info info = arr.request();
			if (arr.dtype().kind() != 'i') {
				std::cout << "Invalid data type, must be int" << std::endl;
				return Game_statu_9928();
			}
			if (info.ndim != 3) {
				std::cout << "Invalid data shape, must be 3D" << std::endl;
				return Game_statu_9928();
			}
			if (info.shape[0] == BOARD_SIZE && info.shape[1] == BOARD_SIZE)
			{
				auto data = static_cast<int*>(info.ptr);
				switch (info.shape[2])
				{
				case COMING_CHESS_NUM + 1: 
					return Game_statu_9928(Game_statu_994(data).to_game_map());
				case INPUT_CHANNEL_SIZE:
					return Game_statu_9928(data);
				default:
					break;
				}
			}
			std::cout << "Invalid data shape, must be " << BOARD_SIZE << "x" << BOARD_SIZE << "x" << INPUT_CHANNEL_SIZE << "/"  << (COMING_CHESS_NUM+1) << std::endl;
			return Game_statu_9928();
			}))
		.def(pybind11::init([](Game_statu_994& game_statu_994) ->Game_statu_9928 {
			return Game_statu_9928(game_statu_994.to_game_map());
			}))
		.def(pybind11::init([](Game_statu_9928& game_statu_9928) ->Game_statu_9928 {
			return game_statu_9928;
			}))
		.def(pybind11::init([](Game_map& game_map) ->Game_statu_9928 {
			return Game_statu_9928(game_map);
			}));

	pybind11::class_<Game_statu_994>(m, "Game_statu_994", pybind11::buffer_protocol())
		.def_buffer([](Game_statu_994& m) -> pybind11::buffer_info {
		return pybind11::buffer_info(
			&m.value,								/* Pointer to buffer */
			sizeof(int),									/* Size of one scalar */
			pybind11::format_descriptor<int>::format(),	/* Python struct-style format descriptor */
			3,												/* Number of dimensions */
			{ BOARD_SIZE,BOARD_SIZE ,COMING_CHESS_NUM+1 },						/* Buffer dimensions */
			{ sizeof(int) * BOARD_SIZE * (COMING_CHESS_NUM+1),sizeof(int) * (COMING_CHESS_NUM+1),sizeof(int)}   /* Strides (in bytes) for each index */
	);

			})
		.def(pybind11::init([](py::array arr) ->Game_statu_994 {
			py::buffer_info info = arr.request();
		if (arr.dtype().kind() != 'i') {
			std::cout << "Invalid data type, must be int" << std::endl;
			return Game_statu_994();
		}
		if (info.ndim != 3) {
			std::cout << "Invalid data shape, must be 3D" << std::endl;
			return Game_statu_994();
		}
		if (info.shape[0] == BOARD_SIZE && info.shape[1] == BOARD_SIZE)
		{
			auto data = static_cast<int*>(info.ptr);
			switch (info.shape[2])
			{
			case COMING_CHESS_NUM + 1:
				return Game_statu_994(data);
			case INPUT_CHANNEL_SIZE:
				return Game_statu_994(Game_statu_9928(data).to_game_map());
			default:
				break;
			}
		}
		std::cout << "Invalid data shape, must be " << BOARD_SIZE << "x" << BOARD_SIZE << "x" << INPUT_CHANNEL_SIZE << "/" << (COMING_CHESS_NUM + 1) << std::endl;
		return Game_statu_994();
			}))
		.def(pybind11::init([](Game_statu_994& game_statu_994) ->Game_statu_994 {
			return game_statu_994;
			}))
		.def(pybind11::init([](Game_statu_9928& game_statu_9928) ->Game_statu_994 {
			return Game_statu_994(game_statu_9928.to_game_map());
			}))
		.def(pybind11::init([](Game_map& game_map) ->Game_statu_994 {
		return Game_statu_994(game_map);
			}));


	pybind11::class_<Point_int>(m, "Point")
		.def(pybind11::init<int, int>())
		.def_readwrite("x", &Point_int::x)
		.def_readwrite("y", &Point_int::y);

}
#endif // _EXPORT