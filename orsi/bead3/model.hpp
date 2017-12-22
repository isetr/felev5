#include <iostream>
#include <vector>
#include <fstream>

std::ofstream debug("debug.txt");

// COLOR
struct Color {
    Color() : R(0), G(0), B(0) {}
    Color(int r, int g, int b) : R(r), G(g), B(b) {}

    int R, G, B;
};

std::istream& operator>>(std::istream& is, Color& color) {
    is >> color.R >> color.G >> color.B;
    debug << "color-read: " << color.R << " " << color.G << " " << color.B << "\n";
    return is;
}

std::ostream& operator<<(std::ostream& os, const Color color) {
    os << "(" << color.R << "," << color.G << "," << color.B << ")";
    return os;
}

bool operator==(const Color& l, const Color& r) {
    return l.R == r.R && l.G == r.G && l.B == r.B;
}

static Color BLACK      = Color(0,0,0);
static Color RED        = Color(255,0,0);
static Color GREEN      = Color(0,255,0);
static Color BLUE       = Color(0,0,255);
static Color CYAN       = Color(0,255,255);
static Color MAGENTA    = Color(255,0,255);
static Color YELLOW     = Color(255,255,0);
static Color WHITE      = Color(255,255,255);

// IMAGE
struct PackedImage {
    int size;
    int data[32][32 * 3];
    int rows[32][32];
    int cols[32][32];
};

class Image {
public:
    Image() : size(0) {}
    Image(int n) {
        size = n;
        data.resize(size);
        rows.resize(size);
        cols.resize(size);
        for(int i = 0; i < size; ++i) {
            data.at(i).resize(size);
            rows.at(i).resize(size);
            cols.at(i).resize(size);
            for(int j = 0; j < size; ++j) {
                data.at(i).at(j) = BLACK;
                rows.at(i).at(j) = 0;
                cols.at(i).at(j) = 0;
            }
        }

    }
    Image(PackedImage packed) {
        size = packed.size;
        data.resize(size);
        rows.resize(size);
        cols.resize(size);
        for(int i = 0; i < size; ++i) {
            data.at(i).resize(size);
            rows.at(i).resize(size);
            cols.at(i).resize(size);
            for(int j = 0; j < size; ++j) {
                data.at(i).at(j) = Color(packed.data[i][j*3], packed.data[i][j*3 + 1], packed.data[i][j*3 + 2]);
                rows.at(i).at(j) = packed.rows[i][j];
                cols.at(i).at(j) = packed.cols[i][j];
            }
        }
    } 
    ~Image(){}

    Color operator()(int i, int j) const {
        return data.at(i).at(j);
    }

    Color& operator()(int i, int j) {
        return data.at(i).at(j);
    }
    int row (int i, int j) const {
        return rows.at(i).at(j);
    }
    int& row (int i, int j) {
        return rows.at(i).at(j);
    }
    int col (int i, int j) const {
        return cols.at(i).at(j);
    }
    int& col (int i, int j) {
        return cols.at(i).at(j);
    }

    int getSize() { return size; } const

    PackedImage pack() {
        PackedImage out;
        // data
        out.size = size;
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; j += 3) {
                out.data[i][j] = data.at(i).at(j).R;
                out.data[i][j + 1] = data.at(i).at(j).G;
                out.data[i][j + 2] = data.at(i).at(j).B;
            }
        }
        // rows, cols
        if(rows.size() > 0 && cols.size() > 0) {
            for(int i = 0; i < size; ++i) {
                for(int j = 0; j < size; ++j) {
                    out.rows[i][j] = rows.at(i).at(j);
                    out.cols[i][j] = rows.at(i).at(j);
                }
            }
        }

        return out;
    }

private:
    int size;
    std::vector<std::vector<Color>> data;
    std::vector<std::vector<int>> rows;
    std::vector<std::vector<int>> cols;
};

std::istream& operator>>(std::istream& is, Image& image) {
    int size;
    is >> size;
    debug << "image-read: size " << size << "\n";
    image = Image(size);
    debug << "image-read: image resized\n";
    for(int i; i < size; ++i) {
        for(int j; j < size; ++j) {
            debug << "image-read: (" << i << "," << j << ")\n";
            is >> image(i, j);
        }
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, Image image) {
    int size = image.getSize();
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++i) {
            os << image(i, j) << " ";
        }
        os << std::endl;
    }
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++i) {
            if(image.row(i, j) != 0) os << image.row(i, j) << " ";
        }
        os << std::endl;
    }
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++i) {
            if(image.col(i, j) != 0) os << image.col(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}