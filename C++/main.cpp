#include <iostream>
#include <fstream>
#include <complex>

#define MAX_ITER 100
#define BAR_WIDTH 60

using namespace std;

int mandelbrot(double x, double y)
{
	complex<double> c(x, y);
	complex<double> z(0, 0);
	int iter = 0;

	while (iter < MAX_ITER && abs(z) <= 2.0)
	{
		z = z * z + c;
		iter++;
	}

	return iter;
}

int lerp(double t, int a, int b)
{
	return t * b + (1.0 - t) * a;
}

int main()
{
	const int WIDTH = 4000;
	const int HEIGHT = 3000;
	ofstream out("mandelbrot.ppm");
		
	out << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

	cout << "[" << std::string(BAR_WIDTH, '.') << "]\r[" << flush;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			double xr = x / (double) WIDTH * 3.0 - 2.0;
			double yr = y / (double) HEIGHT * 2.0 - 1.0;
			int iter = mandelbrot(xr, yr);
			
			unsigned char r, g, b;
			if (iter == MAX_ITER) {
				r = 0; g = 0; b = 0;
			} else {
				double iter_rel = iter / (double) MAX_ITER;
				r = lerp(iter_rel, 194, 140);
				g = lerp(iter_rel, 252, 10);
				b = lerp(iter_rel, 199, 196);
			}

			out << r << g << b;
		}
		if (y % (HEIGHT / BAR_WIDTH) == 0)
		{
			cout << "#" << flush;
		}
	}

	cout << "] done.\n" << flush;

	out.close();
	return 0;
}

