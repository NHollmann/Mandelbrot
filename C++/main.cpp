#include <iostream>
#include <fstream>
#include <complex>

#define MAX_ITER 100
#define BAR_WIDTH 60

using namespace std;

// This function calculates if a point is in the
// mandelbrot set. It returns the iteration count
// needed to leave the circle.
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

// Interpolate from a to b lineary.
int lerp(double t, int a, int b)
{
	return t * b + (1.0 - t) * a;
}

int main()
{
	const int WIDTH = 4000;
	const int HEIGHT = 3000;
	ofstream out("mandelbrot.ppm");
	
	// Output the PPM file header.
	out << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

	// Display a loading bar.
	cout << "[" << std::string(BAR_WIDTH, '.') << "]\r[" << flush;

	// Iterate over all pixels to calculate if 
	// they are in the mandelbrot set or not
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// Calculare the real and imaginary of the position in a coordinate
			// system from -2 to 1 on the x axis and -1 to 1 on the
			// y axis.
			double xr = x / (double) WIDTH * 3.0 - 2.0;
			double yr = y / (double) HEIGHT * 2.0 - 1.0;
			
			// Calculate the iteration steps needed to leave the circle.
			// If iter == MAX_ITER it never leaved the circle.
			int iter = mandelbrot(xr, yr);
			
			unsigned char r, g, b;
			if (iter == MAX_ITER) {
				// Set the pixel to bacl if we think it is in the mandelbrot set.
				r = 0; g = 0; b = 0;
			} else {
				// Calculate the pixel color by linear interpolation based on
				// the iteration count.
				double iter_rel = iter / (double)(MAX_ITER - 1);
				r = lerp(iter_rel, 194, 140);
				g = lerp(iter_rel, 252, 10);
				b = lerp(iter_rel, 199, 196);
			}

			// Output a single pixel
			out << r << g << b;
		}

		// Update the loading bar 
		if (y % (HEIGHT / BAR_WIDTH) == 0)
		{
			cout << "#" << flush;
		}
	}

	// End the loading bar.
	cout << "] done.\n" << flush;

	out.close();
	return 0;
}

