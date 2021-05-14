#include "network.h"

// constructor
Network::Network(int ils, int hln, int hls, int ols) {

	pils = ils;
	phln = hln;
	phls = hls;
	pols = ols;

	reset_nodes();
	reset_edges();

}


//"truly" random number between 0 and 1
double Network::random() {
	std::random_device rd{};
	std::mt19937 engine{rd()};
	std::uniform_real_distribution<double> dist{0.0, 1.0};
	double ret = dist(engine);
	return ret;
}

// sigmoid function approximation
double Network::sigmoid(double in) {
	//return in / (1 + abs(in));
	return tanh(in);

}

// set input nodes
void Network::set_input_nodes(double nodes[]) {
	reset_nodes();
	for (int x = 0; x < pils; x++) {
		input_nodes[x] = nodes[x];
	}
}

// sets all nodes values to 0
void Network::reset_nodes() {

	input_nodes.clear();
	hidden_nodes.clear();
	output_nodes.clear();

	for (int x = 0; x < pils; x++) {
		input_nodes.push_back(0);
	}
	for (int x = 0; x < phln; x++) {

		hidden_nodes.push_back({});
		for (int y = 0; y < phls; y++) {
			hidden_nodes[x].push_back(0);
		}
	}

	for (int x = 0; x < pols; x++) {
		output_nodes.push_back(0);;
	}

}

/// sets all edges to 0
void Network::reset_edges() {

	input_edges.clear();
	hidden_edges.clear();
	output_edges.clear();

	// set input edges
	for (int x = 0; x < pils; x++) {
		input_edges.push_back({});
		for (int y = 0; y < phls; y++) {
			input_edges[x].push_back(0);
		}

	}

	// set hidden edges
	for (int x = 0; x < phln - 1; x++) {
		hidden_edges.push_back({});
		for (int y = 0; y < phls; y++) {
			hidden_edges[x].push_back({});
			for (int z = 0; z < phls; z++) {
				hidden_edges[x][y].push_back(0);
			}
		}
	}

	// set output edges
	for (int x = 0; x < phls; x++) {
		output_edges.push_back({});
		for (int y = 0; y < pols; y++) {
			output_edges[x].push_back(0);
		}
	}


}


void Network::randomize_edges(float val1, float val2, float val3) {

	input_edges.clear();
	hidden_edges.clear();
	output_edges.clear();

	// set input edges
	for (int x = 0; x < pils; x++) {
		input_edges.push_back({});
		for (int y = 0; y < phls; y++) {
			input_edges[x].push_back(2*val1*random() - val1);
		}

	}

	// set hidden edges
	for (int x = 0; x < phln - 1; x++) {
		hidden_edges.push_back({});
		for (int y = 0; y < phls; y++) {
			hidden_edges[x].push_back({});
			for (int z = 0; z < phls; z++) {
				hidden_edges[x][y].push_back(2*val2*random() - val2);
			}
		}
	}

	// set output edges
	for (int x = 0; x < phls; x++) {
		output_edges.push_back({});
		for (int y = 0; y < pols; y++) {
			output_edges[x].push_back(2*val3*random() - val3);
		}
	}


}


// randomly change an edge (used in genetic algorithms)
void Network::mutate(int amount) {

	double mutate_to = amount*random(); // what value to mutate to

	// this part approximates what are the odds of mutation happening on any layer (input, hidden or output)
	double hidden_edges_num = 0;
	if (hidden_edges.size() > 0) {
		hidden_edges_num = hidden_edges.size() * hidden_edges[0].size() * hidden_edges[0][0].size();
	}
	double input_edges_num = input_edges.size() * input_edges[0].size();
	double output_edges_num = output_edges.size() * output_edges[0].size();

	// number of all edges
	double all_edges_num = hidden_edges_num + input_edges_num + output_edges_num;

	double rn = random();

	// odds of mutation on any edge should be equal
	if (rn < input_edges_num / all_edges_num) {

		// choose random edge
		int chosen_edge_1 = (int) (((double)input_edges.size()) * random());
		int chosen_edge_2 = (int) (((double)input_edges[0].size()) * random());
		input_edges[chosen_edge_1][chosen_edge_2] = mutate_to;

	} else if (rn < (input_edges_num + hidden_edges_num) / all_edges_num) {
		// choose random edge
		int chosen_edge_1 = (int)((double)(hidden_edges.size())) * random();
		int chosen_edge_2 = (int)((double)hidden_edges[0].size()) * random();
		int chosen_edge_3 = (int)((double)hidden_edges[0][0].size()) * random();
		hidden_edges[chosen_edge_1][chosen_edge_2][chosen_edge_3] = mutate_to;

	} else {

		// choose random edge
		int chosen_edge_1 = (int) (((double)output_edges.size()) * random());
		int chosen_edge_2 = (int) (((double)output_edges[0].size()) * random());
		output_edges[chosen_edge_1][chosen_edge_2] = mutate_to;
	}

}

// get loss after propagation, provided the expected output
double Network::loss(double expected_output[]) {
	double c = 0;

	for (int x = 0; x < output_nodes.size(); x++) {
		c += (output_nodes[x] - expected_output[x]) * (output_nodes[x] - expected_output[x]);
	}

	return c;

}

// propagate forward
void Network::forward_propagate() {

	// input nodes to hidden (first layer)
	for (int x = 0;	x < input_nodes.size(); x++) {

		for (int y = 0; y < hidden_nodes[0].size(); y++) {

			double insert = input_nodes[x] * input_edges[x][y];
			hidden_nodes[0][y] += insert;

		}

	}

	// forward propagate through hidden layers
	for (int x = 0; x < hidden_nodes.size() - 1; x++) {// for each layer

		for (int y = 0; y < hidden_nodes[x].size(); y++) {// for each node in layer

			hidden_nodes[x][y] = sigmoid(hidden_nodes[x][y]);

			for (int z = 0; z < hidden_nodes[x + 1].size(); z++) {// for each node in next layer

				double insert = hidden_nodes[x][y] * hidden_edges[x][y][z];
				hidden_nodes[x + 1][z] += insert;
			}

		}
	}


	// hidden nodes (last layer) to output
	for (int x = 0;	x < hidden_nodes[hidden_nodes.size() - 1].size(); x++) {

		hidden_nodes[hidden_nodes.size() - 1][x] = sigmoid(hidden_nodes[hidden_nodes.size() - 1][x]);

		for (int y = 0; y < output_nodes.size(); y++) {

			double insert = hidden_nodes[hidden_nodes.size() - 1][x] * output_edges[x][y];

			output_nodes[y] += insert;

		}
	}

	// run output layer through sigmoid
	for (int x = 0; x < output_nodes.size(); x++) {
		output_nodes[x] = sigmoid(output_nodes[x]);
	}

}

// output values for all nodes (mainly for debuging)
void Network::output_all_nodes() {

	std::cout << "Input layer: " << "\n";

	for (int x = 0; x < input_nodes.size(); x++) {
		std::cout << input_nodes[x] << "	";
	}

	std::cout << "\n" << "Hidden layers: " << "\n";

	for (int x = 0; x < hidden_nodes.size(); x++) {

		for (int y = 0; y < hidden_nodes[x].size(); y++) {
			std::cout << hidden_nodes[x][y] << "	";
		}

		std::cout << "\n";
	}

	std::cout << "Output layer: " << "\n";

	for (int x = 0; x < output_nodes.size(); x++) {
		std::cout << output_nodes[x] << "	";
	}

	std::cout << "\n";

}

// save network to file (only saves edges)
void Network::save(std::string file_path) {
	std::ofstream file;
	file.open(file_path);

	// save input edges
	for (int x = 0; x < pils; x++) {
		file << "[ ";
		for (int y = 0; y < phls; y++) {
			file << input_edges[x][y];
			file << " ";
		}
		file << "] ";

	}

	file << "\nnewline\n";
	// save hidden edges
	for (int x = 0; x < phln - 1; x++) {
		for (int y = 0; y < phls; y++) {
			file << "[ ";
			for (int z = 0; z < phls; z++) {
				file << hidden_edges[x][y][z];
				file << " ";
			}
			file << "] ";
		}

		if (x != phln - 2) {
			file << "\nnewlayer\n";
		}
	}

	file << "\nnewline\n";

	// save output edges
	for (int x = 0; x < phls; x++) {
		file << "[ ";
		for (int y = 0; y < pols; y++) {
			file << output_edges[x][y];
			file << " ";
		}
		file << "] ";
	}

	file << "\nend";

	file.close();
}

// load network from file (only loads edges)
void Network::load(std::string file_path) {

	std::ifstream file;
	file.open(file_path);
	std::string word;

	std::vector<std::string> file_vector;

	if (file.is_open()) {
		while (!file.eof()) {
			file >> word;
			file_vector.push_back(word);
		}
	}



	input_edges.clear();
	hidden_edges.clear();
	output_edges.clear();

	int x = -1;
	int y = -1;
	// set input edges
	while (true) {
		word = file_vector[0];
		if (word != "newline") {
			if (word == "[") {
				y = y + 1;
				x = 0;
				input_edges.push_back({});
			} else if (word != "]") {
				x = x + 1;
				input_edges[y].push_back(std::stod(word));
			}
			file_vector.erase(file_vector.begin());
		} else {
			file_vector.erase(file_vector.begin());
			break;
		}
	}


	x = -1;
	y = -1;
	int z = 0;
	hidden_edges.push_back({});
	// set hidden nodes
	while (true) {

		word = file_vector[0];

		if (word != "newline") {

			if (word == "newlayer") {
				y = -1;
				x = 0;
				z = z + 1;
				hidden_edges.push_back({});
			} else {
				if (word == "[") {
					y = y + 1;
					x = 0;
					hidden_edges[z].push_back({});

				} else if (word != "]") {

					x = x + 1;
					hidden_edges[z][y].push_back(std::stod(word));

				}
			}

			file_vector.erase(file_vector.begin());
		} else {
			file_vector.erase(file_vector.begin());
			break;
		}
	}




	x = -1;
	y = -1;
	// set output edges
	while (true) {
		word = file_vector[0];
		if (word != "end") {
			if (word == "[") {
				y = y + 1;
				x = 0;
				output_edges.push_back({});
			} else if (word != "]") {
				x = x + 1;
				output_edges[y].push_back(std::stod(word));
			}
			file_vector.erase(file_vector.begin());
		} else {
			file_vector.erase(file_vector.begin());
			break;
		}
	}


}
