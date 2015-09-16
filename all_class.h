/* Header file for class definitions */

class HVAL
{
	int val; // Replace this with actual heuristic
	public:
	HVAL(int val)
	{
		this->val = val;
	}
	bool operator<(HVAL other_hval)
	{
		return hval < other_hval.val;
	}
};

class STATE
{
	unsigned char pos_of[2][8];
	HVAL my_hval,parent_hval;
	public:
	void gen_all_chidren();
	void eval_config;
};
