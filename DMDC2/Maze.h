
#pragma once

class Maze {
public:
	bool shape[33][33];
	int cx, cy;

	Maze(int cx, int cy): cx(cx), cy(cy) {
	}
	void Set(int x, int y, bool kabe) {
		shape[y][x] = kabe;
	}
	bool Get(int x, int y) {
		return shape[y][x];
	}
	bool GetKabe(int x, int y, int dx, int dy) {
		return Get(1+2*x+dx, 1+2*y+dy);
	}
	void SetKabe(int x, int y, int dx, int dy, bool kabe) {
		Set(1+2*x+dx, 1+2*y+dy, kabe);
	}
	void KabeTaoshi() {
		memset(shape, 0, sizeof(shape));
		for (int y=0; y<cy; y++) {
			for (int x=0; x<cx; x++) {
				Set(1+2*x, 1+2*y, true);
			}
		}
		for (int y = 0; y < cy; y++) {
			for (int x = 0; x < cx; x++) {
				if (y == 0) {
					while (true) {
						int r = rand.Next(4);
						if (false) { }
						else if (r == 0 && GetKabe(x, y, 0, -1) == false) SetKabe(x, y, 0, -1, true);
						else if (r == 1 && GetKabe(x, y, 0, +1) == false) SetKabe(x, y, 0, +1, true);
						else if (r == 2 && GetKabe(x, y, -1, 0) == false) SetKabe(x, y, -1, 0, true);
						else if (r == 3 && GetKabe(x, y, +1, 0) == false) SetKabe(x, y, +1, 0, true);
						else continue;
						break;
					}
				}
				else {
					while (true) {
						int r = rand.Next(3);
						if (false) { }
						else if (r == 0 && GetKabe(x, y, 0, +1) == false) SetKabe(x, y, 0, +1, true);
						else if (r == 1 && GetKabe(x, y, -1, 0) == false) SetKabe(x, y, -1, 0, true);
						else if (r == 2 && GetKabe(x, y, +1, 0) == false) SetKabe(x, y, +1, 0, true);
						else continue;
						break;
					}
				}
			}
		}
	}
	struct Location {
		int x, y;
		Location() { }
		Location(int x, int y): x(x), y(y) { }
	};
	class Random {
	public:
		int Next(int maxv) {
			return ::rand() % maxv;
		}
	};
	Random rand;
	std::stack<Location> backward;
    std::stack<Location> forward;
    bool Walk(int dx, int dy) {
		Location recent = backward.top();
        int x = recent.x + dx;
        int y = recent.y + dy;
        if (0 <= x && 0 <= y && x < cx && y < cy && GetKabe(x, y, 0, 0) == true) {
            SetKabe(x, y, 0, 0, false);
            SetKabe(x, y, -dx, -dy, false);
			backward.push(Location(x, y));
            return true;
        }
        return false;
    }
    bool WalkBack() {
		forward.push(backward.top()), backward.pop();
        if (backward.size() != 0)
            return true;
        return false;
    }
    int retire;
    void Anahori(int retire) {
		memset(shape, 1, sizeof(shape));
		this->retire = retire;
		int tx = rand.Next(cx);
        int ty = rand.Next(cy);
        backward.push(Location(tx, ty));
        SetKabe(tx, ty, 0, 0, false);
        while (true) {
			int baseDir = rand.Next(8);
            int t = 0;
            int step = 0;
            for (; t < 5; t++) {
                int dx = 0, dy = 0;
                switch ((baseDir + t) % 8) {
                    case 0: dx = +1; break; // East
                    case 1: dy = -1; break; // North
                    case 2: dx = +1; break; // East
                    case 3: dy = -1; break; // North
                    case 4: dx = -1; break; // West
                    case 5: dy = +1; break; // South
                    case 6: dx = -1; break; // West
                    case 7: dy = +1; break; // South
                }
                while (true) {
                    if (Walk(dx, dy)) {
                        step++;
                        if (rand.Next(100) < retire) {
                            continue;
                        }
                    }
                    break;
                }
            }
            if (step == 0) {
                if (!WalkBack())
                    break;
            }
        }
    }
};
