

#ifndef MODEL_H
#define MODEL_H
#include <SDL2/SDL_log.h>
constexpr int BlockSize = 30;
inline int Mapw = 17, Maph = 12;
#include <list>
#include <map>
#include <vector>
class Center;
using namespace std;
enum Background_type {
	empty,
	circle_mine,
	triangle_mine,
	center_mine
};
enum Shape_type {
	none_type,
	circle,
	triangle,
	half_circle
};
enum Direction {
	north,
	south,
	west,
	east,
};
enum Rotation {
	r_left,r_right,r_middle
};
enum Machine_type {
	digger,
	translater,
	cutter,
	cutter_righthalf,
	bin,
	center,
	rotator
};
inline int My_money;
inline int My_num;
inline int My_level;
inline Center* My_center;

class Machine;
inline vector<std::vector<Background_type>> background;
inline vector<std::vector<Machine *>> machine_map;

class Shape {
public:
	int val;
	bool can_half;
	Shape_type type;
	double offset;
	int angle;
	static int increasing_value;
	Shape(Shape_type t): type(t), offset(0),angle(0) {
		switch (type) {
			case circle:
				can_half = true;
				val = 3;
			break;
			case triangle:
				can_half = false;
				val = 4;
			break;
			case half_circle:
				can_half = false;
				val = 2;
			break;
			default:
				can_half=false;
				val = 1;
			break;
		}
	}
};

class Timer {
public:
	int curr_time;
	int interval;
	Timer(): curr_time(0), interval(1) {}
	Timer(const int i): curr_time(0), interval(i) {}
	bool update() {
		curr_time++;
		if (curr_time >= interval) {
			curr_time = 0;
			return true;
		}
		return false;
	}
};

class Machine {
public:
	Machine(Machine_type t, Direction d, int x, int y):
	type(t), direction(d), pos_x(x), pos_y(y),
	out_direction(d), is_full(false),
	rotation(Rotation::r_middle), x2(x), y2(y) ,x1(x), y1(y){}
	~Machine();
	Machine_type type;
	virtual void work() = 0;
	std::list<Shape *> shape_list;
	Timer timer;
	bool is_full;

	int pos_x, pos_y;
	int x2, y2;
	int x1, y1;

	Direction direction;
	Direction out_direction;
	Direction get_outdirection(Direction d, Rotation r);
	Rotation rotation;

	Machine* get_neighbor(int x, int y, Direction out_direction);
	bool give_shape(Direction out_dir,Machine* nei, Shape* s);

};


class Digger : public Machine {

public:
	static int speed;
	Shape_type dig_type;

	Digger(Direction d, int x, int y): Machine(digger, d, x, y) {
		switch (background[x][y]) {
			case circle_mine:
				dig_type = circle; break;
			case triangle_mine:
				dig_type = triangle; break;
			default:
				dig_type = none_type; break;
		}
		timer.interval = 50 - 25 * (speed-1);
	}
	void work() {
		if (timer.update()) {
			if (dig_type != none_type) {
				Shape* s = new Shape(dig_type);
				Machine* nei = get_neighbor(pos_x, pos_y, direction);
				if(!nei) {
					delete s;
					return;
				}
				if(!give_shape(direction, nei, s)) delete s;
			}
		}
	}
};

class Translater : public Machine {
public:

	static int speed;
	Translater(Direction d, Rotation r, int x, int y): Machine(translater, d, x, y)
	{
		timer.interval = 20 - 7 * (speed-1);
		rotation = r;
		out_direction = get_outdirection(d, r);
	}

	void work() {
		bool has_given = false;
		for(auto it = shape_list.rbegin(); it != shape_list.rend(); ++it) {
			Shape* s = (*it);
			s->offset += 0.1;
			if(s->offset >= 1) {
				Machine* nei = get_neighbor(pos_x, pos_y, out_direction);
				if(give_shape(out_direction, nei, s)) has_given = true;
				else s->offset = 1;
			}
			bool can_move = true;
			for(auto other: shape_list) {
				if(other->offset > s->offset && other->offset - s->offset < 0.7) {
					can_move = false;
				}
			}
			if(!can_move) s->offset -= 0.1;
		}
		if(has_given) shape_list.pop_back();
		if(shape_list.empty()) is_full = false;
		else if(shape_list.front()->offset <= 0.3 || shape_list.size() >= 2) is_full = true;
		else is_full = false;
	}
};


class Cutter : public Machine {
public:
	static int speed;

	Cutter(Direction d, int x, int y): Machine(cutter, d, x, y) {
		timer.interval = 30 - 15 * (speed-1);
		switch (d) {
			case north: x2 = x+1; break;
			case south: x2 = x-1; break;
			case east: y2 = y+1; break;
			case west: y2 = y-1; break;
		}
	}
	void work() {
		if (timer.update() && !shape_list.empty()){
			Machine* nei1 = get_neighbor(pos_x, pos_y, direction);
			Machine* nei2 = get_neighbor(x2, y2, direction);
			if(nei1 && nei2) {
				Shape* s1 = new Shape(half_circle);
				Shape* s2 = new Shape(half_circle);
				switch (direction) {
					case north: s1->angle = 0; break;
					case east: s1->angle = 90; break;
					case south: s1->angle = 180; break;
					case west: s1->angle = 270; break;
				}
				s2->angle = (s1->angle + 180) %360;
				bool ok1 = give_shape(direction, nei1, s1);
				bool ok2 = give_shape(direction, nei2, s2);
				if(!ok1 && !ok2) {
					delete s1; delete s2;
					is_full = true;
				}
				else if(!ok1) {
					nei2->shape_list.pop_front();
					delete s2;
					is_full = true;
				}
				else if (!ok2) {
					nei1->shape_list.pop_front();
					delete s1;
					is_full = true;
				}
				else {
					is_full = false;
					shape_list.pop_back();
				}
			}
			else is_full = true;
		}
	}
};

class Cutter_righthalf : public Machine {
public:
	Cutter_righthalf(Direction d, int x, int y): Machine(cutter_righthalf, d, x, y) {
		switch (d) {
			case north: x1 = x-1; break;
			case south: x1 = x+1; break;
			case east: y1 = y-1; break;
			case west: y1 = y+1; break;
		}
	}
	void work() {
		;
	}
};


class Bin : public Machine {
public:
	Bin(Direction d, int x, int y): Machine(bin, d, x, y) {}
	void work() {
		while (!shape_list.empty()) {
			Shape* s = shape_list.back(); shape_list.pop_back();delete s;
		}
	}
};


class Center : public Machine {
public:

	Center(Direction d, int x, int y): Machine(center, d, x, y) {
	}

	void work() {
		while (!shape_list.empty()) {
			Shape* s = shape_list.back(); shape_list.pop_back();
			if(s->type == Get_needtype(My_level)){
				if(My_level == 4 && s->angle != 180) continue;
				My_num ++;
				My_money += s->val + Shape::increasing_value;
			}
			delete s;
		}
	}
	Shape_type Get_needtype(int l) {
		switch (l) {
			case 1: return circle;
			case 2: return triangle;
			case 3: return half_circle;
			case 4: return half_circle;
			default:return circle;
		}
	}
	int Get_neednum(int l) {
		switch (l) {
			case 1: return 11;
			case 2: return 12;
			case 3: return 13;
			case 4: return 14;
			default: return 10;
		}
	}
};


class Rotator: public Machine {
public:
	Rotator(Direction d, int x, int y):Machine(rotator, d, x, y) {
		timer.interval = 15;
	};
	void work() {
		if(timer.update() && !shape_list.empty()) {

			Shape* s = shape_list.back();
			s->angle += 90;
			if(give_shape(direction, get_neighbor(pos_x,pos_y,direction), s)) {
				shape_list.pop_back();
				is_full = false;
			}
			else is_full = true;
		}
	}
};

#endif //MODEL_H
