
namespace mm {

	enum class DP_Approach
	{
		naive = 0,
		naive_tail_recursive,
		naive_non_recursive,
		top_down_using_vector,
		top_down_using_map,
		top_down_tail_recursive,
		top_down_non_recursive,
		bottom_up,
		bottom_up_2,
		bottom_up_space_efficient,
		bottom_up_space_efficient_2,
		bottom_up_most_efficient,
		others_1,

		max_approaches
	};

	inline const string& getDPApproachString(DP_Approach approach)
	{
		static vector<string> strings{
			"naive",
			"naive_tail_recursive",
			"naive_non_recursive",
			"top_down_using_vector",
			"top_down_using_map",
			"top_down_tail_recursive",
			"top_down_non_recursive",
			"bottom_up",
			"bottom_up_2",
			"bottom_up_space_efficient",
			"bottom_up_space_efficient_2",
			"bottom_up_most_efficient",
			"others_1"
		};

		return strings[int(approach)];
	}
}
