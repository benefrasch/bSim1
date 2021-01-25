#include <iostream>
#include <tuple>
#include "BMP.h"
#include "Wire_parts.h"
#include "B_Vektor.h"
#include "Constants.h"
#include <Windows.h>
#include <future>

static BMP bmp(image_size_x, image_size_y, false);
static int done_graphs = 0;


bool plot_vector(std::vector<wire_part>* parts, long double y_start, bool inverted) {
	long double x = simulation_size_x / 2,
		y = y_start;

	while (x >= simulation_size_x / 2 && x <= simulation_size_x) {
		Vektor b_vektor = B_Vektor::vektor(parts, x, y, simulation_size_z / 2);
		if (inverted)
			b_vektor.x *= -1, b_vektor.y *= -1, b_vektor.z *= -1;

		x += b_vektor.x, y += b_vektor.y;

		if (!(x<0 || x>simulation_size_x || y<0 || y>simulation_size_y))
			bmp.set_pixel(x * (image_size_x / simulation_size_x), y * (image_size_y / simulation_size_y), 255, 255, 255, 255);
	}
	bmp.write("output.bmp");
	++done_graphs;
	return true;
}

int main()
{

	bmp.set_pixel((simulation_size_x / 2) * (image_size_x / simulation_size_x), (float)(simulation_size_y / 2 - radius) * (image_size_y / simulation_size_y), 255, 0, 0, 255);
	bmp.set_pixel((simulation_size_x / 2) * (image_size_x / simulation_size_x), (float)(simulation_size_y / 2 + radius) * (image_size_y / simulation_size_y), 255, 0, 0, 255);
	std::vector<wire_part> parts = Wire_parts::ring();

	std::vector<double> start_y_values;
	for (long double y_start = simulation_size_y / 2 - radius + 0.01; y_start <= (double)simulation_size_y / 2 + radius - 0.01; y_start += step_size) {
		Vektor b_vektor = B_Vektor::vektor(&parts, simulation_size_x / 2, y_start, simulation_size_z / 2);
		std::cout << y_start << "   " << b_vektor.x << "\n";
		for (long double y_value = y_start; y_value < y_start + step_size; y_value += (long double)1 / (-1000000 * b_vektor.x)) {
			start_y_values.push_back(y_value);
			std::cout << y_value << "\n";
		}
	}

	std::cout << "\n starting calculation...\n\n";

	std::vector<std::future<bool>> graphs;
	for (double y_start : start_y_values)
	{
		std::cout << y_start << "\n";
		graphs.push_back(std::async(plot_vector, &parts, y_start, false)); //plot graph to both left and right side
		graphs.push_back(std::async(plot_vector, &parts, y_start, true));
	}

	while (done_graphs < start_y_values.size() * 2) {
		std::cout << "Waiting for calculations to finish - " << done_graphs << " out of " << (start_y_values.size() * 2) << " done!\n";
		Sleep(10000);
	}
}

