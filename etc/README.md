Stream 500 - etc/ README
Written: 2/12/18

etc/ folder of Stream 500 contains the configuration file for the generator and responder.
# Description of parameters.
----------------------------------------------------------------------------------------------
  ### General graph description
	log_num_nodes - Scale factor of graph. Graph will contain 2^log_num_nodes vertices
	edge_factor - The ratio of the graph’s edge count to its vertex count 
		(i.e., half the average degree of a vertex in the graph)

  ### Parameters for generator object selection
	gen_type - Type of generator to use
		0 - Kronecker
		1 - Erdos-Renyi
		2 - Power Law
		3 - From File (Not currently implemented)
	obf_type - Type of obfuscation to use
		(Should correspond with responder parse type)
		0 - None
		1 - Twitter (Not currently implemented)
	send_type - Type of communication for sending edges/queries from generator to responder
		(Should correspond with responder receive type)
		0 - UDP socket

  ### Parameters for responder object selection
	parse_type - Type of parser to use 
		(Should correspond with generator obfuscator type)
		0 - None
		1 - Twitter (Not currently implemented)
	receive_type - Type of network communication from generator to responder
		(Should correspond with generator sender type)
		0 - UDP socket

  ### Parameters for network communication
	to_resp_port - Port # for communication from generator to responder
	to_gen_port - Port # for communication from responder to generator
	ip_address - IP address of responder

  ### General parameters for generation behavior
	gen_rate - Max rate of edge generation (n/sec). Actual rate may be less depending on processor
	output_interval - Number of seconds between printing of running time output from generator
	per_packet - Number of edges/queries in each packet sent from generator to responder
	result_buf_size - Number of query results to store in circular processing buffer (Query results are dropped if generator adds a query to a full buffer. Buffer size of 8000 was found to be more than sufficient for log_num_nodes up to 31.)
	mask_width - Used for setting "subgraph" bit width
	mask_offset - Used for setting "subgraph" bit offset
	max_msg_len - When obfuscating, max number of characters to be sent in a message
	num_gen - Number of generators running simultaneously (Currently only tested with single generator)

  ### Parameters for general random number generators
	rand_seed - Seed for generating values for determining pattern of generating edges/queries (Also used currently for path length generation)
	min_length - Minimum length of path between guaranteed connected nodes
	max_length - Maximum length of path between guaranteed connected nodes

  ### Probabilities for generation behavior
	gen_edge_prob - Probability of generating an edge (Probability of generating query is 1 - gen_edge_prob)
	
	gen_yes_prob - If generating a query, probability of generating guaranteed YES query (Probability of generating guaranteed NO query is 1 - gen_yes_prob)
	
	from_path_prob - Probability of emitting edge from path hen generating edges for guaranteed YES query (Random edges are generated with probability 1 - from_path_prob)

  ### Parameters specific to Kronecker generator
	mrg_seed_1 - First of 2 seed values for Kronecker generator
	mrg_seed_2 - Second of 2 seed values for Kronecker generator

  ### Parameters specific to Power Law generator
	concentration - determines slope of power law generator.
	Higher concentration -> steeper slope (must be > 0)
	
	kseed - seed for random number generator used internally
