package main

import (
	"fmt"
	"math/cmplx"
	"os"
	"sync"
)

const (
	MAX_ITER = 100
)

// This function calculates if a point is in the
// mandelbrot set. It returns the iteration count
// needed to leave the circle.
func mandelbrot(x float64, y float64) int {
	c := complex(x, y)
	z := complex(0, 0)
	iter := 0

	for iter < MAX_ITER && cmplx.Abs(z) <= 2.0 {
		z = z*z + c
		iter++
	}

	return iter
}

// Interpolate from a to b lineary.
func lerp(t float64, a, b int) int {
	fa := float64(a)
	fb := float64(b)
	res := t*fb + (1.0-t)*fa
	return int(res)
}

func main() {
	const WIDTH = 4000
	const HEIGHT = 3000

	// Display a loading bar.
	fmt.Printf("Generating image...")

	// Create image buffer
	resultImg := make([]byte, WIDTH*HEIGHT*3)

	// Iterate over all pixels to calculate if
	// they are in the mandelbrot set or not
	var wg sync.WaitGroup
	wg.Add(WIDTH * HEIGHT)

	for y := 0; y < HEIGHT; y++ {
		for x := 0; x < WIDTH; x++ {
			go func(xpos, ypos int) {
				// Increment wait group counter when the pixel is finished
				defer wg.Done()

				// Calculare the real and imaginary of the position in a coordinate
				// system from -2 to 1 on the x axis and -1 to 1 on the
				// y axis.
				xr := float64(xpos)/float64(WIDTH)*3.0 - 2.0
				yr := float64(ypos)/float64(HEIGHT)*2.0 - 1.0

				// Calculate the iteration steps needed to leave the circle.
				// If iter == MAX_ITER it never leaved the circle.
				iter := mandelbrot(xr, yr)

				var r, g, b uint8
				if iter == MAX_ITER {
					// Set the pixel to black if we think it is in the mandelbrot set.
					r = 0
					g = 0
					b = 0
				} else {
					// Calculate the pixel color by linear interpolation based on
					// the iteration count.
					iter_rel := float64(iter) / float64(MAX_ITER-1)
					r = uint8(lerp(iter_rel, 194, 140))
					g = uint8(lerp(iter_rel, 252, 10))
					b = uint8(lerp(iter_rel, 199, 196))
				}

				// Save pixel in buffer
				idx := (ypos*WIDTH + xpos) * 3
				resultImg[idx+0] = r
				resultImg[idx+1] = g
				resultImg[idx+2] = b
			}(x, y)
		}
	}

	// Wait for all goroutines to finish
	wg.Wait()

	// Open file
	outfile, _ := os.Create("mandelbrot.ppm")
	defer outfile.Close()

	// Output the PPM file header.
	fmt.Fprintf(outfile, "P6\n%d %d\n255\n", WIDTH, HEIGHT)

	// Output a single pixel
	outfile.Write(resultImg)

	// End the loading bar.
	fmt.Printf(" done.\n")
}
