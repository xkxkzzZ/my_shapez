
#include "Model.h"
int Digger::speed = 1;
int Cutter::speed = 1;
int Translater::speed = 1;
int Shape::increasing_value = 0;

Machine::~Machine() {
    for (auto&s: shape_list)
        delete s;
}

Direction Machine::get_outdirection(Direction d, Rotation r) {
    if(type != translater) return d;
    if (d == north && r == Rotation::r_left) return west;
    if (d == north && r == Rotation::r_middle) return north;
    if (d == north && r == Rotation::r_right) return east;
    if (d == south && r == Rotation::r_left) return east;
    if (d == south && r == Rotation::r_middle) return south;
    if (d == south && r == Rotation::r_right) return west;
    if (d == east && r == Rotation::r_left) return north;
    if (d == east && r == Rotation::r_middle) return east;
    if (d == east && r == Rotation::r_right) return south;
    if (d == west && r == Rotation::r_left) return south;
    if (d == west && r == Rotation::r_middle) return west;
    if (d == west && r == Rotation::r_right) return north;
}



Machine* Machine::get_neighbor(int x, int y, Direction out_direction) {
    switch (out_direction) {
    case north: y--;
        break;
    case south: y++;
        break;
    case east: x++;
        break;
    case west: x--;
        break;
    }
    if (x < 0 || y < 0 || x >= Mapw || y >= Maph) return nullptr;
    return machine_map[x][y];
}

bool Machine::give_shape(Direction out_dir, Machine* nei, Shape* s) {
    if (nei == nullptr) {
        return false;
    }
    switch (nei->type) {
        case translater:
            if (out_dir == nei->direction && !nei->is_full) {
                nei->shape_list.push_front(s);
                s->offset = 0;
                return true;
            }
            break;
        case cutter:
            if (out_dir == nei->direction && s->can_half && !nei->is_full) {
                nei->shape_list.push_front(s);
                s->offset = 0;
                return true;
            }
            break;
        case bin:
            nei->shape_list.push_front(s);
            return true;
            break;
        case center:
            nei->shape_list.push_front(s);
            return true;
            break;
        case rotator: {
            if (out_dir == nei->direction && !nei->is_full) {
                nei->shape_list.push_front(s);
                s->offset = 0;
                return true;
            }
            break;
        }
    default:
        break;
    }
    return false;
}
